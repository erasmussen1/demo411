
#include "utils.h"

#include <gt2test.hpp>

#include <cstdint>

#include <string>


struct UtilsFixture : public gt2::Test {
    void SetUp() override {
        //
    }

    void TearDown(void) override {
        //
    }

    void clear() {
        year = 0;
        month = 0;
        day = 0;
        weekday = 0;
    }

    int len = 0;

    uint8_t year = 26;
    uint8_t month = 9;
    uint8_t day = 21;
    uint8_t weekday = 1;

    uint32_t dr;

    int rc = 0;
};

TEST_F(UtilsFixture, Conver_ymdwd_2_RTC_dr) {
    dr = ymdw2dr(year, month, day, weekday);

    EXPECT_EQ(dr, 0x00262921);
}

TEST_F(UtilsFixture, ConverRTC_dr_2ymdwd) {
    dr = 0x00262921;

    rc = dr2ymdw(dr, &year, &month, &day, &weekday);

    EXPECT_EQ(year, 26);
    EXPECT_EQ(month, 9);
    EXPECT_EQ(day, 21);
    EXPECT_EQ(weekday, 1);
}

TEST_F(UtilsFixture, ConvertDate2uint32Round) {
    dr = ymdw2dr(year, month, day, weekday);

    clear();
    rc = dr2ymdw(dr, &year, &month, &day, &weekday);

    EXPECT_EQ(dr, 0x00262921);
}

TEST_F(UtilsFixture, CalcWeekday20260921_expect_7) {
    day--;

    uint16_t year2 = 2000 + year;
    const uint8_t wd = rtc_weekday(year2, month, day);

    EXPECT_EQ(wd, 7);
}

TEST_F(UtilsFixture, CalcWeekday20260921_expect_1) {
    uint16_t year2 = 2000 + year;
    const uint8_t wd = rtc_weekday(year2, month, day);

    EXPECT_EQ(wd, 1);
}

TEST_F(UtilsFixture, CalcWeekday20260921_expect_1to7) {
    day--;

    for (uint8_t i = 1; i <= 7; i++) {
        uint16_t year2 = 2000 + year;
        uint8_t day2 = day + i;

        uint8_t wd = rtc_weekday(year2, month, day2);

        ASSERT_EQ(wd, i);
    }
}

TEST_F(UtilsFixture, CalcWeekday202609_expect_1to30) {
    day = 0;

    for (uint8_t i = 1; i <= 30; i++) {
        dr = ymdw2dr(year, month, day + i, weekday);

        uint8_t yearOut = 0;
        uint8_t monthOut = 0;
        uint8_t dayOut = 0;
        uint8_t weekdayOut = 0;

        rc = dr2ymdw(dr, &yearOut, &monthOut, &dayOut, &weekdayOut);

        ASSERT_EQ(yearOut, year);
        ASSERT_EQ(monthOut, month);
        ASSERT_EQ(dayOut, day + i);
    }
}

TEST_F(UtilsFixture, trim4timeDate_case_1) {
    char msg[] = "tt210103";

    const auto msg1 = std::string(msg);

    rc = trim4timeDate(msg, &len);

    const auto msg2 = std::string(msg);

    // printf("%s ==> %s\n", msg1.c_str(), msg2.c_str());

    EXPECT_TRUE(msg2 == "210103");
}

TEST_F(UtilsFixture, trim4timeDate_case_2) {
    char msg[] = "tt 210103";

    const auto msg1 = std::string(msg);

    rc = trim4timeDate(msg, &len);

    auto msg2 = std::string(msg);

    // printf("%s ==> %s\n", msg1.c_str(), msg2.c_str());

    EXPECT_TRUE(msg2 == "210103");
}

TEST_F(UtilsFixture, trim4timeDate_case_3) {
    char msg[] = "tt=210103";

    const auto msg1 = std::string(msg);

    rc = trim4timeDate(msg, &len);

    const auto msg2 = std::string(msg);

    // printf("%s ==> %s\n", msg1.c_str(), msg2.c_str());

    EXPECT_TRUE(msg2 == "210103");
}

TEST_F(UtilsFixture, trim4timeDate_case_4) {
    char msg[] = "tt=21/01/03";

    const auto msg1 = std::string(msg);

    rc = trim4timeDate(msg, &len);

    const auto msg2 = std::string(msg);

    // printf("%s ==> %s\n", msg1.c_str(), msg2.c_str());

    EXPECT_TRUE(msg2 == "210103");
}

TEST_F(UtilsFixture, trim4timeDate_case_5) {
    char msg[] = "tt=21,01,03";

    const auto msg1 = std::string(msg);

    rc = trim4timeDate(msg, &len);

    const auto msg2 = std::string(msg);

    // printf("%s ==> %s  %d\n", msg1.c_str(), msg2.c_str(), len);

    EXPECT_TRUE(msg2 == "210103");
}

TEST_F(UtilsFixture, trim4timeDate_case_6) {
    char msg[] = "tt=2026/01/03";

    const auto msg1 = std::string(msg);

    rc = trim4timeDate(msg, &len);

    const auto msg2 = std::string(msg);

    // printf("%s ==> %s  %d\n", msg1.c_str(), msg2.c_str(), len);

    EXPECT_TRUE(msg2 == "20260103");
}

TEST_F(UtilsFixture, trim4timeDate_case_7) {
    char msg[] = "   tt=2026/01/03";

    const auto msg1 = std::string(msg);

    rc = trim4timeDate(msg, &len);

    const auto msg2 = std::string(msg);

    // printf("%s ==> %s  %d\n", msg1.c_str(), msg2.c_str(), len);

    EXPECT_TRUE(msg2 == "20260103");
}

TEST_MAIN();
