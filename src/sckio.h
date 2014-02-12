/* sckio.h - (C) 2014, Timo Buhrmester
 * srsotr - none of your business
 * See README for contact-, COPYING for license information. */

#ifndef SCKIO_H
#define SCKIO_H 1

#if HAVE_CONFIG_H
# include <config.h>
#endif

int sck_listen(const char *localif, unsigned short localport);
int sck_pollln(int sck, char *dest, size_t destsz);
int sck_printf(int sck, const char *fmt, ...);

#endif /* SCKIO_H */


