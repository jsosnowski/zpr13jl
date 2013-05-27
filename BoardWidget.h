/*
 * BoardWidget.h
 *
 *  Created on: May 27, 2013
 *      Author: lgadawski
 */

#ifndef BOARDWIDGET_H_
#define BOARDWIDGET_H_

#include <Wt/WContainerWidget>

/**
 * represents new widget for game board
 * */
class BoardWidget : public Wt::WContainerWidget{
public:
	BoardWidget(const Wt::WContainerWidget *);
	virtual ~BoardWidget();
};

#endif /* BOARDWIDGET_H_ */
