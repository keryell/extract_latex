/** \file

    Censor the content of a list of files by removing everything between markup

    \ifPresentHiddenSlides
    ...
    ...
    \fi

    \author Ronan Keryell
*/
#include <filesystem>
#include <fstream>
#include <span>
#include <string>

#include <range/v3/all.hpp>

namespace stdr = ranges;

std::string start_skipping { R"(\ifPresentHiddenSlides)" };
std::string stop_skipping { R"(\fi)" };

int main(int argc, char* argv[]) {
  // Skip the first argument which is actually the program name
  for (auto const& a :
       std::span { argv + 1, static_cast<std::size_t>(argc - 1) }) {
    // Open each file from the names passed as argument
    std::ifstream input { a };
    // Start in non-censoring mode
    bool skip_line = false;
    auto lines = stdr::getlines_view { input } |
                 stdr::views::filter([&](auto&& line) {
                   if (skip_line) {
                     if (line.starts_with(stop_skipping))
                       // Think about not skipping the line next time
                       skip_line = false;
                     // But still skip this line
                     return false;
                   } else if (line.starts_with(start_skipping)) {
                     // Go into skipping line mode
                     skip_line = true;
                     // Including this line too
                     return false;
                   }
                   // Not in skipping mode: do not skip this line
                   return true;
                 }) |
                 // Put everything in memory to avoid race condition
                 // while rewriting the file in place
                 stdr::to<std::vector>;
    // Close the input before rewriting the same file
    input.close();
    std::ofstream output { a };
    for (auto const& line : lines)
      output << line << std::endl;
  }
}
