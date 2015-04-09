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
    if(i >= value_list.size()) {
        value_list.resize(i + 1);
    }
    value_list[i] = s;
}
