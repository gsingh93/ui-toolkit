#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "widget.h"

#include <string>

#include <cairo-xcb.h>

using namespace std;

class Window;

typedef void (*MouseDownListener)();

class Button : public Widget {
    friend Window;

public:
    Button(uint16_t height, uint16_t width, string label);

    void draw(uint16_t x, uint16_t y, cairo_surface_t *surface) const;
    bool handleEvent(xcb_generic_event_t *event);
    void setMouseDownListener(MouseDownListener listener);

private:
    xcb_connection_t *con;
    MouseDownListener mouseDownListener;
    string label;
};

#endif
