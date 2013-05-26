/*
 * GEvent.cpp
 *
 *  Created on: May 26, 2013
 *      Author: lgadawski
 */

#include "GEvent.h"

const WString GEvent::formattedHTML(const WString& user,
				       TextFormat format) const
{
  switch (type_) {
  case Login:
    return WString::fromUTF8("<span class='chat-info'>")
      + WWebWidget::escapeText(user_) + " joined.</span>";
  case Logout:
    return WString::fromUTF8("<span class='chat-info'>")
      + ((user == user_) ?
	 WString::fromUTF8("You") :
	 WWebWidget::escapeText(user_))
      + " logged out.</span>";
  case Rename:
    return "<span class='chat-info'>"
      + ((user == data_ || user == user_) ?
	 "You are" :
	 (WWebWidget::escapeText(user_) + " is"))
      + " now known as " + WWebWidget::escapeText(data_) + ".</span>";
  case Message:{
    WString result;

    result = WString("<span class='")
      + ((user == user_) ?
	 "chat-self" :
	 "chat-user")
      + "'>" + WWebWidget::escapeText(user_) + ":</span>";

    WString msg
      = (format == XHTMLText ? message_ : WWebWidget::escapeText(message_));

    if (message_.toUTF8().find(user.toUTF8()) != std::string::npos)
      return result + "<span class='chat-highlight'>" + msg + "</span>";
    else
      return result + msg;
  }
  default:
    return "";
  }
}
