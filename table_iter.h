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

#ifndef TABLE_ITER_24948
#define TABLE_ITER_24948

#include "common.h"
#include "row.h"
#include "page.h"

class table_t;

//typedef std::shared_ptr<table_t> table_ptr;
typedef table_t* table_ptr;

class table_iter_t {
public:
    table_iter_t ();
    table_iter_t (table_ptr, const addr_t &);
    table_iter_t (const table_iter_t &);
    table_iter_t& operator=(const table_iter_t &);

    virtual ~table_iter_t ();

    table_iter_t& operator++();
    row_ptr operator*();
    bool operator==(const table_iter_t &);
    bool operator!=(const table_iter_t &);


    void insert(row_ptr);
    void update(row_ptr);
    void delete_();
private:
    /* data */
    bool ensure_page_and_row();

    table_ptr table;
    addr_t addr;

    page_ptr page;
    row_ptr row;
};

#endif /* end of include guard: TABLE_ITER_24948 */
