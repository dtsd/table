#include "table.h"
#include "io.h"

table_t::table_t(const std::string &fn, const io::hint_list_t &hl) : //file_name(fn)
    last_page(-1), last_free_page(-1)
    ,stream(create_stream(fn))
    ,hint_list(hl)
{
}

table_t::~table_t()
{
}

void table_t::dumps()
{
    stream->seekp(0);
    *stream << *this;
    stream->close();
}

void table_t::loads()
{
    stream->seekg(0);
    *stream >> *this;
    stream->close();
}

table_t::stream_ptr table_t::create_stream(const std::string &fn)
{
    stream_ptr f = std::make_shared<std::fstream>();
    f->open(fn
            , std::ios::in 
            | std::ios::out 
            //| std::ios::app 
            | std::ios::binary
        );
    //std::fstream f(file_name.c_str(), std::ios::in | std::ios::out | std::ios::app | std::ios::binary);
    return f;
}

std::ostream& operator<<(std::ostream &os, const table_t &t)
{
    io::dumps(os, t.last_page);
    io::dumps(os, t.last_free_page);
    io::dumps(os, t.hint_list);
    return os;
}

std::istream& operator>>(std::istream &is, table_t &t)
{
    io::loads(is, t.last_page);
    io::loads(is, t.last_free_page);
    io::loads(is, t.hint_list);
    return is;
}

bool table_t::first()
{
    return true;
}

bool table_t::next()
{
    return true;
}

void table_t::insert()
{
}

std::string table_t::get_value(field_index_t i) const
{
    return row->get_value(i);
}

void table_t::set_value(field_index_t i, const std::string &s)
{
    row->set_value(i, s);
}

io::len_t table_t::get_page_offset(page_t::index_t i)
{
    return page_t::len * (i + 1);
}
