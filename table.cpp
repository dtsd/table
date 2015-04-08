#include "table.h"
#include "io.h"

const addr_t table_t::miss
    = std::make_tuple(page_t::index_t(-1), row_t::index_t(-1));

table_t::table_t(const std::string &fn, const io::hint_list_t &hl) : //file_name(fn)
    last_page(-1), last_free_page(-1)
    ,max_page_index(-1)
    ,stream(io::create_stream(fn))
    ,hint_list(hl)
    ,current_page_index(-1)
    ,current_row_index(-1)
{
}

table_t::~table_t()
{
}

void table_t::dumps()
{
    stream->seekp(0);
    *stream << *this;
    stream->close();
}

void table_t::loads()
{
    stream->seekg(0);
    *stream >> *this;
    stream->close();
}


std::ostream& operator<<(std::ostream &os, const table_t &t)
{
    io::dumps(os, t.last_page);
    io::dumps(os, t.last_free_page);
    io::dumps(os, t.max_page_index);
    io::dumps(os, t.hint_list);
    return os;
}

std::istream& operator>>(std::istream &is, table_t &t)
{
    io::loads(is, t.last_page);
    io::loads(is, t.last_free_page);
    io::loads(is, t.max_page_index);
    io::loads(is, t.hint_list);
    return is;
}

row_ptr table_t::last()
{
    row = row_ptr();

    current_page_index = last_page;
    while(current_page_index >= 0) {
        page = load_page(current_page_index);
        assert(page);
        current_row_index = page->header_list.size() - 1;
        while(current_row_index >= 0 
            && page->header_list[current_row_index].len == 0) {
            current_page_index -= 1;
        }
        if(current_row_index < 0) {
            current_page_index = page->prev;
            continue;
        }
        row = make_row();
        io::len_t offset = get_page_offset(current_page_index);
        offset += page->header_list[current_row_index].offset;
        stream->seekg(offset);
        *stream >> *row;
        break;
    }

    return row;
}

row_ptr table_t::prev()
{
    while(current_page_index >= 0) {
        page = load_page(current_page_index);
        assert(page);
        current_row_index = page->header_list.size() - 1;
        while(current_row_index >= 0 
            && page->header_list[current_row_index].len == 0) {
            current_page_index -= 1;
        }
        if(current_row_index < 0) {
            current_page_index = page->prev;
            continue;
        }
        row = make_row();
        io::len_t offset = get_page_offset(current_page_index);
        offset += page->header_list[current_row_index].offset;
        stream->seekg(offset);
        *stream >> *row;

    }
    return row;
}

row_ptr table_t::current() const
{
    return row;
}

row_ptr table_t::make_row() const
{
    return std::make_shared<row_t>(hint_list);
}

void table_t::insert(row_ptr r)
{
    while(!page->append(*row)) {
        page = std::make_shared<page_t>();
        page->prev = last_page;
        current_page_index = last_page = create_page_index();
    }
}

void table_t::update()
{
    assert(page && row);
    page->mark_free(current_page_index);
    while(!page->append(*row)) {
        page = std::make_shared<page_t>();
        page->prev = last_page;
        current_page_index = last_page = create_page_index();
    }
}

void table_t::delete_()
{
}

page_t::index_t table_t::create_page_index()
{
    while(last_free_page >= 0) {
        page_ptr free_page = load_page(last_free_page);
        last_free_page = free_page->prev;
    }
    return ++max_page_index;
}

page_ptr table_t::load_page(page_t::index_t i)
{
    page_ptr page = std::make_shared<page_t>();
    stream->seekg(get_page_offset(i));
    *stream >> *page;
    return page;
}

io::len_t table_t::get_page_offset(page_t::index_t i)
{
    io::len_t offset = 0;
    offset += (i + 1) * page_t::len;
    return offset;
}

table_iter_t table_t::begin()
{
    return table_iter_t(table_ptr(this), addr_t(last_page, 0));
}

table_iter_t table_t::end()
{
    return table_iter_t(table_ptr(this), miss);
}

table_iter_t table_t::find(field_index_t, const std::string &)
{
    return table_iter_t(table_ptr(this), miss);
}
