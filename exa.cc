#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <boost/foreach.hpp>
#include <iostream>
#include <vector>

class BoardWidget : public Wt::WApplication
{
public:
	BoardWidget(const Wt::WEnvironment& env);

private:
	//button,  0 - non marked, 1 - o, 2 - x
	std::vector<std::pair<Wt::WPushButton *, int *> > fields;

//	std::vector<Wt::WPushButton *> buttons;
//	std::vector<int *> values;

	void markField();
};

BoardWidget::BoardWidget(const Wt::WEnvironment &env)
	:Wt::WApplication(env)
{
	setTitle("Hello bitches!");

	std::vector<std::pair<Wt::WPushButton *, int *> > fields(9);

	std::vector<Wt::WPushButton *> buttons(9);
	std::vector<int *> values(9);

	for(int a = 0; a < 9; ++a)
	{
		fields[a] = std::make_pair(new Wt::WPushButton("-", root()), new int(0));

		if((a+1) % 3 == 0)	//cause 0 mod 0 == 0
		{
			root()->addWidget(new Wt::WBreak());
		}
	}

//	BOOST_FOREACH(Wt::WPushButton *b, buttons)
//	{
//		b->clicked().connect(this, &BoardWidget::markField);
//	}
}

void BoardWidget::markField()
{
//	buttons->setText("x");
}

Wt::WApplication *createApplication(const Wt::WEnvironment &env)
{
	return new BoardWidget(env);
}

int main(int argc, char **argv)
{
	return Wt::WRun(argc, argv, &createApplication);
}
