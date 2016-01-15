#ifndef _LIBTUI_TEXT_HH_
#define _LIBTUI_TEXT_HH_

#include <algorithm>
#include <string>

namespace tui {
  enum class TextAlign {
    left, center, right
  };

  std::string fit_string_in_gap(std::string text, size_t width, TextAlign align = TextAlign::left, std::string replacement = "...");
}

#endif /* end of include guard: _LIBTUI_TEXT_HH_ */
