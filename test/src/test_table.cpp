#include <cstdint>

#include <gtest/gtest.h>

#include <gauge/table.hpp>

TEST(TestTable, test_table)
{

    std::string unit("seconds");

    gauge::table t;

    t.add_row();
    t.set_value("iterations", uint64_t(10));

    t.add_row();

    t.add_column("ok", double(0.0));
    t.set_value("ok", double(4.0));
    t.set_value("iterations", uint64_t(10));

    t.add_row();
    t.set_value("iterations", uint64_t(10));
    t.set_value("ok", double(6.5));

    t.add_row();
    t.set_value("iterations", uint64_t(100));
    t.add_row();
    t.set_value("iterations", uint64_t(1000));

    uint32_t rows = 5;
    EXPECT_EQ(t.rows(), rows);

    // The table should now contain the following results
    // iterations: 10 10 10 100    1000
    //         ok: 0  0  4  6.5    0

    auto ok = t.column_as<double>("ok");
    EXPECT_EQ(ok.size(), rows);
    EXPECT_EQ(ok[0], 0);
    EXPECT_EQ(ok[1], 4);
    EXPECT_EQ(ok[2], 6.5);
    EXPECT_EQ(ok[3], 0);
    EXPECT_EQ(ok[4], 0);

    auto iterations = t.column_as<uint64_t>("iterations");
    EXPECT_EQ(iterations.size(), rows);
    EXPECT_EQ(iterations[0], 10U);
    EXPECT_EQ(iterations[1], 10U);
    EXPECT_EQ(iterations[2], 10U);
    EXPECT_EQ(iterations[3], 100U);
    EXPECT_EQ(iterations[4], 1000U);

}


