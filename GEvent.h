/**
* @file GameEvent.h
*
* @date 29-05-2013
*
* @author Gadawski £ukasz, Sosnowski Jacek
*
* @brief Implementation of the GameEvent class.
*
* @par Project
* This is a part of project realized on Warsaw University of Technology
* on ZPR lectures. Project was created to provide noughts and crosses game
* implementation. The application consists of server process wich afford 
* widget object with all game functionality to each client session.
*/

#ifndef GEVENT_H_
#define GEVENT_H_

#include "PlayEvent.h"

#include <Wt/WSignal>
#include <Wt/WString>
#include <Wt/WServer>

/*! \brief Encapsulate a game event.
 */
class GEvent
{
public:
  /*! \brief Enumeration for the event type.
   */
  typedef enum GEType { Login, Logout, Rename, Message,
	  GOffer, GAccept, GReject, PEvent }GEType;

  /*! \brief Get the event type.
   */
  GEType type() const { return type_; }

  /*! \brief Get the user who caused the event.
   */
  const Wt::WString& user() const { return user_; }

  /*! \brief Get the message of the event.
   */
  const Wt::WString& message() const { return message_; }

  /*! \brief Get the extra data for this event.
   */
  const Wt::WString& data() const { return data_; }

  /*! \brief Get PlayEvent object
  */
  PlayEvent getPEvent() const {return pEvent_; }

  /*! \brief Get the message formatted as HTML, rendered for the given user.
   *
   * The \p format indicates how the message should be formatted.
   */
  const Wt::WString formattedHTML(const Wt::WString& user,
				  Wt::TextFormat format) const;

private:
  GEType type_;
  Wt::WString user_;
  Wt::WString data_;
  Wt::WString message_;
  PlayEvent pEvent_;

  /*
   * Both user and html will be formatted as html
   */
  GEvent(const Wt::WString& user, const Wt::WString& message)
    : type_(Message), user_(user), message_(message)
  { }

  GEvent(GEType type, const Wt::WString& user,
	    const Wt::WString& data = Wt::WString::Empty)
    : type_(type), user_(user), data_(data)
  { }

  GEvent(const Wt::WString& user,
	    PlayEvent pE)
		:type_(PEvent), user_(user), pEvent_(pE), 
		message_(Wt::WString("PlayEvent")), data_(Wt::WString::Empty)
  { }

  friend class GameServer;
};


#endif /* GEVENT_H_ */
