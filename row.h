#ifndef ROW_3295
#define ROW_3295

#include "common.h"
#include "io.h"

class row_t {
public:
    typedef uint16_t index_t;

    friend std::ostream& operator<<(std::ostream &, const row_t &t);
    friend std::istream& operator>>(std::istream &, row_t &t);

    row_t (const io::hint_list_t &);
    virtual ~row_t ();

    std::string get_value(field_index_t) const;
    void set_value(field_index_t, const std::string &);
private:
    /* data */
    const io::hint_list_t &hint_list;
    std::vector<std::string> value_list;
};

typedef std::shared_ptr<row_t> row_ptr;

std::ostream& operator<<(std::ostream &, const row_t &t);
std::istream& operator>>(std::istream &, row_t &t);

#endif /* end of include guard: ROW_3295 */


