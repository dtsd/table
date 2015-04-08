#ifndef TABLE_17411
#define TABLE_17411

/*
|first page|
|first free_page|
...
pages
 */

//TODO
//* table defragmentation
//* index growth
//* index defragmentation
//* toast storage
//* toast defragmentation

#include "common.h"
#include "page.h"
#include "row.h"
#include "table_iter.h"

class table_t {
public:
    static const addr_t miss;

    friend std::ostream& operator<<(std::ostream &, const table_t &t);
    friend std::istream& operator>>(std::istream &, table_t &t);

    io::len_t get_page_offset(page_t::index_t);
    
    void dumps();
    void loads();

    table_t (const std::string &, const io::hint_list_t &);
    virtual ~table_t ();


    row_ptr last();
    row_ptr prev();
    row_ptr current() const;

    row_ptr make_row() const;
    void insert(row_ptr);
    void update();
    void delete_();

    table_iter_t begin(); 
    table_iter_t end();
    table_iter_t find(field_index_t, const std::string &);

    page_ptr load_page(page_t::index_t);
    io::hint_list_t get_hint_list() const { return hint_list; }
    page_t::index_t create_page_index();
private:
    /* data */

    //std::string file_name;

    io::hint_list_t hint_list;
public:
    page_t::index_t last_page, last_free_page;
    page_t::index_t max_page_index;

    page_t::index_t current_page_index;
    row_t::index_t current_row_index;

    io::stream_ptr stream;

    page_ptr page;
    row_ptr row;
};


std::ostream& operator<<(std::ostream &, const table_t &t);
std::istream& operator>>(std::istream &, table_t &t);

#endif /* end of include guard: TABLE_17411 */
