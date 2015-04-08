#include "index.h"

index_t::index_t(const std::string &fn, size_t l, io::hint_t h)
    : hint_list({h
            , io::hint_t::size
            , io::hint_t::size
            , io::hint_t::size
            , io::hint_t::size
            })
    , len(l)
    , bucket_begin(sizeof(len) + sizeof(row_end) + sizeof(io::hint_t))
    , stream(io::create_stream(fn)), file_name(fn)
{
    row_end = bucket_begin + sizeof(bucket_t) * len;
}

index_t::~index_t()
{
}

void index_t::init()
{
    stream = std::make_shared<std::fstream>();
    stream = io::create_stream(file_name, true);
    stream->seekp(bucket_begin);
    bucket_t bucket;
    bucket.offset = 0;
    bucket.len = 0;
    for(int i = 0; i < len; ++i) {
        io::dumps(*stream, bucket);
    }
}

std::ostream& operator<<(std::ostream &os, const index_t &i)
{
    assert(i.hint_list.size() > 0);
    io::dumps(os, i.len);
    io::dumps(os, i.row_end);
    io::dumps(os, i.hint_list[0]);
    return os;
}

std::istream& operator>>(std::istream &is, index_t &i)
{
    assert(i.hint_list.size() > 0);
    io::loads(is, i.len);
    io::loads(is, i.row_end);
    io::loads(is, i.hint_list[0]);
    return is;
}

size_t index_t::get_bucket_index(const std::string &s)
{
    size_t h = hasher(s);
    return h % len;
}

row_ptr index_t::make_row()
{
    return std::make_shared<row_t>(hint_list);
}

void index_t::del(const std::string &value, page_t::index_t pi, row_t::index_t ri)
{
    size_t i = get_bucket_index(value);
    size_t 
        bucket_offset = bucket_begin + i * sizeof(bucket_t)
        ,row_offset = -1
        ,row2_offset = -1
    ;
    bucket_t bucket;
    stream->seekg(bucket_offset);
    io::loads(*stream, bucket);

    row_ptr 
        row = make_row()
        ,row2 = make_row()
    ;

    if(bucket.len == 0) {
        return;
    } else {
        row_offset = bucket.offset;
        while(true) {
            stream->seekg(row_offset); 
            *stream >> *row;

            if(row->get_value(field_t::value) == value) {
                break;
            } else {
                row2_offset = row_offset;
                *row2 = *row;
            }

            if(std::stoul(row->get_value(field_t::next_len)) == 0) {
                return;
            } else {
                row_offset = std::stoul(row2->get_value(field_t::next));
            }

        }

        if(row2_offset == -1) {
            bucket.offset = std::stoul(row->get_value(field_t::next));
            bucket.len = std::stoul(row->get_value(field_t::next_len));
            stream->seekp(bucket_offset);
            io::dumps(*stream, bucket);
        } else {
            row2->set_value(field_t::next, row->get_value(field_t::next));
            row2->set_value(field_t::next_len, row->get_value(field_t::next_len));
            stream->seekp(row2_offset);
            *stream << *row2;
        }

    }
}

void index_t::put(const std::string &value, page_t::index_t pi, row_t::index_t ri)
{
    size_t i = get_bucket_index(value);
    size_t offset = bucket_begin + i * sizeof(bucket_t);
    bucket_t bucket;
    stream->seekg(offset);
    io::loads(*stream, bucket);

    row_ptr 
        row = make_row()
        , row2 = make_row()
    ;

    row->set_value(field_t::value, value);
    row->set_value(field_t::page, std::to_string(pi));
    row->set_value(field_t::row, std::to_string(ri));
    row->set_value(field_t::next, std::to_string(-1));
    row->set_value(field_t::next_len, std::to_string(0));

    std::stringstream ss;
    ss << *row;

    std::string s = ss.str();

    if(bucket.len == 0) {
        bucket.offset = row_end;
        bucket.len = s.size();
        row_end += bucket.len;

        stream->seekp(offset);
        io::dumps(*stream, bucket);
        stream->seekp(bucket.offset);
        stream->write(s.c_str(), s.size());
    } else {

        offset = bucket.offset;
        while(true) {
            stream->seekg(offset); 
            *stream >> *row2;
            if(std::stoul(row2->get_value(field_t::next_len)) == 0) {
                break;
            } else {
                offset = std::stoul(row2->get_value(field_t::next));
            }
        }

        stream->seekp(offset);
        row2->set_value(field_t::next, std::to_string(row_end));
        row2->set_value(field_t::next_len, std::to_string(s.size()));
        *stream << *row2;

        stream->seekp(row_end);
        row_end += s.size();
        stream->write(s.c_str(), s.size());
    }
}

std::vector<addr_t> index_t::get(const std::string &value)
{
    std::vector<addr_t> list;

    size_t i = get_bucket_index(value);
    size_t offset = bucket_begin + i * sizeof(bucket_t);
    bucket_t bucket;
    stream->seekg(offset);
    io::loads(*stream, bucket);

    row_ptr 
        row = make_row()
    ;

    if(bucket.len == 0) {
        return list;
    } else {

        offset = bucket.offset;
        while(true) {
            stream->seekg(offset); 
            *stream >> *row;
            
            if(row->get_value(field_t::value) == value) {
                list.push_back(
                    std::make_tuple(
                        page_t::index_t(std::stoul(row->get_value(field_t::page)))
                        ,row_t::index_t(std::stoul(row->get_value(field_t::row)))
                    )
                );
            }

            if(std::stoul(row->get_value(field_t::next_len)) == 0) {
                break;
            } else {
                offset = std::stoul(row->get_value(field_t::next));
            }
        }

        return list;
    }
}
