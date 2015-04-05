#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Main
#include <boost/test/unit_test.hpp>

#include "table.h"

BOOST_AUTO_TEST_CASE(dumpsAndLoadsTest)
{
    table_t t1("test.tab"), t2("test.tab");

    t1.first_page = 123;
    t1.first_free_page = 496;

    t1.set_storage_hint(2, table_t::storage_hint_t::integral);
    t1.set_storage_hint(8, table_t::storage_hint_t::integral);
    t1.set_storage_hint(9, table_t::storage_hint_t::toast);

    t1.dumps();
    t2.loads();

    BOOST_CHECK_EQUAL( t2.first_page , 123 );
    BOOST_CHECK_EQUAL( t2.first_free_page , 496 );

    BOOST_CHECK_EQUAL( t1.storage_hint_list.size() , 10);
    BOOST_CHECK_EQUAL( t2.storage_hint_list.size() , 10);
    BOOST_CHECK_EQUAL( t2.get_storage_hint(2) , table_t::storage_hint_t::integral);
    BOOST_CHECK_EQUAL( t2.get_storage_hint(8) , table_t::storage_hint_t::integral);
    BOOST_CHECK_EQUAL( t2.get_storage_hint(9) , table_t::storage_hint_t::toast);
}

