#ifndef __EVENT__
#define __EVENT__

typedef unsigned int Event;
#define eventCodeMask(event) ((event) & 0b1111)
#define stateMask(event) ((event >> 4) & 0b11)
#define getEvent(eventCode, state) (state << 4 | eventCode)
#define keyNum 4
typedef enum EventCode_enum
{
    NoneEvent = 0,
    Touch,
    Select,
    KEY1,
    KEY2,
    KEY3,
    KEY4
} EventCode;

typedef enum State_enum
{
    NoneState = 0,
    OnClick = 1,
    Hold = 2,
    HoldEnd = 3,
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