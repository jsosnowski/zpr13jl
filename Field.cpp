/*
 * Field.cpp
 *
 *  Created on: May 28, 2013
 *      Author: lgadawski
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

