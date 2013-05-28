/*
 * BoardWidget.h
 *
 *  Created on: May 27, 2013
 *      Author: lgadawski
 */

#ifndef BOARDWIDGET_H_
#define BOARDWIDGET_H_

#include <boost/bind.hpp>

#include <Wt/WContainerWidget>
#include <Wt/WPushButton>

#include "GameServer.h"

class Field;

/**
 * represents new widget for game board
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
	BoardWidget(const Wt::WString &, GameServer &, Side , Wt::WContainerWidget *);
	/** destructor */
	virtual ~BoardWidget();

	/** Manage with playing event processing */
	void processPEvent(const PlayEvent& eve);

private:
	/** pointer to client */
	const Wt::WString& clientName_;
	/** server ref */
	GameServer &server_;
	/** naughts or crosses ?*/
	Side gameSide_;
	/** fields vector */
	std::vector<Field *> fields_;
	/** binded functor to button clicked causing button change
	 * depend on game side
	 */
	void markField(int);
	/** marking foreign move on board */
	void markForeignMove(int);
	/** check if game is finished */
	bool isFinished();
	void disableAllFields(bool flag);

};

#endif /* BOARDWIDGET_H_ */
