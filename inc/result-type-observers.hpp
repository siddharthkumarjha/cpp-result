#include "panic.hpp"
#include "result-helper.hpp"
#include "result-type-definition.hpp"
#include <functional>
#include <type_traits>

template <typename T, typename E> constexpr bool Result<T, E>::is_ok() const noexcept
{
    return result_variant_.index() == ResultKind::Ok;
}
template <typename T, typename E> constexpr bool Result<T, E>::is_err() const noexcept
{
    return result_variant_.index() == ResultKind::Err;
}

template <typename T, typename E> template <typename F> constexpr bool Result<T, E>::is_ok_and(F &&f) const &
{
    using Res = fn_eval_result<F, const T &>;
    static_assert(std::is_same_v<Res, bool>);

    if (is_ok())
    {
        return std::invoke(std::forward<F>(f), std::get<T>(result_variant_));
    }
    return false;
}
template <typename T, typename E> template <typename F> constexpr bool Result<T, E>::is_ok_and(F &&f) &&
{
    using Res = fn_eval_result<F, T &&>;
    static_assert(std::is_same_v<Res, bool>);

    if (is_ok())
    {
        return std::invoke(std::forward<F>(f), std::get<T>(std::move(result_variant_)));
    }
    return false;
}

template <typename T, typename E> template <typename F> constexpr bool Result<T, E>::is_err_and(F &&f) const &
{
    using Res = fn_eval_result<F, const E &>;
    static_assert(std::is_same_v<Res, bool>);

    if (is_err())
    {
        return std::invoke(std::forward<F>(f), std::get<E>(result_variant_));
    }
    return false;
}
template <typename T, typename E> template <typename F> constexpr bool Result<T, E>::is_err_and(F &&f) &&
{
    using Res = fn_eval_result<F, E &&>;
    static_assert(std::is_same_v<Res, bool>);

    if (is_err())
    {
        return std::invoke(std::forward<F>(f), std::get<E>(std::move(result_variant_)));
    }
    return false;
}

template <typename T, typename E>
template <typename OkFn, typename ErrFn>
constexpr auto Result<T, E>::match(OkFn &&ok_fn, ErrFn &&err_fn) & -> std::invoke_result_t<OkFn, T &>
{
    static_assert(std::is_invocable_v<OkFn, T &>);
    static_assert(std::is_invocable_v<ErrFn, E &>);
    using OkRes  = fn_eval_result<OkFn, T &>;
    using ErrRes = fn_eval_result<ErrFn, E &>;
    static_assert(std::is_convertible_v<OkRes, ErrRes>);
    if (is_ok())
    {
        return std::invoke(std::forward<OkFn>(ok_fn), std::get<T>(result_variant_));
    }
    else
    {
        return std::invoke(std::forward<ErrFn>(err_fn), std::get<E>(result_variant_));
    }
}
template <typename T, typename E>
template <typename OkFn, typename ErrFn>
constexpr auto Result<T, E>::match(OkFn &&ok_fn, ErrFn &&err_fn) const & -> std::invoke_result_t<OkFn, T const &>
{
    static_assert(std::is_invocable_v<OkFn, const T &>);
    static_assert(std::is_invocable_v<ErrFn, const E &>);
    using OkRes  = fn_eval_result<OkFn, const T &>;
    using ErrRes = fn_eval_result<ErrFn, const E &>;
    static_assert(std::is_convertible_v<OkRes, ErrRes>);
    if (is_ok())
    {
        return std::invoke(std::forward<OkFn>(ok_fn), std::get<T>(result_variant_));
    }
    else
    {
        return std::invoke(std::forward<ErrFn>(err_fn), std::get<E>(result_variant_));
    }
}
template <typename T, typename E>
template <typename OkFn, typename ErrFn>
constexpr auto Result<T, E>::match(OkFn &&ok_fn, ErrFn &&err_fn) && -> std::invoke_result_t<OkFn, T &&>
{
    static_assert(std::is_invocable_v<OkFn, T &&>);
    static_assert(std::is_invocable_v<ErrFn, E &&>);
    using OkRes  = fn_eval_result<OkFn, T &&>;
    using ErrRes = fn_eval_result<ErrFn, E &&>;
    static_assert(std::is_convertible_v<OkRes, ErrRes>);
    if (is_ok())
    {
        return std::invoke(std::forward<OkFn>(ok_fn), std::get<T>(std::move(result_variant_)));
    }
    else
    {
        return std::invoke(std::forward<ErrFn>(err_fn), std::get<E>(std::move(result_variant_)));
    }
}

template <typename T, typename E> constexpr T &Result<T, E>::unwrap() &
{
    static_assert(std::is_copy_assignable_v<E>, "ill-formed");
    if (is_err())
    {
        panic("called `Result::unwrap()` on an `Err` value ", std::get<E>(result_variant_));
    }
    return std::get<T>(result_variant_);
}
template <typename T, typename E> constexpr const T &Result<T, E>::unwrap() const &
{
    static_assert(std::is_copy_assignable_v<E>, "ill-formed");
    if (is_err())
    {
        panic("called `Result::unwrap()` on an `Err` value ", std::get<E>(result_variant_));
    }
    return std::get<T>(result_variant_);
}
template <typename T, typename E> constexpr T &&Result<T, E>::unwrap() &&
{
    static_assert(std::is_move_assignable_v<E>, "ill-formed");
    if (is_err())
    {
        panic("called `Result::unwrap()` on an `Err` value ", std::get<E>(std::move(result_variant_)));
    }
    return std::get<T>(std::move(result_variant_));
}
template <typename T, typename E> constexpr const T &&Result<T, E>::unwrap() const &&
{
    static_assert(std::is_move_assignable_v<E>, "ill-formed");
    if (is_err())
    {
        panic("called `Result::unwrap()` on an `Err` value ", std::get<E>(std::move(result_variant_)));
    }
    return std::get<T>(std::move(result_variant_));
}

template <typename T, typename E> constexpr E &Result<T, E>::unwrap_err() &
{
    static_assert(std::is_copy_assignable_v<T>, "ill-formed");
    if (is_ok())
    {
        panic("called `Result::unwrap_err()` on an `Ok` value ", std::get<T>(result_variant_));
    }
    return std::get<E>(result_variant_);
}
template <typename T, typename E> constexpr const E &Result<T, E>::unwrap_err() const &
{
    static_assert(std::is_copy_assignable_v<T>, "ill-formed");
    if (is_ok())
    {
        panic("called `Result::unwrap_err()` on an `Ok` value ", std::get<T>(result_variant_));
    }
    return std::get<E>(result_variant_);
}
template <typename T, typename E> constexpr E &&Result<T, E>::unwrap_err() &&
{
    static_assert(std::is_move_assignable_v<T>, "ill-formed");
    if (is_ok())
    {
        panic("called `Result::unwrap_err()` on an `Ok` value ", std::get<T>(std::move(result_variant_)));
    }
    return std::get<E>(std::move(result_variant_));
}
template <typename T, typename E> constexpr const E &&Result<T, E>::unwrap_err() const &&
{
    static_assert(std::is_move_assignable_v<T>, "ill-formed");
    if (is_ok())
    {
        panic("called `Result::unwrap_err()` on an `Ok` value ", std::get<T>(std::move(result_variant_)));
    }
    return std::get<E>(std::move(result_variant_));
}

template <typename T, typename E> template <class U> constexpr T Result<T, E>::unwrap_or(U &&default_value) const &
{
    static_assert(std::is_copy_constructible_v<T>);
    static_assert(std::is_convertible_v<U, T>);

    return is_ok() ? std::get<T>(result_variant_) : static_cast<T>(std::forward<U>(default_value));
}
template <typename T, typename E> template <class U> constexpr T Result<T, E>::unwrap_or(U &&default_value) &&
{
    static_assert(std::is_move_constructible_v<T>);
    static_assert(std::is_convertible_v<U, T>);

    return is_ok() ? std::get<T>(std::move(result_variant_)) : static_cast<T>(std::forward<U>(default_value));
}

template <typename T, typename E> template <class G> constexpr E Result<T, E>::unwrap_err_or(G &&default_value) const &
{
    static_assert(std::is_copy_constructible_v<E>);
    static_assert(std::is_convertible_v<G, E>);

    return is_err() ? std::get<E>(result_variant_) : static_cast<E>(std::forward<G>(default_value));
}
template <typename T, typename E> template <class G> constexpr E Result<T, E>::unwrap_err_or(G &&default_value) &&
{
    static_assert(std::is_move_constructible_v<E>);
    static_assert(std::is_convertible_v<G, E>);

    return is_err() ? std::get<E>(std::move(result_variant_)) : static_cast<E>(std::forward<G>(default_value));
}

/////////////////////////////////////////////////////////////////////////
// void specialization
/////////////////////////////////////////////////////////////////////////

template <typename E> constexpr bool Result<void, E>::is_ok() const noexcept
{
    return result_variant_.index() == ResultKind::Ok;
}
template <typename E> constexpr bool Result<void, E>::is_err() const noexcept
{
    return result_variant_.index() == ResultKind::Err;
}

template <typename E> template <typename F> constexpr bool Result<void, E>::is_ok_and(F &&f)
{
    using Res = fn_eval_result_no_arg<F>;
    static_assert(std::is_same_v<Res, bool>);

    if (is_ok())
    {
        return std::invoke(std::forward<F>(f));
    }
    return false;
}

template <typename E> template <typename F> constexpr bool Result<void, E>::is_err_and(F &&f) const &
{
    using Res = fn_eval_result<F, const E &>;
    static_assert(std::is_same_v<Res, bool>);

    if (is_err())
    {
        return std::invoke(std::forward<F>(f), std::get<E>(result_variant_));
    }
    return false;
}
template <typename E> template <typename F> constexpr bool Result<void, E>::is_err_and(F &&f) &&
{
    using Res = fn_eval_result<F, E &&>;
    static_assert(std::is_same_v<Res, bool>);

    if (is_err())
    {
        return std::invoke(std::forward<F>(f), std::get<E>(std::move(result_variant_)));
    }
    return false;
}

template <typename E>
template <typename OkFn, typename ErrFn>
constexpr auto Result<void, E>::match(OkFn &&ok_fn, ErrFn &&err_fn) & -> std::invoke_result_t<OkFn>
{
    static_assert(std::is_invocable_v<OkFn>);
    static_assert(std::is_invocable_v<ErrFn, E &>);
    using OkRes  = fn_eval_result_no_arg<OkFn>;
    using ErrRes = fn_eval_result<ErrFn, E &>;
    static_assert(std::is_convertible_v<OkRes, ErrRes>);
    if (is_ok())
    {
        return std::invoke(std::forward<OkFn>(ok_fn));
    }
    else
    {
        return std::invoke(std::forward<ErrFn>(err_fn), std::get<E>(result_variant_));
    }
}
template <typename E>
template <typename OkFn, typename ErrFn>
constexpr auto Result<void, E>::match(OkFn &&ok_fn, ErrFn &&err_fn) const & -> std::invoke_result_t<OkFn>
{
    static_assert(std::is_invocable_v<OkFn>);
    static_assert(std::is_invocable_v<ErrFn, const E &>);
    using OkRes  = fn_eval_result_no_arg<OkFn>;
    using ErrRes = fn_eval_result<ErrFn, const E &>;
    static_assert(std::is_convertible_v<OkRes, ErrRes>);
    if (is_ok())
    {
        return std::invoke(std::forward<OkFn>(ok_fn));
    }
    else
    {
        return std::invoke(std::forward<ErrFn>(err_fn), std::get<E>(result_variant_));
    }
}
template <typename E>
template <typename OkFn, typename ErrFn>
constexpr auto Result<void, E>::match(OkFn &&ok_fn, ErrFn &&err_fn) && -> std::invoke_result_t<OkFn>
{
    static_assert(std::is_invocable_v<OkFn>);
    static_assert(std::is_invocable_v<ErrFn, E &&>);
    using OkRes  = fn_eval_result_no_arg<OkFn>;
    using ErrRes = fn_eval_result<ErrFn, E &&>;
    static_assert(std::is_convertible_v<OkRes, ErrRes>);
    if (is_ok())
    {
        return std::invoke(std::forward<OkFn>(ok_fn));
    }
    else
    {
        return std::invoke(std::forward<ErrFn>(err_fn), std::get<E>(std::move(result_variant_)));
    }
}

template <typename E> constexpr void Result<void, E>::unwrap() const &
{
    static_assert(std::is_copy_constructible_v<E>);

    if (is_err())
    {
        panic("called `Result::unwrap()` on an `Err` value ", std::get<E>(result_variant_));
    }
}
template <typename E> constexpr void Result<void, E>::unwrap() &&
{
    static_assert(std::is_copy_constructible_v<E>);
    static_assert(std::is_move_constructible_v<E>);

    if (is_err())
    {
        panic("called `Result::unwrap()` on an `Err` value ", std::get<E>(std::move(result_variant_)));
    }
}

template <typename E> constexpr E &Result<void, E>::unwrap_err() &
{
    if (is_ok())
    {
        panic("called `Result::unwrap_err()` on an void `Ok` value");
    }
    return std::get<E>(result_variant_);
}
template <typename E> constexpr const E &Result<void, E>::unwrap_err() const &
{
    if (is_ok())
    {
        panic("called `Result::unwrap_err()` on an void `Ok` value");
    }
    return std::get<E>(result_variant_);
}
template <typename E> constexpr E &&Result<void, E>::unwrap_err() &&
{
    if (is_ok())
    {
        panic("called `Result::unwrap_err()` on an void `Ok` value");
    }
    return std::get<E>(std::move(result_variant_));
}
template <typename E> constexpr const E &&Result<void, E>::unwrap_err() const &&
{
    if (is_ok())
    {
        panic("called `Result::unwrap_err()` on an void `Ok` value");
    }
    return std::get<E>(std::move(result_variant_));
}

template <typename E> template <class G> constexpr E Result<void, E>::unwrap_err_or(G &&default_value) const &
{
    static_assert(std::is_copy_constructible_v<E>);
    static_assert(std::is_convertible_v<G, E>);

    return is_err() ? std::get<E>(result_variant_) : static_cast<E>(std::forward<G>(default_value));
}
template <typename E> template <class G> constexpr E Result<void, E>::unwrap_err_or(G &&default_value) &&
{
    static_assert(std::is_move_constructible_v<E>);
    static_assert(std::is_convertible_v<G, E>);

    return is_err() ? std::get<E>(std::move(result_variant_)) : static_cast<E>(std::forward<G>(default_value));
}
