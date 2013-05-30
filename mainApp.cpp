/**
* @file mainApp.h
*
* @date 29-05-2013
*
* @author Gadawski £ukasz, Sosnowski Jacek
*
* @brief Implementation of the main application function and ChatApplication class.
*
* @par Project
* This is a part of project realized on Warsaw University of Technology
* on ZPR lectures. Project was created to provide noughts and crosses game
* implementation. The application consists of server process wich afford 
* widget object with all game functionality to each client session.
*/

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WEnvironment>
#include <Wt/WPushButton>
#include <Wt/WServer>
#include <Wt/WText>

#include "GameServer.h"
#include "GameWidget.h"

using namespace Wt;

/**
 * @addtogroup zprgame
 */
/*@{*/

/*! \brief A chat demo application.
 */
class ChatApplication : public WApplication
{
public:
  /*! \brief Create a new instance.
   */
  ChatApplication(const WEnvironment& env, GameServer& server);

private:
  GameServer& server_;

  /*! \brief Add another chat client.
   */
  //void addChatWidget();
};

ChatApplication::ChatApplication(const WEnvironment& env,
				 GameServer& server)
  : WApplication(env),
    server_(server)
{
  setTitle("Wt Chat");
  useStyleSheet("chatapp.css");

  messageResourceBundle().use(appRoot() + "simplechat");

  root()->addWidget(new WText(WString::tr("introduction")));

  GameWidget *chatWidget = new GameWidget(server_, root());
  chatWidget->setStyleClass("chat");

  root()->addWidget(new WText(WString::tr("details")));

  //WPushButton *b = new WPushButton("I'm schizophrenic ...", root());
  //b->clicked().connect(b, &WPushButton::hide);
  //b->clicked().connect(this, &ChatApplication::addChatWidget);
}

//void ChatApplication::addChatWidget()
//{
//  SimpleChatWidget *chatWidget2 = new SimpleChatWidget(server_, root());
//  chatWidget2->setStyleClass("chat");
//}

///*! \brief A chat application widget.
// */
//class ChatWidget : public WApplication
//{
//public:
//  ChatWidget(const WEnvironment& env, SimpleChatServer& server);
//
//private:
//  JSignal<WString> login_;
//};
//
//ChatWidget::ChatWidget(const WEnvironment& env, SimpleChatServer& server)
//  : WApplication(env),
//    login_(this, "login")
//{
//  setCssTheme("");
//  useStyleSheet("chatwidget.css");
//  useStyleSheet("chatwidget_ie6.css", "lt IE 7");
//
//  const std::string *div = env.getParameter("div");
//  std::string defaultDiv = "div";
//  if (!div)
//   div = &defaultDiv;
//
//  if (div) {
//    setJavaScriptClass(*div);
//    PopupChatWidget *chatWidget = new PopupChatWidget(server, *div);
//    bindWidget(chatWidget, *div);
//
//    login_.connect(chatWidget, &PopupChatWidget::setName);
//
//    std::string chat = javaScriptClass();
//    doJavaScript("if (window." + chat + "User) "
//		 + chat + ".emit(" + chat + ", 'login', " + chat + "User);"
//		 + "document.body.appendChild(" + chatWidget->jsRef() + ");");
//  } else {
//    std::cerr << "Missing: parameter: 'div'" << std::endl;
//    quit();
//  }
//}

WApplication *createApplication(const WEnvironment& env,
				GameServer& server)
{
  return new ChatApplication(env, server);
}

//WApplication *createWidget(const WEnvironment& env, SimpleChatServer& server)
//{
//  return new ChatWidget(env, server);
//}

int main(int argc, char **argv)
{
  Wt::WServer server(argv[0]);
  GameServer chatServer(server);

  server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);

  /*
   * We add two entry points: one for the full-window application,
   * and one for a widget that can be integrated in another page.
   */
  server.addEntryPoint(Wt::Application,
		       boost::bind(createApplication, _1,
				   boost::ref(chatServer)));
  //server.addEntryPoint(Wt::WidgetSet,
		//       boost::bind(createWidget, _1,
		//		   boost::ref(chatServer)), "/chat.js");

  if (server.start()) {
    int sig = Wt::WServer::waitForShutdown();
    std::cerr << "Shutting down: (signal = " << sig << ")" << std::endl;
    server.stop();
  }
}

/*@}*/
