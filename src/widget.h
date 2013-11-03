#ifndef _WIDGET_H_
#define _WIDGET_H_

#include <stdint.h>
#include <stdio.h>

#include <cairo-xcb.h>

class Window;

class Widget {
public:
    Widget(uint16_t width, uint16_t height) : width(width), height(height) {}
    virtual void draw(uint16_t x, uint16_t y, cairo_surface_t *surface) const = 0;
    virtual bool handleEvent(xcb_generic_event_t *event) = 0;

    uint16_t x, y;

protected:
    uint16_t width, height;

    bool isInBounds(uint16_t x, uint16_t y) {
        printf("%d, %d, %d, %d\n", this->x, this->y, width, height);
        if (x >= this->x && y >= this->y && x <= this->x + width &&
            y <= this->y + height) {
            return true;
        }
        return false;
    }
};

#endif
