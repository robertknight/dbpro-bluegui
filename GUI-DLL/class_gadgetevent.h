#ifndef CLASS_GADGETEVENT_H
#define CLASS_GADGETEVENT_H

//class queue;
#include <queue>

//Tracks events happening to gadgets

class CGadgetEventItem
{
public:
	DWORD eventSource;
	DWORD eventType;
	DWORD eventData;
	DWORD eventDataEx;
};

class CGadgetEvents
{
public:
	//Constructor / Destructor
	CGadgetEvents();
	~CGadgetEvents();

	//Accessor - called to set the event properties
	void	setEvent(DWORD source,DWORD type, DWORD data, DWORD dataEx);
	DWORD	getEventSource() const {return eventSource;};
	DWORD	getEventType() const {return eventType;};
	DWORD	getEventData() const {return eventData;};
	DWORD	getEventDataEx() const {return eventDataEx;};

	//Stack functions
	void	push(DWORD source, DWORD type, DWORD data, DWORD dataEx);
	void	pop();
	void    flush();

	//Deletes all information about the current event
	void	clearEvent();

private:
	DWORD							eventSource;
	DWORD							eventType;
	DWORD							eventData;
	DWORD							eventDataEx;

	//Information about events is received by the subclass window function and popped onto the event
	//queue which is later accessed by the plugin commands

	std::queue<CGadgetEventItem>	eventsQueue;	
};

#endif
