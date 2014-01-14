News for Gauge
==============

This file lists the major changes between versions. For a more detailed list of
every change, see the Git log.

Latest
------
* Minor: Added a convenience function add_default_printers() to the
  gauge runner. This enables most common printers in a project.

6.2.0
-----
* Minor: runner::run now catches all exceptions. The API has been
  extended with runner::run_unsafe which doesn't catch exceptions.

6.1.0
-----
* Bug: Removed configuration information from the result table.
* Minor: Improved filter options for benchmark testing purposes.

6.0.0
-----
* Major: Started using tables version 4.
* Major: Extracted tables and related logic to its own repository.

5.5.0
-----
* Minor: The python- and CSV-printers can now take the default output filename
  as a parameter.

5.4.0
-----
* Minor: Eased use of python output file.

5.3.0
-----
* Minor: Improved filtering options for filtering benchmarks and results.

5.2.1
-----
* Bug: Fix file printers so that they correctly detect the end of all
  benchmarks.

5.2.0
-----
* Minor: Adding time-stamping to the console printer to give an overview of the
  time spent in every step.
* Minor: Added the option to control the CSV printer's value separator, the
  default is comma however this can now be changed command-line.

5.1.0
-----
* Minor: Cleanup

5.0.0
-----
* Minor: Update waf to version 1.7.10 and use the wurf_install_path tool.
* Minor: Updated to waf-tools version 2.x.y
* Minor: Added CSV (comma separated values) printer.
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
