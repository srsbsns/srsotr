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

void handle_client(int sck);
static bool handle_clt_msg(const char *line);
static bool handle_irc_msg(const char *line);

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
			if (handle_clt_msg(linebuf)) {
				sck_printf(isck, "%s\r\n", linebuf);
				D("sent to ircd: '%s'", linebuf);
			} else
				D("not passed on");
		}

		char *tok[MAX_IRCARGS];

		r = sck_pollln(isck, linebuf, sizeof linebuf);

		if (r == -1)
			CE("failed to read from irc server");

		if (r != 0) {
			D("read line from ircd: '%s'", linebuf);
			if (handle_irc_msg(linebuf)) {
				sck_printf(sck, "%s\r\n", linebuf);
				D("sent to client: '%s'", linebuf);
			} else
				D("not passed on");
		}
	}
}

/* return false to prevent the message from being passed on */
static bool
handle_clt_msg(const char *line)
{
	bool ret = true;

	char l[MAX_IRC_LINE];
	strNcpy(l, line, sizeof l);
	char *tok[MAX_IRCARGS];
	if (irc_tokenize(l, tok, MAX_IRCARGS) != 1) {
		W("failed to tokenize line '%s'. not relaying for "
		    "paranoia reasons.", line);
		ret = false;
	} else if (strcmp(tok[1], "PRIVMSG") == 0
	    && peer_otrchan(tok[2])) {
		N("message to otr chan!");
		ret = false;
	}

	return ret;
}


/* return false to prevent the message from being passed on */
static bool
handle_irc_msg(const char *line)
{
	bool ret = true;

	char l[MAX_IRC_LINE];
	strNcpy(l, line, sizeof l);

	char *tok[MAX_IRCARGS];
	if (irc_tokenize(l, tok, MAX_IRCARGS) != 1) {
		W("failed to tokenize line '%s'. not relaying for "
		    "paranoia reasons.", line);
		ret = false;
	} else if (strcmp(tok[1], "PRIVMSG") == 0 && peer_otrchan(tok[2])) {
		char nick[MAX_NICK_LEN];
		ircpfx_extract_nick(nick, sizeof nick, tok[0]);
		const char *alias = peer_alias(nick, tok[2]);
		N("message in otr chan from '%s' ('%s', canonical: '%s')!",
		    tok[0], nick, alias);
		ret = false;
	}

	return ret;
}
