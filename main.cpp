#include "inc/result.hpp"
#include <iostream>

using DivisionResult = Result<int, std::string>;

DivisionResult Divide(int a, int b)
{
    if (b == 0)
        return DivisionResult::Err("division by 0 detected");

    return DivisionResult::Ok(a / b);
}

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
                                [](std::string const &e) -> DivisionResult
                                {
                                    std::cout << "got err: " << e << '\n';
                                    return DivisionResult::Ok(0);
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
    return 0;
}
