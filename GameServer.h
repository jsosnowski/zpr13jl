/*
 * GameServer.h
 *
 *  Created on: May 22, 2013
 *      Author: lgadawski
 */

#ifndef GAMESERVER_H_
#define GAMESERVER_H_

#include <boost/utility.hpp>
#include <boost/function.hpp>

#include <Wt/WServer>
#include <Wt/WString>

#include "GEvent.h"

//boost::noncopyable becouse we want to prohibit copy contruction
//and copy assigment
class GameServer : boost::noncopyable
{
private:
	Wt::WServer		&server_;

	Wt::WString		message_;

	boost::function<void (const GEvent&)> gameEventCallback;
public:
//	GameServer() {}
	GameServer(Wt::WServer&);

//	void connect();
	void setTextField(const Wt::WString&);
	void postGameEvent(const GEvent&);
};

#endif /* GAMESERVER_H_ */
