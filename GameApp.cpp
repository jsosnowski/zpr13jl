/*
 * GameApp.cpp
 *
 *  Created on: May 22, 2013
 *      Author: lgadawski
 */
#include <Wt/WApplication>
#include <Wt/WEnvironment>
#include <Wt/WContainerWidget>
#include <Wt/WServer>
#include <Wt/WString>
#include <Wt/WPushButton>

#include <boost/bind.hpp>
#include <boost/ref.hpp>

#include "GameServer.h"
#include "GameWidget.h"

class GameApp : public Wt::WApplication
{
private:
	GameServer &server_;

public:
	GameApp(const Wt::WEnvironment &env, GameServer &serv);

};

GameApp::GameApp(const Wt::WEnvironment &env, GameServer &serv)
	: Wt::WApplication(env), server_(serv)
{
	setTitle("hello bitches!");

	GameWidget *gameWidget = new GameWidget(this->server_, root());
}

Wt::WApplication *createApplication(const Wt::WEnvironment& env,
		GameServer& serv)
{
	return new GameApp(env, serv);
}

int main(int argc, char **argv)
{
	Wt::WServer server(argv[0]);
	GameServer gameServer(server);

	server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);

	server.addEntryPoint(Wt::Application,
						boost::bind(createApplication, _1,
									boost::ref(gameServer)));

	if(server.start())
	{
		int sig = Wt::WServer::waitForShutdown();
		server.stop();
	}
}


