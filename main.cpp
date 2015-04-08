#include "common.h"
#include "table.h"
#include "index.h"

int main(int argc, const char *argv[])
{
    std::string fn = "Table_iter.tab";
    io::hint_list_t hl = { io::hint_t::int_, io::hint_t::str };

    {
        table_t t(fn, hl);
        auto it = t.begin();
        auto row = t.make_row();
        row->set_value(0, std::to_string(123));
        row->set_value(1, "abc");
        it.insert(row);
    }

    {
        table_t t(fn);
        auto it = t.begin();

        assert(it != t.end());
        assert(*it);

        row_ptr row = *it;
        row->set_value(1, "ABCDEFGH");
        it.update(row);
    }
}
