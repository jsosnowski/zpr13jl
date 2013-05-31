/**
* @file GEvent.cpp
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

#include "GEvent.h"

const Wt::WString GEvent::formattedHTML(const Wt::WString& user,
				       Wt::TextFormat format) const
{
  switch (type_) {
  case Login:
    return Wt::WString::fromUTF8("<span class='chat-info'>")
      + Wt::WWebWidget::escapeText(user_) + " joined.</span>";
  case Logout:
    return Wt::WString::fromUTF8("<span class='chat-info'>")
      + ((user == user_) ?
	 Wt::WString::fromUTF8("You") :
	 Wt::WWebWidget::escapeText(user_))
      + " logged out.</span>";
  case Rename:
    return "<span class='chat-info'>"
      + ((user == data_ || user == user_) ?
	 "You are" :
	 (Wt::WWebWidget::escapeText(user_) + " is"))
      + " now known as " + Wt::WWebWidget::escapeText(data_) + ".</span>";
  case Message:{
    Wt::WString result;

    result = Wt::WString("<span class='")
      + ((user == user_) ?
	 "chat-self" :
	 "chat-user")
      + "'>" + Wt::WWebWidget::escapeText(user_) + ":</span>";

    Wt::WString msg
      = (format == Wt::XHTMLText ? message_ : Wt::WWebWidget::escapeText(message_));

    if (message_.toUTF8().find(user.toUTF8()) != std::string::npos)
      return result + "<span class='chat-highlight'>" + msg + "</span>";
    else
      return result + msg;
  }
  default:
    return "";
  }
}
