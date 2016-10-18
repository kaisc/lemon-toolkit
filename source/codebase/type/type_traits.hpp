// @date 2016/09/03
// @author Mao Jingkai(oammix@gmail.com)

#pragma once

#include <forwards.hpp>

#include <type_traits>

NS_LEMON_BEGIN

// static max generator
template<size_t S, size_t ... Args> struct static_max;

template<size_t S> struct static_max<S>
{
    static const size_t value = S;
};

template<size_t S1, size_t S2, size_t ... Args> struct static_max<S1, S2, Args...>
{
    static const size_t value = S1 > S2 ? static_max<S1, Args...>::value : static_max<S2, Args...>::value;
};

// incremental id of type
struct TypeInfo
{
    using index_t = size_t;

    template<typename Base, typename Derived> static index_t id()
    {
        static_assert( std::is_base_of<Base, Derived>::value, "D should be derived from B." );
        static index_t sid = counter<Base>::value ++;
        return sid;
    }

protected:
    template<typename Base> struct counter
    {
        static index_t value;
    };
};

template<typename B> TypeInfo::index_t TypeInfo::counter<B>::value = 0;

struct TypeInfoGeneric
{
    using index_t = size_t;

    template<typename Base, typename Type> static index_t id()
    {
        static index_t sid = counter<Base>::value ++;
        return sid;
    }

protected:
    template<typename Base> struct counter
    {
        static index_t value;
    };
};

template<typename B> TypeInfo::index_t TypeInfoGeneric::counter<B>::value = 0;

NS_LEMON_END