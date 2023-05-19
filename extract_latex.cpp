/** \file

    Extract a LaTeX/Beamer presentation from a bigger set of presentations.

    This is very hardcoded for me but could be useful to some people.

    \author Ronan Keryell
*/
#include <filesystem>
#include <fstream>
#include <string>

#include <fmt/ranges.h>
#include <range/v3/all.hpp>

namespace stdr = ranges;

constexpr std::string input_token { "INPUT " };

std::string beamer_directory {
  "/home/rkeryell/Xilinx/Projects/Khronos/SYCL/Presentations/"
};

std::string export_directory { "SYCLcon_2023_tutorial/" };

std::string slide_directory { beamer_directory +
                              "2023/2023-04-18-SYCLcon-tutorial-introduction" };

int main(int argc, char* argv[]) {
  // Parse the LaTeX output containing the list of used files
  std::ifstream file { "2023-04-18-SYCLcon-tutorial-introduction.fls" };
  auto files =
      stdr::getlines_view { file } | stdr::views::filter([](auto&& line) {
        // Select only the input files
        return line.starts_with(input_token);
      }) |
      stdr::views::transform([](auto&& line) {
        // Canonicallize the file names. Beware that it resolve
        // symbolic links, so do not use symbolic links in the .tex
        // file
        return std::string { std::filesystem::canonical(
            std::filesystem::path { line.substr(input_token.size()) }) };
      }) |
      stdr::to<std::vector>;
  stdr::sort(files);
  auto interesting_files =
      files | stdr::views::unique | stdr::views::filter([](auto&& line) {
        // Just keep the interesting files. Could be optimized...
        return !(line.starts_with("/etc") || line.starts_with("/usr") ||
                 line.starts_with("/var") || line.ends_with(".aux") ||
                 line.ends_with(".nav") || line.ends_with(".out") ||
                 line.ends_with(".toc") || line.ends_with(".vrb")) &&
               line.starts_with(beamer_directory);
      });
  // There is a lot of duplication since I did not get how to generate
  // optional elements on the fly
  auto svg_files =
      interesting_files | stdr::views::filter([](auto&& line) {
        // Select only the input files
        return exists(std::filesystem::path { line }.replace_extension("svg"));
      }) |
      stdr::views::transform(
          // Change the extension of the selected file
          [](auto&& line) {
            return std::string {
              std::filesystem::path { line }.replace_extension("svg")
            };
          });
  auto files_to_copy =
      stdr::views::concat(interesting_files, svg_files) |
      stdr::views::transform(
          // Remove the beamer directory path
          [](auto&& line) { return line.substr(beamer_directory.size()); });
  fmt::print("Files to copy: {}\n", files_to_copy);

  // Build a list of all the file directories
  auto directories_to_create =
      files_to_copy | stdr::views::transform([](auto&& file) {
        return std::string { std::filesystem::path { file }.parent_path() };
      }) |
      stdr::to<std::vector>;
  stdr::sort(directories_to_create);
  // So we can recreate a similar directory hierarchy
  auto unique_directories_to_create =
      directories_to_create | stdr::views::unique;
  fmt::print("unique_directories_to_create: {}\n",
             unique_directories_to_create);
  for (auto& d : directories_to_create)
    std::filesystem::create_directories(export_directory + d);

  // The copy the required files at a similar place under the export
  // directory
  for (const auto& f : files_to_copy)
    std::filesystem::copy(std::filesystem::path { beamer_directory } /
                              std::filesystem::path { f },
                          std::filesystem::path { export_directory } /
                              std::filesystem::path { f }.parent_path());
}
