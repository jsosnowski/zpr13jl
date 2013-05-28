/*
 * BoardWidget.cpp
 *
 *  Created on: May 27, 2013
 *      Author: lgadawski
 */

#include "BoardWidget.h"
#include "Field.h"

BoardWidget::BoardWidget(const GameServer::Client *client,
		GameServer &serv, Side side, Wt::WContainerWidget *parent = 0)
	: client_(client),
	  Wt::WContainerWidget(parent),
	  server_(serv),
	  gameSide_(side)
{
	for(int a = 0; a < 9; ++a)
	{
		fields_.push_back(new Field(new Wt::WPushButton("G", this),
				BoardWidget::Side::None, a));

		fields_[a]->getButton()->clicked().connect(
				boost::bind(&BoardWidget::markField, this, a));

		if((a+1) % 3 == 0)	//cause 0 mod 0 == 0
		{
			this->addWidget(new Wt::WBreak());
		}
	}
}

void BoardWidget::markField(int a)
{
	if(this->gameSide_ == BoardWidget::Crosses)
	{
		fields_[a]->getButton()->setText(Wt::WString("x"));
//		server_.postGEvent(GEvent(GE))
	}
	else
	{
		fields_[a]->getButton()->setText("o");
	}
	disableAllFields();
	if(isFinished())
	{
		; //post finish game
	}
}

void disableAllFields()
{

}

void BoardWidget::markForeignMove(int a)
{
	if(this->gameSide_ == BoardWidget::Crosses)
	{
		fields_[a]->getButton()->setText(Wt::WString("o"));
	}
	else
	{
		fields_[a]->getButton()->setText(Wt::WString("x"));
	}
}

bool BoardWidget::isFinished()
{

}

BoardWidget::~BoardWidget()
{
	// TODO Auto-generated destructor stub
}

