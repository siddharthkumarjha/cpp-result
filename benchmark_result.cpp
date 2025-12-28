#include "inc/result.hpp"
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

using namespace result_type;
using namespace std::chrono;

// Simple benchmark for Result<T, E> operations
void benchmark_basic_operations()
{
    constexpr int iterations = 1000000;

    std::cout << "Benchmarking Result<T, E> operations (" << iterations << " iterations)...\n";

    // Test construction and basic operations
    auto start        = high_resolution_clock::now();

    volatile int sink = 0; // Prevent optimization
    for (int i = 0; i < iterations; ++i)
    {
        auto result = make_ok<int, std::string>(i);
        if (result.is_ok())
        {
            sink += result.unwrap();
        }
    }

    auto end      = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    std::cout << "Basic operations: " << duration.count() << " μs\n";
    std::cout << "Per operation: " << static_cast<double>(duration.count()) / iterations << " μs\n";
}

void benchmark_chaining()
{
    constexpr int iterations = 100000;

    std::cout << "\nBenchmarking Result chaining (" << iterations << " iterations)...\n";

    auto start        = high_resolution_clock::now();

    volatile int sink = 0;
    for (int i = 0; i < iterations; ++i)
    {
        auto result = make_ok<int, std::string>(i % 100)
                          .and_then(
                              [](int x) -> Result<int, std::string>
                              {
                                  return make_ok<int, std::string>(x * 2);
                              })
                          .map(
                              [](int x)
                              {
                                  return x + 1;
                              })
                          .and_then(
                              [](int x) -> Result<int, std::string>
                              {
                                  if (x > 150)
                                  {
                                      return make_err<int, std::string>("too big");
                                  }
                                  return make_ok<int, std::string>(x);
                              })
                          .unwrap_or(0);

        sink += result;
    }

    auto end      = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    std::cout << "Chaining operations: " << duration.count() << " μs\n";
    std::cout << "Per operation: " << static_cast<double>(duration.count()) / iterations << " μs\n";
}

void benchmark_vs_exceptions()
{
    constexpr int iterations = 10000;

    std::cout << "\nBenchmarking Result vs Exceptions (" << iterations << " iterations)...\n";

    // Result-based approach
    auto start_result  = high_resolution_clock::now();

    volatile int sink1 = 0;
    for (int i = 0; i < iterations; ++i)
    {
        auto divide = [](int a, int b) -> Result<int, std::string>
        {
            if (b == 0)
                return make_err<int, std::string>("div by zero");
            return make_ok<int, std::string>(a / b);
        };

        auto result = divide(i, (i % 10 == 0) ? 0 : 1);
        sink1 += result.unwrap_or(-1);
    }

    auto end_result      = high_resolution_clock::now();
    auto result_duration = duration_cast<microseconds>(end_result - start_result);

    // Exception-based approach
    auto start_exception = high_resolution_clock::now();

    volatile int sink2   = 0;
    for (int i = 0; i < iterations; ++i)
    {
        auto divide_throw = [](int a, int b) -> int
        {
            if (b == 0)
                throw std::runtime_error("div by zero");
            return a / b;
        };

        try
        {
            sink2 += divide_throw(i, (i % 10 == 0) ? 0 : 1);
        }
        catch (...)
        {
            sink2 += -1;
        }
    }

    auto end_exception      = high_resolution_clock::now();
    auto exception_duration = duration_cast<microseconds>(end_exception - start_exception);

    std::cout << "Result approach: " << result_duration.count() << " μs\n";
    std::cout << "Exception approach: " << exception_duration.count() << " μs\n";
    std::cout << "Result is " << static_cast<double>(exception_duration.count()) / result_duration.count()
              << "x faster\n";
}

int main()
{
    std::cout << "=== Result<T, E> Performance Benchmarks ===\n\n";

    benchmark_basic_operations();
    benchmark_chaining();
    benchmark_vs_exceptions();

    std::cout << "\n=== Benchmark Complete ===\n";
    std::cout << "Note: These are micro-benchmarks and actual performance may vary.\n";
    std::cout << "The main benefit of Result<T, E> is expressiveness and safety, not just speed.\n";

    return 0;
}
