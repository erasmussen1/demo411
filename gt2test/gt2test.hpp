#pragma once

#include <vector>
#include <string>
#include <memory>
#include <functional>
#include <type_traits>
#include <iostream>
#include <iomanip>


/* clang-format off */
#define RED         "\033[1;31m"
#define GREEN       "\033[0;32m"
#define YELLOW      "\033[0;33m"
#define DEF         "\033[0;0m"

#define INFO_PREFIX YELLOW    "[    INFO] " DEF
#define RUN_PREFIX GREEN      "[RUN     ] " DEF
#define OK_PREFIX GREEN       "[      OK] " DEF
#define FAILED_PREFIX RED     "[  FAILED] " DEF

/* clang-format on */

#define ASSERT(cond)       \
    if (!(cond)) {         \
        success = false;   \
        condition = #cond; \
        file = __FILE__;   \
        line = __LINE__;   \
    }

#define ASSERT_TRUE(cond) ASSERT(cond);
#define ASSERT_FALSE(cond) ASSERT(!(cond));

#define ASSERT_NULL(value) ASSERT(value == nullptr);
#define ASSERT_NOTNULL(value) ASSERT(value != nullptr);

template <typename T, typename U>
auto printMsg(const std::string&& msg,  //
              const T& t,               //
              const std::string&& opr,  //
              const U& u                //
              ) -> void {
    std::cout << msg << t << " " << opr << " " << u << "\n";
}

template <typename TV, typename UV>
auto printMsg(const std::string&& msg,    //
              const std::vector<TV>& tv,  //
              const std::string&& opr,    //
              const std::vector<UV>& uv   //
              ) -> void {
    /* TODO(ER) - create function to print container */
    std::cout << msg << "vector A" << " " << opr << " " << "vector B" << "\n";
}


#define ASSERT_EQ(a, b)                                      \
    if (!((a) == (b))) {                                     \
        printMsg(INFO_PREFIX "Actual values: ", a, "==", b); \
    }                                                        \
    ASSERT((a) == (b));

#define ASSERT_NE(a, b)                                      \
    if (!((a) != (b))) {                                     \
        printMsg(INFO_PREFIX "Actual values: ", a, "!=", b); \
    }                                                        \
    ASSERT((a) != (b));

#define ASSERT_NEAR(a, b, e) ASSERT(std::fabs((a) - (b)) < e);

#define EXPECT_TRUE(cond) ASSERT(cond)
#define EXPECT_FALSE(cond) ASSERT(!(cond))

#define EXPECT_NULL(value) ASSERT(value == nullptr);
#define EXPECT_NOTNULL(value) ASSERT(value != nullptr);

#define EXPECT_EQ(a, b) ASSERT_EQ((a), (b));
#define EXPECT_NE(a, b) ASSERT_NE((a), (b));

#define EXPECT_NEAR(a, b, e) ASSERT_NEAR((a), (b), (e));


#define TEST(suite, name)                               \
    void name(bool&, std::string&, std::string&, int&); \
                                                        \
    namespace {                                         \
    bool name##flag = gt2::addTest1(name, #name);       \
    }                                                   \
                                                        \
    void name(bool& success, std::string& condition, std::string& file, int& line)

#define uniqname(a) a##__LINE__

#define TEST_F(suite, name)                                                             \
    struct uniqname(name) : public suite {                                              \
        uniqname(name)() = default;                                                     \
                                                                                        \
        void body(bool& success, std::string& condition, std::string& file, int& line); \
    };                                                                                  \
                                                                                        \
    namespace {                                                                         \
    bool name##__LINE__##flag = gt2::addTest2(new uniqname(name), #name);               \
    }                                                                                   \
                                                                                        \
    void uniqname(name)::body(bool& success, std::string& condition, std::string& file, int& line)


namespace gt2 {

struct Test1 {
    std::string name;
    std::function<void(bool&, std::string&, std::string&, int&)> fn;
};

static auto tests1() -> std::vector<Test1>& {
    static std::vector<Test1> tests1_;
    return tests1_;
}

inline static bool addTest1(std::function<void(bool&, std::string&, std::string&, int&)> fn,
                            const std::string& name) {
    tests1().push_back({name, fn});
    return true;
}

struct Test {
    virtual ~Test() = default;

    virtual void body(bool& success, std::string& condition, std::string& file, int& line) = 0;

    virtual void SetUp() {
    }

    virtual void TearDown() {
    }
};

struct Test2 {
    Test2(const std::string& name_, Test* p_) : name(name_), p(p_) {
    }
    std::string name;
    Test* p{nullptr};
};

static auto tests2() -> std::vector<Test2>& {
    static std::vector<Test2> tests2_;
    return tests2_;
}

inline static bool addTest2(Test* test_, const std::string& name) {
    tests2().emplace_back(name, test_);
    return true;
}

inline static void printPrefix(const std::string& testName) {
    std::cout << RUN_PREFIX << testName << "\n";
}

inline static void printResults(const std::string& testName,
                                const bool success,
                                const std::string& condition,
                                const std::string& filename,
                                const int line,
                                size_t& numberOfFailedTests) {
    if (success) {
        std::cout << GREEN "[      OK] " DEF << testName << "\n";
    } else {
        std::cout << RED "[  FAILED] " DEF << testName << "\n"
                  << "           " RED "Assertion failed: " << condition << DEF "\n"
                  << "           " RED << filename << ":" << line << DEF "\n";
        ++numberOfFailedTests;
    }
}

inline static size_t runAllTests() {
    size_t numberOfFailedTests = 0;

    bool success = true;
    std::string condition;
    std::string filename;
    int line = 0;

    for (const auto& test : tests1()) {
        success = true;

        printPrefix(test.name);
        test.fn(success, condition, filename, line);
        printResults(test.name, success, condition, filename, line, numberOfFailedTests);
    }

    for (const auto& test : tests2()) {
        success = true;

        printPrefix(test.name);
        test.p->SetUp();
        test.p->body(success, condition, filename, line);
        test.p->TearDown();
        printResults(test.name, success, condition, filename, line, numberOfFailedTests);

        delete test.p;
    }

    if (numberOfFailedTests == 0) {
        std::cout << GREEN "[ Summary] All tests succeeded!" DEF "\n";
    } else {
        double percentage =
            100.0 * numberOfFailedTests / (gt2::tests1().size() + gt2::tests2().size());

        std::cout << RED "[ Summary] " << numberOfFailedTests          //
                  << " Tests failed " << percentage << "% " DEF "\n";  //
    }

    return numberOfFailedTests;
}
}  // namespace gt2

#define TEST_MAIN()                     \
    int main() {                        \
        return (int)gt2::runAllTests(); \
    }
