/*
 * GameWidget.h
 *
 *  Created on: May 22, 2013
 *      Author: lgadawski
 */

#ifndef GAMEWIDGET_H_
#define GAMEWIDGET_H_

#include <Wt/WContainerWidget>
#include <Wt/WString>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WBreak>

#include "GameServer.h"

class GameWidget : public Wt::WContainerWidget
{
private:
	GameServer 			&server_;

	Wt::WString 		msg;
	Wt::WLineEdit	 	*msgLineEdit;
	Wt::WPushButton 	*button;
	Wt::WText			*msgToShow;

public:
//	GameWidget() {}

	GameWidget(GameServer &, Wt::WContainerWidget *parent = 0);
};

#endif /* GAMEWIDGET_H_ */
