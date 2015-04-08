#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Table
#include <boost/test/unit_test.hpp>

#include "table.h"
#include "row.h"
#include "io.h"
#include "index.h"
#include "table_iter.h"

BOOST_AUTO_TEST_CASE(Table_serialization)
{
    std::stringstream ss;

    io::hint_list_t hl = { 
        io::hint_t::int_ 
        ,io::hint_t::str 
        ,io::hint_t::int_ 
        ,io::hint_t::int_ 
        ,io::hint_t::str 
        ,io::hint_t::str 
        ,io::hint_t::str 
        ,io::hint_t::str 
        ,io::hint_t::int_ 
        ,io::hint_t::toast 
    };

    table_t t1("test.tab", hl), t2("test.tab", io::hint_list_t());
    t1.last_page_index = 123;
    t1.last_free_page_index = 496;

    ss << t1;
    ss >> t2;

    BOOST_CHECK_EQUAL( t2.last_page_index , 123 );
    BOOST_CHECK_EQUAL( t2.last_free_page_index , 496 );

    BOOST_CHECK_EQUAL( t1.get_hint_list().size() , 10);
    BOOST_CHECK_EQUAL( t2.get_hint_list().size() , 10);
    BOOST_CHECK_EQUAL( t2.get_hint_list()[2] , io::hint_t::int_);
    BOOST_CHECK_EQUAL( t2.get_hint_list()[8] , io::hint_t::int_);
    BOOST_CHECK_EQUAL( t2.get_hint_list()[9] , io::hint_t::toast);
}

#include <boost/filesystem.hpp>

BOOST_AUTO_TEST_CASE(Table_file)
{
    std::string fn = "fileTest.tab";
    /*
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
    */

    std::string s1 = "abc", s2 = "CDE";
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

    BOOST_CHECK_EQUAL("abc", s3);
    BOOST_CHECK_EQUAL("CDE", s4);
}

/*
BOOST_AUTO_TEST_CASE(Table_iter)
{
    size_t size = 10000;
    std::string fn = "Table_iter.tab";
    io::hint_list_t hl = { io::hint_t::int_, io::hint_t::str };
    table_t t(fn, hl);

    auto it = t.begin();

    for(int i = 0; i < size; ++i) {
        auto row = t.make_row();
        row->set_value(0, std::to_string(i));
        row->set_value(1, std::to_string(i * 3));
        it.insert(row);
    }

    int i;
    for(i = 0, it = t.begin(); it != t.end(); ++it, ++i) {
        row_ptr row = *it;
        if(row) {
            std::cout << "row " << i 
                << " is [" 
                << (*it)->get_value(0)
                << ","
                << (*it)->get_value(1)
                << "]" 
                << std::endl;
        } else {
            std::cout << "empty row" << std::endl;
        }
    }

    BOOST_CHECK_EQUAL(i, size);
}
*/

BOOST_AUTO_TEST_CASE(Table_insertDumpAndLoad)
{
    size_t size = 100 
        //* 1000
    ;
    std::string fn = "Table_iter.tab";
    io::hint_list_t hl = { io::hint_t::int_, io::hint_t::str };

    {
        table_t t(fn, hl);

        auto it = t.begin();

        for(int i = 0; i < size; ++i) {

            if(i && !(i % 1000)) {
                std::cout << i << " rows inserted" << std::endl;
            }

            auto row = t.make_row();
            row->set_value(0, std::to_string(i));
            row->set_value(1, std::to_string(i * i));
            it.insert(row);

        }

    }

    {
        table_t t(fn);
        int i;
        table_iter_t it;
        for(i = 0, it = t.begin(); it != t.end(); ++it, ++i) {
            row_ptr row = *it;
            /*
            if(row) {
                std::cout << "row " << i 
                    << " is [" 
                    << (*it)->get_value(0)
                    << ","
                    << (*it)->get_value(1)
                    << "]" 
                    << std::endl;
            } else {
                std::cout << "empty row" << std::endl;
            }
            */
        }

        BOOST_CHECK_EQUAL(i, size);
    }
}

BOOST_AUTO_TEST_CASE(Table_insertAndDelete)
{
    size_t size = 3;
    std::string fn = "Table_iter.tab";
    io::hint_list_t hl = { io::hint_t::int_, io::hint_t::str };

    {
        table_t t(fn, hl);

        auto it = t.begin();

        for(int i = 0; i < size; ++i) {

            if(i && !(i % 1000)) {
                std::cout << i << " rows inserted" << std::endl;
            }

            auto row = t.make_row();
            row->set_value(0, std::to_string(i));
            row->set_value(1, std::to_string(i * i));
            it.insert(row);

        }

    }

    {
        table_t t(fn);
        auto it = t.begin();
        int i = 0;
        while(it != t.end()) {
            it.delete_();
            i++;
        }
        BOOST_CHECK_EQUAL(i, size);
    }
}

BOOST_AUTO_TEST_CASE(Table_insertAndUpdate)
{
    std::string fn = "Table_iter.tab";
    io::hint_list_t hl = { io::hint_t::int_, io::hint_t::str };

    {
        table_t t(fn, hl);
        auto it = t.begin();
        auto row = t.make_row();
        row->set_value(0, std::to_string(123));
        row->set_value(1, "abc");
        it.insert(row);
    }

    {
        table_t t(fn);
        auto it = t.begin();

        BOOST_CHECK(it != t.end());
        BOOST_CHECK(*it);

        row_ptr row = *it;
        row->set_value(1, "ABCDEFGH");
        it.update(row);
    }

    {
        table_t t(fn);
        auto it = t.begin();

        BOOST_CHECK(it != t.end());
        BOOST_CHECK(*it);

        row_ptr row = *it;
        BOOST_CHECK_EQUAL(row->get_value(0), std::to_string(123));
        BOOST_CHECK_EQUAL(row->get_value(1), "ABCDEFGH");
    }

    {
        table_t t(fn);
        auto it = t.begin();
        int i;
        for(i = 0; it != t.end(); ++it, ++i) {}
        BOOST_CHECK_EQUAL(i, 1);
    }
}

BOOST_AUTO_TEST_CASE(Page_serialization)
{

    io::hint_list_t hl = { 
        io::hint_t::int_ 
        ,io::hint_t::str 
    };

    row_t 
        r0(hl), r1(hl), r2(hl)
        ,r20(hl), r21(hl), r22(hl)
    ;
    r0.set_value(0, "1");
    r0.set_value(1, "privet");
    r1.set_value(0, "2");
    r1.set_value(1, "hello");
    r2.set_value(0, "3");
    r2.set_value(1, "hola");

    page_t p, p2;
    BOOST_CHECK_EQUAL(p.append(r0), true);
    BOOST_CHECK_EQUAL(p.append(r1), true);
    BOOST_CHECK_EQUAL(p.append(r2), true);
    BOOST_CHECK_EQUAL(p.header_list.size(), 3);

    /*
    std::stringstream ss;
    ss << p;
    BOOST_CHECK_EQUAL(ss.str().size(), page_t::len);
    */

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

    {
        std::fstream f;
        f.open(fn
                , std::ios::in
                | std::ios::out
                | std::ios::binary
                );

        f.seekg(0);
        f >> p2;

    BOOST_CHECK_EQUAL(p.prev,p2.prev);
    BOOST_CHECK_EQUAL(p.header_end,p2.header_end);
    BOOST_CHECK_EQUAL(p.row_begin,p2.row_begin);
    BOOST_CHECK_EQUAL(p.header_list.size(),p2.header_list.size());

        f.seekg(p2.header_list[0].offset);
        std::cerr << "reading row at " << f.tellg() << std::endl;
        f >> r20;

        f.seekg(p2.header_list[1].offset);
        std::cerr << "reading row at " << f.tellg() << std::endl;
        f >> r21;

        f.seekg(p2.header_list[2].offset);
        std::cerr << "reading row at " << f.tellg() << std::endl;
        f >> r22;


    BOOST_CHECK_EQUAL( r0.get_value(0) , r20.get_value(0) );
    BOOST_CHECK_EQUAL( r0.get_value(1) , r20.get_value(1) );
    BOOST_CHECK_EQUAL( r1.get_value(0) , r21.get_value(0) );
    BOOST_CHECK_EQUAL( r1.get_value(1) , r21.get_value(1) );
    BOOST_CHECK_EQUAL( r2.get_value(0) , r22.get_value(0) );
    BOOST_CHECK_EQUAL( r2.get_value(1) , r22.get_value(1) );
    }

}

BOOST_AUTO_TEST_CASE(Row_serialization)
{
    std::stringstream ss;

    io::hint_list_t hl = { 
        io::hint_t::int_ 
        ,io::hint_t::str 
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

BOOST_AUTO_TEST_CASE(Index_putGet)
{
    std::string 
        fn = "Index_putGet.idx"
        ,fn2 = "Index_putGet2.idx"
    ;
    index_t 
        index(fn, 1000, io::hint_t::int_)
        ,index2(fn2, 1000, io::hint_t::str)
    ;

    index.init();
    index2.init();

    size_t size = 1001;

    for(int i = 0; i < size; ++i) {
        index.put(std::to_string(i), i * 2, i * 3);
        index2.put(std::to_string(i), i * 2, i * 3);
    }

    for(int i = 0; i < size; ++i) {
        auto t = index.get(std::to_string(i));
        BOOST_CHECK(std::find(t.begin(), t.end(), std::make_tuple(i * 2, i * 3)) != t.end());

        auto t2 = index2.get(std::to_string(i));
        BOOST_CHECK(std::find(t2.begin(), t2.end(), std::make_tuple(i * 2, i * 3)) != t2.end());
    }

    std::cout 
        << "index size is "
        << boost::filesystem::file_size(fn)
        << " and index2 size is "
        << boost::filesystem::file_size(fn2)
        << std::endl
    ;
}

BOOST_AUTO_TEST_CASE(Index_andTable)
{
    std::string fn = "Index_andTable.tab";
    io::hint_list_t hl = { io::hint_t::int_, io::hint_t::str };
    {
        table_t t(fn, hl);
        t.create_index(0, 1000);
        t.create_index(1, 2000);
    }
    {
        table_t t(fn);
        BOOST_CHECK_EQUAL(t.get_index_count(), 2);
        BOOST_CHECK_EQUAL(t.get_index(0)->get_len(), 1000);
        BOOST_CHECK_EQUAL(t.get_index(1)->get_len(), 2000);
    }
}

BOOST_AUTO_TEST_CASE(Index_find)
{
    size_t size = 100 
        * 1000
    ;

    std::string fn = "Index_find.tab";
    io::hint_list_t hl = { io::hint_t::int_, io::hint_t::str };

    {
        table_t t(fn, hl);
        t.create_index(0, size * 3 / 2);
        t.create_index(1, size * 3 / 2);

        auto it = t.begin();

        for(int i = 0; i < size; ++i) {

            if(i && !(i % 1000)) {
                std::cout << i << " rows inserted" << std::endl;
            }

            auto row = t.make_row();
            row->set_value(0, std::to_string(i));
            row->set_value(1, std::to_string(i * i));
            it.insert(row);
        }

    }

    {
        table_t t(fn);

        for(int i = 0; i < size; ++i) {
            if(i && !(i % 1000)) {
                std::cout << i * 2 << " index lookups" << std::endl;
            }

            auto it = t.find(0, std::to_string(i));
            BOOST_CHECK(it != t.end());
            BOOST_CHECK(*it);
            row_ptr row = *it;
            BOOST_CHECK_EQUAL(row->get_value(0), std::to_string(i));
            BOOST_CHECK_EQUAL(row->get_value(1), std::to_string(i * i));

            it = t.find(1, std::to_string(i * i));
            BOOST_CHECK(it != t.end());
            BOOST_CHECK(*it);
            row = *it;
            BOOST_CHECK_EQUAL(row->get_value(0), std::to_string(i));
            BOOST_CHECK_EQUAL(row->get_value(1), std::to_string(i * i));
        }
    }

}

BOOST_AUTO_TEST_CASE(Index_miss) 
{
    std::string 
        fn = "Index_putGet.idx"
        ,fn2 = "Index_putGet2.idx"
    ;
    index_t 
        index(fn, 1000, io::hint_t::int_)
        ,index2(fn2, 1000, io::hint_t::str)
    ;

    index.init();
    index2.init();

    BOOST_CHECK( index.get("100").empty() );
    index.put("100", 1, 1);
    BOOST_CHECK( !index.get("100").empty() );
    index.put("100", 2, 2);
    index.put("100", 3, 3);

    index.del("100", 2, 2);
    BOOST_CHECK( !index.get("100").empty() );
    index.del("100", 3, 3);
    BOOST_CHECK( !index.get("100").empty() );
    index.del("100", 1, 1);
    BOOST_CHECK( index.get("100").empty() );
    /*
    BOOST_CHECK_EQUAL( index.get(std::to_string(1000)) != index_t::miss, true );
    BOOST_CHECK_EQUAL( index.get(std::to_string(1001)) == index_t::miss, true );
    */
}


#include <boost/functional/hash.hpp>
BOOST_AUTO_TEST_CASE(Index_hashing)
{
    boost::hash<std::string> string_hasher;
    boost::hash<int> int_hasher;
    
    std::vector<std::string> l = {
        "privet"
        ,"Hello"
        ,"hello"
        ,"PRIVET"
    };
    for(auto const &s : l) {
        std::cout << s << " hash is " << string_hasher(s) << std::endl;
    }

    auto h = string_hasher("test");
    std::cout << "hash size is " << sizeof(h) << std::endl;

    std::vector<int> l2 = {
        1, 2, 3, 4, 5, 6
    };
    for(auto const &s : l2) {
        std::cout << s << " hash is " << int_hasher(s) << std::endl;
    }

    auto h2 = int_hasher(123);
    std::cout << "hash size is " << sizeof(h2) << std::endl;
}
