#ifndef ____LIBDBC__
#define ____LIBDBC__

#include <stdbool.h>

#ifdef __GNUC__
#define likely(x) __builtin_expect(x, true)
#define unlikely(x) __builtin_expect(x, false)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

#endif
