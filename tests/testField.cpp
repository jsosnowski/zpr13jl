/*
 * testServer.cpp
 *
 *  Created on: Jun 1, 2013
 *      Author: lgadawski
 */

#define BOOST_TEST_DYN_LINK
#ifdef STAND_ALONE
#	define BOOST_TEST_MODULE Main
#endif

#include <boost/test/unit_test.hpp>

#include "../Field.h"

BOOST_AUTO_TEST_SUITE(my_test_suite)

BOOST_AUTO_TEST_CASE(fieldTest)
{
	BOOST_REQUIRE(true);

	Field field(new Wt::WPushButton("aa"), BoardWidget::Crosses, 3);

	BOOST_CHECK(field.getPositionOnBoard() == 3);
	BOOST_CHECK(field.getSide() == BoardWidget::Crosses);
}
BOOST_AUTO_TEST_SUITE_END()



