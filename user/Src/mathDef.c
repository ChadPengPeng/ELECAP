#include "mathDef.h"

int getMagnitude(int num)
{
    int magnitude = 1;
    while (num >= 10)
    {
        num /= 10;
        magnitude *= 10;
    }
    return magnitude;
}

int getHighDigit(int num)
{
    int magnitude = getMagnitude(num);
    return num / magnitude;
}

int highDigitPlusOne(int num)
{
    return num + getMagnitude(num);
}

int highDigitMinusOne(int num)
{
    if (getHighDigit(num) == 1)
        return num - getMagnitude(num) / 10;
    else
        return num - getMagnitude(num);
}

// example: 100 to 200 to 500 to 1000;
int HigherMagnitude(int num)
{
    int highDigit = getHighDigit(num);
    switch (highDigit)
    {
    case 1:
        highDigit = 2;
        break;

    case 2:
        highDigit = 5;
        break;

    case 5:
        highDigit = 10;
        break;

    default:
        break;
    }
    return highDigit * getMagnitude(num);
}

int LowerMagnitude(int num)
{
    int highDigit = getHighDigit(num);
    switch (highDigit)
    {
    case 1:
        highDigit = 5;
        break;

    case 2:
        highDigit = 10;
        break;

    case 5:
        highDigit = 20;
        break;

    default:
        break;
    }
    return highDigit * getMagnitude(num) / 10;
}