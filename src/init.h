/* init.h - (C) 2014, Timo Buhrmester
 * srsotr - none of your business
 * See README for contact-, COPYING for license information. */

#ifndef INIT_H
#define INIT_H 1

#include <stdio.h>
#include <stdbool.h>

struct sett_s {
	unsigned short lport;
	char lif[128];
	char rhost[256];
};

const struct sett_s* init(int *argc, char ***argv);

#endif /* INIT_H */
