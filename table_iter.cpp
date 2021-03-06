/*
Copyright (C) 2015  Dmitry Teslenko

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "table_iter.h"
#include "table.h"
#include "index.h"

table_iter_t::table_iter_t()
    :addr(table_t::miss)
{
}

table_iter_t::~table_iter_t()
{
}

table_iter_t::table_iter_t(table_ptr t, const addr_t &a)
    : table(t), addr(a)
{
    ensure_page_and_row();
}

table_iter_t::table_iter_t(const table_iter_t &other)
    :table_iter_t(other.table, other.addr)
{
}

bool table_iter_t::ensure_page_and_row()
{
    while(true) {
        if(!page || page->index != std::get<0>(addr)) {
            page = table->load_page(std::get<0>(addr));

            if(!page) {
                addr = table_t::miss;
                return false;
            }

            if(std::get<1>(addr) == -1) {
                std::get<1>(addr) = page->header_list.size() - 1;
            }
        }

        for(row_index_t i = std::get<1>(addr); i >= 0; --i) {
            if(page->header_list[i].len > 0) {
                std::get<1>(addr) = i;
                goto isok;
            }
        }

        std::get<0>(addr) = page->prev;
    }

isok:
    io::len_t offset = table->get_page_offset(std::get<0>(addr));
    offset += page->header_list[std::get<1>(addr)].offset;
    table->stream->seekg(offset);
    row = table->make_row();
    *(table->stream) >> *row;
    return true;
}

table_iter_t& table_iter_t::operator=(const table_iter_t&other)
{
    if(this != &other) {
        addr = other.addr;
        table = other.table;
        ensure_page_and_row();
    }
    return *this;
}

table_iter_t& table_iter_t::operator++()
{
    std::get<1>(addr) -= 1;
    ensure_page_and_row();
    return *this;
}

row_ptr table_iter_t::operator*()
{
    return row;
}

bool table_iter_t::operator==(const table_iter_t &other)
{
    return addr == other.addr
        && table == other.table
     ;
}

bool table_iter_t::operator!=(const table_iter_t &other)
{
    return addr != other.addr
        || table != other.table
    ;
}

void table_iter_t::insert(row_ptr r)
{
    while(!page || !page->append(*r)) {
        page = table->create_page();
        //std::get<0>(addr) = page->index;
    }
    {
        index_ptr index;
        for(field_index_t i = 0; i < r->size(); ++i) {
            if(index = table->get_index(i)) {
                index->put(
                    r->get_value(i)
                    ,page->index
                    ,page->header_list.size() - 1
                );
            }
        }
    }

    table->save_page(page);
}

void table_iter_t::update(row_ptr r)
{
    if(!ensure_page_and_row()) {
        return;
    }

    {
        index_ptr index;
        for(field_index_t i = 0; i < row->size(); ++i) {
            if(index = table->get_index(i)) {
                index->del(
                    row->get_value(i)
                    ,std::get<0>(addr)
                    ,std::get<1>(addr)
                );
            }
        }
    }

    page->mark_free(std::get<1>(addr));
    insert(r);
    ensure_page_and_row();
}

void table_iter_t::delete_()
{
    if(!ensure_page_and_row()) {
        return;
    }

    {
        index_ptr index;
        for(field_index_t i = 0; i < row->size(); ++i) {
            if(index = table->get_index(i)) {
                index->del(
                    row->get_value(i)
                    ,std::get<0>(addr)
                    ,std::get<1>(addr)
                );
            }
        }
    }

    page->mark_free(std::get<1>(addr));

    ensure_page_and_row();
}
