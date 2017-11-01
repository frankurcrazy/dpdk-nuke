/**
  DPDK Nuke is a very simple packet generator
  that can generate line-rate CBR traffic with 
  various packet sizes
**/
#include <stdio.h>
#include <stdlib.h>

#include <rte_eal.h>
#include <rte_common.h>
#include <rte_log.h>
#include <rte_pci.h>

#include "dpdksend.h"

/* Global variables */
static dpdksend_ctx_t ctx;

int main(int argc, char** argv)
{
    int ret = 0;

    /* Initiialize EAL */
    ret = rte_eal_init(argc, argv);
    if (ret < 0) {
        rte_exit(EXIT_FAILURE, "Failed to initialize EAL.\n");
    }
    argc -= ret;

    dpdksend_ctx_init(&ctx, argv);

    return 0;
}
