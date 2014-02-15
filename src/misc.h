/* misc.h - (C) 2014, Timo Buhrmester
 * srsotr - none of your business
 * See README for contact-, COPYING for license information. */

#ifndef MISC_H
#define MISC_H 1

#include <stdbool.h>
#include <stddef.h>

#define COUNTOF(ARR) (sizeof (ARR) / sizeof (ARR)[0])

#define CASEMAPPING_RFC1459 0
#define CASEMAPPING_STRICT_RFC1459 1
#define CASEMAPPING_ASCII 2

#define MAX_IRCARGS 16
#define MAX_NICK_LEN 32
#define MAX_IRC_LINE 1024

#define MAX_CHAN_NAME 256

void strNcat(char *dest, const char *src, size_t destsz);
char* strNcpy(char *dst, const char *src, size_t len);

void itolower(char *dest, size_t destsz, const char *str, int casemap);
int istrcasecmp(const char *n1, const char *n2, int casemap);
int istrncasecmp(const char *n1, const char *n2, size_t len, int casemap);
int irc_tokenize(char *buf, char **tok, size_t tok_len);
bool ircpfx_extract_nick(char *dest, size_t dest_sz, const char *pfx);

#endif /* MISC_H */
