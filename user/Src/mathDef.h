#ifndef __MATHDEF__
#define __MATHDEF__

#define constrain(x, a, b) ((x) < (a)? (a) : ((x) > (b)? (b) : (x)))
#define constrainMax(x, a) ((x) > (a)? (a) : (x))
#define constrainMin(x, a) ((x) < (a)? (a) : (x))


#endif   // __MATHDEF__