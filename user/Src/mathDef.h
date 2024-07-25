#ifndef __MATHDEF__
#define __MATHDEF__

#define constrain(x, a, b) ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))
#define constrainMax(x, a) ((x) > (a) ? (a) : (x))
#define constrainMin(x, a) ((x) < (a) ? (a) : (x))
#define absM(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : -1)
#define approach(a, b, rate) ((a) + ((b) - (a)) * rate)
#define approachDiv(a, b, _div) (((b) - (a)) / (_div) != 0 ? (a) + ((b) - (a)) / (_div) : absM((b) - (a)) > 2 ? (a) + sign((b) - (a)) \
                                                                                                              : (b))
extern int getMagnitude(int num);
extern int getHighDigit(int num);
extern int highDigitPlusOne(int num1);
extern int highDigitMinusOne(int num1);
#endif // __MATHDEF__