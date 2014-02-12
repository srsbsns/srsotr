/* sig.c - (C) 2014, Timo Buhrmester
 * srsotr - none of your business
 * See README for contact-, COPYING for license information. */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdbool.h>

#include <signal.h>

#include "log.h"

#include "sig.h"

static uint8_t sigflags;

static const char* sig_name(int sig);

// ----- public interface implementation -----

uint8_t
sig_events(void)
{
	uint8_t r = sigflags;
	sigflags = 0;
	return r;
}

void
sig_hnd(int sig)
{
	N("caught signal %d (%s)", sig, sig_name(sig));
	switch(sig) {
	case SIGHUP: /* 1, Term, Hangup or death of controlling process */
		sigflags |= (1<<SBIT_READCFG);
		N("re-read of cfg requested");
		break;
	case SIGINT: /* 2, Term, Interrupt from keyboard */
		sigflags |= (1<<SBIT_SHUTDOWN);
		N("shutdown requested");
		break;
	case SIGTERM: /* 15, Term, Termination signal */
		sigflags |= (1<<SBIT_SHUTDOWN)|(1<<SBIT_HARDSHUTDOWN);
		N("hard shutdown requested");
		break;
	case SIGUSR1: /* 30,10,16, Term, User-defined signal 1 */
		N("something fancy requested");
		break;
	case SIGUSR2: /* 31,12,17, Term, User-defined signal 2 */
		N("something fancy requested");
		break;
	//case SIGPIPE: /* 13, Term, Write to pipe with no readers */
		//break;
	default:
		W("signal %d (%s) is unhandled!", sig, sig_name(sig));
	}
}

// ---- local helpers ---- 

static const char*
sig_name(int sig) {
	return sig == SIGHUP ? "HUP" :
	       sig == SIGINT ? "INT" :
	       sig == SIGQUIT ? "QUIT" :
	       sig == SIGPIPE ? "PIPE" :
	       sig == SIGTERM ? "TERM" :
	       sig == SIGUSR1 ? "USR1" :
	       sig == SIGUSR2 ? "USR2" : "UNKNOWN";
}

