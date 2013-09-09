#include <stdint.h>
#include <cassert>
#include <iostream>
#include <set>

#include <xcb/xcb.h>

#include "window.h"

using namespace std;

Window::Window(int16_t x, int16_t y, uint16_t width, uint16_t height,
               uint16_t borderWidth) : x(x), y(y), width(width), height(height),
               borderWidth(borderWidth), mask(0) {
    con = xcb_connect(NULL, NULL);
    win = xcb_generate_id(con);
    screen = xcb_setup_roots_iterator(xcb_get_setup(con)).data;
}

void Window::show() {
    assert(con);
    assert(win);

    uint32_t *values = genValues();
    xcb_create_window(con,
                      XCB_COPY_FROM_PARENT,          /* depth               */
                      win,
                      screen->root,                  /* parent window       */
                      x, y,
                      width, height,
                      borderWidth,
                      XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
                      screen->root_visual,           /* visual              */
                      mask, values);
    delete[] values;

    xcb_map_subwindows(con, win);
    xcb_map_window(con, win);
    xcb_flush(con);
}

void Window::setBackgroundColor(uint32_t backPixel) {
    mask |= XCB_CW_BACK_PIXEL;
    this->backPixel = backPixel;
    properties.insert(XCB_CW_BACK_PIXEL);
}

void Window::setEventMask(uint32_t eventMask) {
    mask |= XCB_CW_EVENT_MASK;
    this->eventMask = eventMask;
    properties.insert(XCB_CW_EVENT_MASK);
}

uint32_t* Window::genValues() {
    uint32_t *values = new uint32_t[properties.size()];

    int i = 0;
    for (set<xcb_cw_t>::iterator it = properties.begin();
        it != properties.end(); it++) {
        switch (*it) {
        case XCB_CW_EVENT_MASK:
            values[i] = eventMask;
            break;
        case XCB_CW_BACK_PIXEL:
            values[i] = backPixel;
            break;
        default:
            assert(false);
        }
        i++;
    }

    return values;
}

Window::~Window() {
    assert(con);
    xcb_disconnect(con);
}
