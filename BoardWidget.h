/*
 * BoardWidget.h
 *
 *  Created on: May 27, 2013
 *      Author: lgadawski
 */

#ifndef BOARDWIDGET_H_
#define BOARDWIDGET_H_

#include <Wt/WContainerWidget>
#include <Wt/WPushButton>

#include "GameServer.h"

/**
 * represents new widget for game board
 * */
class BoardWidget : public Wt::WContainerWidget
{
public:
	/** def. of game sides */
	enum Side { Naughts, Crosses };
	/** Constructor */
	BoardWidget(const GameServer::Client *, GameServer &, Side , Wt::WContainerWidget *);
	/** binded functor to button clicked causing button change
	 * depend on game side
	 */
	void markField();
	/** destructor */
	virtual ~BoardWidget();

private:
	/** pointer to client */
	const GameServer::Client *client_;
	/** server ref */
	GameServer &server_;
	/** naughts or crosses ?*/
	Side gameSide_;
	/** represents board */
	//button, 0 - non marked, 1 - o, 2 - x
	std::vector<Wt::WPushButton *> fields_;
};

#endif /* BOARDWIDGET_H_ */
