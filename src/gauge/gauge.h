#ifndef GAUGE_GAUGE_H
#define GAUGE_GAUGE_H

#include "iteration_controller.h"
#include "runner.h"
#include "benchmark.h"
#include "time_benchmark.h"

#define BENCHMARK_CLASS_NAME_(testcase_name, benchmark_name)     \
    testcase_name ## _ ## benchmark_name ## _Benchmark

#define REG_BENCHMARK_(testcase_name, benchmark_name) \
    testcase_name ## _ ## benchmark_name ## _Benchmark_RegClass

#define BENCHMARK_(__testcase_name,                                         \
                   __benchmark_name,                                        \
                   fixture_class_name,                                      \
                   __runs)                                                  \
    class BENCHMARK_CLASS_NAME_(__testcase_name, __benchmark_name)          \
        :   public fixture_class_name                                       \
    {                                                                       \
    public:                                                                 \
        BENCHMARK_CLASS_NAME_(__testcase_name, __benchmark_name)()          \
            {                                                               \
            };                                                              \
                                                                            \
        std::string testcase_name() const                                   \
            { return #__testcase_name; }                                    \
                                                                            \
        std::string benchmark_name() const                                  \
            { return #__benchmark_name; }                                   \
                                                                            \
        virtual uint32_t runs() const                                       \
            {                                                               \
                assert(__runs > 0);                                         \
                return __runs;                                              \
            }                                                               \
                                                                            \
        static uint32_t benchmark_id()                                      \
            {                                                               \
                 static uint32_t id = gauge::runner::register_id();         \
                 return id;                                                 \
            }                                                               \
                                                                            \
    protected:                                                              \
        virtual void test_body();                                           \
                                                                            \
    };                                                                      \
                                                                            \
    static struct REG_BENCHMARK_(__testcase_name, __benchmark_name)         \
    {                                                                       \
        REG_BENCHMARK_(__testcase_name, __benchmark_name)()                 \
        {                                                                   \
            typedef BENCHMARK_CLASS_NAME_(__testcase_name, __benchmark_name)\
                benckmark_type;                                             \
            gauge::runner::instance().register_benchmark<benckmark_type>(); \
        }                                                                   \
    } x ## __testcase_name ## _ ## __benchmark_name##_RegClass ;            \
                                                                            \
    void BENCHMARK_CLASS_NAME_(__testcase_name, __benchmark_name)::test_body()

#define BENCHMARK_F(fixture_name,                        \
                    testcase_name,                       \
                    benchmark_name,                      \
                    runs)                                \
    BENCHMARK_(testcase_name,                            \
               benchmark_name,                           \
               fixture_name,                             \
               runs)

#define BENCHMARK(testcase_name,                         \
                  benchmark_name,                        \
                  runs)                                  \
    BENCHMARK_(testcase_name,                            \
               benchmark_name,                           \
               gauge::time_benchmark,                    \
               runs)

#define RUN \
    for (gauge::iteration_controller __controller(benchmark_id()); \
            __controller.is_done() == false; __controller.next())



#endif

