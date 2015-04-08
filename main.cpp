#include "common.h"
#include "table.h"
#include "index.h"

int main(int argc, const char *argv[])
{
    table_t t("test.tab", io::hint_list_t());

    index_t index("Index_putGet.idx", 1000, io::hint_t::int_);
    index.init();

    for(int i = 0; i < 1; ++i) {
        index.put(std::to_string(i), i * 2, i * 3);
    }

    return 0;
}
