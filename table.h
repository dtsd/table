#ifndef TABLE_17411
#define TABLE_17411

/*
|first page|
|first free_page|
...
pages
 */

#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <cstdint>
#include <vector>
#include <cassert>

#define PAGE_SIZE ( 8 * 1024 )

class table_t {
public:
    typedef uint16_t page_index_t;
    typedef uint64_t page_offset_t;
    typedef uint32_t field_index_t;

    typedef std::shared_ptr<std::fstream> stream_ptr;

    friend std::ostream& operator<<(std::ostream &, const table_t &t);
    friend std::istream& operator>>(std::istream &, table_t &t);
    
    void dumps();
    void loads();

    enum storage_hint_t : uint8_t { literal = 0, integral, toast };
    void set_storage_hint(field_index_t i, storage_hint_t);
    storage_hint_t get_storage_hint(field_index_t i) const;
    page_offset_t get_page_offset(page_index_t);

    table_t (const std::string &);
    virtual ~table_t ();
private:
    /* data */
    stream_ptr create_stream();
    void init();

    std::string file_name;

public:
    page_index_t first_page, first_free_page;
    std::vector<storage_hint_t> storage_hint_list;
};

std::ostream& operator<<(std::ostream &, const table_t &t);
std::istream& operator>>(std::istream &, table_t &t);


#endif /* end of include guard: TABLE_17411 */
