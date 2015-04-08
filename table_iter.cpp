#include "table_iter.h"
#include "table.h"

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

}

table_iter_t::table_iter_t(const table_iter_t &other)
    :table(other.table), addr(other.addr)
{
}

table_iter_t& table_iter_t::operator=(const table_iter_t&other)
{
    if(this != &other) {
        addr = other.addr;
        table = other.table;
    }
    return *this;
}

table_iter_t& table_iter_t::operator++()
{
}

row_ptr table_iter_t::operator*()
{
    return table->make_row();
}

bool table_iter_t::operator==(const table_iter_t &other)
{
    return addr == other.addr
        && table.get() == other.table.get()
     ;
}

bool table_iter_t::operator!=(const table_iter_t &other)
{
    return addr != other.addr
        || table.get() != other.table.get()
    ;
}
