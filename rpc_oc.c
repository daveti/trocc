/*
 * rpc_oc.c
 * RPC open/close context illustration
 * All the headers files are from Trousers
 * Sep 2, 2013
 * root@davejingtian.org
 * http://davejingtian.org
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

/* NOTE: the order of include does matter! */

#include "trousers/tss.h"
#include "trousers/trousers.h"

#include "hosttable.h"
#include "tcsd_wrap.h"
#include "tcsd.h"
#include "threads.h"
#include "tcs_tsp.h"
#include "rpc_tcstp.h"
#include "rpc_tcstp_tsp.h"

#define REMOTE_TCSD_IP		"128.223.6.113"

int main()
{
	TSS_RESULT res;
	struct host_table_entry hte;
	UINT32 tpm_version;
	TCS_CONTEXT_HANDLE tcsContext;

	/* Init host table entry */
/*
struct host_table_entry {
	struct host_table_entry *next;
	TSS_HCONTEXT tspContext;
	TCS_CONTEXT_HANDLE tcsContext;
	BYTE *hostname;
	int type;
	int socket;
	struct tcsd_comm_data comm;
	MUTEX_DECLARE(lock);
};
struct tcsd_packet_hdr {
	UINT32 packet_size;
	union {
		UINT32 ordinal;
		UINT32 result;
	} u;
	UINT32 num_parms;
	UINT32 type_size;
	UINT32 type_offset;
	UINT32 parm_size;
	UINT32 parm_offset;
} STRUCTURE_PACKING_ATTRIBUTE;

struct tcsd_comm_data {
	BYTE *buf;
	UINT32 buf_size;
	struct tcsd_packet_hdr hdr;
} STRUCTURE_PACKING_ATTRIBUTE;

#define TCSD_INIT_TXBUF_SIZE	1024
#define TCSD_INCR_TXBUF_SIZE	4096
*/
	memset(&hte, 0, sizeof(hte));
	hte.hostname = (BYTE *)strdup(REMOTE_TCSD_IP);
	hte.comm.buf = (BYTE *)malloc(TCSD_INCR_TXBUF_SIZE);
	hte.comm.buf_size = TCSD_INCR_TXBUF_SIZE;

	/* RPC open the context */
	res = RPC_OpenContext_TP(&hte, &tpm_version, &tcsContext);
	if (res == TSS_SUCCESS)
	{
		printf("RPC_OpenContext_TP succeeded, tpm_version [%u], tcsContext [0x%x]\n",
			tpm_version, tcsContext);
		/* Save the tcsContext into hte */
		hte.tcsContext = tcsContext;
	}
	else
	{
		printf("RPC_OpenContext_TP failed\n");
		free(hte.comm.buf);
		return -1;
	}

	/* Debug */
	printf("hte debug: \n"
		"next = %p\n"
		"tspContext = 0x%x\n"
		"tcsContext = 0x%x\n"
		"hostname = %s\n"
		"type = %d\n"
		"socket = %d\n",
		hte.next,
		hte.tspContext,
		hte.tcsContext,
		hte.hostname,
		hte.type,
		hte.socket);

	/* RPC close the context */
	memset(hte.comm.buf, 0, TCSD_INCR_TXBUF_SIZE);
	hte.comm.buf_size = TCSD_INCR_TXBUF_SIZE;
	res = RPC_CloseContext_TP(&hte);
	if (res == TSS_SUCCESS)
		printf("RPC_CloseContext_TP succeeded\n");
	else
		printf("RPC_CloseContext_TP failed\n");

	/* Free the comm buff */
	free(hte.comm.buf);

	return 0;
}
