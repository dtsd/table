#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Table
#include <boost/test/unit_test.hpp>

#include "table.h"
#include "row.h"
#include "io.h"

BOOST_AUTO_TEST_CASE(Table_serialization)
{
    std::stringstream ss;

    io::hint_list_t hl = { 
        io::hint_t::integral 
        ,io::hint_t::literal 
        ,io::hint_t::integral 
        ,io::hint_t::integral 
        ,io::hint_t::literal 
        ,io::hint_t::literal 
        ,io::hint_t::literal 
        ,io::hint_t::literal 
        ,io::hint_t::integral 
        ,io::hint_t::toast 
    };

    table_t t1("test.tab", hl), t2("test.tab", io::hint_list_t());
    t1.last_page = 123;
    t1.last_free_page = 496;

    ss << t1;
    ss >> t2;

    BOOST_CHECK_EQUAL( t2.last_page , 123 );
    BOOST_CHECK_EQUAL( t2.last_free_page , 496 );

    BOOST_CHECK_EQUAL( t1.get_hint_list().size() , 10);
    BOOST_CHECK_EQUAL( t2.get_hint_list().size() , 10);
    BOOST_CHECK_EQUAL( t2.get_hint_list()[2] , io::hint_t::integral);
    BOOST_CHECK_EQUAL( t2.get_hint_list()[8] , io::hint_t::integral);
    BOOST_CHECK_EQUAL( t2.get_hint_list()[9] , io::hint_t::toast);
}

#include <boost/filesystem.hpp>

BOOST_AUTO_TEST_CASE(Table_file)
{
    std::string fn = "fileTest.tab";
    {
        std::fstream f;
        f.open(fn
                , std::ios::in
                | std::ios::out
                | std::ios::binary
                );
        f.write(fn.c_str(), fn.size());
    }

    uint16_t offset = 8 * 1024;
    {
        std::fstream f;
        f.open(fn
                , std::ios::in
                | std::ios::out
                | std::ios::binary
                );
        f.seekp(offset);
        f.write(fn.c_str(), fn.size());
    }
    
    BOOST_CHECK_EQUAL(offset + fn.size(), boost::filesystem::file_size(fn));

    std::string s1 = "gavno", s2 = "МЁД";
    std::string s3, s4;
    {
        std::fstream f;
        f.open(fn
                , std::ios::in
                | std::ios::out
                | std::ios::binary
                | std::ios::trunc
                );
        io::dumps(f, s1);
        io::dumps(f, s2);
    }

    {
        std::fstream f;
        f.open(fn
                , std::ios::in
                | std::ios::out
                | std::ios::binary
                );
        io::loads(f, s3);
        io::loads(f, s4);
    }

    BOOST_CHECK_EQUAL(s1, s3);
    BOOST_CHECK_EQUAL(s2, s4);

    BOOST_CHECK_EQUAL("gavno", s3);
    BOOST_CHECK_EQUAL("МЁД", s4);
}

BOOST_AUTO_TEST_CASE(Page_serialization)
{
    std::stringstream ss;

    io::hint_list_t hl = { 
        io::hint_t::integral 
        ,io::hint_t::literal 
    };

    row_t r(hl);
    r.set_value(0, "1");
    r.set_value(1, "privet");

    page_t p;
    BOOST_CHECK_EQUAL(p.append(r), true);
    BOOST_CHECK_EQUAL(p.append(r), true);
    BOOST_CHECK_EQUAL(p.append(r), true);
    BOOST_CHECK_EQUAL(p.append(r), true);

    BOOST_CHECK_EQUAL(p.header_list.size(), 4);

    ss << p;

    BOOST_CHECK_EQUAL(ss.str().size(), page_t::len);


    std::string fn = "Page_serialization.dat";
    {
        std::fstream f;
        f.open(fn
                , std::ios::in
                | std::ios::out
                | std::ios::binary
                | std::ios::trunc
                );
        f << p;
    }

    BOOST_CHECK_EQUAL(boost::filesystem::file_size(fn), page_t::len);
}

BOOST_AUTO_TEST_CASE(Row_seiralization)
{
    std::stringstream ss;

    io::hint_list_t hl = { 
        io::hint_t::integral 
        ,io::hint_t::literal 
    };

    row_t r(hl), r2(hl);
    r.set_value(0, "1");
    r.set_value(1, "privet");

    ss << r;
    ss >> r2;

    BOOST_CHECK_EQUAL( r.get_value(0) , r2.get_value(0) );
    BOOST_CHECK_EQUAL( r.get_value(1) , r2.get_value(1) );
    BOOST_CHECK_EQUAL( "1" , r2.get_value(0) );
    BOOST_CHECK_EQUAL( "privet" , r2.get_value(1) );
}
