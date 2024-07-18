#ifndef __EVENT__
#define __EVENT__

typedef int Event;
#define eventCodeMask(event) ((event)&0b1111)
enum Event_enum
{
    TouchingEnd = 3,
    Touching = 2,
    OnClick = 1,
    NoneEvent = 0,
    KEY1 = -1,
    KEY2 = -2,
    KEY3 = -3,
    KEY4 = -4 // x<<12+y<<4+eventcode as touch event
};

typedef struct EventBuffer_struct
{
    Event eventList[256];
    unsigned char head;
    unsigned char tail;
} EventBuffer;

extern EventBuffer eventBuffer;
extern void eventInit();
extern void addEvent(Event event);
extern Event dequeueEvent();
extern int haveEvent();
#endif // !__EVENT__