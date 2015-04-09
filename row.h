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

#ifndef ROW_3295
#define ROW_3295

#include "common.h"
#include "io.h"

class row_t {
public:
    friend std::ostream& operator<<(std::ostream &, const row_t &t);
    friend std::istream& operator>>(std::istream &, row_t &t);

    row_t (const io::hint_list_t &);
    virtual ~row_t ();

    std::string get_value(field_index_t) const;
    void set_value(field_index_t, const std::string &);

    field_index_t size() const { return value_list.size(); };
private:
    /* data */
    io::hint_list_t hint_list;
    std::vector<std::string> value_list;
};

typedef std::shared_ptr<row_t> row_ptr;

std::ostream& operator<<(std::ostream &, const row_t &t);
std::istream& operator>>(std::istream &, row_t &t);

#endif /* end of include guard: ROW_3295 */


