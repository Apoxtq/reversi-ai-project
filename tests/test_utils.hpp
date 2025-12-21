/*
 * test_utils.hpp - Lightweight testing utilities
 * COMP390 Honours Year Project - Reversi AI
 * 
 * Simple testing framework without external dependencies
 * Provides Google Test-like syntax for assertions and benchmarking
 */

#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>

namespace test {

// Global test counters
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

// ANSI color codes for terminal output
#ifdef _WIN32
    // Windows may not support ANSI colors by default
    #define COLOR_RESET   ""
    #define COLOR_GREEN   ""
    #define COLOR_RED     ""
    #define COLOR_YELLOW  ""
    #define COLOR_BLUE    ""
#else
    #define COLOR_RESET   "\033[0m"
    #define COLOR_GREEN   "\033[32m"
    #define COLOR_RED     "\033[31m"
    #define COLOR_YELLOW  "\033[33m"
    #define COLOR_BLUE    "\033[34m"
#endif

/**
 * @brief Assert that two values are equal
 */
#define ASSERT_EQ(actual, expected) \
    do { \
        ++test::tests_run; \
        if ((actual) != (expected)) { \
            std::cerr << COLOR_RED << "✗ FAIL" << COLOR_RESET << ": " \
                      << __FILE__ << ":" << __LINE__ << "\n" \
                      << "  Expected: " << (expected) << "\n" \
                      << "  Actual:   " << (actual) << "\n"; \
            ++test::tests_failed; \
        } else { \
            ++test::tests_passed; \
        } \
    } while(0)

/**
 * @brief Assert that expression is true
 */
#define ASSERT_TRUE(expr) \
    do { \
        ++test::tests_run; \
        if (!(expr)) { \
            std::cerr << COLOR_RED << "✗ FAIL" << COLOR_RESET << ": " \
                      << __FILE__ << ":" << __LINE__ << "\n" \
                      << "  Expression '" #expr "' is false\n"; \
            ++test::tests_failed; \
        } else { \
            ++test::tests_passed; \
        } \
    } while(0)

/**
 * @brief Assert that expression is false
 */
#define ASSERT_FALSE(expr) \
    do { \
        ++test::tests_run; \
        if (expr) { \
            std::cerr << COLOR_RED << "✗ FAIL" << COLOR_RESET << ": " \
                      << __FILE__ << ":" << __LINE__ << "\n" \
                      << "  Expression '" #expr "' is true\n"; \
            ++test::tests_failed; \
        } else { \
            ++test::tests_passed; \
        } \
    } while(0)

/**
 * @brief Comparison assertions
 */
#define ASSERT_GT(a, b) ASSERT_TRUE((a) > (b))
#define ASSERT_LT(a, b) ASSERT_TRUE((a) < (b))
#define ASSERT_GE(a, b) ASSERT_TRUE((a) >= (b))
#define ASSERT_LE(a, b) ASSERT_TRUE((a) <= (b))
#define ASSERT_NE(a, b) ASSERT_TRUE((a) != (b))

/**
 * @brief Print test summary
 */
inline void print_summary() {
    std::cout << "\n========================================\n";
    std::cout << "Test Summary:\n";
    std::cout << "  Total:  " << tests_run << "\n";
    std::cout << COLOR_GREEN << "  Passed: " << tests_passed << " [OK]" << COLOR_RESET << "\n";
    
    if (tests_failed > 0) {
        std::cout << COLOR_RED << "  Failed: " << tests_failed << " [FAIL]" << COLOR_RESET << "\n";
    } else {
        std::cout << "  Failed: 0\n";
    }
    
    std::cout << "========================================\n";
    
    if (tests_failed == 0) {
        std::cout << COLOR_GREEN << "ALL TESTS PASSED!" << COLOR_RESET << "\n";
    } else {
        std::cout << COLOR_RED << "SOME TESTS FAILED" << COLOR_RESET << "\n";
    }
}

/**
 * @brief High-resolution timer for benchmarking
 */
class Timer {
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    
    TimePoint start_;
    
public:
    Timer() : start_(Clock::now()) {}
    
    /**
     * @brief Reset the timer
     */
    void reset() {
        start_ = Clock::now();
    }
    
    /**
     * @brief Get elapsed time in milliseconds
     */
    double elapsed_ms() const {
        auto end = Clock::now();
        return std::chrono::duration<double, std::milli>(end - start_).count();
    }
    
    /**
     * @brief Get elapsed time in microseconds
     */
    double elapsed_us() const {
        auto end = Clock::now();
        return std::chrono::duration<double, std::micro>(end - start_).count();
    }
    
    /**
     * @brief Get elapsed time in seconds
     */
    double elapsed_sec() const {
        auto end = Clock::now();
        return std::chrono::duration<double>(end - start_).count();
    }
};

/**
 * @brief Format large numbers with commas
 */
inline std::string format_number(long long num) {
    std::string str = std::to_string(num);
    int pos = str.length() - 3;
    while (pos > 0) {
        str.insert(pos, ",");
        pos -= 3;
    }
    return str;
}

/**
 * @brief Format speed in operations per second
 */
inline std::string format_speed(double ops_per_sec) {
    if (ops_per_sec >= 1e9) {
        return std::to_string(ops_per_sec / 1e9) + " G ops/s";
    } else if (ops_per_sec >= 1e6) {
        return std::to_string(ops_per_sec / 1e6) + " M ops/s";
    } else if (ops_per_sec >= 1e3) {
        return std::to_string(ops_per_sec / 1e3) + " K ops/s";
    } else {
        return std::to_string(ops_per_sec) + " ops/s";
    }
}

} // namespace test




