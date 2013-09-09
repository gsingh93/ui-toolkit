#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <stdint.h>
#include <set>

#include <xcb/xcb.h>

using namespace std;

class GraphicsContext;

class Window {
public:
    friend GraphicsContext;

    Window(int16_t x, int16_t y, uint16_t width, uint16_t height,
           uint16_t borderWidth);
    ~Window();

    void show();
    void setBackgroundColor(uint32_t backPixel);
    void setEventMask(uint32_t eventMask);

private:
    int16_t x, y, width, height, borderWidth;

    set<xcb_cw_t> properties;

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

    uint32_t* genValues();
};

#endif
