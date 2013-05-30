/**
* @file GameServer.h
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
#include <string.h>
#include <boost/thread.hpp>

#include "GEvent.h"

typedef boost::function<void (const GEvent&)> GameEventCallback;

/**
 * @addtogroup zprgame
 */
/*@{*/

/** 
 * @brief A noughts and crosses game server.
 * @details Provide client session objects, views and communication between clients.
 **/
class GameServer : boost::noncopyable
{
public:
  /*
   * @brief Class which provide reference to client.
   * @details This helps server with recognizing clients process
   *          By using pointers to this objects as a base of Widget (GameWidget)
   **/
  class Client
  {
  };

  /** 
  * @brief Create a new game server.
  * @param[in] server Wt::WServer respond to lower connection service.
  **/
  GameServer(Wt::WServer& server);

  /**
   * @brief Connects to the game server.
   *
   * The passed callback method is posted to when a new chat event is
   * received.
   *
   * Returns whether the client has been connected (or false if the client
   * was already connected).
   */
  bool connect(Client *client, const Wt::WString &,
		  const GameEventCallback& handleEvent);

  bool initGame(Client *client, const Wt::WString &clientName,
		  const Wt::WString &oponent);

  /**
  * @brief Client answers that want fight or that no want fight with oponent
  **/
  bool initGameAns(Client *client, const GEvent::GEType ans,
		  const Wt::WString &clientName);//, const Wt::WString &oponent);

  /**
  * @brief Sending pEvent message to the client's oponent.
  * oponent session is known in the server
  **/
  bool sendPlayEvent(const Wt::WString& clientName, const PlayEvent pEvent);

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

  void remGameStructures(const Wt::WString& user);

  /*! \brief Changes the name.
   */
  bool changeName(const Wt::WString& user, const Wt::WString& newUser);

  /*! \brief Get a suggestion for a guest user name.
   */
  Wt::WString suggestGuest();

  /*! \brief Send a message on behalve of a user.
   */
  void sendMessage(const Wt::WString& user, const Wt::WString& message);

  /*! \brief Typedef for a collection of user names.
   */
  typedef std::set<Wt::WString> UserSet;

  /*! \brief Get the users currently logged in.
   */
  UserSet users();

private:
  struct ClientInfo {
    std::string sessionId;
	bool busy;
    GameEventCallback eventCallback;
  };

  typedef std::map<Client *, ClientInfo> ClientMap;
  typedef std::map<Wt::WString, Client *> NameClientMap;
  typedef std::map<Client *, Client *> FightersMap;

  Wt::WServer& server_;
  boost::recursive_mutex mutex_;
  ClientMap clients_;
  UserSet users_;
  /** */
  NameClientMap names_clients_;
  /** */
  FightersMap fighters_;
  /** */
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
