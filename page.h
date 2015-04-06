#ifndef PAGE_19541
#define PAGE_19541

#include "common.h"
#include "row.h"

class page_t {
public:
    typedef uint16_t index_t;
    typedef uint16_t len_t;

    struct header_t {
        len_t offset, len;
    };

    static const len_t len = 8 * 1024;

    friend std::ostream& operator<<(std::ostream &, const page_t &t);
    friend std::istream& operator>>(std::istream &, page_t &t);

    page_t ();
    virtual ~page_t ();

    bool append(const row_t &);
    void mark_free(row_t::index_t);

    //row_ptr load_row(row_t::index_t);
private:
    /* data */

    std::set<row_t::index_t> dirty_header_set;
    std::vector<std::string> dirty_row_list_reversed;
public:
    index_t prev;
    len_t header_end, row_begin;
    std::vector<header_t> header_list;
};

typedef std::shared_ptr<page_t> page_ptr;

std::ostream& operator<<(std::ostream &, const page_t &t);
std::istream& operator>>(std::istream &, page_t &t);

#endif /* end of include guard: PAGE_19541 */

