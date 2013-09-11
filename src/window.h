#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <stdint.h>
#include <set>
#include <list>

#include <xcb/xcb.h>

#include "button.h"

using namespace std;

class GraphicsContext;

class ChildElement {
public:
    ChildElement(uint16_t x, uint16_t y, Widget *const w) : x(x), y(y), w(w) {}
    uint16_t x;
    uint16_t y;
    Widget *const w;
};

class Window {
public:
    friend GraphicsContext;
    friend Button;

    Window(int16_t x, int16_t y, uint16_t width, uint16_t height,
           uint16_t borderWidth);
    ~Window();

    void show();
    void addChild(uint16_t x, uint16_t y, Widget *const w);
    void setBackgroundColor(uint32_t backPixel);
    void setEventMask(uint32_t eventMask);

private:
    void startEventLoop();
    bool sendEvent(xcb_generic_event_t *event);
    void draw();
    uint32_t* genValues();

    int16_t x, y;
    uint16_t width, height, borderWidth;

    set<xcb_cw_t> properties;
    list<ChildElement*> children;

    xcb_connection_t *con;
    xcb_window_t     win;
    xcb_screen_t     *screen;

    uint32_t mask;

    uint32_t backPixmap;
    uint32_t backPixel;
    uint32_t borderPixmap;
    uint32_t borderPixel;
    uint32_t bitGravity;
    uint32_t winGravity;
    uint32_t backingStore;
    uint32_t backingPixel;
    uint32_t overrideRedirect;
    uint32_t saveUnder;
    uint32_t eventMask;
    uint32_t dontPropagate;
    uint32_t colormap;
    uint32_t cursor;
};

#endif
