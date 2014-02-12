/* sckio.c - (C) 2014, Timo Buhrmester
 * srsotr - none of your business
 * See README for contact-, COPYING for license information. */

#include <stdio.h>
#include <string.h>

#include <errno.h>
#include <sys/select.h>

#include <libsrsbsns/addr.h>
#include <libsrsbsns/io.h>

#include "log.h"

#include "sckio.h"

int
sck_listen(const char *localif, unsigned short localport)
{
	int listen_sck = addr_bind_socket(localif, localport);
	if (listen_sck == -1) {
		E("couldn't bind to '%s:%hu'", localif, localport);
		return -1;
	}

	D("bound socket to '%s:%hu'", localif, localport);

	if (listen(listen_sck, 128) != 0) {
		EE("failed to listen()");
		return -1;
	}
	
	D("socket listening");

	return listen_sck;
}

int
sck_pollln(int sck, char *dest, size_t destsz)
{
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(sck, &fds);
	struct timeval to = {0, 0};

	int r = select(sck+1, &fds, NULL, NULL, &to);
	if (r == -1)
		E("select() failed");

	if (r == 1) {
		r = io_read_line(sck, dest, destsz);
		if (r == -1) {
			W("io_read_line() failed");
			return -1;
		}

		char *end = dest + strlen(dest) - 1;
		while (end >= dest && (*end == '\r' || *end == '\n'
		                                           || *end == ' '))
			*end-- = '\0';
		return strlen(dest);
	}

	return 0;
}

int
sck_printf(int sck, const char *fmt, ...)
{
	va_list l;
	va_start(l, fmt);
	char buf[1024];
	vsnprintf(buf, sizeof buf, fmt, l);
	char *p = buf+strlen(buf)-1;
	if (*p != '\n')
		W("line doesntend with \\n:");
	while(p >= buf && (*p == '\n' || *p == '\r'))
		*p-- = '\0';
	va_end(l);
	va_start(l, fmt);
	int r = io_vfprintf(sck, fmt, l);
	va_end(l);
	return r;
}		



