/*
 * GameWidget.cpp
 *
 *  Created on: May 22, 2013
 *      Author: lgadawski
 */

#include "GameWidget.h"


GameWidget::GameWidget(GameServer &serv, Wt::WContainerWidget *parent)
	: server_(serv), Wt::WContainerWidget(parent)
{
	this->msgLineEdit = new Wt::WLineEdit(msg);
	parent->addWidget(msgLineEdit);

	this->button = new Wt::WPushButton("submit");
	parent->addWidget(button);

	this->button->clicked().connect(this, &GameWidget::setTextField);

	//add line break
	parent->addWidget(new Wt::WBreak());

	this->msgToShow = new Wt::WText("po submitowaniu jakiejs wiadomosci ma sie zmienic");
	parent->addWidget(msgToShow);

//	if(this->server_.connect(
//			this, boost::bind(&processGameEvent, this, _1)))
//		Wt::WApplication::instance()->enableUpdates(true);
}

void GameWidget::setTextField()
{
	if(!msgLineEdit->text().empty())
	{
		server_.setTextField(msgLineEdit->text());
	}
}

void GameWidget::processGameEvent(const GEvent &event)
{
	if(event.getType() == GEvent::Message )
	{
		msgToShow->setText(msg);
	}
}
