#include <stdio.h>

#if defined(__is_libk)
/* TODO: Need to implement. */
#endif

int putchar(int ic) {
#if defined(__is_libk)
    /* TODO: Need to implement. */
#else
	/* TODO: Implement stdio and the write system call. */
#endif
	return ic;
}
