#include "page.h"
#include "io.h"

const page_t::len_t page_t::len = 8 * 1024;

page_t::page_t() : prev(-1)
    , header_end(sizeof(prev) + sizeof(header_end) + sizeof(row_begin) + 1)
    , row_begin(len)
{
} 

page_t::~page_t()
{
}


std::ostream& operator<<(std::ostream &os, const page_t &p)
{
    io::len_t offset = os.tellp();

    io::dumps(os, p.prev);
    io::dumps(os, p.header_end);
    io::dumps(os, p.row_begin);

    for(auto i : p.dirty_header_set) {
        assert(i < p.header_list.size());

        os.seekp(offset + p.header_end 
                - (p.header_list.size() - i) * sizeof(page_t::header_t));
        io::dumps(os, p.header_list[i]);
    }

    os.seekp(offset + p.row_begin);
    for(int i = p.dirty_row_list_reversed.size() - 1; i >= 0; --i) {
        os << p.dirty_row_list_reversed[i];
    }
    return os;
}

std::istream& operator>>(std::istream &is, page_t &p)
{
    io::len_t offset = is.tellg();

    io::loads(is, p.prev);
    io::loads(is, p.header_end);
    io::loads(is, p.row_begin);

    page_t::header_t header;
    while(is.tellg() < offset + p.header_end) {
        io::loads(is, header);
        p.header_list.push_back(header);
    }

    return is;
}

bool page_t::append(const row_t &row)
{
    std::stringstream ss;
    ss << row;
    std::string s = ss.str();

    if((s.size() + sizeof(header_t)) > (row_begin - header_end)) {
        return false;
    }
    
    header_t header;
    header.len = s.size();
    row_begin -= header.len;
    header.offset = row_begin;
    header_end += sizeof(header_t);

    row_t::index_t i = header_list.size();
    dirty_header_set.insert(i);
    header_list.push_back(header);
    dirty_row_list_reversed.push_back(s);

    return true;
}

void page_t::mark_free(row_t::index_t i)
{
    assert(header_list.size() > i);
    header_list[i].len = 0;
    dirty_header_set.insert(i);
}

/*
row_ptr page_t::load_row(row_t::index_t i)
{
    row_ptr r;
    return r;
}
*/
