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

class index_t;
typedef std::shared_ptr<index_t> index_ptr;

class table_t {
public:
    static const addr_t miss;

    friend std::ostream& operator<<(std::ostream &, const table_t &t);
    friend std::istream& operator>>(std::istream &, table_t &t);

    io::len_t get_page_offset(page_index_t);
    
    void dumps();
    void loads();

    table_t (const std::string &);
    table_t (const std::string &, const io::hint_list_t &);
    virtual ~table_t ();


    row_ptr make_row() const;
    table_iter_t begin(); 
    table_iter_t end();
    table_iter_t find(field_index_t, const std::string &);

    page_ptr load_page(page_index_t);
    page_ptr create_page();
    void save_page(page_ptr);


    io::hint_list_t get_hint_list() const { return hint_list; }
    page_index_t create_page_index();
private:
    /* data */

    //std::string file_name;
    void create_index(field_index_t, size_t);

    io::hint_list_t hint_list;
    std::vector<index_ptr> index_list;
public:
    page_index_t last_page_index, last_free_page_index;
    page_index_t max_page_index;

    page_index_t current_page_index;
    row_index_t current_row_index;

    io::stream_ptr stream;

    page_ptr page;
    row_ptr row;

    std::string file_name;
};


std::ostream& operator<<(std::ostream &, const table_t &t);
std::istream& operator>>(std::istream &, table_t &t);

#endif /* end of include guard: TABLE_17411 */
