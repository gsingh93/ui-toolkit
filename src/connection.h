#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <xcb/xcb.h>

extern xcb_connection_t *con;

static inline xcb_connection_t *getConnection() {
    if (con == NULL) {
        con = xcb_connect(NULL, NULL);
    }
    return con;
}

#endif
