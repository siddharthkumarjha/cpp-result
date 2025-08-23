#include "result-helper.hpp"
#include "result-type-definition.hpp"
#include <functional>
template <typename T, typename E> template <typename F, typename> constexpr auto Result<T, E>::and_then(F &&f) &
{
    using U = fn_eval_result<F, T &>;

    static_assert(is_result_type<U>);
    static_assert(std::is_same_v<typename U::error_type, E>);

    if (is_ok())
    {
        return std::invoke(std::forward<F>(f), std::get<T>(result_variant_));
    }
    else
    {
        return make_err<typename U::value_type, typename U::error_type>(std::get<E>(result_variant_));
    }
}
template <typename T, typename E> template <typename F, typename> constexpr auto Result<T, E>::and_then(F &&f) const &
{
    using U = fn_eval_result<F, const T &>;

    static_assert(is_result_type<U>);
    static_assert(std::is_same_v<typename U::error_type, E>);

    if (is_ok())
    {
        return std::invoke(std::forward<F>(f), std::get<T>(result_variant_));
    }
    else
    {
        return make_err<typename U::value_type, typename U::error_type>(std::get<E>(result_variant_));
    }
}
template <typename T, typename E> template <typename F, typename> constexpr auto Result<T, E>::and_then(F &&f) &&
{
    using U = fn_eval_result<F, T &&>;

    static_assert(is_result_type<U>);
    static_assert(std::is_same_v<typename U::error_type, E>);

    if (is_ok())
    {
        return std::invoke(std::forward<F>(f), std::get<T>(std::move(result_variant_)));
    }
    else
    {
        return make_err<typename U::value_type, typename U::error_type>(std::get<E>(std::move(result_variant_)));
    }
}
template <typename T, typename E> template <typename F, typename> constexpr auto Result<T, E>::and_then(F &&f) const &&
{
    using U = fn_eval_result<F, const T &&>;

    static_assert(is_result_type<U>);
    static_assert(std::is_same_v<typename U::error_type, E>);

    if (is_ok())
    {
        return std::invoke(std::forward<F>(f), std::get<T>(std::move(result_variant_)));
    }
    else
    {
        return make_err<typename U::value_type, typename U::error_type>(std::get<E>(std::move(result_variant_)));
    }
}

template <typename T, typename E> template <typename F, typename> constexpr auto Result<T, E>::or_else(F &&f) &
{
    using G = fn_eval_result<F, E &>;

    static_assert(is_result_type<G>);
    static_assert(std::is_same_v<typename G::value_type, T>);

    if (is_err())
    {
        return std::invoke(std::forward<F>(f), std::get<E>(result_variant_));
    }
    else
    {
        return make_ok<typename G::value_type, typename G::error_type>(std::get<T>(result_variant_));
    }
}
template <typename T, typename E> template <typename F, typename> constexpr auto Result<T, E>::or_else(F &&f) const &
{
    using G = fn_eval_result<F, const E &>;

    static_assert(is_result_type<G>);
    static_assert(std::is_same_v<typename G::value_type, T>);

    if (is_err())
    {
        return std::invoke(std::forward<F>(f), std::get<E>(result_variant_));
    }
    else
    {
        return make_ok<typename G::value_type, typename G::error_type>(std::get<T>(result_variant_));
    }
}
template <typename T, typename E> template <typename F, typename> constexpr auto Result<T, E>::or_else(F &&f) &&
{
    using G = fn_eval_result<F, E &&>;

    static_assert(is_result_type<G>);
    static_assert(std::is_same_v<typename G::value_type, T>);

    if (is_err())
    {
        return std::invoke(std::forward<F>(f), std::get<E>(std::move(result_variant_)));
    }
    else
    {
        return make_ok<typename G::value_type, typename G::error_type>(std::get<T>(std::move(result_variant_)));
    }
}
template <typename T, typename E> template <typename F, typename> constexpr auto Result<T, E>::or_else(F &&f) const &&
{
    using G = fn_eval_result<F, const E &&>;

    static_assert(is_result_type<G>);
    static_assert(std::is_same_v<typename G::value_type, T>);

    if (is_err())
    {
        return std::invoke(std::forward<F>(f), std::get<E>(std::move(result_variant_)));
    }
    else
    {
        return make_ok<typename G::value_type, typename G::error_type>(std::get<T>(std::move(result_variant_)));
    }
}

template <typename T, typename E> template <typename F, typename> constexpr auto Result<T, E>::map(F &&f) &
{
    using U = fn_eval_result_xform<F, T &>;

    if (is_ok())
    {
        if constexpr (std::is_void_v<U>)
        {
            std::invoke(std::forward<F>(f), std::get<T>(result_variant_));
            return make_ok<void, E>();
        }
        else
        {
            return make_ok<U, E>(std::invoke(std::forward<F>(f), std::get<T>(result_variant_)));
        }
    }
    else
    {
        return make_err<U, E>(std::get<E>(result_variant_));
    }
}
template <typename T, typename E> template <typename F, typename> constexpr auto Result<T, E>::map(F &&f) const &
{
    using U = fn_eval_result_xform<F, const T &>;

    if (is_ok())
    {
        if constexpr (std::is_void_v<U>)
        {
            std::invoke(std::forward<F>(f), std::get<T>(result_variant_));
            return make_ok<void, E>();
        }
        else
        {
            return make_ok<U, E>(std::invoke(std::forward<F>(f), std::get<T>(result_variant_)));
        }
    }
    else
    {
        return make_err<U, E>(std::get<E>(result_variant_));
    }
}
template <typename T, typename E> template <typename F, typename> constexpr auto Result<T, E>::map(F &&f) &&
{
    using U = fn_eval_result_xform<F, T>;

    if (is_ok())
    {
        if constexpr (std::is_void_v<U>)
        {
            std::invoke(std::forward<F>(f), std::get<T>(std::move(result_variant_)));
            return make_ok<void, E>();
        }
        else
        {
            return make_ok<U, E>(std::invoke(std::forward<F>(f), std::get<T>(std::move(result_variant_))));
        }
    }
    else
    {
        return make_err<U, E>(std::get<E>(std::move(result_variant_)));
    }
}
template <typename T, typename E> template <typename F, typename> constexpr auto Result<T, E>::map(F &&f) const &&
{
    using U = fn_eval_result_xform<F, const T>;

    if (is_ok())
    {
        if constexpr (std::is_void_v<U>)
        {
            std::invoke(std::forward<F>(f), std::get<T>(std::move(result_variant_)));
            return make_ok<void, E>();
        }
        else
        {
            return make_ok<U, E>(std::invoke(std::forward<F>(f), std::get<T>(std::move(result_variant_))));
        }
    }
    else
    {
        return make_err<U, E>(std::get<E>(std::move(result_variant_)));
    }
}

template <typename T, typename E> template <typename F, typename> constexpr auto Result<T, E>::map_err(F &&f) &
{
    using G = fn_eval_result_xform<F, E &>;

    if (is_err())
    {
        return make_err<T, G>(std::invoke(std::forward<F>(f), std::get<E>(result_variant_)));
    }
    else
    {
        return make_ok<T, G>(std::get<T>(result_variant_));
    }
}
template <typename T, typename E> template <typename F, typename> constexpr auto Result<T, E>::map_err(F &&f) const &
{
    using G = fn_eval_result_xform<F, const E &>;

    if (is_err())
    {
        return make_err<T, G>(std::invoke(std::forward<F>(f), std::get<E>(result_variant_)));
    }
    else
    {
        return make_ok<T, G>(std::get<T>(result_variant_));
    }
}
template <typename T, typename E> template <typename F, typename> constexpr auto Result<T, E>::map_err(F &&f) &&
{
    using G = fn_eval_result_xform<F, E &&>;

    if (is_err())
    {
        return make_err<T, G>(std::invoke(std::forward<F>(f), std::get<E>(std::move(result_variant_))));
    }
    else
    {
        return make_ok<T, G>(std::get<T>(std::move(result_variant_)));
    }
}
template <typename T, typename E> template <typename F, typename> constexpr auto Result<T, E>::map_err(F &&f) const &&
{
    using G = fn_eval_result_xform<F, const E &&>;

    if (is_err())
    {
        return make_err<T, G>(std::invoke(std::forward<F>(f), std::get<E>(std::move(result_variant_))));
    }
    else
    {
        return make_ok<T, G>(std::get<T>(std::move(result_variant_)));
    }
}

/////////////////////////////////////////////////////////////////////////
// void specialization
/////////////////////////////////////////////////////////////////////////

template <typename E> template <typename F, typename> constexpr auto Result<void, E>::and_then(F &&f) &
{
    using U = fn_eval_result_no_arg<F>;

    static_assert(is_result_type<U>);
    static_assert(std::is_same_v<typename U::error_type, E>);

    if (is_ok())
    {
        return std::invoke(std::forward<F>(f));
    }
    else
    {
        return make_err<typename U::value_type, typename U::error_type>(std::get<E>(result_variant_));
    }
}
template <typename E> template <typename F, typename> constexpr auto Result<void, E>::and_then(F &&f) const &
{
    using U = fn_eval_result_no_arg<F>;

    static_assert(is_result_type<U>);
    static_assert(std::is_same_v<typename U::error_type, E>);

    if (is_ok())
    {
        return std::invoke(std::forward<F>(f));
    }
    else
    {
        return make_err<typename U::value_type, typename U::error_type>(std::get<E>(result_variant_));
    }
}
template <typename E> template <typename F, typename> constexpr auto Result<void, E>::and_then(F &&f) &&
{
    using U = fn_eval_result_no_arg<F>;

    static_assert(is_result_type<U>);
    static_assert(std::is_same_v<typename U::error_type, E>);

    if (is_ok())
    {
        return std::invoke(std::forward<F>(f));
    }
    else
    {
        return make_err<typename U::value_type, typename U::error_type>(std::get<E>(std::move(result_variant_)));
    }
}
template <typename E> template <typename F, typename> constexpr auto Result<void, E>::and_then(F &&f) const &&
{
    using U = fn_eval_result_no_arg<F>;

    static_assert(is_result_type<U>);
    static_assert(std::is_same_v<typename U::error_type, E>);

    if (is_ok())
    {
        return std::invoke(std::forward<F>(f));
    }
    else
    {
        return make_err<typename U::value_type, typename U::error_type>(std::get<E>(std::move(result_variant_)));
    }
}

template <typename E> template <typename F> constexpr auto Result<void, E>::or_else(F &&f) &
{
    using G = fn_eval_result<F, E &>;

    static_assert(is_result_type<G>);
    static_assert(std::is_same_v<typename G::value_type, void>);

    if (is_err())
    {
        return std::invoke(std::forward<F>(f), std::get<E>(result_variant_));
    }
    else
    {
        return make_ok<void, typename G::error_type>();
    }
}
template <typename E> template <typename F> constexpr auto Result<void, E>::or_else(F &&f) const &
{
    using G = fn_eval_result<F, const E &>;

    static_assert(is_result_type<G>);
    static_assert(std::is_same_v<typename G::value_type, void>);

    if (is_err())
    {
        return std::invoke(std::forward<F>(f), std::get<E>(result_variant_));
    }
    else
    {
        return make_ok<void, typename G::error_type>();
    }
}
template <typename E> template <typename F> constexpr auto Result<void, E>::or_else(F &&f) &&
{
    using G = fn_eval_result<F, E &&>;

    static_assert(is_result_type<G>);
    static_assert(std::is_same_v<typename G::value_type, void>);

    if (is_err())
    {
        return std::invoke(std::forward<F>(f), std::get<E>(std::move(result_variant_)));
    }
    else
    {
        return make_ok<void, typename G::error_type>();
    }
}
template <typename E> template <typename F> constexpr auto Result<void, E>::or_else(F &&f) const &&
{
    using G = fn_eval_result<F, const E &&>;

    static_assert(is_result_type<G>);
    static_assert(std::is_same_v<typename G::value_type, void>);

    if (is_err())
    {
        return std::invoke(std::forward<F>(f), std::get<E>(std::move(result_variant_)));
    }
    else
    {
        return make_ok<void, typename G::error_type>();
    }
}

template <typename E> template <typename F, typename> constexpr auto Result<void, E>::map(F &&f) &
{
    using U = fn_eval_result_xform_no_arg<F>;

    if (is_ok())
    {
        return make_ok<U, E>(std::invoke(std::forward<F>(f)));
    }
    else
    {
        return make_err<U, E>(std::get<E>(result_variant_));
    }
}
template <typename E> template <typename F, typename> constexpr auto Result<void, E>::map(F &&f) const &
{
    using U = fn_eval_result_xform_no_arg<F>;

    if (is_ok())
    {
        return make_ok<U, E>(std::invoke(std::forward<F>(f)));
    }
    else
    {
        return make_err<U, E>(std::get<E>(result_variant_));
    }
}
template <typename E> template <typename F, typename> constexpr auto Result<void, E>::map(F &&f) &&
{
    using U = fn_eval_result_xform_no_arg<F>;

    if (is_ok())
    {
        return make_ok<U, E>(std::invoke(std::forward<F>(f)));
    }
    else
    {
        return make_err<U, E>(std::get<E>(std::move(result_variant_)));
    }
}
template <typename E> template <typename F, typename> constexpr auto Result<void, E>::map(F &&f) const &&
{
    using U = fn_eval_result_xform_no_arg<F>;

    if (is_ok())
    {
        return make_ok<U, E>(std::invoke(std::forward<F>(f)));
    }
    else
    {
        return make_err<U, E>(std::get<E>(std::move(result_variant_)));
    }
}

template <typename E> template <typename F> constexpr auto Result<void, E>::map_err(F &&f) &
{
    using G = fn_eval_result_xform<F, E &>;

    if (is_err())
    {
        return make_err<void, G>(std::invoke(std::forward<F>(f), std::get<E>(result_variant_)));
    }
    else
    {
        return make_ok<void, G>();
    }
}
template <typename E> template <typename F> constexpr auto Result<void, E>::map_err(F &&f) const &
{
    using G = fn_eval_result_xform<F, const E &>;

    if (is_err())
    {
        return make_err<void, G>(std::invoke(std::forward<F>(f), std::get<E>(result_variant_)));
    }
    else
    {
        return make_ok<void, G>();
    }
}
template <typename E> template <typename F> constexpr auto Result<void, E>::map_err(F &&f) &&
{
    using G = fn_eval_result_xform<F, E &&>;

    if (is_err())
    {
        return make_err<void, G>(std::invoke(std::forward<F>(f), std::get<E>(std::move(result_variant_))));
    }
    else
    {
        return make_ok<void, G>();
    }
}
template <typename E> template <typename F> constexpr auto Result<void, E>::map_err(F &&f) const &&
{
    using G   = fn_eval_result_xform<F, const E &&>;
    using Res = Result<void, G>;

    if (is_err())
    {
        return make_err<void, G>(std::invoke(std::forward<F>(f), std::get<E>(std::move(result_variant_))));
    }
    else
    {
        return make_ok<void, G>();
    }
}
