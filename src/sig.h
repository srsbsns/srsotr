/* sig.h - (C) 2014, Timo Buhrmester
 * srsotr - none of your business
 * See README for contact-, COPYING for license information. */

#ifndef SIG_H
#define SIG_H 1

#include <stdint.h>

#define SBIT_READCFG 0
#define SBIT_DUMPCFG 1
#define SBIT_REINIT 2
#define SBIT_SHUTDOWN 3
#define SBIT_HARDSHUTDOWN 4

void sig_hnd(int sig);

/* returns and clears event flags */
uint8_t sig_events(void);

#endif /* SIG_H */
