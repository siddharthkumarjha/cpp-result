#pragma once
#include "result-helper.hpp"
#include <utility>

template <typename T> struct [[nodiscard]] Ok : check_value_type<T>
{
    using value_type = T;

    template <typename Tp, typename Er> friend class Result;

    explicit constexpr Ok(T &&value) : value_(std::move(value)) {}
    explicit constexpr Ok(const T &value) : value_(value) {}

    constexpr T copy() const { return value_; }
    constexpr T &&move() { return std::move(value_); }
    constexpr T &ref() { return value_; }
    constexpr T const &cref() const { return value_; }

private:
    T value_;
};

template <typename E> struct [[nodiscard]] Err : check_value_type<E>
{
    using value_type = E;

    template <typename Tp, typename Er> friend class Result;

    explicit constexpr Err(E &&value) : value_(std::move(value)) {}
    explicit constexpr Err(const E &value) : value_(value) {}

    constexpr E copy() const { return value_; }
    constexpr E &&move() { return std::move(value_); }
    constexpr E &ref() { return value_; }
    constexpr E const &cref() const { return value_; }

private:
    E value_;
};

/////////////////////////////////////////////////////////////////////////
// void specialization
/////////////////////////////////////////////////////////////////////////

template <> struct [[nodiscard]] Ok<void>
{
    using value_type = void;

    template <typename Tp, typename Er> friend class Result;
    explicit constexpr Ok() = default;
};
