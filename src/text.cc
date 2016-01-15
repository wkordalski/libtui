#include "text.hh"

namespace tui {
  std::string fit_string_in_gap(std::string text, size_t width, TextAlign align, std::string replacement) {
    if(text.size() <= width) return text;
    if(replacement.size() > width) return replacement.substr(0, width);
    if(replacement.size() == width) return replacement;
    size_t text_part = std::max(size_t(0), width - replacement.size());
    switch(align) {
      case TextAlign::left:
      case TextAlign::center:
        // simple
        return text.substr(0, text_part) + replacement;
      case TextAlign::right:
        // right
        return replacement + text.substr(text.size() - text_part, text_part);
    }
  }
}
