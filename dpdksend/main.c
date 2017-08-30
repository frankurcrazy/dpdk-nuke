#include <stdio.h>
#include <stdlib.h>

#include <rte_eal.h>
#include <rte_common.h>
#include <rte_log.h>

#define RTE_LOGTYPE_DPDKSEND RTE_LOGTYPE_USER1


int main(int argc, char** argv)
{
    int ret = 0;

    /* Initiialize EAL */
    ret = rte_eal_init(argc, argv);
    if (ret < 0) {
        rte_exit(EXIT_FAILURE, "Failed to initialize EAL.\n");
    }
    argc -= ret;



    return 0;
}
