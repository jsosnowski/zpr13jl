/*
 * GEvent.h
 *
 *  Created on: May 26, 2013
 *      Author: lgadawski
 */

#ifndef GEVENT_H_
#define GEVENT_H_

#include <Wt/WSignal>
#include <Wt/WString>
#include <Wt/WServer>

using namespace Wt;

/*! \brief Encapsulate a chat event.
 */
class GEvent
{
public:
  /*! \brief Enumeration for the event type.
   */
  typedef enum xType { Login, Logout, Rename, Message,
	  GOffer, GAccept, GReject, But1, But2 }GEType;

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

  friend class GameServer;
};


#endif /* GEVENT_H_ */
