/* init.c - (C) 2014, Timo Buhrmester
 * srsotr - none of your business
 * See README for contact-, COPYING for license information. */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include <stdlib.h>
#include <string.h>

#include <getopt.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>


#include "sig.h"
#include "misc.h"
#include "log.h"
#include "otr.h"
#include "peerlist.h"

#include "init.h"

static char *s_suid;
static char *s_sgid;
static bool s_nodaemon;
static int s_verb = LOG_WARNING;

struct sett_s sett = { 7776,
                       "127.0.0.1",
		       "irc.example.org:6667",
		       ""};

static void process_args(int *argc, char ***argv);
static void setugid(void);
static void detach(void);
static void daemonize(void);
static void usage(FILE *str, const char *a0, int ec);

// ----- public interface implementation -----

const struct sett_s*
init(int *argc, char ***argv)
{
	process_args(argc, argv);

	if (s_nodaemon)
		log_stderr();
	else
		log_syslog(PACKAGE_NAME, LOG_DAEMON);

	log_setlvl(s_verb);

	if (!sett.datadir[0]) {
		int r = getenv_r("HOME", sett.datadir, sizeof sett.datadir);
		if (r == -1)
			CE("unable to locate datadir ($HOME not set?!)");

		strNcat(sett.datadir, "/.srsotr", sizeof sett.datadir);
		if (mkdir(sett.datadir, S_IRWXU) == -1 && errno != EEXIST)
			CE("couldn't mkdir datadir '%s'", sett.datadir);
	}

	if (!s_nodaemon)
		daemonize();

	if (s_suid || s_sgid)
		setugid();

	if (chdir(sett.datadir) == -1)
		CE("couldn't chdir() into datadir '%s'", sett.datadir);
	
	FILE *fp = fopen(".writetest", "w+");
	if (!fp)
		CE("can't write inside datadir '%s'", sett.datadir);
	fclose(fp);
	remove(".writetest");

	peer_init();

	signal(SIGHUP, sig_hnd);
	signal(SIGINT, sig_hnd);
	signal(SIGPIPE, sig_hnd);
	signal(SIGTERM, sig_hnd);
	signal(SIGUSR1, sig_hnd);
	signal(SIGUSR2, sig_hnd);

	otr_init();

	return &sett;
}

// ---- local helpers ---- 

static void
setugid(void)
{
	if (getuid() != 0) {
		E("not started as root, can't setuid/setgid");
		return;
	}

	struct group *gp = NULL;
	struct passwd *up = NULL;

	if (s_sgid && !(gp = getgrnam(s_sgid)))
		E("unknown group/gid '%s', will not setgid", s_sgid);
	if (s_suid && !(up = getpwnam(s_suid)))
		E("unknown user/uid '%s', will not setuid", s_suid);

	if (gp && setgid(gp->gr_gid) != 0)
		E("failed to setgid('%d')", gp->gr_gid);
	if (up && setuid(up->pw_uid) != 0)
		E("failed to setuid('%d')", up->pw_uid);
}


static void
detach(void)
{
	int r;

	/* ignore signals associated with job control */
	signal(SIGTSTP, SIG_IGN);
	signal(SIGTTOU, SIG_IGN);
	signal(SIGTTIN, SIG_IGN);

	if ((r = fork()) == -1)
		C("failed to fork, exiting!");
	else if (r > 0)
		exit(EXIT_SUCCESS);

	setsid(); /* create a new session, become session leader */
}


static void
daemonize(void)
{
	if (getppid() != 1)
		detach();

	struct rlimit rlim;
	int fdmax;
	if (getrlimit(RLIMIT_NOFILE, &rlim) != 0) {
		E("getrlimit() failed when asked for RLIMIT_NOFILE");
		fdmax = 2; /* assume only std{in,out,err} are open */
	} else
		fdmax = rlim.rlim_max;

	/* close all file descriptors */
	for (int i = 0; i < fdmax; i++)
		close(i);

	umask(0);
	if (chdir("/") != 0)
		E("couldn't chdir to /");
}


static void
process_args(int *argc, char ***argv)
{
	char *a0 = (*argv)[0];

	for(int ch; (ch = getopt(*argc, *argv, "s:i:p:u:d:nvqch")) != -1;) {
		switch (ch) {
		case 'u':{
			s_suid = strdup(optarg);
			char *ptr = strchr(s_suid, ':');
			if (ptr) {
				*ptr = '\0';
				s_sgid = strdup(ptr+1);
			}
			break;}
		case 'i':
			strNcpy(sett.lif, optarg, sizeof sett.lif);
			break;
		case 'p':
			sett.lport =
			    (unsigned short)strtoul(optarg, NULL, 0);
			break;
		case 's':
			strNcpy(sett.rhost, optarg, sizeof sett.rhost);
			break;
		case 'n':
			s_nodaemon = true;
			break;
		case 'd':
			strNcpy(sett.datadir, optarg, sizeof sett.datadir);
			break;
		case 'h':
			usage(stdout, a0, EXIT_SUCCESS);
			break;
		case 'c':
			log_setfancy(true);
			break;
		case 'v':
			s_verb++;
			break;
		case 'q':
			s_verb--;
			break;
		case '?':
		default:
			usage(stderr, a0, EXIT_FAILURE);
		}
	}

	*argc -= optind;
	*argv += optind;
}


static void
usage(FILE *str, const char *a0, int ec)
{
	#define U(STR) fputs(STR "\n", str)
	U("====================================");
	U("== srsotr "PACKAGE_VERSION" - none of your business ==");
	U("====================================");
	fprintf(str, "usage: %s [-h]\n", a0);
	U("");
	U("  -n: do not daemonize, log to stderr instead of syslog");
	U("  -v: be more verbose (may be specified multiple times)");
	U("  -q: be less verbose (may be specified multiple times)");
	U("  -u <user>[:<group>]: setuid() and optionally also setgid()");
	U("  -h: Display brief usage statement and terminate");
	U("");
	U("The <user> and <group> arguments to -u can be either");
	U("  textual user/group names, or numerical uid/gid values.");
	U("");
	I("(C) 2014, srsbsns (contact: #OpenFNG on irc.quakenet.org)");
	#undef U
	exit(ec);
}
