#include <xcb/xcb.h>

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
