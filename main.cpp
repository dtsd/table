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

#include "common.h"
#include "table.h"
#include "index.h"

int main(int argc, const char *argv[])
{
    std::string fn = "Index_andTable.tab";
    io::hint_list_t hl = { io::hint_t::int_, io::hint_t::str };
    {
        table_t t(fn, hl);
        t.create_index(0, 1000);
        t.create_index(1, 1000);
    }
    {
        table_t t(fn);
        std::cout << t.get_index_count() << std::endl;
    }
}
