/*
 * Copyright (C) 2008 Emweb bvba, Heverlee, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "GameServer.h"

#include <Wt/WServer>

#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace Wt;

GameServer::GameServer(WServer& server)
  : server_(server)
{ }

bool GameServer::connect(Client *client, const Wt::WString &name,
			       const GameEventCallback& handleEvent)
{
  boost::recursive_mutex::scoped_lock lock(mutex_);

  if (clients_.count(client) == 0) {
    ClientInfo clientInfo;
  
    clientInfo.sessionId = WApplication::instance()->sessionId();
    clientInfo.eventCallback = handleEvent;

    clients_[client] = clientInfo;
    names_clients_[name] = client;

    return true;
  } else
    return false;
}

bool GameServer::initGame(Client *client,
		const Wt::WString &clientName, const Wt::WString &oponent)
{
	boost::recursive_mutex::scoped_lock lock(mutex_);

	//if client has already fight with somebody
	if (fighters_.count(client) != 0)
		return false;
	//if client wants to fight with somebody who doesn't exists
	if (names_clients_.count(oponent) != 1)
		return false;

	// find opnents identifier
	Client* op = names_clients_[oponent];
	if (fighters_.count(op) != 0)
		return false;
	
	// link client with oponents
	prepareFighters_[client] = op;
	prepareFighters_[op] = client;

	postGEvent(GEvent(GEvent::Type::GOffer, clientName),
			clients_[op].sessionId);

	return true;
}

bool GameServer::initGameAns(Client *client, const GEvent::Type ans,
		const Wt::WString &clientName)
//		, const Wt::WString &clientName, const Wt::WString &oponent)
{
	if (ans == GEvent::Type::GAccept)
		postGEvent(GEvent(GEvent::Type::GAccept, clientName),
				clients_[prepareFighters_[client]].sessionId);
	else
		postGEvent(GEvent(GEvent::Type::GReject, clientName),
				clients_[prepareFighters_[client]].sessionId);

	return true;
}

bool GameServer::disconnect(Client *client)
{
  boost::recursive_mutex::scoped_lock lock(mutex_);

  return clients_.erase(client) == 1;
}

bool GameServer::login(const WString& user)
{
  boost::recursive_mutex::scoped_lock lock(mutex_);
  
  if (users_.find(user) == users_.end()) {
    users_.insert(user);

    postGEvent(GEvent(GEvent::Login, user));

    return true;
  } else
    return false;
}

void GameServer::logout(const WString& user)
{
  boost::recursive_mutex::scoped_lock lock(mutex_);

  UserSet::iterator i = users_.find(user);

  if (i != users_.end()) {
    users_.erase(i);

    postGEvent(GEvent(GEvent::Logout, user));
  }
}

bool GameServer::changeName(const WString& user, const WString& newUser)
{
  if (user == newUser)
    return true;

  boost::recursive_mutex::scoped_lock lock(mutex_);
  
  UserSet::iterator i = users_.find(user);

  if (i != users_.end()) {
    if (users_.count(newUser) == 0) {
      users_.erase(i);
      users_.insert(newUser);

      postGEvent(GEvent(GEvent::Rename, user, newUser));

      return true;
    } else
      return false;
  } else
    return false;
}

WString GameServer::suggestGuest()
{
  boost::recursive_mutex::scoped_lock lock(mutex_);

  for (int i = 1;; ++i) {
    std::string s = "guest " + boost::lexical_cast<std::string>(i);
    WString ss = s;

    if (users_.find(ss) == users_.end())
      return ss;
  }
}

void GameServer::sendMessage(const WString& user, const WString& message)
{
  postGEvent(GEvent(user, message));
}

//moje
void GameServer::sendBut(const GEvent::Type mtyp, const WString& user) {
	postGEvent(GEvent(mtyp, user));
}

void GameServer::postGEvent(const GEvent& event)
{
  boost::recursive_mutex::scoped_lock lock(mutex_);

  WApplication *app = WApplication::instance();

  for (ClientMap::const_iterator i = clients_.begin(); i != clients_.end();
       ++i) {
    /*
     * If the user corresponds to the current application, we directly
     * call the call back method. This avoids an unnecessary delay for
     * the update to the user causing the event.
     *
     * For other uses, we post it to their session. By posting the
     * event, we avoid dead-lock scenarios, race conditions, and
     * delivering the event to a session that is just about to be
     * terminated.
     */
    if (app && app->sessionId() == i->second.sessionId)
      i->second.eventCallback(event);
    else
      server_.post(i->second.sessionId,
		   boost::bind(i->second.eventCallback, event));
  }
}

void GameServer::postGEvent(const GEvent& event,
		const std::string &toSession)
{
  boost::recursive_mutex::scoped_lock lock(mutex_);

  WApplication *app = WApplication::instance();

  for (ClientMap::const_iterator i = clients_.begin(); i != clients_.end();
       ++i) {

//    if (app && app->sessionId() == i->second.sessionId)
//      i->second.eventCallback(event);
//    else
		if (i->second.sessionId == toSession)
			server_.post(i->second.sessionId,
				boost::bind(i->second.eventCallback, event));
  }
}

GameServer::UserSet GameServer::users()
{
  boost::recursive_mutex::scoped_lock lock(mutex_);

  UserSet result = users_;

  return result;
}
