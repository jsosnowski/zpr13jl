/**
* @file PlayEvent.h
*
* @date 29-05-2013
*
* @author Gadawski £ukasz, Sosnowski Jacek
*
* @brief Implementation of the PlayEvent class.
*
* @par Project
* This is a part of project realized on Warsaw University of Technology
* on ZPR lectures. Project was created to provide noughts and crosses game
* implementation. The application consists of server process wich afford 
* widget object with all game functionality to each client session.
*/

#ifndef PLAYEVENT_H_
#define PLAYEVENT_H_

/**
 * @addtogroup zprgame
 */
/*@{*/

/**
* @brief Class which store Play Event
* @details This event is associated only with game.
**/
class PlayEvent
{
public:
  /** 
  * @brief Enumeration for the event type.
  */
	enum PEvent { 
		None, /**< None event is store in this object */
		Move, /**< Move event - click X or O */
		Win,  /**< Somebody wins event */
		Draw, /**< Draw is happen */
		Pass  /**< Somebody pass game. */};

	/**
	* @brief Default constructor. Build event object with error state associated.
	* @details This object will store None event type and error field position (-1).
	* @see PlayEvent(PEvent t, int f = -1)
	**/
	PlayEvent()
		:type_(None), field_(-1)
	{ };

	/** 
	* @brief Build correct event object.
	* @param[in] t PEvent type of play situtation events.
	* @param[in] f int which contains field position on board which was clicked
	*              It is important only if t is equal Move type.
	*              otherwise it might be -1 by defaul.
	**/
	PlayEvent(PEvent t, int f = -1)
		: type_(t), field_(f)
	{ };
	
	/**
	* @brief Return storage information about event type.
	* @return PEvnet type of event contain with strict game
	**/
	PEvent type() const { return type_; };
	/** 
	* @brief get field position on board 
	* @return int - position of field which is storage in this object
	*				while type of event is MOVE
	**/
	int getField() const { return field_; };

	/**
	* @brief Checks if event is associated with game end
	* @return true if game is end, false otherwise
	**/
	bool ifEndOfGame() const;

private:
	/** Type of event which is storage in this object
	*	Event is exacly associated with playing game situations. **/
	PEvent type_;
	/** Contains field id which was clicked. (default is -1)
	*	It is provide with type_ == Move. Otherwise it could be -1 (wrong value) **/
	int field_;
};

/*@}*/

#endif /* PLAYEVENT_H_ */

