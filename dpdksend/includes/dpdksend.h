#ifndef __DPDKSEND_H__
#define __DPDKSEND_H__

#include <unistd.h>
#include <getopt.h>

/** Custom log type */
#define RTE_LOGTYPE_DPDKSEND RTE_LOGTYPE_USER8

/** Macros to check whether operation success */
#define SUCCESS(ret) (ret == DPDKSEND_SUCCESS)
#define FAILED(ret) (ret == DPDKSEND_FAILED)

/** Return status definition */
#define DPDKSEND_FAILED     EXIT_FAILURE
#define DPDKSEND_SUCCESS    EXIT_SUCCESS

enum {
    false,
    true
};

/** DPDKSEND context state */
enum {
    DPDKSEND_STATE_UNINITIALIZED,
    DPDKSEND_STATE_INITIALIZED,
};

/** Memory pool naming format 
 *  
 *  The naming convention of memory pool is shown as below,
 *  for TX pool the name is prefixed with "txpool", and for 
 *  RX pool the name is prefixed with "rxpool".
 *
 *  The format of the name is "{tx|rx}pool_{nic_id}_{queue_id}",
 *  where "nic_id" denotes the identifier of network interfaces and
 *  "queue_id" denotes the queue identifier, this is used to seperate
 *  memory access in multiple queue scenario.
 */
#define DPDKSEND_TX_POOL_BASENAME "txpool_%d_%d"
#define DPDKSEND_RX_POOL_NAME "rxpool_%d_%d"

#include <rte_log.h>
#include <rte_ethdev.h>

/**
  * dpdksend port structure
  */
typedef struct dpdksend_port {

    struct rte_mempool *tx_pool;            /** Memory pools for TXQs */
    struct rte_mempool *rx_pool;            /** Memory pools for RXQs */
    struct rte_eth_link link;               /** Link status information */

    uint8_t nb_txq;                         /** Number of TXQs */
    uint8_t nb_rxq;                         /** Number of RXQs */

} dpdksend_port_t;

/**
  * dpdksend context
  */
typedef struct dpdksend_ctx {
    uint16_t state;
    
    uint32_t port_mask;
    uint8_t nb_ports;

    dpdksend_port_t ports[RTE_MAX_ETHPORTS];

} dpdksend_ctx_t;


/** 
  * Interrupt event callback
  *
  * This function is called upon link state change
  *
  */
static inline void dpdksend_ev_callback(uint8_t port_id, enum rte_eth_event_type type, void *params)
{
    struct rte_eth_link *link_ptr = NULL;
    dpdksend_ctx_t *ctx = (dpdksend_ctx_t *) params;

    link_ptr = &(ctx->ports[port_id].link);
    
    switch (type) {
        case RTE_ETH_EVENT_INTR_LSC:
            rte_eth_link_get_nowait(port_id, link_ptr);
            if (link_ptr->link_status) {
                RTE_LOG(INFO, DPDKSEND, "Port %u is UP!. Speed %u [%s].\n", port_id, 
                                link_ptr->link_speed, 
                                link_ptr->link_duplex?"Full-Duplex":"Half-Duplex");

            } else {
                RTE_LOG(INFO, DPDKSEND, "Port %u is DOWN!", port_id);
            }

            break;
        default:
            // Not supported
            RTE_LOG(WARNING, DPDKSEND, "RTE Event %d is received but not handled.\n",
                             type);
    }
}

/**
  * Initialize memory pool for each queue on the designated NIC with size 
  *
  * @return int success
  */
static inline int dpdksend_mempool_init(dpdksend_ctx_t *ctx, uint8_t port_id, uint16_t size)
{
    if (!ctx) return DPDKSEND_FAILED;
    RTE_LOG(DEBUG, DPDKSEND, "Initializing mempool for port: %u, size: %u ctx: %p .\n",
                    port_id, size, ctx);

    dpdksend_port_t *port = NULL;

    port = ctx->ports + port_id;

    /* Initialize */

    return DPDKSEND_SUCCESS;
}

/** Initializer for the dpdksend context
  * 
  */
static inline int dpdksend_ctx_init(dpdksend_ctx_t *ctx, char **argv)
{
    if (!ctx) return DPDKSEND_FAILED;

    if (ctx->state == DPDKSEND_STATE_INITIALIZED) {
        RTE_LOG(WARNING, DPDKSEND, 
                         "Dpdksend context is already initialized.\n"
                         "But I'm going to initialize it anyway. \n");
    }

    /** Before initializing dpdksend context,
      * we reset state to UNINITIALIZED.
      */
    RTE_LOG(DEBUG, DPDKSEND, "Dpdksend context state set to uninitialize.\n");
    ctx->state = DPDKSEND_STATE_UNINITIALIZED;

    /** Main initialization process */


    /** End of main initialization process */

    /** After successful initialization we set state 
      * to INITIALIZED
      */
    RTE_LOG(DEBUG, DPDKSEND, "Dpdksend context state set to initialize.\n");
    ctx->state = DPDKSEND_STATE_INITIALIZED;

    return DPDKSEND_SUCCESS;

    /** If error occurs during initialization,
      * this is the place to go
      */
    onerror:
    RTE_LOG(ERR, DPDKSEND, "Failed to initialize Dpdksend context.\n");
    ctx->state = DPDKSEND_STATE_UNINITIALIZED;

    return DPDKSEND_FAILED;
}

/** 
  *
  */
static inline int dpdksend_probe_interface(dpdksend_ctx_t *ctx);

/**
  *
  */
static inline int dpdksend_parse_args(dpdksend_ctx_t *ctx, int argc, char **args)
{
    int opt;
    int idx;
    struct option long_opts[] = {
        {"portmask", required_argument, NULL, 'p'},             /** Portmask in hex */
        {"portmap", required_argument, NULL, 'm'},              /** (TBD) Core assignment */
        {0, 0, 0, 0}
    };

    while ( (opt = getopt_long(argc, args, "p:m:", long_opts, &idx)) != -1 ) {
        switch (opt) {
            case 'm':
                /** Core assignment to each TXQ, RXQ */
                // TBD
            case 'p':
                dpdksend_parse_portmask(ctx, optarg);
                break;
            default:
                //
        }
    }
}

/** Parse portmask
  *
  */
static inline int dpdksend_parse_portmask(dpdksend_ctx_t *ctx, const char *portmask_str)
{
   uint32_t *portmask = ;


}
#endif
