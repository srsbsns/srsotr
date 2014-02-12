/* log.h - (C) 2014, Timo Buhrmester
 * srsotr - none of your business
 * See README for contact-, COPYING for license information. */

#ifndef LOG_H
#define LOG_H 1

#include <syslog.h>

//[DINWE](): log message with Debug, Info, Notice, Warning, Error severity.
//[DINWE]E(): similar, but also append ``errno'' message
//*X(): similar, but exit(1) after logging
 
// ----- logging interface -----

#define D(F,A...)                                               \
    log_log(LOG_DEBUG,-1,__FILE__,__LINE__,__func__,F,##A)

#define DE(F,A...)                                              \
    log_log(LOG_DEBUG,errno,__FILE__,__LINE__,__func__,F,##A)

#define I(F,A...)                                               \
    log_log(LOG_INFO,-1,__FILE__,__LINE__,__func__,F,##A)

#define IE(F,A...)                                              \
    log_log(LOG_INFO,errno,__FILE__,__LINE__,__func__,F,##A)

#define N(F,A...)                                               \
    log_log(LOG_NOTICE,-1,__FILE__,__LINE__,__func__,F,##A)

#define NE(F,A...)                                              \
    log_log(LOG_NOTICE,errno,__FILE__,__LINE__,__func__,F,##A)

#define W(F,A...)                                               \
    log_log(LOG_WARNING,-1,__FILE__,__LINE__,__func__,F,##A)

#define WE(F,A...)                                              \
    log_log(LOG_WARNING,errno,__FILE__,__LINE__,__func__,F,##A)

#define E(F,A...)                                               \
    log_log(LOG_ERR,-1,__FILE__,__LINE__,__func__,F,##A)

#define EE(F,A...)                                              \
    log_log(LOG_ERR,errno,__FILE__,__LINE__,__func__,F,##A)

#define EX(F,A...)                                              \
    do{log_log(LOG_ERR,-1,__FILE__,__LINE__,__func__,F,##A);    \
                      exit(EXIT_FAILURE); }while(0)
#define EEX(F,A...)                                             \
    do{log_log(LOG_ERR,errno,__FILE__,__LINE__,__func__,F,##A); \
                       exit(EXIT_FAILURE); }while(0)

// ----- logger control interface -----

void log_stderr(void);
void log_syslog(const char *ident, int facility);

void log_setlvl(int lvl);
int log_getlvl(void);


// ----- backend -----
void log_log(int lvl, int errn, const char *file, int line,
    const char *func, const char *fmt, ...);

#endif /* LOG_H */