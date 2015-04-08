#include "row.h"
#include "io.h"

row_t::row_t(const io::hint_list_t &hl) : hint_list(hl)
{
}

row_t::~row_t()
{
}

std::ostream& operator<<(std::ostream &os, const row_t &r)
{
    /*
    std::cout 
        << " r.value_list " << r.value_list.size() 
        << " r.hint_list " << r.hint_list.size() 
        << std::endl
    ;
    */
    assert(r.value_list.size() <= r.hint_list.size());
    //for(auto const &value : r.value_list) {
    for(field_index_t i = 0; i < r.value_list.size(); ++i) {
        switch(r.hint_list[i]) {
            case io::hint_t::int_: {
                    int value = std::stoi(r.value_list[i]);
                    io::dumps(os, value);
                }
                break;
            case io::hint_t::size: {
                    size_t value = std::stoul(r.value_list[i]);
                    io::dumps(os, value);
                }
                break;
            default: {
                    io::dumps(os, r.value_list[i]);
                }
        }
    }
    return os;
}

std::istream& operator>>(std::istream &is, row_t &r)
{
    //for(auto &value : r.value_list) {
    r.value_list.resize(r.hint_list.size());
    for(field_index_t i = 0; i < r.hint_list.size(); ++i) {
        switch(r.hint_list[i]) {
            case io::hint_t::int_: {
                    int value;
                    io::loads(is, value);
                    r.value_list[i] = std::to_string(value);
                }
                break;
            case io::hint_t::size: {
                    size_t value;
                    io::loads(is, value);
                    r.value_list[i] = std::to_string(value);
                }
                break;
            default: {
                    io::loads(is, r.value_list[i]);
                }
        }
    }
    return is;
}

std::string row_t::get_value(field_index_t i) const
{
    assert(i < value_list.size());
    assert(i < hint_list.size());
    return value_list[i];
}

void row_t::set_value(field_index_t i, const std::string &s)
{
    assert(i < hint_list.size());
    value_list.resize(i + 1);
    value_list[i] = s;
}
