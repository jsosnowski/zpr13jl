/*
 * Field.h
 *
 *  Created on: May 28, 2013
 *      Author: lgadawski
 */

#ifndef FIELD_H_
#define FIELD_H_

#include <Wt/WPushButton>
#include "BoardWidget.h"

/**
 * @addtogroup zprgame
 */
/*@{*/

/**
 * class represents one field on board
 */
class Field {
private:
	Wt::WPushButton 	*button_;
	BoardWidget::Side	side_;
	short 				positionOnBoard;
public:
	Field(Wt::WPushButton *, BoardWidget::Side, int);

	Wt::WPushButton* 	getButton() 		 const  { return button_; }
	BoardWidget::Side 	getSide() 			 const	{ return side_; }
	short 				getPositionOnBoard() const 	{ return positionOnBoard; }

	void 				setSide(BoardWidget::Side side)  { this->side_ = side;}

	virtual ~Field();
};

/*@{*/

#endif /* FIELD_H_ */
