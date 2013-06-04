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

#include "../GameServer.h"
#include "../BoardWidget.h"
#include "../Field.h"

#include <boost/test/unit_test.hpp>

//class TestServer : public GameServer
//{
//private:
//	const char *argv = "--docroot . --http-address 0.0.0.0 --http-port 9090";
//public:
//	TestServer()
//		: GameServer(* new Wt::WServer(argv))
//	{
//	}
//};

BOOST_AUTO_TEST_SUITE(myTestSuite)

BOOST_AUTO_TEST_CASE(serverT)
{
//	BOOST_REQUIRE(true);
//
//	std::string argv = "--docroot . --http-address 0.0.0.0 --http-port 9090";
//	std::string aa = "";
//	Wt::WServer server(aa, argv);
//	GameServer ts(server);
//
//	Wt::WContainerWidget *wwc = new Wt::WContainerWidget();
//
//	Wt::WString client("client");
//	Wt::WString oponent("oponent");
//
//	std::cout << "!!!!!!!! jestem 3 !!!!!!!!!!!" << std::endl;
//
//	BoardWidget bw1(client, oponent, ts,
//			BoardWidget::Side::Crosses, wwc);
//
//	std::cout << "!!!!!!!! jestem 4 !!!!!!!!!!!" << std::endl;
//
//	BoardWidget *bw2 = new BoardWidget(Wt::WString("oponent"),
//			Wt::WString("client"), ts, BoardWidget::Side::Naughts, wwc);
//
//	bw1.markField(2);
//	BOOST_CHECK(bw1.getField(2)->getSide() == BoardWidget::Crosses);
}
BOOST_AUTO_TEST_SUITE_END()
