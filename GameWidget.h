// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2008 Emweb bvba, Heverlee, Belgium.
 *
 * See the LICENSE file for terms of use.
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

  void sendBut1(); //moje
  void sendBut2();

  void logout();

  GameServer& server() { return server_; }

  int userCount() { return users_.size(); }

  const Wt::WString& userName() const { return user_; }

protected:
  virtual void createLayout(Wt::WWidget *messages, Wt::WWidget *userList,
			    Wt::WWidget *messageEdit,
			    Wt::WWidget *sendButton, Wt::WWidget *logoutButton,
				Wt::WWidget *but1, Wt::WWidget *but2);

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

  Wt::WPushButton *but1_;
  Wt::WPushButton *but2_;
  Wt::WPushButton *inviteButton;

  void login();
  void send();
  void updateUserOld();
  void updateUser();

  void rejectGame();
  void sendAccept();
  void beginGame();
  void clearInvitation();
  void showRejectedMsg(const WString &);
  void drawInvitation(const GEvent&);

  /* called from another session */
  void processGEvent(const GEvent& event);
};

/*@}*/

#endif // GAMEWIDGET_H_
