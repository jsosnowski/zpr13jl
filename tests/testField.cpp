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

	Field fieldX(new Wt::WPushButton("aa"), BoardWidget::Crosses, 3);
	Field fieldO(new Wt::WPushButton("aa"), BoardWidget::Naughts, 4);

	BOOST_CHECK(fieldX.getPositionOnBoard() == 3);
	BOOST_CHECK(fieldX.getSide() == BoardWidget::Crosses);
	BOOST_CHECK(fieldX.getButton()->text() == "x");

	BOOST_CHECK(fieldO.getPositionOnBoard() == 3);
	BOOST_CHECK(fieldO.getSide() == BoardWidget::Naughts);
	BOOST_CHECK(fieldO.getButton()->text() == "o");
}
BOOST_AUTO_TEST_SUITE_END()



