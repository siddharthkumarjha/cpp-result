#include "inc/result.hpp"
#include <iostream>
#include <memory>

using namespace std::literals;
using DivisionResult = Result<int, std::string_view>;

DivisionResult Divide(int a, int b)
{
    if (b == 0)
        return Err("division by 0"sv);

    return Ok(a / b);
}

using PtrRes = Result<std::unique_ptr<int>, std::string_view>;

PtrRes make_some_memory() { return Ok(std::make_unique<int>(2)); }

int main(int argc, char *argv[])
{
    const auto result = Divide(8, 3)
                            .and_then(
                                [](int a) -> DivisionResult
                                {
                                    std::cout << "div res1 got " << a << " from op\n";
                                    return Divide(a, 0);
                                })
                            .or_else(
                                [](std::string_view e) -> DivisionResult
                                {
                                    std::cout << "got err: " << e << '\n';
                                    return Ok(0);
                                })
                            .and_then(
                                [](int a) -> DivisionResult
                                {
                                    std::cout << "div res2 got " << a << " from op\n";
                                    return Divide(a, 1);
                                })
                            .is_ok_and(
                                [](int res) -> bool
                                {
                                    std::cout << "finally got value: " << res << '\n';
                                    return true;
                                });
    std::cout << "result: " << std::boolalpha << result << '\n';

    std::cout << "\n=====================================================\n";

    Divide(8, 0).match([](int a) -> void { std::cout << "got result: " << a << '\n'; },
                       [](std::string_view e) -> void { std::cout << "got err: " << e << '\n'; });

    std::cout << "\n=====================================================\n";

    auto e = make_ok<void, std::string>();
    e.match([]() -> void { std::cout << "got void result\n"; },
            [](std::string &e) -> void { std::cout << "got err: " << e << '\n'; });

    std::cout << "\n=====================================================\n";
    auto ptr_val   = make_some_memory();
    auto ptr_deref = std::move(ptr_val).match([](std::unique_ptr<int> ptr) -> int { return *ptr; },
                                              [](std::string_view e) -> int
                                              {
                                                  std::cerr << "err: " << e << '\n';
                                                  return -1;
                                              });
    std::cout << "ptr_val: " << ptr_deref << '\n';
    return 0;
}
