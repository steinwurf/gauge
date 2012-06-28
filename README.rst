Introduction
------------

**gauge** is a flexible C++ benchmarking tool.

Dependencies
----------
* We rely on functionality from the Boost C++ libraries.
* We are migrating to C++11 and currently use a few features.

Build
-----
We use the ``waf`` build-system to build the gauge static library.
With some additional tools which may be found at external-waf_

.. _external-waf: https://github.com/steinwurf/external-waf


::
  ./waf configure --bundle=ALL --bundle-path=~/dev/bundle_dependencies
  ./waf build

You should now have the ``libgauge.a`` static lib. It also build the boost
dependencies as static libs. In the gauge directory type ``find . -name "*.a"``
to see the libraries produced. We need to use the path to the libraries when
using gauge with our own applications as shown in the following.

Example Use
-----------

See various use-cases in the ``examples`` folder. The following can be found
in the ``examples/sample_makefile`` folder. To try it out either use the
code provide or save the following code in a file called ``main.cpp``
::
  #include <gauge/gauge.h>
  #include <gauge/console_printer.h>
  #include <vector>

  BENCHMARK(MyTest, RunThis, 100)
  {
      std::vector<int> integers;
      for(int i = 0; i < 2048; ++i)
      {
          integers.push_back(i);
      }

      // This is where the clock runs
      RUN{
          for(int i = 1; i < integers.size(); ++i)
          {
              integers[i] += integers[i-1];
          }
      }
  }

  int main(int argc, const char* argv[])
  {
      gauge::runner::instance().printers().push_back(
         std::make_shared<gauge::console_printer>());

      gauge::runner::run_benchmarks(argc, argv);
      return 0;
  }



Using ``g++`` the example code may be compiled as::
::
  g++ main.cpp -o benchmark --std=c++0x -I../path_to_gauge/ -L../path_to_libguage -lgauge -L../path_to_libboostxyz -lboost_chrono -lboost_program_options -lboost_system -lrt

We assume you are using the boost libs built together with gauge. In the
future we will attempt also support boost libraries installed using your
package manager or similar.

You should now be able to run the benchmark using:
::
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


