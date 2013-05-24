/*
 * GameServer.h
 *
 *  Created on: May 22, 2013
 *      Author: lgadawski
 */

#ifndef GAMESERVER_H_
#define GAMESERVER_H_

#include <Wt/WServer>
#include <Wt/WString>

class GameServer
{
private:
	Wt::WServer		&server_;

	Wt::WString		message_;

public:
//	GameServer() {}
	GameServer(Wt::WServer&);
};

#endif /* GAMESERVER_H_ */
