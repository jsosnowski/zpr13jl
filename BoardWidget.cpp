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
	  server_(serv),
	  gameSide_(side),
	  Wt::WContainerWidget(parent)
{
	for(int a = 0; a < pow(BoardWidget::BOARD_SIZE, 2) ; ++a)
	{
		fields_.push_back(new Field(new Wt::WPushButton(" - ", this),
				BoardWidget::Side::None, a));

		fields_[a]->getButton()->clicked().connect(
				boost::bind(&BoardWidget::markField, this, a));

		if((a+1) % BoardWidget::BOARD_SIZE == 0)	//cause 0 mod 0 == 0
		{
			this->addWidget(new Wt::WBreak());
		}
	}
}

void BoardWidget::markField(int a)
{
	if(this->gameSide_ == BoardWidget::Crosses)
	{
		setFieldText(a, Wt::WString("x"));
		setFieldSide(a, BoardWidget::Crosses);
	}
	else
	{
		setFieldText(a, Wt::WString("o"));
		setFieldSide(a, BoardWidget::Naughts);
	}
	disableAllFields(true);


	// sending to oponent info about move (done action)
	server_.sendPlayEvent(clientName_, PlayEvent(PlayEvent::Move, a));

	if(isFinished())
	{
		std::cout << std::endl;
		std::cout << "!!!!!!!!!!!! KONIEC GRY !!!!!!!!!!!!";
		std::cout << std::endl;
		server_.sendPlayEvent(clientName_, PlayEvent(PlayEvent::Win));
	}
}

void BoardWidget::disableAllFields(bool flag)
{
	std::for_each(fields_.begin(), fields_.end(), [&](Field* u)
	{
		u->getButton()->setDisabled(flag);
	});
}

void BoardWidget::markForeignMove(int a)
{
	if(this->gameSide_ == BoardWidget::Crosses)
	{
		setFieldText(a, Wt::WString("o"));
	}
	else
	{
		setFieldText(a, Wt::WString("x"));
	}
	disableAllFields(false);
}

bool BoardWidget::isFinished()
{
	short count = 0, row = 0;
	// horizontal and vertical checks
	for (int i = 0; i < BoardWidget::BOARD_SIZE ; ++i)
	{
		count = 0, row = 0;
		BOOST_FOREACH(Field *f, fields_)
		{
			// check rows
			if (( row > 0) && (f->getSide() == gameSide_))
			{
				++row;
			}
			// check in column
			if ((f->getPositionOnBoard() % BoardWidget::BOARD_SIZE) == i)
			{
				row = 0;
				if (f->getSide() == gameSide_)
				{
					++count;
					++row;
				}
			}
			if ((count == BoardWidget::BOARD_SIZE)
					|| (row == BoardWidget::BOARD_SIZE))
			{
				return true;
			}
		}
//		std::for_each(fields_.begin(), fields_.end(), [&](Field* f)
//		{
//			// check in columns
//			if ((f->getPositionOnBoard() % BoardWidget::BOARD_SIZE) == i)
//			{
//				row = 0;
//				if (f->getSide() == gameSide_)
//				{
//					++count;
//					++row;
//				}
//			}
//			if (( row > 0) && (f->getSide() == gameSide_))
//			{
//				++row;
//			}
//			if ((count == BoardWidget::BOARD_SIZE)
//					|| (row == BoardWidget::BOARD_SIZE))
//			{
//				std::cout << std::endl;
//				std::cout << "!!!!!!!! koniec gry !!!!!!!!!!!" << std::endl;
//				return true;
//			}
//		});
	}
	count = 0;
	// check on diagonals
	for (int i = 0; i < pow(BoardWidget::BOARD_SIZE, 2) ; i = i + (BoardWidget::BOARD_SIZE + 1))
	{
		if (fields_[i]->getSide() == gameSide_)
		{
			++count;
		}
		else
		{
			break;
		}
		if (count == BoardWidget::BOARD_SIZE)
		{
			return true;
		}
	}
	count = 0;
	for (int i = BoardWidget::BOARD_SIZE - 1 ;
			i <= (pow(BoardWidget::BOARD_SIZE, 2) - BoardWidget::BOARD_SIZE) ;
				i = (i + BoardWidget::BOARD_SIZE - 1))
	{
		if (fields_[i]->getSide() == gameSide_)
		{
			++count;
		}
		else
		{
			break;
		}
		if (count == BoardWidget::BOARD_SIZE)
		{
			return true;
		}
	}
	return false;
}

void BoardWidget::setFieldText(int fieldNo, Wt::WString string)
{
	fields_[fieldNo]->getButton()->setText(string);
}

void BoardWidget::setFieldSide(int fieldNo, Side side)
{
//	fields_[fieldNo]->side_ = side;
	fields_[fieldNo]->setSide(side);
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

