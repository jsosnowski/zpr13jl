/**
* @file BoardWidget.h
*
* @date 29-05-2013
*
* @author Gadawski £ukasz, Sosnowski Jacek
*
* @brief Implementation of the BoardWidget class.
*
* @par Project
* This is a part of project realized on Warsaw University of Technology
* on ZPR lectures. Project was created to provide noughts and crosses game
* implementation. The application consists of server process wich afford 
* widget object with all game functionality to each client session.
*/

#ifndef BOARDWIDGET_H_
#define BOARDWIDGET_H_

#include <boost/bind.hpp>
#include <boost/foreach.hpp>

#include <algorithm>
#include <cmath>

#include <Wt/WContainerWidget>
#include <Wt/WPushButton>

#include "GameServer.h"

class Field;

/**
 * @addtogroup zprgame
 */
/*@{*/

/**
 * @brief Represents new widget for game board
 * @details It produce visual board like:
 *  __________
 * |0 | 1 | 2 |
 * |3 | 4 | 5 |
 * |6 | 7 | 8 |
 *  ----------
 *
 * */
class BoardWidget : public Wt::WContainerWidget
{
public:
	/** def. of game sides */
	enum Side { Naughts, Crosses, None };
	/** Constructor */
	BoardWidget(const Wt::WString &,const Wt::WString&, GameServer &, Side , Wt::WContainerWidget *);
	/** destructor */
	virtual ~BoardWidget();

	/** Manage with playing event processing */
	void processPEvent(const PlayEvent& eve);
	/** def of board size */
	static const int BOARD_SIZE = 3;

private:
	/** pointer to client */
	const Wt::WString clientName_;
	/** oponent name */
	const Wt::WString opponent_;
	/** main contener - Message contener */
	Wt::WContainerWidget *mess_;
	/** server ref */
	GameServer &server_;
	/** naughts or crosses ?*/
	Side gameSide_;
	/** fields vector */
	std::vector<Field *> fields_;
	/** binded functor to button clicked, causing button change
	 * depend on game side
	 */
	void markField(int);
	/** marking foreign move on board */
	void markForeignMove(int);
	/** check if game is finished */
	bool isFinished();
	/** move causing field disable */
	void disableAllFields(bool flag);
	/** sets side */
	void setFieldSide(int fieldNo, Side);
	/** sets button text */
	void setFieldText(int fieldNo, Wt::WString);
};

/*@}*/

#endif /* BOARDWIDGET_H_ */

