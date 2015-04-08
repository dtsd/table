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
