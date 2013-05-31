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

/**
 * @brief Callback function type which should provide each client application object
 * @details This is function which will be post (call) by server when it post some event to client session.
 *          This function should have one argument type const GEvent& and return void.
 *          This functor type is used in server clients_ structure.
 **/
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

  /**
  * @brief This function sending game offer to opponent session.
  * @details Before sending it checks if required opponent is free 
  *          - not playing or waiting for any game.
  *          This function might be call from client session.
  * @param[in] client Client* it is a pointer to client session object.
  *            in our project it GameWidget
  * @param[in] clientName WString name of client which caused this event
  * @param[in] oponent WString name of oponent who we want to play with
  * @return bool true if post offer event done
  *                 false when oponent or even client is busy.
  **/
  bool initGame(Client *client, const Wt::WString &clientName,
		  const Wt::WString &oponent);

  /**
  * @brief Client answers that want fight or that no want fight with oponent
  * @details This is an answer to the offer which was send by initGame
  *          This function might be call from client session.
  * @param[in] client Client* it is a pointer to client session object.
  *            in our project it GameWidget
  * @param[in] ans GEType it should be only GAccept or GReject
  *            GAccept tells that previous game offer is accept and game is starting
  *            GReject tells that previous game offer is reject and game will not start
  * @param[in] clientName WString name of client which cause this event
  *            This is a client which gave a game offer before - now he give an answer.
  * @return bool true if game is correct started, false otherwise
  **/
  bool initGameAns(Client *client, const GEvent::GEType ans,
		  const Wt::WString &clientName);//, const Wt::WString &oponent);

  /**
  * @brief Sending pEvent message to the client's oponent.
  * @details Oponent session is known in the server.
  *          This might be call from client session during game playing.
  * @param[in] clientName WString name of client who caused this event
  * @param[in] pEvent PlayEvent which cointain all information about event occured
  *            PlayEvent object could be build by any client session object
  *            in this project it come from BoardWidget
  * @return bool false if given client is not playing any game
  *              true if event posting done correct
  **/
  bool sendPlayEvent(const Wt::WString& clientName, const PlayEvent pEvent);

  /**
  * @brief Disconnect from the chat server.
   *
   * Returns whether the client has been disconnected (or false if the client
   * was not connected).
   * @param[in] client is a pointer to client which was disconnect 
   */  
  bool disconnect(Client *client);

  /**
   * @brief Try to login with given user name.
   * @details This method checks if user name is free at this time.
   *
   * @param[in] user WString user name proposition.
   * @return bool false if the login was not successful.
   */
  bool login(const Wt::WString& user);

  /**
  * @brief Logout from the server.
  *
  * @param[in] user WString name of user who wants to logout
   */
  void logout(const Wt::WString& user);

  /**
  * @brief Delete all structures which is associated with current playing game by the user.
  * @details This means also that opponent to this user lost his oponent and all game.
  *          This action is call by logout method.
  * @param[in] user WString user name whose structur should be removed.
  **/
  void remGameStructures(const Wt::WString& user);

  /**
  * @brief Changes the name.
   */
  bool changeName(const Wt::WString& user, const Wt::WString& newUser);

  /**
  * @brief Get a suggestion for a guest user name.
  * @return WString suggested name for current guest.
   */
  Wt::WString suggestGuest();

  /**
  * @brief Send a message on behalve of a user.
   */
  void sendMessage(const Wt::WString& user, const Wt::WString& message);

  /**
  * @brief Typedef for a collection of user names.
   */
  typedef std::set<Wt::WString> UserSet;

  /**
  * @brief Get all users currently logged in.
   */
  UserSet users();

private:
	/**
	* @brief A structure which associate the client sessionId with his state and his event process function.
	**/
  struct ClientInfo {
    std::string sessionId;
	bool busy;
    GameEventCallback eventCallback;
  };

  /** @brief Association between client and his ClientInfo structure. */
  typedef std::map<Client *, ClientInfo> ClientMap;
  /** @brief Association between client name and his application object pointer */
  typedef std::map<Wt::WString, Client *> NameClientMap;
  /** @brief Association between two fighting clients application objects - currently playing */
  typedef std::map<Client *, Client *> FightersMap;

  Wt::WServer& server_;
  boost::recursive_mutex mutex_;
  ClientMap clients_;
  UserSet users_;
  /** @brief Association between client name and his application object pointer */
  NameClientMap names_clients_;
  /** @brief Contain all currently playing game users associated with his opponents. 
  * @details Each user has one opponent in this map and his opponent has also him as opponent in this map.
  */
  FightersMap fighters_;
  /** @brief Contain all users currently waiting for game start - waiting for another side answer (accept or reject) */
  FightersMap prepareFighters_;

  /**
  * @brief Sending event to all client sessions.
  * @details For example it could be event about login or logout
  * @param[in] event which contain all information about occured situation.
  **/
  void postGEvent(const GEvent& event);

  /**
  * @brief Send message direct into given session.
  *
  * @param[in] event which contain all information about occured situation.
  * @param[in] toSession the session identifier charged from clients_ structure
  *            this event will be send exacly into toSession session.
  **/
  void postGEvent(const GEvent& event, const std::string& toSession);
};

/*@}*/

#endif // GAMESERVER_H_
