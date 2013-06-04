/**
* @file GEvent.h
*
* @date 29-05-2013
*
* @author Gadawski �ukasz, Sosnowski Jacek
*
* @brief Implementation of the GEvent class.
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

/**
 * @addtogroup zprgame
 */
/*@{*/

/**
 * @brief Encapsulate a game or communication event.
 */
class GEvent
{
public:
  /** 
  * @brief Enumeration for the event type.
   */
  enum GEType { 
	  Login,  /**< Login by user() */
	  Logout, /**< Logout by user() */
	  Rename, /**< Rename user name */
	  Message,/**< Plain Message */
	  GOffer, /**< Game offer from user() */
	  GAccept,/**< Game offer accept by user() */
	  GReject,/**< Game offer reject by user() */
	  PEvent  /**< Play Event store in event */
  };

  /**
  * @brief Get the event type.
  * @return GEType - type of event which is store in this object.
   */
  GEType type() const { return type_; }

  /**
  * @brief Get the user who caused the event.
  * @return Wt::WString which is the name of user from come event.
   */
  const Wt::WString& user() const { return user_; }

  /**
  * @brief Get the message of the event.
  * @return Wt::WString of plain message text.
   */
  const Wt::WString& message() const { return message_; }

  /**
  * @brief Get the extra data for this event.
   */
  const Wt::WString& data() const { return data_; }

  /**
  * @brief Get PlayEvent object
  * @details It is important only if type() == PEvent
  * @return PlayEvent object which contains event associated only with game.
  */
  PlayEvent getPEvent() const {return pEvent_; }

  /**
  * @brief Get the message formatted as HTML, rendered for the given user.
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

  /**
  * @brief Constructs event object which store GEvent situations.
  * @details This include every communication situations.
  * @param[in] type GEType which telling what situation happen.
  * @param[in] user is the user who caused this event
  * @param[in] data is an extra data associate with this event
  *            usually and by default it is empty WString.
  **/
  GEvent(GEType type, const Wt::WString& user,
	    const Wt::WString& data = Wt::WString::Empty)
    : type_(type), user_(user), data_(data)
  { }

  /**
  * @brief Construct event object which store PlayEvent situations
  * @param[in] user is name of user which cause this event sytuation.
  * @param[in] pE is an event which occurs during game playing
  **/
  GEvent(const Wt::WString& user,
	    PlayEvent pE)
		:type_(PEvent), user_(user), data_(Wt::WString::Empty),
		 message_(Wt::WString("PlayEvent")), pEvent_(pE)
  { }

  /**
  * @brief Friendship because only GameServer should have priviliges to create GEvent objects.
  * @details This restriction is used becouse only server responds to communication between clients sessions
  **/
  friend class GameServer;
};

/*@}*/

#endif /* GEVENT_H_ */
