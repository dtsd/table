#include "table.h"
#include "io.h"
#include "index.h"

const addr_t table_t::miss
    = std::make_tuple(page_index_t(-1), row_index_t(-1));

table_t::table_t(const std::string &fn)
    : stream(io::create_stream(fn))
      ,file_name(fn)
{
    loads();
}

table_t::table_t(const std::string &fn, const io::hint_list_t &hl) : //file_name(fn)
    last_page_index(-1)
    ,last_free_page_index(-1)
    ,max_page_index(-1)
    ,hint_list(hl)
    ,stream(io::create_stream(fn, true))
    ,file_name(fn)
{
}

table_t::~table_t()
{
    dumps();
}

void table_t::dumps()
{
    stream->seekp(0);
    *stream << *this;
}

void table_t::loads()
{
    stream->seekg(0);
    *stream >> *this;
}


std::ostream& operator<<(std::ostream &os, const table_t &t)
{
    io::dumps(os, t.last_page_index);
    io::dumps(os, t.last_free_page_index);
    io::dumps(os, t.max_page_index);
    io::dumps(os, t.hint_list);
    io::dumps(os, t.index_file_name_list);
    return os;
}

std::istream& operator>>(std::istream &is, table_t &t)
{
    io::loads(is, t.last_page_index);
    io::loads(is, t.last_free_page_index);
    io::loads(is, t.max_page_index);
    io::loads(is, t.hint_list);
    io::loads(is, t.index_file_name_list);
    t.load_index_list();
    return is;
}

row_ptr table_t::make_row() const
{
    return std::make_shared<row_t>(hint_list);
}

page_index_t table_t::create_page_index()
{
    while(last_free_page_index != -1) {
        page_ptr free_page = load_page(last_free_page_index);
        last_free_page_index = free_page->prev;
    }
    return ++max_page_index;
}

page_ptr table_t::load_page(page_index_t i)
{
    page_ptr page;
    if(i != -1) {
        page = std::make_shared<page_t>();
        stream->seekg(get_page_offset(i));
        *stream >> *page;
        page->index = i;
    }
    return page;
}

page_ptr table_t::create_page()
{
    page = std::make_shared<page_t>();
    page->prev = last_page_index;
    last_page_index = create_page_index();
    page->index = last_page_index;
    //std::cout << "create_page()" << page->index << std::endl;
    return page;
}

void table_t::save_page(page_ptr p)
{
    assert(p);
    assert(p->index != -1);

    stream->seekp(get_page_offset(p->index));
    *stream << *p;
}

io::len_t table_t::get_page_offset(page_index_t i)
{
    io::len_t offset = 0;
    offset += (i + 1) * page_t::len;
    return offset;
}

table_iter_t table_t::begin()
{
    return table_iter_t(table_ptr(this), addr_t(last_page_index, -1));
}

table_iter_t table_t::end()
{
    return table_iter_t(table_ptr(this), miss);
}

table_iter_t table_t::find(field_index_t i, const std::string &s)
{
    index_ptr index = get_index(i);
    if(index) {
        auto l = index->get(s);
        if(l.empty()) {

        } else {
            return table_iter_t(
                table_ptr(this)
                , l[0]
            );
        }
    } else {
        for(auto it = begin(); it != end(); ++it) {
            row_ptr row = *it;
            if(row->get_value(i) == s) {
                return it;
            }
        }
    }
    return end();
}

void table_t::create_index(field_index_t i, size_t size)
{
    assert(i < hint_list.size());
    if(i >= index_list.size()) {
        index_list.resize(i + 1);
    }

    index_file_name_list.resize(index_list.size());

    std::stringstream ss;
    ss << file_name << "." << i << ".idx";

    index_file_name_list[i] = ss.str();

    index_list[i] = std::make_shared<index_t>(
            index_file_name_list[i]
            , size
            , hint_list[i]
        );
    index_list[i]->init();

}

void table_t::load_index_list()
{
    assert(index_file_name_list.size() <= hint_list.size());

    index_list.resize(index_file_name_list.size());
    for(field_index_t i = 0; i < index_file_name_list.size(); ++i) {
        std::string file_name = index_file_name_list[i];

        if(file_name.empty()) {
            continue;
        }

        index_ptr index = std::make_shared<index_t>(
            file_name 
            , 0
            , hint_list[i]
        );
        index->loads();
        index_list[i] = index;
    }
}

index_ptr table_t::get_index(field_index_t i) const
{
    if(i < index_list.size()) {
        return index_list[i];
    }
    return index_ptr();
}
