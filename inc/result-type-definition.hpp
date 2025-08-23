#pragma once
#include "result-type-constructor.hpp"
#include <utility>
#include <variant>

template <typename T, typename E> class Result : check_value_type<T>, check_value_type<E>
{
    static_assert(std::is_nothrow_move_constructible_v<T>, "T must be nothrow move constructible");
    static_assert(std::is_nothrow_move_constructible_v<E>, "E must be nothrow move constructible");

    enum ResultKind : std::size_t
    {
        Ok = 0,
        Err,
    };

public:
    using value_type = T;
    using error_type = E;
    using storage    = std::variant<T, E>;

private:
    storage result_variant_;

    constexpr explicit Result(storage &&v) : result_variant_(std::move(v)) {}

public:
    // constructors of result type
    constexpr Result(::Ok<T> &&ok) : Result{storage{std::in_place_index<ResultKind::Ok>, ok.move()}} {}
    constexpr Result(::Err<E> &&err) : Result{storage{std::in_place_index<ResultKind::Err>, err.move()}} {}

    constexpr Result &operator=(::Ok<T> &&other)
    {
        result_variant_ = storage{std::in_place_index<ResultKind::Ok>, other.move()};
        return *this;
    }
    constexpr Result &operator=(::Err<E> &&other)
    {
        result_variant_ = storage{std::in_place_index<ResultKind::Err>, other.move()};
        return *this;
    }

    constexpr Result(Result &&)                      = default;
    constexpr Result &operator=(Result &&)           = default;

    constexpr Result()                               = delete;
    constexpr Result(Result const &other)            = delete;
    constexpr Result &operator=(Result const &other) = delete;

    /////////////////////////////////////////////////////////////////////////
    // Observers
    /////////////////////////////////////////////////////////////////////////

    // check for result type
    constexpr bool is_ok() const noexcept;
    constexpr bool is_err() const noexcept;

    // Returns `true` if the result is [`Ok`] and the value inside of it matches a predicate.
    template <typename F> constexpr bool is_ok_and(F &&f) const &;
    template <typename F> constexpr bool is_ok_and(F &&f) &&;
    // Returns `true` if the result is [`Err`] and the value inside of it matches a predicate.
    template <typename F> constexpr bool is_err_and(F &&f) const &;
    template <typename F> constexpr bool is_err_and(F &&f) &&;

    // Calls the parameter `ok_fn` with the value if this result is an `Ok<T>`,
    // else calls `err_fn` with the error.
    //
    // The return type of both parameters must be convertible. They can also both
    // return nothing ( `void` ).
    //
    //
    // # Examples
    //
    // Basic usage:
    //
    // ``` cpp
    // auto i = make_ok<int, string_view>(99);
    //
    // auto j = move(i).match([](int value) { return value; },
    //                        [](string_view) { return -1; });
    // ASSERT_EQ(j, 99);
    //
    //
    // auto x = make_err<int, string_view>("404 Not Found"sv);
    // // you can return nothing (void)
    // x.match([](int&) {},
    //         [](string_view& s) { std::cout << "Error: " << s << "\n"; });
    // ```
    template <typename OkFn, typename ErrFn>
    constexpr auto match(OkFn &&ok_fn, ErrFn &&err_fn) & -> std::invoke_result_t<OkFn, T &>;
    template <typename OkFn, typename ErrFn>
    constexpr auto match(OkFn &&ok_fn, ErrFn &&err_fn) const & -> std::invoke_result_t<OkFn, T const &>;
    template <typename OkFn, typename ErrFn>
    constexpr auto match(OkFn &&ok_fn, ErrFn &&err_fn) && -> std::invoke_result_t<OkFn, T &&>;

    // fetch `Ok` value if it exists, otherwise throw
    constexpr T &unwrap() &;
    constexpr const T &unwrap() const &;
    constexpr T &&unwrap() &&;
    constexpr const T &&unwrap() const &&;

    // fetch `Err` value if it exists, otherwise throw
    constexpr E &unwrap_err() &;
    constexpr const E &unwrap_err() const &;
    constexpr E &&unwrap_err() &&;
    constexpr const E &&unwrap_err() const &&;

    // fetch `Ok` value if it exists, otherwise return default value
    template <typename U = std::remove_cv_t<T>> constexpr T unwrap_or(U &&default_value) const &;
    template <typename U = std::remove_cv_t<T>> constexpr T unwrap_or(U &&default_value) &&;

    // fetch `Err` value if it exists, otherwise return default value
    template <typename G = E> constexpr E unwrap_err_or(G &&default_value) const &;
    template <typename G = E> constexpr E unwrap_err_or(G &&default_value) &&;

    /////////////////////////////////////////////////////////////////////////
    // Monadics
    /////////////////////////////////////////////////////////////////////////

    // call fn `f` on `Ok` type if it exists, return Result<T, E> from `f(T)` or `Err(E)`
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<E, E &>>>
    constexpr auto and_then(F &&f) &;
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<E, const E &>>>
    constexpr auto and_then(F &&f) const &;
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<E, E>>> constexpr auto and_then(F &&f) &&;
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<E, const E>>>
    constexpr auto and_then(F &&f) const &&;

    // call fn `f` on `Err` type if it exists, return Result<T, E> from `f(E)` or `Ok(T)`
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<T, T &>>> constexpr auto or_else(F &&f) &;
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<T, const T &>>>
    constexpr auto or_else(F &&f) const &;
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<T, T>>> constexpr auto or_else(F &&f) &&;
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<T, const T>>>
    constexpr auto or_else(F &&f) const &&;

    /////////////////////////////////////////////////////////////////////////
    // Transforming contained values
    /////////////////////////////////////////////////////////////////////////

    // Maps a `Result<T, E>` to `Result<U, E>` by applying a function `f(T) -> U` to a
    // contained [`Ok`] value, leaving an [`Err`] value untouched.
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<E, E &>>> constexpr auto map(F &&f) &;
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<E, const E &>>>
    constexpr auto map(F &&f) const &;
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<E, E>>> constexpr auto map(F &&f) &&;
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<E, const E>>>
    constexpr auto map(F &&f) const &&;

    // Maps a `Result<T, E>` to `Result<T, G>` by applying a function `f(E) -> G` to a
    // contained [`Err`] value, leaving an [`Ok`] value untouched.
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<T, T &>>> constexpr auto map_err(F &&f) &;
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<T, const T &>>>
    constexpr auto map_err(F &&f) const &;
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<T, T>>> constexpr auto map_err(F &&f) &&;
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<T, const T>>>
    constexpr auto map_err(F &&f) const &&;
};

/////////////////////////////////////////////////////////////////////////
// void specialization
/////////////////////////////////////////////////////////////////////////

template <typename E> class Result<void, E> : check_value_type<E>
{
    static_assert(std::is_nothrow_move_constructible_v<E>, "E must be nothrow move constructible");
    enum ResultKind : std::size_t
    {
        Ok = 0,
        Err,
    };

public:
    using value_type = void;
    using error_type = E;
    using storage    = std::variant<std::monostate, E>;

private:
    storage result_variant_;

    explicit Result(storage &&v) : result_variant_(std::move(v)) {}

public:
    // constructors of result type
    constexpr Result(::Ok<void> &&ok) : Result{storage{std::in_place_index<ResultKind::Ok>, std::monostate{}}} {}
    constexpr Result(::Err<E> &&err) : Result{storage{std::in_place_index<ResultKind::Err>, err.move()}} {}

    constexpr Result &operator=(::Ok<void> &&other)
    {
        result_variant_ = storage{std::in_place_index<ResultKind::Ok>, std::monostate{}};
        return *this;
    }
    constexpr Result &operator=(::Err<E> &&other)
    {
        result_variant_ = storage{std::in_place_index<ResultKind::Err>, other.move()};
        return *this;
    }

    constexpr Result(Result &&)                      = default;
    constexpr Result &operator=(Result &&)           = default;

    constexpr Result()                               = delete;
    constexpr Result(Result const &other)            = delete;
    constexpr Result &operator=(Result const &other) = delete;

    /////////////////////////////////////////////////////////////////////////
    // Observers
    /////////////////////////////////////////////////////////////////////////

    // check for result type
    constexpr bool is_ok() const noexcept;
    constexpr bool is_err() const noexcept;

    // Returns `true` if the result is [`Ok`] and the value inside of it matches a predicate.
    template <typename F> constexpr bool is_ok_and(F &&f);
    // Returns `true` if the result is [`Err`] and the value inside of it matches a predicate.
    template <typename F> constexpr bool is_err_and(F &&f) const &;
    template <typename F> constexpr bool is_err_and(F &&f) &&;

    // Calls the parameter `ok_fn` with the value if this result is an `Ok<T>`,
    // else calls `err_fn` with the error.
    //
    // The return type of both parameters must be convertible. They can also both
    // return nothing ( `void` ).
    //
    template <typename OkFn, typename ErrFn>
    constexpr auto match(OkFn &&ok_fn, ErrFn &&err_fn) & -> std::invoke_result_t<OkFn>;
    template <typename OkFn, typename ErrFn>
    constexpr auto match(OkFn &&ok_fn, ErrFn &&err_fn) const & -> std::invoke_result_t<OkFn>;
    template <typename OkFn, typename ErrFn>
    constexpr auto match(OkFn &&ok_fn, ErrFn &&err_fn) && -> std::invoke_result_t<OkFn>;

    // fetch `Ok` value if it exists, otherwise throw
    constexpr void unwrap() const &;
    constexpr void unwrap() &&;

    // fetch `Err` value if it exists, otherwise throw
    constexpr E &unwrap_err() &;
    constexpr const E &unwrap_err() const &;
    constexpr E &&unwrap_err() &&;
    constexpr const E &&unwrap_err() const &&;

    // fetch `Err` value if it exists, otherwise return default value
    template <typename G = E> constexpr E unwrap_err_or(G &&default_value) const &;
    template <typename G = E> constexpr E unwrap_err_or(G &&default_value) &&;

    /////////////////////////////////////////////////////////////////////////
    // Monadics
    /////////////////////////////////////////////////////////////////////////

    // call fn `f` on `Ok` type if it exists, return Result<T, E> from `f(T)` or `Err(E)`
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<E, E &>>>
    constexpr auto and_then(F &&f) &;
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<E, const E &>>>
    constexpr auto and_then(F &&f) const &;
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<E, E>>> constexpr auto and_then(F &&f) &&;
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<E, const E>>>
    constexpr auto and_then(F &&f) const &&;

    // call fn `f` on `Err` type if it exists, return Result<T, E> from `f(E)` or `Ok(T)`
    template <typename F> constexpr auto or_else(F &&f) &;
    template <typename F> constexpr auto or_else(F &&f) const &;
    template <typename F> constexpr auto or_else(F &&f) &&;
    template <typename F> constexpr auto or_else(F &&f) const &&;

    /////////////////////////////////////////////////////////////////////////
    // Transforming contained values
    /////////////////////////////////////////////////////////////////////////

    // Maps a `Result<T, E>` to `Result<U, E>` by applying a function `f(T) -> U` to a
    // contained [`Ok`] value, leaving an [`Err`] value untouched.
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<E, E &>>> constexpr auto map(F &&f) &;
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<E, const E &>>>
    constexpr auto map(F &&f) const &;
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<E, E>>> constexpr auto map(F &&f) &&;
    template <typename F, typename = std::enable_if_t<std::is_constructible_v<E, const E>>>
    constexpr auto map(F &&f) const &&;

    // Maps a `Result<T, E>` to `Result<T, G>` by applying a function `f(E) -> G` to a
    // contained [`Err`] value, leaving an [`Ok`] value untouched.
    template <typename F> constexpr auto map_err(F &&f) &;
    template <typename F> constexpr auto map_err(F &&f) const &;
    template <typename F> constexpr auto map_err(F &&f) &&;
    template <typename F> constexpr auto map_err(F &&f) const &&;
};

/// Basic usage:
///
///
/// ``` cpp
/// // these are some of the various ways to construct on Result<T, E> with an
/// // Ok<T> value
/// Result<int, string> a = Ok(8);
/// Result<int, string> b = Ok<int>(8);
///
/// // to make it easier and less verbose:
/// auto c = make_ok<int, string>(9); // 'c' = Result<string, int>
template <typename T, typename E> constexpr auto make_ok(T &&value) -> Result<T, E>
{
    return Ok<T>(std::forward<T>(value));
}
template <typename T, typename E> constexpr auto make_ok(const T &value) -> Result<T, E> { return Ok<T>(value); }
template <typename = void, typename E> constexpr auto make_ok() -> Result<void, E> { return Ok<void>(); }

/// Basic usage:
///
///
/// ``` cpp
///
/// // these are some of the various ways to construct on Result<T, E> with an
/// // Err<E> value
/// Result<int, string> a = Err("foo"s);
/// Result<int, string> b = Err<string>("foo"s);
///
/// // to make it easier and less verbose:
/// auto c = make_err<int, string>("bar"s); // 'c' = Result<int, string>
template <typename T, typename E> constexpr auto make_err(E &&err) -> Result<T, E>
{
    return Err<E>(std::forward<E>(err));
}
template <typename T, typename E> constexpr auto make_err(const E &err) -> Result<T, E> { return Err<E>(err); }
