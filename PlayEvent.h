
#ifndef PLAYEVENT_H_
#define PLAYEVENT_H_


class PlayEvent
{
public:
	enum PEvent { None, Move, Win, Draw, Pass };

	PlayEvent()
		:type_(None), field_(-1)
	{ };

	PlayEvent(PEvent t, int f = -1)
		: type_(t), field_(f)
	{ };
	
	PEvent type() const { return type_; };
	int getField() const { return field_; };

private:
	PEvent type_;
	int field_;

};

#endif /* PLAYEVENT_H_ */
