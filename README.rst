Introduction
------------

gauge is a flexible C++ benchmarking tool.

.. image:: http://buildbot.steinwurf.dk/svgstatus?project=gauge
    :target: http://buildbot.steinwurf.dk/stats?projects=gauge

Dependencies
------------
* We rely on functionality from the Boost C++ libraries, currently the
  gauge build-tool will automatically download these dependencies.
* We are migrating to C++11 and currently use a few features, you therefore
  have to supply the appropiate C++11 compiler switch when using gauge.

Platforms
---------
We have tested gauge on various versions of Windows, Linux and Mac. You can
see the status by selecting the cxx-gauge project on the Steinwurf `buildbot
page <http://176.28.49.184:12344/>`_:

Build
-----
We use the ``waf`` build-system to build the gauge static library.
With some additional tools which may be found at external-waf_

.. _external-waf: https://github.com/steinwurf/external-waf

To build gauge run the following command:
::
  ./waf configure --bundle=ALL --bundle-path=~/dev/bundle_dependencies
  ./waf build

Substitute the ``~/dev/bundle_dependencies`` with the path where you wish
waf to download gauge's dependencies. You may omit this option then waf
will download the dependencies into the gauge project folder.

You should now have the ``libgauge.a`` static lib. It also build the boost
dependencies as static libs. In the gauge directory type ``find . -name "*.a"``
to see the libraries produced. We need to use the path to the libraries when
using gauge with our own applications as shown in the following.

When building the static lib, waf will also build the ``gauge_example`` executable.
Depending on your platform you should be able to launch it by running:
::
  ./build/linux/examples/sample_benchmarks/gauge_example

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

In the above we use the ``BENCHMARK`` macro which takes 3 parameters:

1. The name of the test-case in this case ``MyTest``
2. The name of the benchmark in this case ``RunThis``
3. The number of runs to complete in this case 100.

The measurement will not start until we hit the ``RUN`` macro. Depending
on the type of benchmark (the default is time) the code inside ``RUN`` will
be executed several times (we refer to this as the number of iterations).
When gauge is satisfied with the measurement we exit the run loop. For every
``BENCHMARK`` we may only call ``RUN`` once.

Using ``g++`` the example code may be compiled as:
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
