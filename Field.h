/**
* @file Field.h
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

/*@}*/

#endif /* FIELD_H_ */
