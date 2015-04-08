#ifndef IO_6442
#define IO_6442

#include "common.h"

namespace io {

enum hint_t : uint8_t { str = 0, int_, toast, size };
typedef std::vector<hint_t> hint_list_t;

typedef uint64_t len_t;

typedef std::shared_ptr<std::fstream> stream_ptr;

inline
stream_ptr create_stream(const std::string &fn, bool trunc = false)
{
    stream_ptr f = std::make_shared<std::fstream>();
    std::ios::openmode m = std::ios::in 
            | std::ios::out 
            | std::ios::binary
    ;
    if(trunc) {
        m |= std::ios::trunc;
    }
    f->open(fn, m);
    return f;
}


template<typename T>
void loads(std::istream &is, T &t);

template<typename T>
void loads(std::istream &is, T &t, std::true_type)
{
    is.read(reinterpret_cast<char *>(&t), sizeof(t));
}

template<typename T>
void loads(std::istream &is, T &t, std::false_type)
{
    auto size = t.size();
    loads(is, size);
    t.resize(size);
    for(auto &item : t) {
        loads(is, item);
    }
}

template<typename T>
void loads(std::istream &is, T &t)
{
    loads(is, t, std::integral_constant<bool, std::is_pod<T>::value>() );
}

template<typename T>
void dumps(std::ostream &os, const T &t);

template<typename T>
void dumps(std::ostream &os, const T &t, std::true_type)
{
    os.write(reinterpret_cast<const char *>(&t), sizeof(t));
}

template<typename T>
void dumps(std::ostream &os, const T &t, std::false_type)
{
    auto size = t.size();
    os.write(reinterpret_cast<const char *>(&size), sizeof(size));
    for(auto const &item : t) {
        dumps(os, item);
    }
}

template<typename T>
void dumps(std::ostream &os, const T &t)
{
    dumps(os, t, std::integral_constant<bool, std::is_pod<T>::value>());
}

}


#endif /* end of include guard: IO_6442 */
