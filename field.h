#ifndef FIELD_25206
#define FIELD_25206

class field_t {
public:
    friend std::ostream& operator<<(std::ostream &, const field_t &t);
    friend std::istream& operator>>(std::istream &, field_t &t);

    field_t ();
    virtual ~field_t ();
private:
    /* data */
};

#endif /* end of include guard: FIELD_25206 */
