#ifndef TABLE_ITER_24948
#define TABLE_ITER_24948

#include "common.h"
#include "row.h"
#include "page.h"

class table_t;

typedef std::shared_ptr<table_t> table_ptr;

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
private:
    /* data */

    table_ptr table;
    addr_t addr;
};

#endif /* end of include guard: TABLE_ITER_24948 */
