/*
 * GameServer.cpp
 *
 *  Created on: May 22, 2013
 *      Author: lgadawski
 */

#include "GameServer.h"

GameServer::GameServer(Wt::WServer &serv)
	: server_(serv)
{
}

void GameServer::setTextField(const Wt::WString &msg)
{
	postGameEvent(GEvent(msg));
}

void GameServer::postGameEvent(const GEvent &event)
{
	gameEventCallback(event);
}
