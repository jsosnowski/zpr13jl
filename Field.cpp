/**
* @file Field.cpp
*
* @date 29-05-2013
*
* @author Gadawski £ukasz, Sosnowski Jacek
*
* @brief Implementation of the Field class.
*
* @par Project
* This is a part of project realized on Warsaw University of Technology
* on ZPR lectures. Project was created to provide noughts and crosses game
* implementation. The application consists of server process wich afford 
* widget object with all game functionality to each client session.
*/

#include "Field.h"

Field::Field(Wt::WPushButton *b, const BoardWidget::Side s, const int p)
	: button_(b),
	  side_(s),
	  positionOnBoard(p)
{

}

Field::~Field() {
	// TODO Auto-generated destructor stub
}

