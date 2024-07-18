#include "event.h"

EventBuffer eventBuffer;
void eventInit()
{
    eventBuffer.head = eventBuffer.tail = 0;
}

void addEvent(Event event)
{
    eventBuffer.eventList[eventBuffer.head++] = event;
}  

Event dequeueEvent()
{
    return eventBuffer.eventList[eventBuffer.tail++];
}

int haveEvent()
{
    return (eventBuffer.head != eventBuffer.tail);
}