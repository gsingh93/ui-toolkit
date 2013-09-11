#include <xcb/xcb.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "window.h"
#include "button.h"

void buttonClicked() {
    printf("Clicked\n");
}

int main() {
    Window window(0, 0, 150, 150, 10);
    Button *b = new Button(100, 30);
    b->setMouseDownListener(buttonClicked);

    window.setBackgroundColor(0xFFFF00);
    window.setEventMask(XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS);
    window.addChild(20, 20, b);
    window.show();
}
