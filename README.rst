gauge
-----

|Linux make-specs| |Windows make-specs| |MacOS make-specs| |Valgrind| |No Assertions| |Clang Format| |Cppcheck|

.. |Linux make-specs| image:: https://github.com/steinwurf/abacus/actions/workflows/linux_mkspecs.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/linux_mkspecs.yml
   
.. |Windows make-specs| image:: https://github.com/steinwurf/abacus/actions/workflows/windows_mkspecs.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/windows_mkspecs.yml

.. |MacOS make-specs| image:: https://github.com/steinwurf/abacus/actions/workflows/macos_mkspecs.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/macos_mkspecs.yml

.. |Clang Format| image:: https://github.com/steinwurf/abacus/actions/workflows/clang-format.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/clang-format.yml

.. |No Assertions| image:: https://github.com/steinwurf/abacus/actions/workflows/nodebug.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/nodebug.yml

.. |Valgrind| image:: https://github.com/steinwurf/abacus/actions/workflows/valgrind.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/valgrind.yml

.. |Cppcheck| image:: https://github.com/steinwurf/abacus/actions/workflows/cppcheck.yml/badge.svg
   :target: https://github.com/steinwurf/abacus/actions/workflows/cppcheck.yml

gauge is a flexible C++ benchmarking tool.

.. contents:: Table of Contents:
   :local:

Build
-----

We use the ``waf`` build system to build the gauge static library.
We have some additional tools which may be found at waf_

.. _waf: https://github.com/steinwurf/waf

If you already installed a C++14 compiler, git and python on your system,
then you can clone this repository to a suitable folder::

    git clone git@github.com:steinwurf/gauge.git

Configure and build the project::

    cd gauge
    python waf configure
    python waf build

Run the unit tests::

    python waf --run_tests

You should now have the gauge static lib and also its dependencies compiled
as static libs.

When building the static lib, waf will also build the ``gauge_example``
executable.

Depending on your platform you should be able to launch it by running::

  ./build/linux/examples/sample_benchmarks/gauge_example

Example Use
-----------

See various use cases in the ``examples`` folder. The following will be used to
explain the basic concepts of gauge. To try it out save the following code in a
file called ``main.cpp``::

  #include <gauge/gauge.hpp>

  #include <vector>

  BENCHMARK(MyTest, RunThis, 100)
  {
      std::vector<int> integers;
      for(int i = 0; i < 2048; ++i)
      {
          integers.push_back(i);
      }

      // This is where the clock runs
      RUN
      {
          for(uint i = 1; i < integers.size(); ++i)
          {
              integers[i] += integers[i-1];
          }
      }
  }

  int main(int argc, const char* argv[])
  {
      gauge::runner::add_default_printers();
      gauge::runner::run_benchmarks(argc, argv);
      return 0;
  }

In the above we use the ``BENCHMARK`` macro which takes 3 parameters:

1. The name of the test-case in this case ``MyTest``
2. The name of the benchmark in this case ``RunThis``
3. The number of runs to complete in this case 100.

The measurement will not start until we hit the ``RUN`` macro. Depending
on the type of benchmark (the default is time) the code inside ``RUN`` will
be executed several times (we refer to this as the number of iterations).
When gauge is satisfied with the measurement we exit the run loop. For every
``BENCHMARK`` we may only call ``RUN`` once.

Using ``g++`` the example code may be compiled as::

  g++ main.cpp -o benchmark --std=c++14 -I../path_to_gauge/ -L../path_to_libguage -lgauge -ltables

You should now be able to run the benchmark using::

  ./benchmark

License
-------

gauge is available under the BSD license, see the LICENSE.rst file.

Credits
-------

We have create gauge to fit our specific purpose, however we hope
that others may also find it useful. When designing gauge we found
inspiration in these other nice projects:

* Qt's benchmark tools part of QTestLib_.
* Nick Bruun's Hayai_ C++ benchmark tool, who also provided the nice
  terminal colors.
* The `Google Test`_ framework.

.. _QTestLib: http://qt-project.org/doc/qt-4.8/qtestlib-tutorial5.html
.. _Hayai: https://github.com/nickbruun/hayai
.. _`Google Test`: http://code.google.com/p/googletest/

Thanks for all the fish.
