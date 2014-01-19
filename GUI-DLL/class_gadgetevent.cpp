#include "windows.h"
#include "class_gadgetevent.h"
#include <queue>
   
CGadgetEvents::CGadgetEvents()
{
	
}

CGadgetEvents::~CGadgetEvents()
{
	
}

void CGadgetEvents::setEvent(DWORD source,DWORD type,DWORD data,DWORD dataEx)
{
	eventSource=source;
	eventType=type;
	eventData=data;
	eventDataEx=dataEx;
}

void CGadgetEvents::clearEvent()
{
	  
	eventSource=NULL;
	eventType=NULL;
	eventData=NULL;
	eventDataEx=NULL;
}

void CGadgetEvents::push(DWORD source, DWORD type, DWORD data, DWORD dataEx)
{
	//Only allow a max size of 32
	if (eventsQueue.size() >= 32)
	{
		return;
	}

	
	CGadgetEventItem newEvent;

	newEvent.eventSource=source;
	newEvent.eventType=type;
	newEvent.eventData=data;
	newEvent.eventDataEx=dataEx;

	eventsQueue.push(newEvent);
}

void CGadgetEvents::pop()
{
	if (eventsQueue.size()==0)
	{
		eventSource=0;
		eventType=0;
		eventData=0;
		eventDataEx=0;
		return;
	}

	CGadgetEventItem event=eventsQueue.front();
	eventsQueue.pop();

	
	eventSource = event.eventSource;
	eventType = event.eventType;
	eventData = event.eventData;
	eventDataEx = event.eventDataEx;
}

void CGadgetEvents::flush()
{
	//Empty all the event messages
	while (!eventsQueue.empty())
	{
		eventsQueue.pop();
	}

	//Clear the current event
	clearEvent();
}

