This is a small hack to extract a LaTeX/Beamer presentation from a big
pile of presentations

This is very hardcoded but it might be useful to some people.
This has been used to generate https://github.com/keryell/SYCLcon_2023_tutorial

Configure the build system by creating a `build` subdirectory::

  cmake -B build . -DCMAKE_EXPORT_COMPILE_COMMANDS=1

Generate the executable in `build` subdirectory::

  cmake --build build --verbose --parallel `nproc`

Run the main command from the directory where the main LaTeX source
is::

  cd .../2023/2023-04-18-SYCLcon-tutorial-introduction
  .../extract_latex/build/extract_latex

and it should generate the local directory `SYCLcon_2023_tutorial`
with the minimal set of files needed to compile the presentation.

Finish with manual copy of the Makefile infrastructure::

  cp Makefile SYCLcon_2023_tutorial/2023/2023-04-18-SYCLcon-tutorial-introduction
  cp -a ../../Makefiles SYCLcon_2023_tutorial

Edit the Makefile to remove the SYCL binary compilation used for the
code sample but not used by the presentation itself.

There are some slides which are skipped in the tutorial with some
private content to be removed with::

  ../extract_latex/build/remove_skipped_slides SYCLcon_2023_tutorial/2023/2023-04-18-SYCLcon-tutorial-introduction/2023-04-18-SYCLcon-tutorial-introduction.tex
