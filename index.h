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

#ifndef INDEX_12187
#define INDEX_12187

#include "common.h"
#include "table.h"
#include "io.h"
#include "row.h"
#include "page.h"

#include <boost/functional/hash.hpp>

class index_t {
public:
    index_t(const std::string &, size_t, io::hint_t);
    virtual ~index_t ();

    struct bucket_t {
        size_t offset, len;
    };

    friend std::ostream& operator<<(std::ostream &, const index_t &t);
    friend std::istream& operator>>(std::istream &, index_t &t);

    void put(const std::string &, page_index_t, row_index_t);
    std::vector<addr_t> get(const std::string &s);
    void del(const std::string &, page_index_t, row_index_t);

    size_t get_bucket_index(const std::string &);
    row_ptr make_row();

    enum field_t : uint8_t { value = 0, page, row, next, next_len };

    void init();
    void loads();
    void dumps();

    std::string get_file_name() const { return file_name; }
    size_t get_len() const { return len; }
private:
    /* data */
    size_t len, row_end;
    const size_t bucket_begin;

    io::stream_ptr stream;
    std::string file_name;

    boost::hash<std::string> hasher;
    io::hint_list_t hint_list;
};


std::ostream& operator<<(std::ostream &, const index_t &t);
std::istream& operator>>(std::istream &, index_t &t);

#endif /* end of include guard: INDEX_12187 */
