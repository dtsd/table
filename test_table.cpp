#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Table
#include <boost/test/unit_test.hpp>

#include "table.h"
#include "io.h"

BOOST_AUTO_TEST_CASE(dumpsAndLoadsTest)
{
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

    t1.dumps();
    t2.loads();

    BOOST_CHECK_EQUAL( t2.last_page , 123 );
    BOOST_CHECK_EQUAL( t2.last_free_page , 496 );

    BOOST_CHECK_EQUAL( t1.get_hint_list().size() , 10);
    BOOST_CHECK_EQUAL( t2.get_hint_list().size() , 10);
    BOOST_CHECK_EQUAL( t2.get_hint_list()[2] , io::hint_t::integral);
    BOOST_CHECK_EQUAL( t2.get_hint_list()[8] , io::hint_t::integral);
    BOOST_CHECK_EQUAL( t2.get_hint_list()[9] , io::hint_t::toast);
}

BOOST_AUTO_TEST_CASE(streamTest)
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

BOOST_AUTO_TEST_CASE(fileTest)
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
