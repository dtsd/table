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

#include "page.h"
#include "io.h"

const page_len_t page_t::len = 8 * 1024;

page_t::page_t() : prev(-1), index(-1)
    , header_end(sizeof(prev) + sizeof(header_end) + sizeof(row_begin))
    , row_begin(len)
{
    //std::cerr << "header_end is " << header_end << std::endl;
} 

page_t::~page_t()
{
}


std::ostream& operator<<(std::ostream &os, const page_t &p)
{
    io::len_t offset = os.tellp();

    io::dumps(os, p.prev);
    io::dumps(os, p.header_end);
    io::dumps(os, p.row_begin);

    for(auto i : p.dirty_header_set) {
        assert(i < p.header_list.size());

        io::len_t header_offset = offset 
            + p.header_end 
            - (p.header_list.size() - i) * sizeof(page_t::header_t)
        ;
        os.seekp(header_offset);
        //std::cerr << "writing header at " << os.tellp() << std::endl;
        io::dumps(os, p.header_list[i]);
    }

    os.seekp(offset + p.row_begin);
    for(int i = p.dirty_row_list_reversed.size() - 1; i >= 0; --i) {
        const std::string &s = p.dirty_row_list_reversed[i];
        //std::cerr << "writing row at " << os.tellp() << " with " << s.size() << std::endl;
        os.write(s.c_str(), s.size());
    }
    return os;
}

std::istream& operator>>(std::istream &is, page_t &p)
{
    io::len_t offset = is.tellg();

    io::loads(is, p.prev);
    io::loads(is, p.header_end);
    io::loads(is, p.row_begin);

    page_t::header_t header;
    while(is.tellg() < offset + p.header_end - 1) {
        //std::cerr << "reading header at " << is.tellg() << std::endl;
        io::loads(is, header);
        p.header_list.push_back(header);
    }

    return is;
}

bool page_t::append(const row_t &row)
{
    std::stringstream ss;
    ss << row;
    std::string s = ss.str();

    if((s.size() + sizeof(header_t)) > (row_begin - header_end)) {
        return false;
    }
    
    header_t header;
    header.len = s.size();
    row_begin -= header.len;
    header.offset = row_begin;
    header_end += sizeof(header_t);

    row_index_t i = header_list.size();
    dirty_header_set.insert(i);
    header_list.push_back(header);
    dirty_row_list_reversed.push_back(s);

    return true;
}

void page_t::mark_free(row_index_t i)
{
    assert(header_list.size() > i);
    header_list[i].len = 0;
    dirty_header_set.insert(i);
}

/*
row_ptr page_t::load_row(row_index_t i)
{
    row_ptr r;
    return r;
}
*/
