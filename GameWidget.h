/**
* @file GameWidget.h
*
* @date 29-05-2013
*
* @author Gadawski �ukasz, Sosnowski Jacek
*
* @brief Implementation of the GameWidget class.
*
* @par Project
* This is a part of project realized on Warsaw University of Technology
* on ZPR lectures. Project was created to provide noughts and crosses game
* implementation. The application consists of server process wich afford 
* widget object with all game functionality to each client session.
*/

#ifndef GAMEWIDGET_H_
#define GAMEWIDGET_H_

#include <algorithm>

#include <Wt/WContainerWidget>
#include <Wt/WJavaScript>
#include <Wt/WSound>

#include "GameServer.h"
#include "BoardWidget.h"

namespace Wt {
  class WApplication;
  class WPushButton;
  class WText;
  class WLineEdit;
  class WTextArea;
}

class GEvent;

/**
 * \defgroup zprgame Noughts and Crosses
 */
/*@{*/

/** 
 * @brief A self-contained game widget.
 */
class GameWidget : public Wt::WContainerWidget,
			 public GameServer::Client
{
public:
  /**
   * @brief Create a game widget that will connect to the given server.
   */
  GameWidget(GameServer& server, Wt::WContainerWidget *parent = 0);

  /**
   * @brief Delete a game widget.
   */
  ~GameWidget();

  void connect();
  void disconnect();

  //void inviteClick();

  GameServer& server() { return server_; }

  /** @brief Returns number of users */
  int userCount() { return users_.size(); }

  /** @brief Returns this user name */
  const Wt::WString& userName() const { return user_; }

  /**
   * @brief Start a game for the given user.
   *
   * @return false if the user could not login.
   */
  bool startGame(const Wt::WString& user);
  BoardWidget * getBoardWidget() const {return boardWidget_;}

protected:
  virtual void createLayout(Wt::WWidget *messages, Wt::WWidget *userList,
			    /*Wt::WWidget *messageEdit,*/
			    Wt::WWidget *sendButton, Wt::WWidget *logoutButton);

  virtual void updateUsers();
  virtual void updateUsersOld();
  virtual void newMessage();

  virtual void render(Wt::WFlags<Wt::RenderFlag> flags);

protected:
  bool loggedIn() const;

private:
  typedef std::map<Wt::WString, bool> UserMap;
  UserMap users_;

  GameServer&     server_;
  bool                  loggedIn_;

  Wt::JSlot             clearInput_;

  Wt::WString           user_;
  Wt::WString           tmpOpponent;

  Wt::WLineEdit        *userNameEdit_;
  Wt::WText            *statusMsg_;

  Wt::WContainerWidget *messages_;
  Wt::WContainerWidget *messageEditArea_;
  //Wt::WTextArea        *messageEdit_;
  Wt::WPushButton      *sendButton_;
  Wt::WContainerWidget *userList_;
  Wt::WSelectionBox	   *userBox_;

  /** container created when invitation is sent */
  Wt::WContainerWidget *invitationContainer;

  BoardWidget 			*boardWidget_;

  Wt::WSound* messageReceived_;

  Wt::WPushButton *inviteButton;

  /** @brief Slot associate with Login button. Cause login action */
  void login();
  void send();
  void updateUserOld();


  void logout();
  /**
   * @brief Show a simple login screen.
   */
  void letLogin();
  /**
  * @brief Sends game invitation (offer) to client which is choosen by right widget panel (from user list)
  * @details Might not be done correctly for example when you want to invite yourself.
  **/
  void sendInvitation();
  /**
  * @brief Slot associate with Reject WPushButton
  * @details Send to opponent side that we don't want play with him.
  **/
  void rejectGame();
  /**
  * @brief Slot associate with Accept WPushButton.
  * @details Send to opponent side that we want to take challenge and play with him 
  **/
  void sendAccept();
  /**
  * @brief This method is call when processGEvent obtains event 
  *        which tells that game which we waiting for is accepted - game is start
  **/
  void beginGame();
  /**
  * @brief This method is called when processGEvent obtains event
  *        which tells that game offer made by this client is rejected by opponent side
  **/
  void showRejectedMsg(const Wt::WString &);
  /** @brief Clear invitation offer component */
  void clearInvitation();
  /** @brief Printing (drawing) invitation offer component on the screen */
  void drawInvitation(const GEvent&);

  /**
  * @brief Function callback called from another session to process given event
  * @param[in] event to process on client side
  */
  void processGEvent(const GEvent& event);
};

/*@}*/

#endif // GAMEWIDGET_H_
