// Copyright (c) 2012 Steinwurf ApS
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "benchmark.hpp"
#include "iteration_controller.hpp"
#include "runner.hpp"
#include "time_benchmark.hpp"

#include <string>

// This file contains a number of macros used to setup the the benchmarks
// using gauge.
//
// Using the macros we support three ways of defining a benchmark:
//
// 1) Using the "inline" option with the BENCHMARK macro. An example would
//    look like this:
//
//    BENCHMARK(test_speed, brew_coffee, 10)
//    {
//        // Call some function we want to measure under the RUN macro. Only
//        // code called in the RUN scope will be measured
//        RUN
//        {
//             brew_some_coffee();
//        }
//    }
//
// 2) By using a fixture, i.e. a class as a minimum defining a test_body()
//    function. Example:
//
//    class benchmark_my_brewer : public gauge::time_benchmark
//    {
//    public:
//
//        void setup()
//        {
//            m_brewer.turn_on();
//        }
//
//        void tear_down()
//        {
//            m_brewer.turn_off();
//        }
//
//        void test_body() final
//        {
//            // Call some function we want to measure under the RUN macro.
//            // Only code called in the RUN scope will be measured
//            RUN
//            {
//                 m_brewer.brew_some_coffee();
//            }
//        }
//
//        mocca_master m_brewer;
//    };
//
//    BENCHMARK_F(benchmark_my_brewer, test_speed, brew_coffee, 10);
//
// 3) Using a fixture but defining the test body inline. Example:
//
//    class benchmark_my_brewer : public gauge::time_benchmark
//    {
//    public:
//
//        void setup()
//        {
//            m_brewer.turn_on();
//        }
//
//        void tear_down()
//        {
//            m_brewer.turn_off();
//        }
//
//        mocca_master m_brewer;
//    };
//
//    BENCHMARK_F_INLINE(benchmark_my_brewer, test_speed, brew_coffee, 10)
//    {
//        // Call some function we want to measure under the RUN macro. Only
//        // code called in the RUN scope will be measured
//        RUN
//        {
//             m_brewer.brew_some_coffee();
//        }
//    }
//

// Define the name of the benchmark class
#define BENCHMARK_NAME_(testcase, benchmark) testcase##_##benchmark##_Benchmark

// Define the name of the benchmark class defining a test body function
#define BENCHMARK_TEST_BODY_NAME_(testcase, benchmark) \
    testcase##_##benchmark##_Benchmark_TestBody

// Define the name of the struct that registers a benchmark with the runner
#define REG_BENCHMARK_NAME_(testcase, benchmark) \
    testcase##_##benchmark##_Benchmark_RegClass

// Define a benchmark class not declaring a benchmark::test_body()
// function. This means that this should only be used with a fixture that
// defines it.
#define BENCHMARK_CLASS_(testcase, benchmark, fixture, __runs)  \
    class BENCHMARK_NAME_(testcase, benchmark) : public fixture \
    {                                                           \
    public:                                                     \
        std::string testcase_name() const                       \
        {                                                       \
            return #testcase;                                   \
        }                                                       \
                                                                \
        std::string benchmark_name() const                      \
        {                                                       \
            return #benchmark;                                  \
        }                                                       \
                                                                \
        virtual uint32_t runs() const                           \
        {                                                       \
            assert(__runs > 0);                                 \
            return __runs;                                      \
        }                                                       \
                                                                \
        static uint32_t benchmark_id()                          \
        {                                                       \
            static uint32_t id = gauge::runner::register_id();  \
            return id;                                          \
        }                                                       \
    };

// This macro expands into a class that defines the benchmark::test_body()
// function. The benchmark::test_body() function is invoked by the runner
// when it starts a benchmark. The macro also "opens" the function
// definition, so that the user can specify the test code.
//
// When using this class we need to first define use the BENCHMARK_CLASS_
// macro to define the necessary base class (we inherit from it).
#define BENCHMARK_TEST_BODY_CLASS_(testcase, benchmark)    \
                                                           \
    class BENCHMARK_TEST_BODY_NAME_(testcase, benchmark) : \
        public BENCHMARK_NAME_(testcase, benchmark)        \
    {                                                      \
    public:                                                \
        virtual void test_body();                          \
    };

// Define a registration benchmark class that registers the benchmark class
// not defining the benchmark::test_body() function.
//
// Notice that we do not specify the last semicolon. This macro is used by
// the BENCHMARK_F macro that which
#define REG_BENCHMARK_CLASS_(testcase, benchmark)                             \
                                                                              \
    static struct REG_BENCHMARK_NAME_(testcase, benchmark)                    \
    {                                                                         \
        REG_BENCHMARK_NAME_(testcase, benchmark)()                            \
        {                                                                     \
            using benchmark_type = BENCHMARK_NAME_(testcase, benchmark);      \
                                                                              \
            auto& runner = gauge::runner::instance();                         \
            runner.register_benchmark<benchmark_type>(#testcase, #benchmark); \
        }                                                                     \
    } x##testcase##_##benchmark##_RegClass

// Define a registration benchmark class that registers the benchmark class
// not defining the benchmark::test_body() function.
#define REG_BENCHMARK_TEST_BODY_CLASS_(testcase, benchmark)                   \
                                                                              \
    static struct REG_BENCHMARK_NAME_(testcase, benchmark)                    \
    {                                                                         \
        REG_BENCHMARK_NAME_(testcase, benchmark)()                            \
        {                                                                     \
            using benchmark_type =                                            \
                BENCHMARK_TEST_BODY_NAME_(testcase, benchmark);               \
                                                                              \
            auto& runner = gauge::runner::instance();                         \
            runner.register_benchmark<benchmark_type>(#testcase, #benchmark); \
        }                                                                     \
    } x##testcase##_##benchmark##_RegClass;

/// Macro creating a benchmark using a fixture class
#define BENCHMARK_F(fixture, testcase, benchmark, runs)  \
    BENCHMARK_CLASS_(testcase, benchmark, fixture, runs) \
    REG_BENCHMARK_CLASS_(testcase, benchmark)

/// Macro creating a benchmark using a fixture class with inline test body
#define BENCHMARK_F_INLINE(fixture, testcase, benchmark, runs) \
    BENCHMARK_CLASS_(testcase, benchmark, fixture, runs)       \
    BENCHMARK_TEST_BODY_CLASS_(testcase, benchmark)            \
    REG_BENCHMARK_TEST_BODY_CLASS_(testcase, benchmark)        \
    void BENCHMARK_TEST_BODY_NAME_(testcase, benchmark)::test_body()

/// The default macro which will register and run a benchmark measuring
/// the time elapsed.
#define BENCHMARK(testcase, benchmark, runs)                           \
    BENCHMARK_CLASS_(testcase, benchmark, gauge::time_benchmark, runs) \
    BENCHMARK_TEST_BODY_CLASS_(testcase, benchmark)                    \
    REG_BENCHMARK_TEST_BODY_CLASS_(testcase, benchmark)                \
    void BENCHMARK_TEST_BODY_NAME_(testcase, benchmark)::test_body()

// Macro for starting the measurement, using the iteration controller.
//
// RUN
// {
//     test_something();
// }
//
#define RUN                                        \
    for (gauge::iteration_controller __controller; \
         __controller.is_done() == false; __controller.next())

// Define the name of the class used to register options
#define BENCHMARK_OPTION_CLASS_NAME_(option_name) \
    opt_##option_name##_Optionclass_

// Defines and instantiates an options class which allows us to specify
// options for the benchmarks
#define BENCHMARK_OPTION(option_name)                       \
    static struct BENCHMARK_OPTION_CLASS_NAME_(option_name) \
    {                                                       \
        BENCHMARK_OPTION_CLASS_NAME_(option_name)()         \
        {                                                   \
            add_options();                                  \
        }                                                   \
                                                            \
        static void add_options();                          \
                                                            \
    } add_option_reg##option_name;                          \
                                                            \
    void BENCHMARK_OPTION_CLASS_NAME_(option_name)::add_options()
