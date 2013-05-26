/*
 * GEvent.cpp
 *
 *  Created on: May 26, 2013
 *      Author: lgadawski
 */

#include "GEvent.h"

GEvent::GEvent(const Wt::WString &msg)
		: type(Message)
	{ }

GEvent::Type GEvent::getType() const
{
	return this->type;
}
