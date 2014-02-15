/* peerlist.c - (C) 2014, Timo Buhrmester
 * srsotr - none of your business
 * See README for contact-, COPYING for license information. */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>

#include <libsrsbsns/strlist.h>
#include <libsrsbsns/hashmap.h>
#include <libsrsbsns/strmap.h>

#include "misc.h"
#include "log.h"

#include "peerlist.h"

smap_t s_chanmap;
smap_t s_aliasmap;

static bool load_list(void);
static void handle_peerline(const char *line);

const char*
peer_alias(const char *nick, const char *chan)
{
	slist_t alis = smap_get(s_chanmap, chan);
	if (!alis)
		return NULL;
	
	const char *a = slist_first(alis);
	do {
		slist_t nlis = smap_get(s_aliasmap, a);
		if (!nlis) {
			W("no entry in aliasmap for '%s'", a);
			continue;
		}

		const char *n = slist_first(nlis);
		do {
			if (istrcasecmp(n, nick, CASEMAPPING_RFC1459) == 0) //XXX
				return a;
		} while ((n = slist_next(nlis)));
	} while ((a = slist_next(alis)));

	return NULL;
}

void
peer_init(void)
{
	s_aliasmap = smap_init(256);
	s_chanmap = smap_init(256);
	load_list();
}

static void
dumpstr(const void *s)
{
	fprintf(stderr, "%s", (const char*)s);
}

static void
dumpslist(const void *s)
{
	slist_t l = (slist_t)s;
	slist_dump(l);
}

static bool
load_list(void)
{
	FILE *fp = fopen("peerlist", "r");
	if (!fp) {
		W("file 'peerlist' not found in datadir");
		return false;
	}

	char *line = NULL;
	size_t linesize = 0;
	ssize_t linelen;

	while ((linelen = getline(&line, &linesize, fp)) != -1) {
		char *ptr = line;
		while (isspace(*ptr))
			ptr++;

		if (*ptr == '#' || *ptr == '\0')
			continue;
			
		char *end = ptr + strlen(ptr) - 1;
		while (isspace(*end))
			*end-- = '\0';

		if (!*ptr)
			continue;

		handle_peerline(ptr);
	}

	if (ferror(fp)) {
		WE("getline");
		return smap_count(s_chanmap) > 0;
	}

	hmap_dump(s_chanmap, dumpstr, dumpslist);
	hmap_dump(s_aliasmap, dumpstr, dumpslist);

	return true;
}

static void
handle_peerline(const char *line)
{
	char *l = strdup(line);
	char *chan = strtok(l, "\t ");
	if (!chan) {
		E("parse error on line '%s'", line);
		free(l);
		return;
	}

	char *tmp = malloc(strlen(chan)+1);
	itolower(tmp, sizeof tmp, chan, CASEMAPPING_RFC1459); //TODO get cm from 005
	chan = tmp;

	char *alias = strtok(NULL, "\t ");
	if (!alias) {
		E("parse error on line '%s'", line);
		free(l);
		free(tmp);
		return;
	}

	slist_t ll = smap_get(s_chanmap, chan);

	if (!ll)
		smap_put(s_chanmap, chan, ll = slist_init());

	slist_insert(ll, 0, alias);

	slist_t sl = smap_get(s_aliasmap, alias);
	if (!sl)
		smap_put(s_aliasmap, alias, sl = slist_init());

	slist_insert(sl, 0, alias);

	char *nick;
	while ((nick = strtok(NULL, "\t ")))
		slist_insert(sl, 0, nick);

	free(tmp);
	free(l);
}

