#include "button.h"
#include "connection.h"
#include "util.h"
#include "window.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <cairo-xcb.h>

using namespace std;

xcb_visualtype_t *get_root_visual_type(xcb_screen_t *s) {
    xcb_visualtype_t *visual_type = NULL;
    xcb_depth_iterator_t depth_iter;

    depth_iter = xcb_screen_allowed_depths_iterator(s);

    for(;depth_iter.rem; xcb_depth_next(&depth_iter)) {
            xcb_visualtype_iterator_t visual_iter;

            visual_iter = xcb_depth_visuals_iterator(depth_iter.data);
            for(;visual_iter.rem;xcb_visualtype_next(&visual_iter)) {
                if(s->root_visual == visual_iter.data->visual_id) {
                    visual_type = visual_iter.data;
                    break;
                }
            }
    }

    return visual_type;
}

Button::Button(uint16_t width, uint16_t height, string label)
    : Widget(width, height), label(move(label)) {
    con = getConnection();
}

bool Button::handleEvent(xcb_generic_event_t *event) {
    switch (event->response_type) {
        case XCB_BUTTON_PRESS: {
            xcb_button_press_event_t *bp = (xcb_button_press_event_t *) event;
            if (mouseDownListener != 0 && isInBounds(bp->event_x, bp->event_y)) {
                mouseDownListener();
                return true;
            }
            break;
        }
    }

    return false;
}

void Button::setMouseDownListener(MouseDownListener listener) {
    mouseDownListener = listener;
}

void Button::draw(uint16_t x, uint16_t y, cairo_surface_t *surface) const {
    cairo_t *cr;

    cr = cairo_create(surface);

    cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);

    cairo_rectangle(cr, x, y, width, height);
    cairo_fill(cr);

    if (cairo_status(cr)) {
        printf("Error: %s\n", cairo_status_to_string(cairo_status(cr)));
        exit(1);
    }

    cairo_set_source_rgb(cr, 0.1, 0.1, 0.1);
    cairo_set_font_size(cr, 13);
    cairo_move_to(cr, 40, 40);
    cairo_show_text(cr, label.c_str());

    cairo_destroy(cr);

    xcb_flush(con);
}
