/* misc.h - (C) 2014, Timo Buhrmester
 * srsotr - none of your business
 * See README for contact-, COPYING for license information. */

#ifndef MISC_H
#define MISC_H 1

#include <stddef.h>

#define COUNTOF(ARR) (sizeof (ARR) / sizeof (ARR)[0])

void strNcat(char *dest, const char *src, size_t destsz);
char* strNcpy(char *dst, const char *src, size_t len);

#endif /* MISC_H */
