/* log.c - (C) 2014, Timo Buhrmester
 * srsotr - none of your business
 * See README for contact-, COPYING for license information. */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

#include "log.h"


static bool s_open;
static bool s_stderr;
static int s_lvl;

static const char* lvlnam(int lvl);


// ----- public interface implementation -----


void
log_syslog(const char *ident, int facility)
{
	if (s_open)
		closelog();
	openlog(ident, LOG_PID, facility);
	s_open = true;
	s_stderr = false;
}


void
log_stderr(void)
{
	if (s_open)
		closelog();
	
	s_stderr = true;
}


void
log_setlvl(int lvl)
{
	s_lvl = lvl;
}


int
log_getlvl(void)
{
	return s_lvl;
}


void
log_log(int lvl, int errn, const char *file, int line, const char *func,
    const char *fmt, ...)
{
	if (lvl > s_lvl)
		return;

	char buf[4096];

	va_list vl;
	va_start(vl, fmt);

	char errmsg[256];
	errmsg[0] = '\0';
	if (errn >= 0) {
		errmsg[0] = ':';
		errmsg[1] = ' ';
		strerror_r(errn, errmsg + 2, sizeof errmsg - 2);
	}

	if (s_stderr) {
		snprintf(buf, sizeof buf, "%s: %s:%d:%s(): %s%s\n",
		    lvlnam(lvl), file, line, func, fmt, errmsg);
		vfprintf(stderr, buf, vl);

	} else {
		snprintf(buf, sizeof buf, "%s:%d:%s(): %s%s",
		    file, line, func, fmt, errmsg);
		vsyslog(lvl, buf, vl);
	}

	va_end(vl);
}


// ---- local helpers ---- 


static const char*
lvlnam(int lvl)
{
	return lvl == LOG_DEBUG ? "DBG" :
	       lvl == LOG_INFO ? "INF" :
	       lvl == LOG_NOTICE ? "NOT" :
	       lvl == LOG_WARNING ? "WRN" :
	       lvl == LOG_ERR ? "ERR" : "???";
}
