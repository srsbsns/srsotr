/* misc.c - (C) 2014, Timo Buhrmester
 * srsotr - none of your business
 * See README for contact-, COPYING for license information. */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "log.h"
#include "misc.h"

void
strNcat(char *dest, const char *src, size_t destsz)
{
	size_t len = strlen(dest);
	if (len + 1 >= destsz)
		return;
	size_t rem = destsz - (len + 1);

	char *ptr = dest + len;
	while(rem-- && *src) {
		*ptr++ = *src++;
	}
	*ptr = '\0';
}

char*
strNcpy(char *dst, const char *src, size_t len)
{
	char *r = strncpy(dst, src, len);
	dst[len-1] = '\0';
	return r;
}

void
itolower(char *dest, size_t destsz, const char *str, int casemap)
{
	int rangeinc;
	switch (casemap)
	{
	case CASEMAPPING_RFC1459:
		rangeinc = 4;
		break;
	case CASEMAPPING_STRICT_RFC1459:
		rangeinc = 3;
		break;
	default:
		rangeinc = 0;
	}

	size_t c = 0;
	char *ptr = dest;
	while(c < destsz) {
		if (*str >= 'A' && *str <= ('Z'+rangeinc))
			*ptr++ = *str + ('a'-'A');
		else
			*ptr++ = *str;

		if (!*str)
			break;
		str++;
	}

	dest[destsz-1] = '\0';
}

int
istrcasecmp(const char *n1, const char *n2, int casemap)
{
	size_t l1 = strlen(n1);
	size_t l2 = strlen(n2);

	return istrncasecmp(n1, n2, (l1 < l2) ? (l1 + 1) : (l2 + 1), casemap);
}

int
istrncasecmp(const char *n1, const char *n2, size_t len, int casemap)
{
	if (len == 0)
		return 0;

	char *d1 = strdup(n1);
	char *d2 = strdup(n2);

	itolower(d1, strlen(d1) + 1, n1, casemap);
	itolower(d2, strlen(d2) + 1, n2, casemap);

	int i = strncmp(d1, d2, len);

	free(d1);
	free(d2);
	return i;
}


static char*
skip2lws(char *s, bool tab_is_ws)
{
	while(*s && (!isspace(*s) || (*s == '\t' && !tab_is_ws)))
		s++;
	return *s ? s : NULL;
}

int
irc_tokenize(char *buf, char **tok, size_t tok_len)
{
	if (!buf || !tok || tok_len < 2)
		return -1;

	for(size_t i = 0; i < tok_len; ++i)
		tok[i] = NULL;

	while(isspace(*buf))
		*buf++ = '\0';

	size_t len = strlen(buf);
	if (len == 0)
		return 0;

	if (*buf == ':')
	{
		tok[0] = buf + 1;
		buf = skip2lws(buf, true);
		if (!buf) {
			W("parse erro, pfx but no cmd");
			return -1;//parse err, pfx but no cmd
		}
		while(isspace(*buf))
			*buf++ = '\0';
	}

	tok[1] = buf;
	buf = skip2lws(buf, true);
	if (buf) {
		while(isspace(*buf))
			*buf++ = '\0';
	}

	size_t argc = 2;
	while(buf && *buf && argc < tok_len)
	{
		if (*buf == ':')
		{
			tok[argc++] = buf + 1;
			break;
		}
		tok[argc++] = buf;

		/* have seen a channel with <Tab> in its name */
		buf = skip2lws(buf, false);
		if (buf) {
			while(isspace(*buf))
				*buf++ = '\0';
		}
	}

	return 1;
}

bool
ircpfx_extract_nick(char *dest, size_t dest_sz, const char *pfx)
{
	if (!dest || !dest_sz || !pfx)
		return false;
	strncpy(dest, pfx, dest_sz);
	dest[dest_sz-1] = '\0';

	char *ptr = strchr(dest, '@');
	if (ptr)
		*ptr = '\0';

	ptr = strchr(dest, '!');
	if (ptr)
		*ptr = '\0';

	return true;
}
