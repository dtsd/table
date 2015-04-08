#ifndef INDEX_12187
#define INDEX_12187

#include "common.h"
#include "table.h"
#include "io.h"
#include "row.h"
#include "page.h"

#include <boost/functional/hash.hpp>

class index_t {
public:
    index_t(const std::string &, size_t, io::hint_t);
    virtual ~index_t ();

    struct bucket_t {
        size_t offset, len;
    };

    friend std::ostream& operator<<(std::ostream &, const index_t &t);
    friend std::istream& operator>>(std::istream &, index_t &t);

    void put(const std::string &, page_t::index_t, row_t::index_t);
    std::vector<addr_t> get(const std::string &s);
    void del(const std::string &, page_t::index_t, row_t::index_t);

    size_t get_bucket_index(const std::string &);
    row_ptr make_row();

    enum field_t : uint8_t { value = 0, page, row, next, next_len };

    void init();
private:
    /* data */
    size_t len, row_end;
    const size_t bucket_begin;

    io::stream_ptr stream;
    std::string file_name;

    boost::hash<std::string> hasher;
    io::hint_list_t hint_list;
};

typedef std::shared_ptr<index_t> index_ptr;

std::ostream& operator<<(std::ostream &, const index_t &t);
std::istream& operator>>(std::istream &, index_t &t);

#endif /* end of include guard: INDEX_12187 */
