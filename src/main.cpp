#include "window.h"
#include "button.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <xcb/xcb.h>

void buttonClicked() {
    printf("Clicked\n");
}

int main() {
    Window window(0, 0, 150, 150, 10, "My Window");
    Button *b = new Button(100, 30, "Click Me!");
    b->setMouseDownListener(buttonClicked);

    window.setBackgroundColor(0xFFFF00);
    window.setEventMask(XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS);
    window.addChild(20, 20, b);
    window.show();
}
