/**
* @file PlayEvent.cpp
*
* @date 29-05-2013
*
* @author Gadawski £ukasz, Sosnowski Jacek
*
* @brief Implementation of the PlayEvent class.
*
* @par Project
* This is a part of project realized on Warsaw University of Technology
* on ZPR lectures. Project was created to provide noughts and crosses game
* implementation. The application consists of server process wich afford 
* widget object with all game functionality to each client session.
*/

#include "PlayEvent.h"

bool PlayEvent::ifEndOfGame() const
{
	if (type_ == Win
			|| type_ == Draw
			|| type_ == Pass )
		return true;

	return false;
}
