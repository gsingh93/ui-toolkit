#include <xcb/xcb.h>
#include <unistd.h>

#include "window.h"

int main() {
    Window window(0, 0, 150, 150, 10);
    window.setBackgroundColor(0xFFFF00);
    window.setEventMask(XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS);
    window.show();
    pause();
}
