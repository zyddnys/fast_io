/*
This is the <concepts> header migrated from Microsoft Visual C++ 14.23.28019 to GCC 10.0
Migrator: Cqwrteur
*/

#ifndef _GLIBCXX___TYPE_TRAITS_H
#define _GLIBCXX___TYPE_TRAITS_H 1

#pragma GCC system_header
#if __cplusplus >= 201703L
#include<type_traits>

namespace std _GLIBCXX_VISIBILITY(default)
{
_GLIBCXX_BEGIN_NAMESPACE_VERSION

// STRUCT TEMPLATE basic_common_reference
template <class, class, template <class> class, template <class> class>
struct basic_common_reference {};

// FUNCTION TEMPLATE _Returns_exactly
template <class _Ty>
_Ty _Returns_exactly() noexcept; // not defined

// ALIAS TEMPLATE _Cond_res
template <class _Ty1, class _Ty2>
using _Cond_res = // N4810 [meta.trans.other]/2.4
    decltype(false ? _Returns_exactly<_Ty1>() : _Returns_exactly<_Ty2>());

// ALIAS TEMPLATE _Copy_cv
template <class _From>
struct _Copy_cv_impl {
    template <class _To>
    using _Apply = _To;
};
template <class _From>
struct _Copy_cv_impl<const _From> {
    template <class _To>
    using _Apply = const _To;
};
template <class _From>
struct _Copy_cv_impl<volatile _From> {
    template <class _To>
    using _Apply = volatile _To;
};
template <class _From>
struct _Copy_cv_impl<const volatile _From> {
    template <class _To>
    using _Apply = const volatile _To;
};
template <class _From, class _To>
using _Copy_cv = // N4810 [meta.trans.other]/2.3
    typename _Copy_cv_impl<_From>::template _Apply<_To>;

// STRUCT TEMPLATE _Add_qualifiers
template <class _Ty1>
struct _Add_qualifiers { // _Add_qualifiers<A>::template _Apply is XREF(A) from N4810 [meta.trans.other]/2.1
    template <class _Ty2>
    using _Apply = _Copy_cv<_Ty1, _Ty2>;
};
template <class _Ty1>
struct _Add_qualifiers<_Ty1&> {
    template <class _Ty2>
    using _Apply = add_lvalue_reference_t<_Copy_cv<_Ty1, _Ty2>>;
};
template <class _Ty1>
struct _Add_qualifiers<_Ty1&&> {
    template <class _Ty2>
    using _Apply = add_rvalue_reference_t<_Copy_cv<_Ty1, _Ty2>>;
};

// STRUCT TEMPLATE common_reference
template <class...>
struct common_reference;

// ALIAS TEMPLATE common_reference_t
template <class... _Types>
using common_reference_t = typename common_reference<_Types...>::type;

// N4810 [meta.trans.other]/5.1: "If sizeof...(T) is zero ..."
template <>
struct common_reference<> {};

// N4810 [meta.trans.other]/5.2: "...if sizeof...(T) is one ..."
template <class _Ty>
struct common_reference<_Ty> {
    using type = _Ty;
};

// N4810 [meta.trans.other]/5.3: "...if sizeof...(T) is two..."

// N4810 [meta.trans.other]/5.3.4: "if common_type_t<T1, T2> is well-formed..."
// N4810 [meta.trans.other]/5.3.5: "Otherwise, there shall be no member type."
template <class _Ty1, class _Ty2, class = void>
struct _Common_reference2C : common_type<_Ty1, _Ty2> {};

// N4810 [meta.trans.other]/5.3.3: "if COND_RES(T1, T2) is well-formed..."
template <class _Ty1, class _Ty2>
struct _Common_reference2C<_Ty1, _Ty2, void_t<_Cond_res<_Ty1, _Ty2>>> {
    using type = _Cond_res<_Ty1, _Ty2>;
};

// N4810 [meta.trans.other]/5.3.2: "if basic_common_reference<[...]>::type is well-formed..."
template <class _Ty1, class _Ty2>
using _Basic_specialization = typename basic_common_reference<remove_cvref_t<_Ty1>, remove_cvref_t<_Ty2>,
    _Add_qualifiers<_Ty1>::template _Apply, _Add_qualifiers<_Ty2>::template _Apply>::type;

template <class _Ty1, class _Ty2, class = void>
struct _Common_reference2B : _Common_reference2C<_Ty1, _Ty2> {};

template <class _Ty1, class _Ty2>
struct _Common_reference2B<_Ty1, _Ty2, void_t<_Basic_specialization<_Ty1, _Ty2>>> {
    using type = _Basic_specialization<_Ty1, _Ty2>;
};

// N4810 [meta.trans.other]/5.3.1: "If T1 and T2 are reference types and COMMON_REF(T1, T2) is well-formed..."
template <class _Ty1, class _Ty2, class = void>
struct _Common_reference2A : _Common_reference2B<_Ty1, _Ty2> {};

template <class _Ty1, class _Ty2, class _Result = _Cond_res<_Copy_cv<_Ty1, _Ty2>&, _Copy_cv<_Ty2, _Ty1>&>,
    enable_if_t<is_lvalue_reference_v<_Result>, int> = 0>
using _LL_common_ref = _Result;

template <class _Ty1, class _Ty2>
struct _Common_reference2A<_Ty1&, _Ty2&, void_t<_LL_common_ref<_Ty1, _Ty2>>> {
    using type = _LL_common_ref<_Ty1, _Ty2>; // "both lvalues" case from N4810 [meta.trans.other]/2.5
};

template <class _Ty1, class _Ty2>
struct _Common_reference2A<_Ty1&&, _Ty2&, enable_if_t<is_convertible_v<_Ty1&&, _LL_common_ref<const _Ty1, _Ty2>>>> {
    using type = _LL_common_ref<const _Ty1, _Ty2>; // "rvalue and lvalue" case from N4810 [meta.trans.other]/2.7
};

template <class _Ty1, class _Ty2>
struct _Common_reference2A<_Ty1&, _Ty2&&, enable_if_t<is_convertible_v<_Ty2&&, _LL_common_ref<const _Ty2, _Ty1>>>> {
    using type = _LL_common_ref<const _Ty2, _Ty1>; // "lvalue and rvalue" case from N4810 [meta.trans.other]/2.8
};

template <class _Ty1, class _Ty2>
using _RR_common_ref = remove_reference_t<_LL_common_ref<_Ty1, _Ty2>>&&;

template <class _Ty1, class _Ty2>
struct _Common_reference2A<_Ty1&&, _Ty2&&,
    enable_if_t<
        is_convertible_v<_Ty1&&, _RR_common_ref<_Ty1, _Ty2>> && is_convertible_v<_Ty2&&, _RR_common_ref<_Ty1, _Ty2>>>> {
    using type = _RR_common_ref<_Ty1, _Ty2>; // "both rvalues" case from N4810 [meta.trans.other]/2.6
};

template <class _Ty1, class _Ty2>
struct common_reference<_Ty1, _Ty2> : _Common_reference2A<_Ty1, _Ty2> {};

// N4810 [meta.trans.other]/5.4: "if sizeof...(T) is greater than two..."
template <class _Void, class _Ty1, class _Ty2, class... _Types>
struct _Fold_common_reference {};
template <class _Ty1, class _Ty2, class... _Types>
struct _Fold_common_reference<void_t<common_reference_t<_Ty1, _Ty2>>, _Ty1, _Ty2, _Types...>
    : common_reference<common_reference_t<_Ty1, _Ty2>, _Types...> {};

template <class _Ty1, class _Ty2, class _Ty3, class... _Rest>
struct common_reference<_Ty1, _Ty2, _Ty3, _Rest...> : _Fold_common_reference<void, _Ty1, _Ty2, _Ty3, _Rest...> {};


_GLIBCXX_END_NAMESPACE_VERSION
}
#endif
#endif