#include "progressbar.hh"

namespace tui {
  ProgressBar::ProgressBar(Application *app) :
    Widget(app) {
    locator = [this] (Size s) -> std::pair<Point, Size>
      { return {this->position, this->size}; };
  }

  void ProgressBar::set_unknown_progress(bool is_unknown) {
    if(is_unknown == unknown) return;
    {
      std::lock_guard<std::mutex> lock(this->mutex);
      unknown = is_unknown;
    }
    if(unknown) {
      shift = -(size.w * mq_width / 100);
      ms_per_shift = 1000*100/((size.w == 0?1:size.w) * velocity);
      clock = std::thread(&ProgressBar::clock_procedure, this);
    } else {
      clock.join();
    }
  }
}
