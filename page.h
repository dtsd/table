#ifndef PAGE_19541
#define PAGE_19541

#include "common.h"
#include "row.h"

class page_t {
public:

    struct header_t {
        page_len_t offset, len;
    };

    static const page_len_t len ;

    friend std::ostream& operator<<(std::ostream &, const page_t &t);
    friend std::istream& operator>>(std::istream &, page_t &t);

    page_t ();
    virtual ~page_t ();

    bool append(const row_t &);
    void mark_free(row_index_t);

    //row_ptr load_row(row_index_t);
private:
    /* data */

    std::set<row_index_t> dirty_header_set;
    std::vector<std::string> dirty_row_list_reversed;
public:
    page_index_t prev, index;
    page_len_t header_end, row_begin;
    std::vector<header_t> header_list;
};

typedef std::shared_ptr<page_t> page_ptr;

std::ostream& operator<<(std::ostream &, const page_t &t);
std::istream& operator>>(std::istream &, page_t &t);

#endif /* end of include guard: PAGE_19541 */

