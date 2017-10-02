#ifndef __DPDKSEND_H__
#define __DPDKSEND_H__

#define RTE_LOGTYPE_DPDKSEND RTE_LOGTYPE_USER1
#define DPDKSEND_TX_POOL_BASENAME "txpool_%d_%d"
#define DPDKSEND_RX_POOL_NAME "rxpool_%d_%d"

#define DPDKSEND_STATE_UNINITIALIZED 0
#define DPDKSEND_STATE_INITIALIZED 1

#define DPDKSEND_FAILED
#define DPDKSEND_SUCCESS

#define SUCCESS(ret) (ret == DPDKSEND_SUCCESS)
#define FAILED(ret) (ret == DPDKSEND_FAILED)

#define RTE_LOGTYPE_DPDKSEND RTE_LOGTYPE_USER8

#include <rte_log.h>
#include <rte_ethdev.h>

/**
  * dpdksend port structure
  */
typedef struct dpdksend_port {
    uint8_t state :1;
    struct rte_mempool *tx_pool;
    struct rte_mempool *rx_pool;
    struct rte_eth_link link;

    uint8_t nb_txq;
    uint8_t nb_rxq;
} dpdksend_port_t;

/**
  * dpdksend context
  */
typedef struct dpdksend_ctx {
    uint16_t state :1;

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
    RTE_LOG(DEBUG, DPDKSEND, "Initializing mempool for port: %u, size: %u ctx: %p .\n"
                    port_id, size, ctx);

    dpdksend_port_t *port = NULL;

    port = ctx->ports + port_id;

    /* Initialize */

    return DPDKSEND_SUCCESS;
}

/**
  * 
  */
static inline int dpdksend_ctx_init(dpdksend_ctx_t *ctx, char **argv)
{
    if (!ctx) return DPDKSEND_FAILED;

    if (ctx->state == DPDKSEND_STATE_INITIALIZED) {
        RTE_LOG(WARNING, DPDKSEND, "Dpdksend context is already initialized. \n"
                         "But I'm going to initialize anyway. \n");
    }

    RTE_LOG(DEBUG, DPDKSEND, "Dpdksend context state set to uninitialize.\n");
    ctx->state = DPDKSEND_STATE_UNINITIALIZED;

    RTE_LOG(DEBUG, DPDKSEND, "Dpdksend context state set to initialize.\n");
    ctx->state = DPDKSEND_STATE_INITIALIZED
}


#endif
