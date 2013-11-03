#include "window.h"
#include "connection.h"
#include "util.h"

#include <cassert>
#include <iostream>
#include <set>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include <cairo-xcb.h>
#include <xcb/xcb.h>

using namespace std;

Window::Window(int16_t x, int16_t y, uint16_t width, uint16_t height,
               uint16_t borderWidth, string title) : x(x), y(y), width(width),
               height(height), borderWidth(borderWidth), mask(0), title(move(title)) {
    con = getConnection();
    assert(con != NULL);
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
    xcb_change_property(con, XCB_PROP_MODE_REPLACE, win, XCB_ATOM_WM_NAME,
                        XCB_ATOM_STRING, 8, title.length(), title.c_str());
    xcb_map_window(con, win);
    xcb_flush(con);

    draw();

    // TODO: Put in event thread
    startEventLoop();
}

void Window::draw() {
    cairo_surface_t *surface;

    surface = cairo_xcb_surface_create(con, win, ::get_root_visual_type(screen),
                                       width, height);

    for (list<ChildElement*>::iterator it = children.begin(); it != children.end(); it++) {
        ChildElement *e = *it;
        e->w->draw(e->x, e->y, surface);
    }

    xcb_flush(con);
}

void Window::addChild(uint16_t x, uint16_t y, Widget *const w) {
    w->x = x;
    w->y = y;
    children.push_back(new ChildElement(x, y, w));
}

void print_modifiers (uint32_t mask) {
    const char *MODIFIERS[] = {
            "Shift", "Lock", "Ctrl", "Alt",
            "Mod2", "Mod3", "Mod4", "Mod5",
            "Button1", "Button2", "Button3", "Button4", "Button5"
    };

    printf ("Modifier mask: ");
    for (const char **modifier = MODIFIERS ; mask; mask >>= 1, ++modifier) {
        if (mask & 1) {
            printf (*modifier);
        }
    }
    printf ("\n");
}

void Window::startEventLoop() {
    while (1) {
        xcb_generic_event_t *event = xcb_wait_for_event(con);

        if (!event) {
            return;
        }

        switch (event->response_type) {
            case XCB_EXPOSE: {
                xcb_expose_event_t *expose = (xcb_expose_event_t *) event;
                draw();
                printf ("Window %ld exposed. Region to be redrawn at location (%d,%d), with dimension (%d,%d)\n",
                        expose->window, expose->x, expose->y, expose->width, expose->height);
                break;
            }
            case XCB_BUTTON_PRESS: {
                if (sendEvent(event)) {
                    continue;
                }

                xcb_button_press_event_t *bp = (xcb_button_press_event_t *) event;
                print_modifiers(bp->state);

                switch (bp->detail) {
                case 4:
                    printf ("Wheel Button up in window %ld, at coordinates (%d,%d)\n",
                            bp->event, bp->event_x, bp->event_y);
                    break;
                case 5:
                    printf ("Wheel Button down in window %ld, at coordinates (%d,%d)\n",
                            bp->event, bp->event_x, bp->event_y);
                    break;
                default:
                    printf ("Button %d pressed in window %ld, at coordinates (%d,%d)\n",
                            bp->detail, bp->event, bp->event_x, bp->event_y);
                    break;
                }
                break;
            }
            case XCB_BUTTON_RELEASE: {
                xcb_button_release_event_t *br = (xcb_button_release_event_t *) event;
                print_modifiers(br->state);

                printf ("Button %d released in window %ld, at coordinates (%d,%d)\n",
                        br->detail, br->event, br->event_x, br->event_y);
                break;
            }
            case XCB_MOTION_NOTIFY: {
                xcb_motion_notify_event_t *motion = (xcb_motion_notify_event_t *) event;

                printf ("Mouse moved in window %ld, at coordinates (%d,%d)\n",
                        motion->event, motion->event_x, motion->event_y);
                break;
            }
            case XCB_ENTER_NOTIFY: {
                xcb_enter_notify_event_t *enter = (xcb_enter_notify_event_t *) event;

                printf ("Mouse entered window %ld, at coordinates (%d,%d)\n",
                        enter->event, enter->event_x, enter->event_y);
                break;
            }
            case XCB_LEAVE_NOTIFY: {
                xcb_leave_notify_event_t *leave = (xcb_leave_notify_event_t *) event;

                printf ("Mouse left window %ld, at coordinates (%d,%d)\n",
                        leave->event, leave->event_x, leave->event_y);
                break;
            }
            case XCB_KEY_PRESS: {
                xcb_key_press_event_t *kp = (xcb_key_press_event_t *) event;
                ::print_modifiers(kp->state);

                printf ("Key pressed in window %ld\n",
                        kp->event);
                break;
            }
            case XCB_KEY_RELEASE: {
                xcb_key_release_event_t *kr = (xcb_key_release_event_t *) event;
                ::print_modifiers(kr->state);

                printf ("Key released in window %ld\n",
                        kr->event);
                break;
            }
            default: {
                /* Unknown event type, ignore it */
                printf ("Unknown event: %d\n",
                        event->response_type);
                break;
            }

            free (event);
        }
    }
}

bool Window::sendEvent(xcb_generic_event_t *event) {
    for (list<ChildElement*>::iterator it = children.begin(); it != children.end(); it++) {
        if ((*it)->w->handleEvent(event)) {
            return true;
        }
    }

    return false;
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
