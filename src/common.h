#ifndef __COMMON_H
#define __COMMON_H

#define INCL_GPI
#define INCL_WIN
#include <os2.h>
#include <math.h>
#include <stdlib.h>

/* Borland compat macros for Open Watcom */
#ifndef random
#define random(n)   (rand() % (n))
#endif
#ifndef randomize
#define randomize() srand((unsigned)time(NULL))
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#endif /* __COMMON_H */
