/* otr.c - (C) 2014, Timo Buhrmester
 * srsotr - none of your business
 * See README for contact-, COPYING for license information. */

#include "log.h"

#include <libotr/proto.h>

#include "otr.h"

void
otr_init(void)
{
	OTRL_INIT;
}
