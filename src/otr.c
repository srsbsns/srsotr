/* otr.c - (C) 2014, Timo Buhrmester
 * srsotr - none of your business
 * See README for contact-, COPYING for license information. */

#include <stdbool.h>

#include "log.h"

#include <libotr/proto.h>
#include <libotr/message.h>
#include <libotr/privkey.h>
#include <libotr/instag.h>

#include "otr.h"


OtrlUserState us;
const char *pkfile = "/tmp/pkfile";
const char *itfile = "/tmp/itfile";
const char *fpfile = "/tmp/fpfile";


static bool canread(const char *file);

OtrlMessageAppOps mao;


void
otr_init(void)
{
	OTRL_INIT;
	us = otrl_userstate_create();

	if (!canread(pkfile))
		W("couldn't read privkey file at '%s'", pkfile);
	else
		otrl_privkey_read(us, pkfile);

	if (!canread(itfile))
		W("couldn't read insttag file at '%s'", itfile);
	else
		otrl_instag_read(us, itfile);

	if (!canread(pkfile))
		W("couldn't read fprnt file at '%s'", fpfile);
	else
		otrl_privkey_read_fingerprints(us, fpfile, NULL, NULL);
		//otrl_privkey_read_fingerprints(us, fpfile, add_app_info, add_app_info_data);

}

OtrlUserState
otr_ustate()
{
	return us;
}

static bool
canread(const char *file)
{
	FILE *fp;
	bool ret = false;

	if ((fp = fopen(pkfile, "r"))) {
		ret = true;
		fclose(fp);
	}

	return ret;
}


OtrlPolicy 
mao_policy(void *b, ConnContext *ctx)
{
	N("mao_policy called! (%p)", b);
	return -1;
}

void 
mao_create_privkey(void *b, const char *accnm, const char *prot)
{
	N("mao_create_privkey called! (%p)", b);
}

int 
mao_is_logged_in(void *b, const char *accnm, const char *prot,
    const char *rcpt)
{
	N("mao_is_logged_in called! (%p)", b);
	return -1;
}

void 
mao_inject_message(void *b, const char *accnm, const char *prot,
    const char *rcpt, const char *msg)
{
	N("mao_inject_message called! (%p)", b);
}

void 
mao_update_context_list(void *b)
{
	N("mao_update_context_list called! (%p)", b);
}

void 
mao_new_fingerprint(void *b, OtrlUserState us, const char *accnm,
    const char *prot, const char *uname, unsigned char fprnt[20])
{
	N("mao_new_fingerprint called! (%p)", b);
}

void 
mao_write_fingerprints(void *b)
{
	N("mao_write_fingerprints called! (%p)", b);
}

void 
mao_gone_secure(void *b, ConnContext *ctx)
{
	N("mao_gone_secure called! (%p)", b);
}

void 
mao_gone_insecure(void *b, ConnContext *ctx)
{
	N("mao_gone_insecure called! (%p)", b);
}

void 
mao_still_secure(void *b, ConnContext *ctx, int is_reply)
{
	N("mao_still_secure called! (%p)", b);
}

int 
mao_max_message_size(void *b, ConnContext *ctx)
{
	N("mao_max_message_size called! (%p)", b);
	return 400;
}

const char*
mao_account_name(void *b, const char *account, const char *prot)
{
	N("mao_account_name called! (%p)", b);
	return NULL;
}

void 
mao_account_name_free(void *b, const char *account_name)
{
	N("mao_account_name_free called! (%p)", b);
}

void 
mao_received_symkey(void *b, ConnContext *ctx, unsigned int use,
    const unsigned char *usedata, size_t usedatalen,
    const unsigned char *symkey)
{
	N("mao_received_symkey called! (%p)", b);
}

const char*
mao_otr_error_message(void *b, ConnContext *ctx, OtrlErrorCode err_code)
{
	N("mao_otr_error_message called! (%p)", b);
	return NULL;
}

void 
mao_otr_error_message_free(void *b, const char *err_msg)
{
	N("mao_otr_error_message_free called! (%p)", b);
}

const char*
mao_resent_msg_prefix(void *b, ConnContext *ctx)
{
	N("mao_resent_msg_prefix called! (%p)", b);
	return NULL;
}

void 
mao_resent_msg_prefix_free(void *b, const char *prefix)
{
	N("mao_resent_msg_prefix_free called! (%p)", b);
}

void 
mao_handle_smp_event(void *b, OtrlSMPEvent smp_event, ConnContext *ctx,
    unsigned short progress_percent, char *question)
{
	N("mao_handle_smp_event called! (%p)", b);
}

void 
mao_handle_msg_event(void *b, OtrlMessageEvent msg_event, ConnContext *ctx,
    const char *msg, gcry_error_t err)
{
	N("mao_handle_msg_event called! (%p)", b);
}

void 
mao_create_instag(void *b, const char *accnm, const char *prot)
{
	N("mao_create_instag called! (%p)", b);
}

void 
mao_convert_msg(void *b, ConnContext *ctx, OtrlConvertType convert_type,
    char ** dest, const char *src)
{
	N("mao_convert_msg called! (%p)", b);
}

void 
mao_convert_free(void *b, ConnContext *ctx, char *dest)
{
	N("mao_convert_free called! (%p)", b);
}

void 
mao_timer_control(void *b, unsigned int interval)
{
	N("mao_timer_control called! (%p)", b);
}

OtrlMessageAppOps mao = {
	.policy = mao_policy,
	.create_privkey = mao_create_privkey,
	.is_logged_in = mao_is_logged_in,
	.inject_message = mao_inject_message,
	.update_context_list = mao_update_context_list,
	.new_fingerprint = mao_new_fingerprint,
	.write_fingerprints = mao_write_fingerprints,
	.gone_secure = mao_gone_secure,
	.gone_insecure = mao_gone_insecure,
	.still_secure = mao_still_secure,
	.max_message_size = mao_max_message_size,
	.account_name = mao_account_name,
	.account_name_free = mao_account_name_free,
	.received_symkey = mao_received_symkey,
	.otr_error_message = mao_otr_error_message,
	.otr_error_message_free = mao_otr_error_message_free,
	.resent_msg_prefix = mao_resent_msg_prefix,
	.resent_msg_prefix_free = mao_resent_msg_prefix_free,
	.handle_smp_event = mao_handle_smp_event,
	.handle_msg_event = mao_handle_msg_event,
	.create_instag = mao_create_instag,
	.convert_msg = mao_convert_msg,
	.convert_free = mao_convert_free,
	.timer_control = mao_timer_control
};

