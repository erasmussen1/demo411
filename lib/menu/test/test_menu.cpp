
#include <gt2test.hpp>

#include <menu.h>
#include <amenu.h>
#include <bmenu.h>
#include <menu_util.h>

#include <string>
#include <vector>

constexpr size_t COMMAND_BUFFER_SIZE = 30;


struct UtilMenuFixture : public gt2::Test {
    void SetUp() {
        //
    }

    void TearDown(void) {
        //
    }

    std::string strCommand;
    unsigned long l = 0;
    unsigned long value = 0;

    unsigned short shortValue = 33;

    char buffer[COMMAND_BUFFER_SIZE + 1];

    int rc = 0;
};

TEST_F(UtilMenuFixture, SetBitMaskOf8bits_03_OK) {
    strCommand = "zz00000011";

    rc = UTIL_get_mask(strCommand.data(), 8, &l);

    ASSERT_EQ(l, 0x03);
}

TEST_F(UtilMenuFixture, SetBitMaskOf8bits_80_OK) {
    strCommand = "zz10000000";

    rc = UTIL_get_mask(strCommand.data(), 8, &l);

    ASSERT_EQ(l, 0x80);
}

TEST_F(UtilMenuFixture, SetBitMaskOf8bits_FF_OK) {
    strCommand = "zz11111111";

    rc = UTIL_get_mask(strCommand.data(), 8, &l);

    ASSERT_EQ(l, 0xFF);
}

TEST_F(UtilMenuFixture, SetBitMaskOf8bitsStrToShort_NotOK) {
    strCommand = "zz10001";

    rc = UTIL_get_mask(strCommand.data(), 8, &l);

    ASSERT_EQ(l, 0x00);
}

TEST_F(UtilMenuFixture, GetAndCheckValue_0_100_valid) {
    strCommand = "at55";

    UTIL_get_value(&shortValue, 0, 100, strCommand.data());

    EXPECT_EQ(shortValue, 55);
}

TEST_F(UtilMenuFixture, GetAndCheckValue_10_100_valid_77) {
    strCommand = "at77";

    UTIL_get_value(&shortValue, 10, 100, strCommand.data());

    EXPECT_EQ(shortValue, 77);
}

TEST_F(UtilMenuFixture, GetAndCheckValue_10_100_inValid_7) {
    strCommand = "at7";

    UTIL_get_value(&shortValue, 10, 100, strCommand.data());

    EXPECT_NE(shortValue, 7);
}

TEST_F(UtilMenuFixture, GetAndCheckValue_0_100_inValid) {
    strCommand = "at105";

    UTIL_get_value(&shortValue, 0, 100, strCommand.data());

    EXPECT_NE(shortValue, 105);
}

TEST_F(UtilMenuFixture, checkInit_A_Commands) {
    ME_initCommandBuffer(buffer, COMMAND_BUFFER_SIZE);

    AM_values_t* p = AM_getData();

    EXPECT_EQ(p->mask, 0x8001);
}

TEST_F(UtilMenuFixture, checkInit_B_Commands) {
    ME_initCommandBuffer(buffer, COMMAND_BUFFER_SIZE);

    BM_values_t* p = BM_getData();

    EXPECT_EQ(p->delay, 250);
}

TEST_F(UtilMenuFixture, CommandProcessAZ) {
    strCommand = "az=78";

    std::vector<char> buf(strCommand.begin(), strCommand.end());

    rc = ME_commandProcess(buf.data(), (int)strCommand.size() + 1);
    EXPECT_EQ(rc, 0);

    AM_values_t* p = AM_getData();

    EXPECT_EQ(p->prime1, 78);
}

TEST_MAIN();
