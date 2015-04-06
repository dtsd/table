#ifndef TABLE_17411
#define TABLE_17411

/*
|first page|
|first free_page|
...
pages
 */

#include "common.h"
#include "page.h"
#include "row.h"

class table_t {
public:

    typedef std::shared_ptr<std::fstream> stream_ptr;

    friend std::ostream& operator<<(std::ostream &, const table_t &t);
    friend std::istream& operator>>(std::istream &, table_t &t);

    io::len_t get_page_offset(page_t::index_t);
    
    void dumps();
    void loads();

    table_t (const std::string &, const io::hint_list_t &);
    virtual ~table_t ();


    bool first();
    bool next();
    void insert();
    void delete_();
    std::string get_value(field_index_t) const;
    void set_value(field_index_t, const std::string &);

    page_ptr load_page(page_t::index_t);
    io::hint_list_t get_hint_list() const { return hint_list; }
private:
    /* data */
    stream_ptr create_stream(const std::string &);

    //std::string file_name;

    io::hint_list_t hint_list;
public:
    page_t::index_t last_page, last_free_page;

    page_ptr page;
    stream_ptr stream;
    row_ptr row;
};

std::ostream& operator<<(std::ostream &, const table_t &t);
std::istream& operator>>(std::istream &, table_t &t);

#endif /* end of include guard: TABLE_17411 */
