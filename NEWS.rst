News for gauge
==============

This file lists the major changes between versions. For a more detailed list
of every change, see the Git log.

Latest
------
* tbd

5.0.0
-----
* Minor: Update waf to version 1.7.10 and use the wurf_install_path tool.
* Minor: Updated to waf-tools version 2.x.y
* Minor: Added csv (comma separated values) printer.
* Major: Changed the benchmark interface. Implementers of new benchmarks may
  now register several measurement results in a single run. The new benchmark
  also supports specifying custom benchmark options (available on the
  command-line).
* Minor: The benchmark fixtures are now only allocated when needed and destroyed
  after completing a benchmark. This releases resources allocated by previous
  benchmarks.
* Minor: Added a number of unit-test for testing gauge itself.

4.0.0
-----
* Updated waf to use new external waf tools repository

3.0.0
-----
* Switched to mkspec enabled builds

2.0.0
-----
* Using .hpp extension instead of .h

1.0.0
-----
* Initial release
* Tested on Linux, Mac and Windows

