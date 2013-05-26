#ifndef GEVENT_H_
#define GEVENT_H_

#include <Wt/WString>

class GEvent
{
public:
	/*** event types */
	enum Type { Message };

	GEvent(const Wt::WString& msg);

	Type getType() const;

private:
	Wt::WString msg;
	Type type;

};

#endif
