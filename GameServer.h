// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2008 Emweb bvba, Heverlee, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef GAMESERVER_H_
#define GAMESERVER_H_

#include <boost/noncopyable.hpp>

#include <Wt/WSignal>
#include <Wt/WString>

namespace Wt {
  class WServer;
}

#include <set>
#include <map>
#include <boost/thread.hpp>

#include "GEvent.h"

/**
 * @addtogroup chatexample
 */
/*@{*/

typedef boost::function<void (const GEvent&)> GameEventCallback;

/*! \brief A simple chat server
 */
class GameServer : boost::noncopyable
{
public:
  /*
   * A reference to a client.
   */
  class Client
  {
  };

  /*! \brief Create a new chat server.
   */
  GameServer(Wt::WServer& server);

  /*! \brief Connects to the chat server.
   *
   * The passed callback method is posted to when a new chat event is
   * received.
   *
   * Returns whether the client has been connected (or false if the client
   * was already connected).
   */
  bool connect(Client *client, const GameEventCallback& handleEvent);

  bool initGame(Client *client, const Wt::WString &clientName, const Wt::WString &oponent);

  /**
  * @brief Client answers that want fight or that no want fight with oponent
  **/
  bool initGameAns(Client *client, const GEvent::Type ans, const Wt::WString &clientName, const Wt::WString &oponent);

  /*! \brief Disconnect from the chat server.
   *
   * Returns whether the client has been disconnected (or false if the client
   * was not connected).
   */  
  bool disconnect(Client *client);

  /*! \brief Try to login with given user name.
   *
   * Returns false if the login was not successful.
   */
  bool login(const Wt::WString& user);

  /*! \brief Logout from the server.
   */
  void logout(const Wt::WString& user);

  /*! \brief Changes the name.
   */
  bool changeName(const Wt::WString& user, const Wt::WString& newUser);

  /*! \brief Get a suggestion for a guest user name.
   */
  Wt::WString suggestGuest();

  /*! \brief Send a message on behalve of a user.
   */
  void sendMessage(const Wt::WString& user, const Wt::WString& message);
  void sendBut(const GEvent::Type wtyp, const Wt::WString& user); //moje

  /*! \brief Typedef for a collection of user names.
   */
  typedef std::set<Wt::WString> UserSet;

  /*! \brief Get the users currently logged in.
   */
  UserSet users();

private:
  struct ClientInfo {
    std::string sessionId;
    GameEventCallback eventCallback;
  };

  typedef std::map<Client *, ClientInfo> ClientMap;
  typedef std::map<Wt::WString, Client *> NameClientMap;
  typedef std::map<Client *, Client *> FightersMap;

  Wt::WServer& server_;
  boost::recursive_mutex mutex_;
  ClientMap clients_;
  UserSet users_;
  NameClientMap names_clients_;



  FightersMap fighters_;
  FightersMap prepareFighters_;


  void postGEvent(const GEvent& event);
  /**
  * @brief Send message direct into given session.
  *
  **/
  void postGEvent(const GEvent& event, const std::string &toSession);
};

/*@}*/

#endif // GAMESERVER_H_
