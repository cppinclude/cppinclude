#pragma once

#include <boost/test/unit_test.hpp>

#define TEST_GROUP_NAME( testGroup, testFixture ) \
	BOOST_FIXTURE_TEST_SUITE( testGroup, testFixture ) // NOLINT(fuchsia-statically-constructed-objects,cert-err58-cpp)

#define TEST_GROUP_END	\
	BOOST_AUTO_TEST_SUITE_END() // NOLINT(fuchsia-statically-constructed-objects,cert-err58-cpp)

#define TEST_CASE( caseName ) \
	BOOST_AUTO_TEST_CASE( caseName ) // NOLINT(fuchsia-statically-constructed-objects,cert-err58-cpp)

#define TEST_CHECK( conditional ) \
	BOOST_CHECK( conditional ) // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)

#define TEST_REQUIRE( conditional ) \
	BOOST_REQUIRE( conditional ) // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)

#define TEST_CHECK_EQUAL( L, R ) \
	BOOST_CHECK_EQUAL( L, R ) // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)

#define TEST_CHECK_EQUAL_COLLECTIONS( beginLeft, endLeft, beginRight, endRight ) \
	BOOST_CHECK_EQUAL_COLLECTIONS( beginLeft, endLeft, beginRight, endRight ) // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)

#define TEST_REQUIRE_EQUAL_COLLECTIONS( beginLeft, endLeft, beginRight, endRight ) \
	BOOST_REQUIRE_EQUAL_COLLECTIONS( beginLeft, endLeft, beginRight, endRight ) // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)

#define TEST_CHECK_THROW( S, E ) \
	BOOST_CHECK_THROW( S, E ) // NOLINT(cppcoreguidelines-pro-type-vararg,hicpp-vararg)
