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

bool GameServer::connect(Client *client,
			       const GameEventCallback& handleEvent)
{
  boost::recursive_mutex::scoped_lock lock(mutex_);

  if (clients_.count(client) == 0) {
    ClientInfo clientInfo;
  
    clientInfo.sessionId = WApplication::instance()->sessionId();
    clientInfo.eventCallback = handleEvent;

    clients_[client] = clientInfo;

    return true;
  } else
    return false;
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

GameServer::UserSet GameServer::users()
{
  boost::recursive_mutex::scoped_lock lock(mutex_);

  UserSet result = users_;

  return result;
}

