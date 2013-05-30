/**
* @file GameWidget.h
*
* @date 29-05-2013
*
* @author Gadawski £ukasz, Sosnowski Jacek
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
 * \defgroup chatexample Chat example
 */
/*@{*/

/*! \brief A self-contained chat widget.
 */
class GameWidget : public Wt::WContainerWidget,
			 public GameServer::Client
{
public:
  /*! \brief Create a chat widget that will connect to the given server.
   */
  GameWidget(GameServer& server, Wt::WContainerWidget *parent = 0);

  /*! \brief Delete a chat widget.
   */
  ~GameWidget();

  void connect();
  void disconnect();

//  void inviteClick(const Wt::WString&);
  void inviteClick();
  void sendInvitation();

  /*! \brief Show a simple login screen.
   */
  void letLogin();

  /*! \brief Start a chat for the given user.
   *
   * Returns false if the user could not login.
   */
  bool startChat(const Wt::WString& user);

  void logout();

  GameServer& server() { return server_; }

  int userCount() { return users_.size(); }

  const Wt::WString& userName() const { return user_; }

protected:
  virtual void createLayout(Wt::WWidget *messages, Wt::WWidget *userList,
			    Wt::WWidget *messageEdit,
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
  Wt::WTextArea        *messageEdit_;
  Wt::WPushButton      *sendButton_;
  Wt::WContainerWidget *userList_;
  Wt::WSelectionBox	   *userBox_;

  /** container created when invitation is sent */
  Wt::WContainerWidget *invitationContainer;

  BoardWidget 			*boardWidget_;

  Wt::WSound* messageReceived_;

  Wt::WPushButton *inviteButton;

  void login();
  void send();
  void updateUserOld();
  void updateUser();

  void rejectGame();
  void sendAccept();
  void beginGame();
  void showRejectedMsg(const Wt::WString &);
  void clearInvitation();
  void drawInvitation(const GEvent&);

  /* called from another session */
  void processGEvent(const GEvent& event);
};

/*@}*/

#endif // GAMEWIDGET_H_
