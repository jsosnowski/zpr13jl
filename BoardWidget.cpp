/*
 * BoardWidget.cpp
 *
 *  Created on: May 27, 2013
 *      Author: lgadawski
 */

#include "BoardWidget.h"

BoardWidget::BoardWidget(const GameServer::Client *client, GameServer &serv,
					Side side, Wt::WContainerWidget *parent = 0)
	: client_(client),
	  Wt::WContainerWidget(parent),
	  server_(serv),
	  gameSide_(side)
{
	for(int a = 0; a < 9; ++a)
	{
		fields_.push_back(new Wt::WPushButton("-", this));
		fields_[a]->clicked().connect(this,
				&BoardWidget::markField);
		if((a+1) % 3 == 0)	//cause 0 mod 0 == 0
		{
			this->addWidget(new Wt::WBreak());
		}
	}
}

void BoardWidget::markField()
{
	if(this->gameSide_ == BoardWidget::Crosses)
	{
		fields_[0]->setText(Wt::WString("x"));
	}
	else
	{
		fields_[0]->setText("o");
	}
}

BoardWidget::~BoardWidget()
{
	// TODO Auto-generated destructor stub
}

