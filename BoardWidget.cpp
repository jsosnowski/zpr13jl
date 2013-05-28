/*
 * BoardWidget.cpp
 *
 *  Created on: May 27, 2013
 *      Author: lgadawski
 */

#include "BoardWidget.h"
#include "Field.h"

BoardWidget::BoardWidget(const Wt::WString& client,
		GameServer &serv, Side side, Wt::WContainerWidget *parent = 0)
	: clientName_(client),
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
	}
	else
	{
		fields_[a]->getButton()->setText("o");
	}

	// sending to oponent info about move (done action)
	server_.sendPlayEvent(clientName_, PlayEvent(PlayEvent::Move, a));

	disableAllFields(true);
	if(isFinished())
	{
		; //post finish game
	}
}

void BoardWidget::disableAllFields(bool flag)
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
	disableAllFields(false);
}

bool BoardWidget::isFinished()
{
	// bo kompilator krzyczal ze musi byc return
	return false;
}

void BoardWidget::processPEvent(const PlayEvent& eve)
{
	switch(eve.type())
	{
	case PlayEvent::Move:
		int f;
		if ( (f = eve.getField()) != -1)
			this->markForeignMove(f);
		break;
	case PlayEvent::Win:
		//robimy cos jesli nasz przeciwnik oznajmil ze wygral
		break;
	case PlayEvent::Pass:
		//costam robimy na wypadek jesli nasz przeciwnik sie poddal
		break;
	case PlayEvent::Draw:
		//robimy cos jesli nasz przeciwnik mowi ze jest remis
		break;
	default:
		//domyslnie robimy to:
		break;
	}
}

BoardWidget::~BoardWidget()
{
	// TODO Auto-generated destructor stub
}

