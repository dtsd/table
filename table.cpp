#include "table.h"
#include "io.h"

table_t::table_t(const std::string &fn) : file_name(fn)
    ,first_page(-1), first_free_page(-1)
{
}

table_t::~table_t()
{
}

void table_t::dumps()
{
    stream_ptr f = create_stream();
    *f << *this;
    f->close();
}

void table_t::loads()
{
    stream_ptr f = create_stream();
    //f->seekg(0);
    *f >> *this;
    f->close();
}

table_t::stream_ptr table_t::create_stream()
{
    stream_ptr f = std::make_shared<std::fstream>();
    f->open(file_name.c_str()
            , std::ios::in 
            | std::ios::out 
            //| std::ios::app 
            | std::ios::binary
        );
    //std::fstream f(file_name.c_str(), std::ios::in | std::ios::out | std::ios::app | std::ios::binary);
    return f;
}

table_t::page_offset_t table_t::get_page_offset(page_index_t i)
{
    page_offset_t t = 0;
    t += PAGE_SIZE;
    t += i * PAGE_SIZE;
    return t;
}

void table_t::set_storage_hint(field_index_t i, storage_hint_t sh)
{
    if(i >=  storage_hint_list.size()) {
        storage_hint_list.resize(i + 1);
    }
    storage_hint_list[i] = sh;
}

table_t::storage_hint_t table_t::get_storage_hint(field_index_t i) const
{
    assert(i < storage_hint_list.size());
    return storage_hint_list[i];
}

std::ostream& operator<<(std::ostream &os, const table_t &t)
{
    dumps(os, t.first_page);
    dumps(os, t.first_free_page);

    table_t::field_index_t size = t.storage_hint_list.size();
    dumps(os, size);
    for(std::vector<table_t::storage_hint_t>::const_iterator it = t.storage_hint_list.begin()
            ; it != t.storage_hint_list.end(); ++it) {
        table_t::storage_hint_t hint = *it;
        dumps(os, hint);
    }
    return os;
}

std::istream& operator>>(std::istream &is, table_t &t)
{
    loads(is, t.first_page);
    loads(is, t.first_free_page);

    table_t::field_index_t size;
    table_t::storage_hint_t hint;
    loads(is, size);
    t.storage_hint_list.resize(size);
    for(int i = 0; i < size; ++i) {
        loads(is, hint);
        t.storage_hint_list[i] = hint;
    }
    return is;
}
