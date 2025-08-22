#pragma once
#include "result-type-definition.hpp"
#include <type_traits>

template <class T> struct remove_cvref
{
    using type = std::remove_cv_t<std::remove_reference_t<T>>;
};
template <class T> using remove_cvref_t                                       = typename remove_cvref<T>::type;

template <typename T> constexpr bool is_result_type                           = false;
template <typename T, typename E> constexpr bool is_result_type<Result<T, E>> = true;

template <typename F, typename T> using fn_eval_result       = remove_cvref_t<std::invoke_result_t<F &&, T &&>>;
template <typename F, typename T> using fn_eval_result_xform = std::remove_cv_t<std::invoke_result_t<F &&, T &&>>;

template <typename F> using fn_eval_result_no_arg            = remove_cvref_t<std::invoke_result_t<F &&>>;
template <typename F> using fn_eval_result_xform_no_arg      = std::remove_cv_t<std::invoke_result_t<F &&>>;
