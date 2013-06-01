/**
* @file BoardWidget.cpp
*
* @date 29-05-2013
*
* @author Gadawski �ukasz, Sosnowski Jacek
*
* @brief Implementation of the BoardWidget class.
*
* @par Project
* This is a part of project realized on Warsaw University of Technology
* on ZPR lectures. Project was created to provide noughts and crosses game
* implementation. The application consists of server process wich afford 
* widget object with all game functionality to each client session.
*/

#include "BoardWidget.h"
#include "Field.h"

BoardWidget::BoardWidget(const Wt::WString& client, const Wt::WString& opponent,
		GameServer &serv, Side side, Wt::WContainerWidget *parent = 0)
	: clientName_(client),
	  opponent_(opponent),
	  server_(serv),
	  gameSide_(side),
	  Wt::WContainerWidget(parent)
{
	for(int a = 0; a < pow(BoardWidget::BOARD_SIZE, 2) ; ++a)
	{
		fields_.push_back(new Field(new Wt::WPushButton(" - ", this),
				BoardWidget::Side::None, a));

		fields_[a]->getButton()->clicked().connect(
				boost::bind(&BoardWidget::checkField, this, a));

		if((a+1) % BoardWidget::BOARD_SIZE == 0)	//cause 0 mod 0 == 0
		{
			this->addWidget(new Wt::WBreak());
		}
	}
}

void BoardWidget::checkField(int a)
{
	if (fields_[a]->getButton()->text() != Wt::WString("o") &&
			fields_[a]->getButton()->text() != Wt::WString("x"))
	{
		markField(a);
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

		this->clear();
		//Wt::WString m("You win");
		Wt::WString m("<span class='chat-info'><b>You</b> win!!!</span>");
		this->addWidget(new Wt::WText(m));
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
	Wt::WString ret = "<span class='chat-info'><b>";
	ret += this->opponent_;
	ret += "</b> ";

	switch(eve.type())
	{
	case PlayEvent::Move:
		int f;
		if ( (f = eve.getField()) != -1)
			this->markForeignMove(f);
		break;
	case PlayEvent::Win:
		this->clear();
		ret += "win.</span>";
		this->addWidget(new Wt::WText(ret));
		break;
	case PlayEvent::Pass:
		this->clear();
		ret += "pass.</span>";
		this->addWidget(new Wt::WText(ret));
		break;
	case PlayEvent::Draw:
		this->clear();
		ret += "draw.</span>";
		this->addWidget(new Wt::WText(ret));
		break;
	default:
		break;
	}
}

BoardWidget::~BoardWidget()
{
	// TODO Auto-generated destructor stub
}

