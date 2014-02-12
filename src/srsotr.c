/* srsotr.c - (C) 2014, srsbsns
 * srsotr - none of your business
 * See README for contact-, COPYING for license information. */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>



#include <libsrsbsns/addr.h>
#include <libsrsbsns/io.h>

#include "init.h"
#include "log.h"

int
main(int argc, char **argv)
{
	const struct sett_s *sett = init(&argc, &argv);

	D("lport: %hu, lif: '%s'", sett->lport, sett->lif);

	return EXIT_SUCCESS;
}
