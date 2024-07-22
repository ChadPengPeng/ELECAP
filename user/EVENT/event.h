#ifndef __EVENT__
#define __EVENT__

typedef unsigned int Event;
#define eventCodeMask(event) ((event)&0b1111)
#define stateMask(event) ((event>>4)&0b11)
#define getEvent(eventCode, state) (state<<4 | eventCode )
#define keyNum 4
typedef enum EventCode_enum
{
    NoneEvent = 0,
    Touch = 1,
    KEY1 = 2,
    KEY2 = 3,
    KEY3 = 4,
    KEY4 = 5 // x<<12+y<<4+eventcode as touch event
} EventCode;

typedef enum State_enum
{
    NoneState = 0,
    OnClick = 1,
    Hold = 2,
    HoldEnd = 3
} State;


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