#pragma once
#include "result-type-definition.hpp"
template <typename T, typename E> Result<T, E> Result<T, E>::Ok(T &&value)
{
    return Result<T, E>{std::variant<T, E>{std::in_place_index<ResultKind::Ok>, std::move(value)}};
}
template <typename T, typename E> Result<T, E> Result<T, E>::Err(E &&error)
{
    return Result<T, E>{std::variant<T, E>{std::in_place_index<ResultKind::Err>, std::move(error)}};
}

template <typename T, typename E> Result<T, E> Result<T, E>::Ok(const T &value)
{
    return Result<T, E>{std::variant<T, E>{std::in_place_index<ResultKind::Ok>, value}};
}
template <typename T, typename E> Result<T, E> Result<T, E>::Err(const E &error)
{
    return Result<T, E>{std::variant<T, E>{std::in_place_index<ResultKind::Err>, error}};
}

/////////////////////////////////////////////////////////////////////////
// void specialization
/////////////////////////////////////////////////////////////////////////

template <typename E> Result<void, E> Result<void, E>::Ok()
{
    return Result<void, E>{std::variant<std::monostate, E>{std::in_place_index<ResultKind::Ok>, std::monostate{}}};
}

template <typename E> Result<void, E> Result<void, E>::Err(E &&error)
{
    return Result<void, E>{std::variant<std::monostate, E>{std::in_place_index<ResultKind::Err>, std::move(error)}};
}
template <typename E> Result<void, E> Result<void, E>::Err(const E &error)
{
    return Result<void, E>{std::variant<std::monostate, E>{std::in_place_index<ResultKind::Err>, error}};
}
