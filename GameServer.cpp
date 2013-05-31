/**
* @file GameServer.cpp
*
* @date 29-05-2013
*
* @author Gadawski £ukasz, Sosnowski Jacek
*
* @brief Implementation of the GameServer class.
*
* @par Project
* This is a part of project realized on Warsaw University of Technology
* on ZPR lectures. Project was created to provide noughts and crosses game
* implementation. The application consists of server process wich afford 
* widget object with all game functionality to each client session.
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
	clientInfo.busy = false;
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

	//check if client is busy at this time - meybe fighting with somebody
	if (clients_[client].busy == true)
		return false;
	//if client wants to fight with somebody who doesn't exists
	if (names_clients_.count(oponent) != 1)
		return false;

	// find oponents identifier
	Client* op = names_clients_[oponent];
	// oponent should be free to take challange
	if (clients_[op].busy == true)
		return false;

	// link client with oponents
	prepareFighters_[client] = op;
	prepareFighters_[op] = client;

	// now they are both busy
	clients_[client].busy = true;
	clients_[op].busy = true;

	postGEvent(GEvent(GEvent::GEType::GOffer, clientName), clients_[op].sessionId);

	return true;
}

bool GameServer::initGameAns(Client *client, const GEvent::GEType ans,
		const Wt::WString &clientName)
//		, const Wt::WString &clientName, const Wt::WString &oponent)
{
	boost::recursive_mutex::scoped_lock lock(mutex_);

	// error if client doesn't play with anybody
	if (prepareFighters_.count(client) != 1) {
		return false;
	}

	Client *op = prepareFighters_[client];

	// when oponent is dead
	if (prepareFighters_.count(op) != 1)
		return false;

	if (ans == GEvent::GEType::GAccept) {
		postGEvent(GEvent(GEvent::GEType::GAccept, clientName),
				clients_[prepareFighters_[client]].sessionId);

		fighters_[client] = op;
		fighters_[op] = client;
//		postGEvent(GEvent(GEvent::GEType::GAccept, clientName), clients_[client].sessionId);
	}
	else {
		// in case of reject fighting all of them are free
		postGEvent(GEvent(GEvent::GEType::GReject, clientName),
				clients_[prepareFighters_[client]].sessionId);

		clients_[client].busy = false;
		clients_[op].busy = false;
//		postGEvent(GEvent(GEvent::GEType::GReject, clientName), clients_[client].sessionId);
	}

	prepareFighters_.erase(client);
	prepareFighters_.erase(op);

	return true;
}

bool GameServer::sendPlayEvent(const Wt::WString& clientName, const PlayEvent pEvent)
{
	boost::recursive_mutex::scoped_lock lock(mutex_);

	if (names_clients_.count(clientName) == 0)
		return false;
	Client *client = names_clients_[clientName];

	if (fighters_.count(client) != 0) {
		Client *oponent = fighters_[client];
		postGEvent(GEvent(clientName, pEvent), clients_[oponent].sessionId);

		if (pEvent.ifEndOfGame()) {
			postGEvent(GEvent("", pEvent), clients_[client].sessionId);
			remGameStructures(clientName);
		}

		return true;
	}

	return false;
}

bool GameServer::disconnect(Client *client)
{
  boost::recursive_mutex::scoped_lock lock(mutex_);

  // delete client from game structures
  fighters_.erase(client);
  prepareFighters_.erase(client);

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

void GameServer::logout(const Wt::WString& user)
{
  boost::recursive_mutex::scoped_lock lock(mutex_);

  UserSet::iterator i = users_.find(user);

  if (i != users_.end()) {
    users_.erase(i);

	  //if this user has game - delete all game structure
	remGameStructures(user);

    postGEvent(GEvent(GEvent::Logout, user));
  }

  //delete association between user name and user client object
  names_clients_.erase(user);
}

void GameServer::remGameStructures(const WString& user) 
{
	// checks if given name is assigned to a client object
	if (names_clients_.count(user) == 0)
		return;

	Client *client = names_clients_[user];

	// if client have some game or waiting for play:
	if( clients_[client].busy == true ) {
		if (prepareFighters_.count(client)) { //if waiting for play
			// now oponent is not busy
			clients_[prepareFighters_[client]].busy = false;
			clients_[client].busy = false; // client also isn't busy
			//erase info about client oponent
			prepareFighters_.erase(prepareFighters_[client]);
			prepareFighters_.erase(client);
		}
		if (fighters_.count(client)) { //if play game
			// now oponent is not busy
			clients_[fighters_[client]].busy = false;
			clients_[client].busy = false; // client also isn't busy
			//erase info about client oponent
			fighters_.erase(fighters_[client]);
			fighters_.erase(client);
		}
	}

}

bool GameServer::changeName(const WString& user, const WString& newUser)
{
  if (user == newUser)
    return true;

  boost::recursive_mutex::scoped_lock lock(mutex_);

  //check if association beetwen user name and user client object exist and change it
  if (names_clients_.count(user) ) {
	  if (names_clients_.count(newUser) == 0) {
		  names_clients_[newUser] = names_clients_[user];
		  names_clients_.erase(user);
	  }
  }

  
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

  //TODO app don't used
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

