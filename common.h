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

#ifndef COMMON_17696
#define COMMON_17696

#include <sstream>
#include <fstream>
#include <iostream>
#include <sstream>

#include <type_traits>
#include <cassert>
#include <cstdint>

#include <iterator>
#include <vector>
#include <string>
#include <set>
#include <tuple>

#include <memory>

typedef int64_t page_index_t;
typedef uint16_t page_len_t;
typedef int16_t row_index_t;

typedef uint32_t field_index_t;
typedef std::tuple<page_index_t, row_index_t> addr_t;


#endif /* end of include guard: COMMON_17696 */
