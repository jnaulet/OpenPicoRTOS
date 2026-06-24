#ifndef HAL_CAN_H
#define HAL_CAN_H

#include <errno.h>

#include <stddef.h>
#include "picoRTOS_types.h"

struct can;

/* CAN IDs */
typedef unsigned int can_id_t;

/* Constants: General CAN constants
 *
 * CAN_ID_COUNT - Number of 11-bit identifiers
 * CAN_EXTID_COUNT - Number of 29-bit identifiers
 * CAN_DATA_COUNT - Maximum size of a CAN message (8)
 */
#define CAN_ID_COUNT    2048        /* 11-bit identifier */
#define CAN_EXTID_COUNT 536870912   /* 29-bit identifier */
#define CAN_DATA_COUNT  8

#define CAN_ACCEPT_ANY        0x0
#define CAN_ACCEPT_STRICT     0x7ff
#define CAN_ACCEPT_STRICT_EXT 0x1fffffff

typedef enum {
    CAN_TX_AUTO_ABORT_OFF,
    CAN_TX_AUTO_ABORT_ON,
    CAN_TX_AUTO_ABORT_COUNT
} can_tx_auto_abort_t;

typedef enum {
    CAN_RX_OVERWRITE_OFF,
    CAN_RX_OVERWRITE_ON,
    CAN_RX_OVERWRITE_COUNT
} can_rx_overwrite_t;

typedef enum {
    CAN_LOOPBACK_OFF,
    CAN_LOOPBACK_ON,
    CAN_LOOPBACK_COUNT
} can_loopback_t;

struct can_settings {
    unsigned long bitrate;
    size_t tx_mailbox_count; /* how many mbs are dedicated to tx */
    can_tx_auto_abort_t tx_auto_abort;
    can_rx_overwrite_t rx_overwrite;
    can_loopback_t loopback;
};

/**
 * int **can_setup**(**struct can** \*<ins>ctx</ins>, 
 * **const struct can_settings** \*<ins>settings</ins>);
 * > Configures a CAN interface according to <ins>settings</ins>
 * ### RETURN
 * > Returns 0 in case of success, -errno otherwise
 */
int can_setup(struct can *ctx, const struct can_settings *settings);

/**
 * int **can_accept**(**struct can** \*<ins>ctx</ins>, 
 * **can_id_t** <ins>id</ins>, **can_id_t** <ins>accept_mask</ins>);
 * > Accepts messages according to <ins>id</ins> & <ins>accept_mask</ins> on
 * > can interface <ins>ctx</ins>
 *
 * ### NOTES
 * > CAN_ACCEPT_STRICT(_EXT) as acceptance mask means only the specified ID 
 * > will be accepted.<br>
 * > Acceptance masks act as a logical AND between the received ID, the
 * > acceptance mask and the specified id
 *
 * > - CAN_ACCEPT_STRICT(_EXT) will only accept the specified ID
 * > - CAN_ACCEPT_ANY will accept any message (mostly for debug purposes)
 *
 * ### RETURN
 * > Returns 0 in case of success, -errno otherwise
 */
int can_accept(struct can *ctx, can_id_t id, can_id_t accept_mask);

/**
 * int **can_write**(**struct can** \*<ins>ctx</ins>, 
 * **can_id_t** <ins>id</ins>, **const void** \*<ins>buf</ins>, 
 * **size_t** <ins>n</ins>);
 * > Sends a message on the CAN interface
 *
 * ### RETURN
 * > Returns the number of bytes sent in case of success, 
 * > -errno otherwise
 */
int can_write(struct can *ctx, can_id_t id, const void *buf, size_t n);

/**
 * int **can_read**(**struct can** \*<ins>ctx</ins>, 
 * **can_id_t** \*<ins>id</ins>, **void** \*<ins>buf</ins>,
 * **size_t** <ins>n</ins>);
 * > Receives a message from the CAN interface
 *
 * ### RETURN
 * > Returns the number of bytes received in case of success, 
 * > -errno otherwise
 */
int can_read(struct can *ctx, can_id_t *id, void *buf, size_t n);

/**
 * int **can_request_frame**(**struct can** \*<ins>ctx</ins>, **can_id_t** <ins>id</ins>);
 * > Send a RTR frame with the specified <ins>id</ins>
 *
 * ### RETURN
 * > Returns 0 if success, -errno otherwise
 */
int can_request_frame(struct can *ctx, can_id_t id);

#endif
