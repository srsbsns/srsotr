/* peerlist.h - (C) 2014, Timo Buhrmester
 * srsotr - none of your business
 * See README for contact-, COPYING for license information. */

#ifndef PEERLIST_H
#define PEERLIST_H 1

#include <stdbool.h>

//TODO improve this, use regex
const char* peer_alias(const char *nick, const char *chan);
bool peer_otrchan(const char *chan);
void peer_init(void);

#endif /* PEERLIST_H */
