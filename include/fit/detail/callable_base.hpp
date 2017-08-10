/*=============================================================================
    Copyright (c) 2015 Paul Fultz II
    callable_base.h
    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef FIT_GUARD_CALLABLE_BASE_H
#define FIT_GUARD_CALLABLE_BASE_H

#include <fit/detail/delegate.hpp>
#include <fit/detail/result_of.hpp>
#include <fit/apply.hpp>

#ifndef FIT_HAS_TEMPLATE_ALIAS
#if (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7)
#define FIT_HAS_TEMPLATE_ALIAS 0
#else
#define FIT_HAS_TEMPLATE_ALIAS 1
#endif
#endif

namespace fit { namespace detail {

template<class F>
struct non_class_function
{
    F f;
    FIT_DELGATE_CONSTRUCTOR(non_class_function, F, f)

    template<class... Ts>
    constexpr FIT_SFINAE_RESULT(apply_f, id_<F>, id_<Ts>...) 
    operator()(Ts&&... xs) const FIT_SFINAE_RETURNS
    (
        fit::apply(f, FIT_FORWARD(Ts)(xs)...)
    );
};

template<class F>
struct callable_base_type
: std::conditional<(FIT_IS_CLASS(F) && !FIT_IS_FINAL(F) && !FIT_IS_POLYMORPHIC(F)), F, non_class_function<F>>
{};

#if FIT_HAS_TEMPLATE_ALIAS
template<class F>
using callable_base = typename callable_base_type<F>::type;
#else
template<class F>
struct callable_base
: callable_base_type<F>::type
{
    typedef typename callable_base_type<F>::type base;
    FIT_INHERIT_CONSTRUCTOR(callable_base, base)
};

template<class F>
struct callable_base_type<callable_base<F>>
: callable_base_type<F>
{};

#endif

}} // namespace fit

#endif
