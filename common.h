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
