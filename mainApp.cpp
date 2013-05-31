/**
* @file mainApp.cpp
*
* @date 29-05-2013
*
* @author Gadawski £ukasz, Sosnowski Jacek
*
* @brief Implementation of the main application function and mainApplication class.
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

/**
 * @brief A game application widget supply to client browswer.
 * @details There is one this type object per one client session.
 */
class GameApplication : public WApplication
{
public:
  /**
  * @brief Create a new instance of application object.
  * @details This create first widget and include GameWidget
  *          which provide all visual structure (such clients list etc)
  * @param[in] env - Environment variable
  * @param[in] server - the server object.
   */
  GameApplication(const WEnvironment& env, GameServer& server);

private:
  GameServer& server_;

};

GameApplication::GameApplication(const WEnvironment& env,
				 GameServer& server)
  : WApplication(env),
    server_(server)
{
  setTitle("Noughts and Crosses Game");
  useStyleSheet("app.css");

  messageResourceBundle().use(appRoot() + "gameIntro");

  root()->addWidget(new WText(WString::tr("introduction")));

  // creating main game widget
  GameWidget *gameWidget = new GameWidget(server_, root());
  gameWidget->setStyleClass("chat");

  root()->addWidget(new WText(WString::tr("details")));

}

/**
 * @brief Callback function require by Wt library to provide an widget object
 * @details In our case this create all visual boards for client browswer.
 * @param[in] env - Environment variable
 * @param[in] server - the server object.
 * @return WApplication pointer to new application object which is associated with
 *         client session.
**/
WApplication *createApplication(const WEnvironment& env,
				GameServer& server)
{
  return new GameApplication(env, server);
}

/**
 * @brief The main application function.
 * @details This create server process and entry points for clients sessions.
**/
int main(int argc, char **argv)
{
  Wt::WServer server(argv[0]);
  GameServer gameServer(server);

  server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);

  /*
   * We add two entry points: one for the full-window application,
   * and one for a widget that can be integrated in another page.
   */
  server.addEntryPoint(Wt::Application,
		       boost::bind(createApplication, _1,
				   boost::ref(gameServer)));

  if (server.start()) {
    int sig = Wt::WServer::waitForShutdown();
    std::cerr << "Shutting down: (signal = " << sig << ")" << std::endl;
    server.stop();
  }
}

/*@}*/
