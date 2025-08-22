#pragma once
#include <utility>
#include <variant>

enum ResultKind : std::size_t
{
    Ok = 0,
    Err,
};

template <typename T, typename E> class Result
{
    static_assert(std::is_nothrow_move_constructible_v<T>, "T must be nothrow move constructible");
    static_assert(std::is_nothrow_move_constructible_v<E>, "E must be nothrow move constructible");

public:
    using value_type = T;
    using error_type = E;

private:
    std::variant<T, E> result_variant_;

    explicit Result(std::variant<T, E> &&v) : result_variant_(std::move(v)) {}

public:
    // constructors of result type
    static Result<T, E> Ok(T &&value);
    static Result<T, E> Err(E &&error);

    static Result<T, E> Ok(const T &value);
    static Result<T, E> Err(const E &error);

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

template <typename E> class Result<void, E>
{
    static_assert(std::is_nothrow_move_constructible_v<E>, "E must be nothrow move constructible");

public:
    using value_type = void;
    using error_type = E;

private:
    std::variant<std::monostate, E> result_variant_;

    explicit Result(std::variant<std::monostate, E> &&v) : result_variant_(std::move(v)) {}

public:
    // constructors of result type
    static Result<void, E> Ok();
    static Result<void, E> Err(E &&error);
    static Result<void, E> Err(const E &error);

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
