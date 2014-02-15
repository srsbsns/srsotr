/* srsotr.c - (C) 2014, srsbsns
 * srsotr - none of your business
 * See README for contact-, COPYING for license information. */

#if HAVE_CONFIG_H
# include <config.h>
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>

#include <sys/socket.h>

#include <libsrsbsns/addr.h>

#include "peerlist.h"
#include "init.h"
#include "misc.h"
#include "sckio.h"
#include "log.h"

#define MAX_IRCARGS 16

void handle_client(int sck);
static void handle_clt_msg(const char *line);
static void handle_irc_msg(const char *line);

const struct sett_s *settings;

int
main(int argc, char **argv)
{
	settings = init(&argc, &argv);

	D("lport: %hu, lif: '%s'", settings->lport, settings->lif);

	int lsck = sck_listen(settings->lif, settings->lport);

	if (lsck == -1)
		C("failed to make listener");

	D("opened listener");

	for (int csck = accept(lsck, NULL, NULL); csck != -1;
	                                   csck = accept(lsck, NULL, NULL))
		handle_client(csck);

	EE("failed to accept()");

	close(lsck);
	D("closed listener");

	return EXIT_SUCCESS;
}

void
handle_client(int sck)
{
	char host[256];
	unsigned short port = 6667;
	addr_parse_hostspec(host, sizeof host, &port, settings->rhost);
	D("remote host: '%s:%hu'", host, port);
	int isck = addr_connect_socket(host, port);
	if (isck == -1)
		C("failed to make or connect socket");

	for (;;) {
		char linebuf[1024];
		int r = sck_pollln(sck, linebuf, sizeof linebuf);
		if (r == -1)
			CE("failed to read line from client");

		if (r != 0) {
			D("read line from client: '%s'", linebuf);
			handle_clt_msg(linebuf);
			sck_printf(isck, "%s\r\n", linebuf);
			D("sent to ircd: '%s'", linebuf);
		}

		char *tok[MAX_IRCARGS];

		r = sck_pollln(isck, linebuf, sizeof linebuf);

		if (r == -1)
			CE("failed to read from irc server");

		if (r != 0) {
			D("read line from ircd: '%s'", linebuf);
			handle_irc_msg(linebuf);
			sck_printf(sck, "%s\r\n", linebuf);
			D("sent to client: '%s'", linebuf);
		}
	}
}
static void
handle_clt_msg(const char *line)
{
	if (strncmp(line, "PRIVMSG ", 8) == 0) {
		N("PRIVMSG sent from client");
	}
}


static void
handle_irc_msg(const char *line)
{
	const char *cmdpos = line;
	if (line[0] == ':')
		cmdpos = strchr(line, ' ');

	if (!cmdpos)
		C("parse error (line: '%s')", line);
	
	cmdpos++;
	
	if (strncmp(cmdpos, "PRIVMSG ", 8) == 0) {
		N("PRIVMSG sent from server");
	}
}
