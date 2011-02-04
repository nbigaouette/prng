#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

// Example taken from git://github.com/jsankey/boost.test-examples.git

int add2(int i, int j)
{
    return i + j;
}

BOOST_AUTO_TEST_CASE(universeInOrder)
{
    BOOST_CHECK_MESSAGE(add2(2, 2) == 4, "The universe is *not* in order.");
}

