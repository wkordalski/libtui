#include "positioning.hh"

#include "tui.hh"

namespace tui {
  Rectangle center_screen_locator(Rectangle parent, Application *app, Size target) {
    Size screen = app->get_screen_size();
    return {{(screen.w-target.w)/2, (screen.h-target.h)/2}, target};
  }
}
