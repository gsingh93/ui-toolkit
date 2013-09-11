#ifndef _UTIL_H_
#define _UTIL_H_

#include <xcb/xcb.h>

extern xcb_visualtype_t *get_root_visual_type(xcb_screen_t *s);

#endif
