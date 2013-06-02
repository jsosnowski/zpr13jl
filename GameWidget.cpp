/**
* @file GameWidget.cpp
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

#include <functional>

#include <boost/foreach.hpp>

#include "GameWidget.h"
#include "GameServer.h"
#include "BoardWidget.h"

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WEnvironment>
#include <Wt/WHBoxLayout>
#include <Wt/WVBoxLayout>
#include <Wt/WLabel>
#include <Wt/WLineEdit>
#include <Wt/WText>
#include <Wt/WTextArea>
#include <Wt/WPushButton>
#include <Wt/WCheckBox>
#include <Wt/WSelectionBox>

#include <iostream>

using namespace Wt;

GameWidget::GameWidget(GameServer& server,
				   Wt::WContainerWidget *parent)
  : WContainerWidget(parent),
    server_(server),
    loggedIn_(false),
    userList_(0),
    messageReceived_(0),
    userBox_(0),
    invitationContainer(0),
    boardWidget_(0)
{
  user_ = server_.suggestGuest();
  letLogin();
}

GameWidget::~GameWidget()
{
  logout();
  disconnect();
  delete messageReceived_;
}

void GameWidget::connect()
{
  if (server_.connect
      (this, user_, boost::bind(&GameWidget::processGEvent, this, _1)))
    Wt::WApplication::instance()->enableUpdates(true);
}

void GameWidget::disconnect()
{
  if (server_.disconnect(this))
    Wt::WApplication::instance()->enableUpdates(false);
}

void GameWidget::letLogin()
{
  disconnect();

  clear();

  WVBoxLayout *vLayout = new WVBoxLayout();
  setLayout(vLayout);

  WHBoxLayout *hLayout = new WHBoxLayout();
  vLayout->addLayout(hLayout, 0, AlignTop | AlignLeft);

  hLayout->addWidget(new WLabel("User name:"), 0, AlignMiddle);
  hLayout->addWidget(userNameEdit_ = new WLineEdit(user_), 0, AlignMiddle);
  userNameEdit_->setFocus();

  WPushButton *b = new WPushButton("Login");
  hLayout->addWidget(b, 0, AlignMiddle);

  b->clicked().connect(this, &GameWidget::login);
  userNameEdit_->enterPressed().connect(this, &GameWidget::login);

  vLayout->addWidget(statusMsg_ = new WText());
  statusMsg_->setTextFormat(PlainText);
}

void GameWidget::login()
{
  if (!loggedIn()) {
    WString name = userNameEdit_->text();

    if (!messageReceived_)
      messageReceived_ = new WSound("sounds/message_received.mp3");

    if (!startGame(name))
      statusMsg_->setText("Sorry, name '" + escapeText(name) +
			  "' is already taken.");
  }
}

void GameWidget::logout()
{
  if (loggedIn()) {
    loggedIn_ = false;
    server_.logout(user_);

    letLogin();
  }
}

void GameWidget::createLayout(WWidget *messages, WWidget *userList,
				    /*WWidget *messageEdit,*/
				    WWidget *sendButton, WWidget *logoutButton)
{
  /*
   * Create a vertical layout, which will hold 3 rows,
   * organized like this:
   *
   * WVBoxLayout
   * --------------------------------------------
   * | nested WHBoxLayout (vertical stretch=1)  |
   * |                              |           |
   * |  messages                    | userList  |
   * |   (horizontal stretch=1)     |           |
   * |                              |           |
   * --------------------------------------------
   * | message edit area                        |
   * --------------------------------------------
   * | WHBoxLayout                              |
   * | send | logout                            |
   * --------------------------------------------
   */
  WVBoxLayout *vLayout = new WVBoxLayout();

  // Create a horizontal layout for the messages | userslist.
  WHBoxLayout *hLayout = new WHBoxLayout();

  // Add widget to horizontal layout with stretch = 1
  hLayout->addWidget(messages, 1);
  messages->setStyleClass("chat-msgs");

    // Add another widget to horizontal layout with stretch = 0
  hLayout->addWidget(userList);
  userList->setStyleClass("chat-users");

  hLayout->setResizable(0, true);

  // Add nested layout to vertical layout with stretch = 1
  vLayout->addLayout(hLayout, 1);

  // Add widget to vertical layout with stretch = 0
  vLayout->addWidget(messageEditArea_);
  messageEditArea_->setStyleClass("foot-noedit");

  // Create a horizontal layout for the buttons.
  hLayout = new WHBoxLayout();

  // Add button to horizontal layout with stretch = 0
  hLayout->addWidget(sendButton);

  // Add button to horizontal layout with stretch = 0
  hLayout->addWidget(logoutButton);

  // Add nested layout to vertical layout with stretch = 0
  vLayout->addLayout(hLayout, 0, AlignLeft);

  WHBoxLayout *hButtomLayout = new WHBoxLayout();
  vLayout->addLayout(hButtomLayout);

  setLayout(vLayout);
}

bool GameWidget::loggedIn() const
{
  return loggedIn_;
}

void GameWidget::render(WFlags<RenderFlag> flags)
{
  if (flags & RenderFull) {
    if (loggedIn()) {
      /* Handle a page refresh correctly */
      //messageEdit_->setText(WString::Empty);
      doJavaScript("setTimeout(function() { "
		   + messages_->jsRef() + ".scrollTop += "
		   + messages_->jsRef() + ".scrollHeight;}, 0);");
    }
  }

  WContainerWidget::render(flags);
}

bool GameWidget::startGame(const WString& user)
{
  /*
   * When logging in, we pass our processGEvent method as the function that
   * is used to indicate a new game event for this user.
   */
  if (server_.login(user)) {
    loggedIn_ = true;
    user_ = user;
    connect();

    user_ = user;

    clear();
    userNameEdit_ = 0;

    messages_ = new WContainerWidget();
    userList_ = new WContainerWidget();
	messageEditArea_ = new WContainerWidget();
    //messageEdit_ = new WTextArea();
    //messageEdit_->setRows(2);
    //messageEdit_->setFocus();

    // Display scroll bars if contents overflows
    messages_->setOverflow(WContainerWidget::OverflowAuto);
    userList_->setOverflow(WContainerWidget::OverflowAuto);
	messageEditArea_->setOverflow(WContainerWidget::OverflowAuto);

    sendButton_ = new WPushButton("Send");
    WPushButton *logoutButton = new WPushButton("Logout");

    createLayout(messages_, userList_, /*messageEdit_,*/ sendButton_, logoutButton);

    /*
     * Connect event handlers:
     *  - click on button
     *  - enter in text area
     *
     * We will clear the input field using a small custom client-side
     * JavaScript invocation.
     */

    // Create a JavaScript 'slot' (JSlot). The JavaScript slot always takes
    // 2 arguments: the originator of the event (in our case the
    // button or text area), and the JavaScript event object.
    /*clearInput_.setJavaScript
      ("function(o, e) { setTimeout(function() {"
       "" + messageEdit_->jsRef() + ".value='';"
       "}, 0); }");*/

    // Bind the C++ and JavaScript event handlers.
    sendButton_->clicked().connect(this, &GameWidget::send);
    //messageEdit_->enterPressed().connect(this, &GameWidget::send);
    sendButton_->clicked().connect(clearInput_);
    //messageEdit_->enterPressed().connect(clearInput_);
    //sendButton_->clicked().connect(messageEdit_, &WLineEdit::setFocus);
    //messageEdit_->enterPressed().connect(messageEdit_, &WLineEdit::setFocus);

    // Prevent the enter from generating a new line, which is its default
    // action
    //messageEdit_->enterPressed().preventDefaultAction();

    logoutButton->clicked().connect(this, &GameWidget::logout);

    WText *msg = new WText
      ("<div><span class='chat-info'>You are joining as "
       + escapeText(user_) + ".</span></div>",
       messages_);
    msg->setStyleClass("chat-msg");

    if (!userList_->parent()) {
      delete userList_;
      userList_ = 0;
    }

    if (!sendButton_->parent()) {
      delete sendButton_;
      sendButton_ = 0;
    }

    if (!logoutButton->parent())
      delete logoutButton;

    updateUsers();
    
    return true;
  } else
    return false;
}

void GameWidget::send()
{
  //if (!messageEdit_->text().empty())
    //server_.sendMessage(user_, messageEdit_->text());
}

void GameWidget::updateUsersOld()
{
  if (userList_) {
    userList_->clear();

    GameServer::UserSet users = server_.users();

    UserMap oldUsers = users_;
    users_.clear();

    for (GameServer::UserSet::iterator i = users.begin();
	 i != users.end(); ++i) {
      WCheckBox *w = new WCheckBox(escapeText(*i), userList_);

      w->setInline(false);

      UserMap::const_iterator j = oldUsers.find(*i);
      if (j != oldUsers.end())
	w->setChecked(j->second);
      else
	w->setChecked(true);

      users_[*i] = w->isChecked();
      w->changed().connect(this, &GameWidget::updateUserOld);

      if (*i == user_)
	w->setStyleClass("chat-self");
    }
  }
}

void GameWidget::updateUsers()
{
	if (userList_)
	{
		// checks enablement state of userList before update
		// new list should have the same state
		const bool ifDisable = userList_->isDisabled();
		userList_->clear();
		userBox_ = new WSelectionBox(userList_);

		inviteButton = new WPushButton("Send invitation", userList_);
		inviteButton->setFocus();

		GameServer::UserSet users = server_.users();

		std::for_each(users.begin(), users.end(), [&](Wt::WString u)
		{
			userBox_->addItem(u);
		});

		/*userBox_->activated().connect(
				this, &GameWidget::inviteClick);*/
		inviteButton->clicked().connect(
				this, &GameWidget::sendInvitation);

		// set appropriate enablement state from previous widget
		if (ifDisable) {
			userBox_->disable();
			inviteButton->disable();
			userList_->disable();
		}


//		BOOST_FOREACH(Wt::WString i, users)
//		{
//			std::cout << "ilosc: " << users.size() << std::endl;
//			userBox_->addItem(i);
//		}
//
//		UserMap oldUsers = users_;
//		users_.clear();
//
//		for (GameServer::UserSet::iterator i = users.begin();
//		 i != users.end(); ++i)
//		{
//	      WCheckBox *w = new WCheckBox(escapeText(*i), userList_);
//	      w->setInline(false);
//
//	      UserMap::const_iterator j = oldUsers.find(*i);
//	      if (j != oldUsers.end())
//	    	 w->setChecked(j->second);
//	      else
//	    	  w->setChecked(true);
//
//	      users_[*i] = w->isChecked();
//	      users_[*i] = true;
//	      w->changed().connect(this, &GameWidget::updateUser);
//
//	      if (*i == user_)
//		w->setStyleClass("chat-self");
//		}
	}
}

void GameWidget::sendInvitation()
{
	if(user_ != userBox_->currentText())
	{
		bool b = server_.initGame(this, user_, userBox_->currentText());
		Wt::WString playerName = userBox_->currentText();
		if(b == false) //becouse user can be busy
		{
			Wt::WString msg = "User: <b>" + playerName + 
				"</b> that you are trying to invite is currently playing game";
			messages_->addWidget(new WText(msg));
			messages_->addWidget(new WBreak());
		}
		else
		{
			messages_->addWidget(new WText("Waiting for <b>" + 
				playerName + "</b> response"));
			messages_->addWidget(new WBreak());
			tmpOpponent = playerName;
		}
	}
	else
	{
		messages_->addWidget(new WText("You can't invite yourself!"));
		messages_->addWidget(new WBreak());
	}
}

void GameWidget::newMessage()
{ }

void GameWidget::updateUserOld()
{
  WCheckBox *b = dynamic_cast<WCheckBox *>(sender());
  users_[b->text()] = b->isChecked();
}

void GameWidget::drawInvitation(const GEvent &event)
{
	invitationContainer = new WContainerWidget(messages_);
	WText *w = new WText("User: <b>"+ event.user() +"</b> invited U",
			invitationContainer);
	invitationContainer->addWidget(new WBreak());
	tmpOpponent = event.user();
	WPushButton *accept = new WPushButton("Accept", invitationContainer);
	WPushButton *reject = new WPushButton("Reject", invitationContainer);

	accept->clicked().connect(this, &GameWidget::sendAccept);
	reject->clicked().connect(this, &GameWidget::rejectGame);
}

void GameWidget::beginGame()
{
	messages_->clear();
	messages_->addWidget(new WText("Game accepted, beginning game"));
	messages_->addWidget(new WBreak());
	//passed this, to know which client's board is
	boardWidget_ = new BoardWidget(user_, tmpOpponent, server_, BoardWidget::Naughts, messages_);
	// after game started, the userList is no necessary
	userList_->disable(); // it will be enable after game end
}

void GameWidget::sendAccept()
{
	clearInvitation();
	this->messages_->clear();
	messages_->addWidget(new WText("Enjoy play:"));
	if(server_.initGameAns(this,GEvent::GAccept, user_))
		boardWidget_ = new BoardWidget(user_, tmpOpponent, server_, BoardWidget::Crosses, messages_);
	else
		messages_->addWidget(new WText("Error occurs while game init. Please logout and login again"));
	// after game started, the userList is no necessary
	userList_->disable(); // it will be enable after game end
}

void GameWidget::rejectGame()
{
	clearInvitation();
	this->messages_->clear();
	tmpOpponent = WString::Empty; 
	server_.initGameAns(this,GEvent::GReject, user_);
}


void GameWidget::clearInvitation()
{
	invitationContainer->clear();
}

void GameWidget::showRejectedMsg(const WString &userName)
{
	messages_->addWidget(new WText("user: <b>"
			+ userName + "</b> rejected your invitation"));
	messages_->addWidget(new WBreak());
	tmpOpponent = WString::Empty;
}

void GameWidget::processGEvent(const GEvent& event)
{
  WApplication *app = WApplication::instance();

  /*
   * This is where the "server-push" happens. The game server posts to this
   * event from other sessions, see GameServer::postGEvent()
   */

  /*
   * Format and append the line to the conversation.
   *
   * This is also the step where the automatic XSS filtering will kick in:
   * - if another user tried to pass on some JavaScript, it is filtered away.
   * - if another user did not provide valid XHTML, the text is automatically
   *   interpreted as PlainText
   */

  /*
   * If it is not a plain message, also update the user list.
   */
  if (event.type() != GEvent::Message) {
    if (event.type() == GEvent::Rename && event.user() == user_)
      user_ = event.data();

    updateUsers();
  }

  newMessage();

  /*
   * Anything else doesn't matter if we are not logged in.
   */
  if (!loggedIn()) {
    app->triggerUpdate();
    return;
  }

  if(event.type() == GEvent::GOffer)
  {
	  std::cout << std::endl;
	  std::cout << " $$$$$$$$ ########### $$$$$$$$$$ " << std::endl;
	  drawInvitation(event);
  }

  if(event.type() == GEvent::GReject)
  {
	  showRejectedMsg(event.user());
  }

  if(event.type() == GEvent::GAccept)
  {
	  beginGame();
  }

  // if somebody is logout we must check if it is not our opponent
  if(event.type() == GEvent::Logout) {
	  if (event.user() == tmpOpponent) {
		  WString out("<span class='chat-info'>Your opponent: <b>");
		  out += tmpOpponent;
		  out += "</b> logged out so you win this game. </span>";

		  messages_->addWidget(new WText(out));
		  messages_->addWidget(new WBreak());
		  boardWidget_->setDisabled(true);
		  userBox_->setDisabled(false);
		  inviteButton->setDisabled(false);
		  userList_->setDisabled(false);
		  tmpOpponent = WString::Empty;
	  }
  }

  // if game is playing now - the PEvent events occurs
  if (event.type() == GEvent::PEvent && !event.user().empty()) //empty user means only unlock right panel
	  boardWidget_->processPEvent(event.getPEvent());

  // if it is game end we must unblock right panel
  if (event.type() == GEvent::PEvent && event.getPEvent().ifEndOfGame()) {
	  userBox_->setDisabled(false);
	  inviteButton->setDisabled(false);
	  userList_->setDisabled(false);
	  tmpOpponent = WString::Empty;
  }

  // display boolean choose if we should print some message on screen or add new user
  bool display = event.type() != GEvent::Message
    || !userList_
    || (users_.find(event.user()) != users_.end() && users_[event.user()]);

  if (display) {
	  WText *w = new WText();

    /*
     * If it fails, it is because the content wasn't valid XHTML
     */
    if (!w->setText(event.formattedHTML(user_, XHTMLText))) {
      w->setText(event.formattedHTML(user_, PlainText));
      w->setTextFormat(XHTMLText);
    }

    w->setInline(false);
    w->setStyleClass("chat-msg");

    /*
     * Leave no more than 100 messages in the back-log
     */
    if (messageEditArea_->count() > 100)
      delete messageEditArea_->children()[0];

	// if insert message is not empty, display it
	if (!w->text().empty())
		messageEditArea_->addWidget(w);

    /*
     * Little javascript trick to make sure we scroll along with new content
     */
    app->doJavaScript(messages_->jsRef() + ".scrollTop += "
		       + messages_->jsRef() + ".scrollHeight;");

	app->doJavaScript(messageEditArea_->jsRef() + ".scrollTop += "
	       + messages_->jsRef() + ".scrollHeight;");

    /* If this message belongs to another user, play a received sound */
    if (event.user() != user_ && messageReceived_)
      messageReceived_->play();
  
  }

  /*
   * This is the server push action: we propagate the updated UI to the client,
   * (when the event was triggered by another user)
   */
  app->triggerUpdate();
}
