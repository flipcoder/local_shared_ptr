#ifndef BOOST_SHARED_PTR_HPP_INCLUDED
#define BOOST_SHARED_PTR_HPP_INCLUDED

//
//  shared_ptr.hpp
//
//  (C) Copyright Greg Colvin and Beman Dawes 1998, 1999.
//  Copyright (c) 2001-2008 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/smart_ptr/shared_ptr.htm for documentation.
//

#include <memory>

#include <boost/detail/checked_delete.hpp>
#include <boost/detail/shared_count.hpp>
#include <boost/detail/sp_convertible.hpp>
#include <cassert>

#include <algorithm> // for std::swap
#include <cstddef> // for std::size_t
#include <functional> // for std::less
#include <typeinfo> // for std::bad_cast
#include <iosfwd> // for std::basic_ostream

namespace boost {

  template <class T>
  class shared_ptr;
  template <class T>
  class weak_ptr;
  template <class T>
  class enable_shared_from_this;
  class enable_shared_from_raw;

  namespace detail {

    // sp_element, element_type

    template <class T>
    struct sp_element {
      typedef T type;
    };

    template <class T>
    struct sp_element<T[]> {
      typedef T type;
    };

    template <class T, std::size_t N>
    struct sp_element<T[N]> {
      typedef T type;
    };

    // sp_dereference, return type of operator*

    template <class T>
    struct sp_dereference {
      typedef T &type;
    };

    template <>
    struct sp_dereference<void> {
      typedef void type;
    };

    template <>
    struct sp_dereference<void const> {
      typedef void type;
    };

    template <>
    struct sp_dereference<void volatile> {
      typedef void type;
    };

    template <>
    struct sp_dereference<void const volatile> {
      typedef void type;
    };

    template <class T>
    struct sp_dereference<T[]> {
      typedef void type;
    };

    template <class T, std::size_t N>
    struct sp_dereference<T[N]> {
      typedef void type;
    };

    // sp_member_access, return type of operator->

    template <class T>
    struct sp_member_access {
      typedef T *type;
    };

    template <class T>
    struct sp_member_access<T[]> {
      typedef void type;
    };

    template <class T, std::size_t N>
    struct sp_member_access<T[N]> {
      typedef void type;
    };

    // sp_array_access, return type of operator[]

    template <class T>
    struct sp_array_access {
      typedef void type;
    };

    template <class T>
    struct sp_array_access<T[]> {
      typedef T &type;
    };

    template <class T, std::size_t N>
    struct sp_array_access<T[N]> {
      typedef T &type;
    };

    // sp_extent, for operator[] index check

    template <class T>
    struct sp_extent {
      enum _vt { value = 0 };
    };

    template <class T, std::size_t N>
    struct sp_extent<T[N]> {
      enum _vt { value = N };
    };

    // enable_shared_from_this support

    template <class X, class Y, class T>
    inline void sp_enable_shared_from_this(boost::shared_ptr<X> const *ppx, Y const *py,
                                           boost::enable_shared_from_this<T> const *pe) {
      if (pe != 0) {
        pe->_internal_accept_owner(ppx, const_cast<Y *>(py));
      }
    }

    template <class X, class Y>
    inline void sp_enable_shared_from_this(boost::shared_ptr<X> *ppx, Y const *py,
                                           boost::enable_shared_from_raw const *pe);

    inline void sp_enable_shared_from_this(...) {
    }

    // sp_assert_convertible

    template <class Y, class T>
    inline void sp_assert_convertible() {
      // static_assert( sp_convertible< Y, T >::value );
      typedef char tmp[sp_convertible<Y, T>::value ? 1 : -1];
      (void)sizeof(tmp);
    }

    // pointer constructor helper

    template <class T, class Y>
    inline void sp_pointer_construct(boost::shared_ptr<T> *ppx, Y *p,
                                     boost::detail::shared_count &pn) {
      boost::detail::shared_count(p).swap(pn);
      boost::detail::sp_enable_shared_from_this(ppx, p, p);
    }

    template <class T, class Y>
    inline void sp_pointer_construct(boost::shared_ptr<T[]> * /*ppx*/, Y *p,
                                     boost::detail::shared_count &pn) {
      sp_assert_convertible<Y[], T[]>();
      boost::detail::shared_count(p, boost::checked_array_deleter<T>()).swap(pn);
    }

    template <class T, std::size_t N, class Y>
    inline void sp_pointer_construct(boost::shared_ptr<T[N]> * /*ppx*/, Y *p,
                                     boost::detail::shared_count &pn) {
      sp_assert_convertible<Y[N], T[N]>();
      boost::detail::shared_count(p, boost::checked_array_deleter<T>()).swap(pn);
    }

    // deleter constructor helper

    template <class T, class Y>
    inline void sp_deleter_construct(boost::shared_ptr<T> *ppx, Y *p) {
      boost::detail::sp_enable_shared_from_this(ppx, p, p);
    }

    template <class T, class Y>
    inline void sp_deleter_construct(boost::shared_ptr<T[]> * /*ppx*/, Y * /*p*/) {
      sp_assert_convertible<Y[], T[]>();
    }

    template <class T, std::size_t N, class Y>
    inline void sp_deleter_construct(boost::shared_ptr<T[N]> * /*ppx*/, Y * /*p*/) {
      sp_assert_convertible<Y[N], T[N]>();
    }

  } // namespace detail

  //
  //  shared_ptr
  //
  //  An enhanced relative of scoped_ptr with reference counted copy semantics.
  //  The object pointed to is deleted when the last shared_ptr pointing to it
  //  is destroyed or reset.
  //

  template <class T>
  class shared_ptr {
  private:
    // Borland 5.5.1 specific workaround
    typedef shared_ptr<T> this_type;

  public:
    typedef typename boost::detail::sp_element<T>::type element_type;

    shared_ptr() noexcept : px(0),
                            pn() // never throws in 1.30+
    {
    }

    shared_ptr(std::nullptr_t) noexcept : px(0),
                                          pn() // never throws
    {
    }

    template <class Y>
    explicit shared_ptr(Y *p)
        : px(p)
        , pn() // Y must be complete
    {
      boost::detail::sp_pointer_construct(this, p, pn);
    }

    //
    // Requirements: D's copy constructor must not throw
    //
    // shared_ptr will release p by calling d(p)
    //

    template <class Y, class D>
    shared_ptr(Y *p, D d)
        : px(p)
        , pn(p, d) {
      boost::detail::sp_deleter_construct(this, p);
    }

    template <class D>
    shared_ptr(std::nullptr_t p, D d)
        : px(p)
        , pn(p, d) {
    }

    // As above, but with allocator. A's copy constructor shall not throw.

    template <class Y, class D, class A>
    shared_ptr(Y *p, D d, A a)
        : px(p)
        , pn(p, d, a) {
      boost::detail::sp_deleter_construct(this, p);
    }

    template <class D, class A>
    shared_ptr(std::nullptr_t p, D d, A a)
        : px(p)
        , pn(p, d, a) {
    }

    //  generated copy constructor, destructor are fine...

    // ... except in C++0x, move disables the implicit copy

    shared_ptr(shared_ptr const &r) noexcept : px(r.px), pn(r.pn) {
    }

    template <class Y>
    explicit shared_ptr(weak_ptr<Y> const &r)
        : pn(r.pn) // may throw
    {
      boost::detail::sp_assert_convertible<Y, T>();

      // it is now safe to copy r.px, as pn(r.pn) did not throw
      px = r.px;
    }

    template <class Y>
    shared_ptr(weak_ptr<Y> const &r, boost::detail::sp_nothrow_tag) noexcept
        : px(0),
          pn(r.pn, boost::detail::sp_nothrow_tag()) {
      if (!pn.empty()) {
        px = r.px;
      }
    }

    template <class Y>
    shared_ptr(shared_ptr<Y> const &r, typename boost::detail::sp_enable_if_convertible<Y, T>::type
                                       = boost::detail::sp_empty())

        noexcept : px(r.px),
                   pn(r.pn) {
      boost::detail::sp_assert_convertible<Y, T>();
    }

    // aliasing
    template <class Y>
    shared_ptr(shared_ptr<Y> const &r, element_type *p) noexcept : px(p), pn(r.pn) {
    }

    template <class Y, class D>
    shared_ptr(std::unique_ptr<Y, D> &&r)
        : px(r.get())
        , pn() {
      boost::detail::sp_assert_convertible<Y, T>();

      typename std::unique_ptr<Y, D>::pointer tmp = r.get();
      pn = boost::detail::shared_count(r);

      boost::detail::sp_deleter_construct(this, tmp);
    }

    // assignment

    shared_ptr &operator=(shared_ptr const &r) noexcept {
      this_type(r).swap(*this);
      return *this;
    }

    template <class Y>
    shared_ptr &operator=(shared_ptr<Y> const &r) noexcept {
      this_type(r).swap(*this);
      return *this;
    }

    template <class Y, class D>
    shared_ptr &operator=(std::unique_ptr<Y, D> &&r) {
      this_type(static_cast<std::unique_ptr<Y, D> &&>(r)).swap(*this);
      return *this;
    }

    // Move support

    shared_ptr(shared_ptr &&r) noexcept : px(r.px), pn() {
      pn.swap(r.pn);
      r.px = 0;
    }

    template <class Y>
    shared_ptr(shared_ptr<Y> &&r, typename boost::detail::sp_enable_if_convertible<Y, T>::type
                                  = boost::detail::sp_empty()) noexcept : px(r.px),
                                                                          pn() {
      boost::detail::sp_assert_convertible<Y, T>();

      pn.swap(r.pn);
      r.px = 0;
    }

    shared_ptr &operator=(shared_ptr &&r) noexcept {
      this_type(static_cast<shared_ptr &&>(r)).swap(*this);
      return *this;
    }

    template <class Y>
    shared_ptr &operator=(shared_ptr<Y> &&r) noexcept {
      this_type(static_cast<shared_ptr<Y> &&>(r)).swap(*this);
      return *this;
    }

    shared_ptr &operator=(std::nullptr_t) noexcept // never throws
    {
      this_type().swap(*this);
      return *this;
    }

    void reset() noexcept // never throws in 1.30+
    {
      this_type().swap(*this);
    }

    template <class Y>
    void reset(Y *p) // Y must be complete
    {
      assert(p == 0 || p != px); // catch self-reset errors
      this_type(p).swap(*this);
    }

    template <class Y, class D>
    void reset(Y *p, D d) {
      this_type(p, d).swap(*this);
    }

    template <class Y, class D, class A>
    void reset(Y *p, D d, A a) {
      this_type(p, d, a).swap(*this);
    }

    template <class Y>
    void reset(shared_ptr<Y> const &r, element_type *p) {
      this_type(r, p).swap(*this);
    }

    // never throws (but has a BOOST_ASSERT in it, so not marked with noexcept)
    typename boost::detail::sp_dereference<T>::type operator*() const {
      assert(px != 0);
      return *px;
    }

    // never throws (but has a BOOST_ASSERT in it, so not marked with noexcept)
    typename boost::detail::sp_member_access<T>::type operator->() const {
      assert(px != 0);
      return px;
    }

    // never throws (but has a BOOST_ASSERT in it, so not marked with noexcept)
    typename boost::detail::sp_array_access<T>::type operator[](std::ptrdiff_t i) const {
      assert(px != 0);
      assert(i >= 0 && (i < boost::detail::sp_extent<T>::value
                        || boost::detail::sp_extent<T>::value == 0));

      return static_cast<typename boost::detail::sp_array_access<T>::type>(px[i]);
    }

    element_type *get() const noexcept {
      return px;
    }

    // implicit conversion to "bool"
    explicit operator bool () const noexcept
    {
      return px != 0;
    }

    bool operator! () const noexcept
    {
      return px == 0;
    }

    bool unique() const noexcept {
      return pn.unique();
    }

    long use_count() const noexcept {
      return pn.use_count();
    }

    void swap(shared_ptr &other) noexcept {
      std::swap(px, other.px);
      pn.swap(other.pn);
    }

    template <class Y>
    bool owner_before(shared_ptr<Y> const &rhs) const noexcept {
      return pn < rhs.pn;
    }

    template <class Y>
    bool owner_before(weak_ptr<Y> const &rhs) const noexcept {
      return pn < rhs.pn;
    }

    void *_internal_get_deleter(std::type_info const &ti) const noexcept {
      return pn.get_deleter(ti);
    }

    void *_internal_get_untyped_deleter() const noexcept {
      return pn.get_untyped_deleter();
    }

    bool _internal_equiv(shared_ptr const &r) const noexcept {
      return px == r.px && pn == r.pn;
    }

    // Tasteless as this may seem, making all members public allows member templates
    // to work in the absence of member template friends. (Matthew Langston)

  private:
    template <class Y>
    friend class shared_ptr;
    template <class Y>
    friend class weak_ptr;

    element_type *px; // contained pointer
    boost::detail::shared_count pn; // reference counter

  }; // shared_ptr

  template <class T, class U>
  inline bool operator==(shared_ptr<T> const &a, shared_ptr<U> const &b) noexcept {
    return a.get() == b.get();
  }

  template <class T, class U>
  inline bool operator!=(shared_ptr<T> const &a, shared_ptr<U> const &b) noexcept {
    return a.get() != b.get();
  }

  template <class T>
  inline bool operator==(shared_ptr<T> const &p, std::nullptr_t) noexcept {
    return p.get() == 0;
  }

  template <class T>
  inline bool operator==(std::nullptr_t, shared_ptr<T> const &p) noexcept {
    return p.get() == 0;
  }

  template <class T>
  inline bool operator!=(shared_ptr<T> const &p, std::nullptr_t) noexcept {
    return p.get() != 0;
  }

  template <class T>
  inline bool operator!=(std::nullptr_t, shared_ptr<T> const &p) noexcept {
    return p.get() != 0;
  }

  template <class T, class U>
  inline bool operator<(shared_ptr<T> const &a, shared_ptr<U> const &b) noexcept {
    return a.owner_before(b);
  }

  template <class T>
  inline void swap(shared_ptr<T> &a, shared_ptr<T> &b) noexcept {
    a.swap(b);
  }

  template <class T, class U>
  shared_ptr<T> static_pointer_cast(shared_ptr<U> const &r) noexcept {
    (void)static_cast<T *>(static_cast<U *>(0));

    typedef typename shared_ptr<T>::element_type E;

    E *p = static_cast<E *>(r.get());
    return shared_ptr<T>(r, p);
  }

  template <class T, class U>
  shared_ptr<T> const_pointer_cast(shared_ptr<U> const &r) noexcept {
    (void)const_cast<T *>(static_cast<U *>(0));

    typedef typename shared_ptr<T>::element_type E;

    E *p = const_cast<E *>(r.get());
    return shared_ptr<T>(r, p);
  }

  template <class T, class U>
  shared_ptr<T> dynamic_pointer_cast(shared_ptr<U> const &r) noexcept {
    (void)dynamic_cast<T *>(static_cast<U *>(0));

    typedef typename shared_ptr<T>::element_type E;

    E *p = dynamic_cast<E *>(r.get());
    return p ? shared_ptr<T>(r, p) : shared_ptr<T>();
  }

  template <class T, class U>
  shared_ptr<T> reinterpret_pointer_cast(shared_ptr<U> const &r) noexcept {
    (void)reinterpret_cast<T *>(static_cast<U *>(0));

    typedef typename shared_ptr<T>::element_type E;

    E *p = reinterpret_cast<E *>(r.get());
    return shared_ptr<T>(r, p);
  }

  // get_pointer() enables boost::mem_fn to recognize shared_ptr

  template <class T>
  inline typename shared_ptr<T>::element_type *get_pointer(shared_ptr<T> const &p) noexcept {
    return p.get();
  }

  // operator<<

  template <class E, class T, class Y>
  std::basic_ostream<E, T> &operator<<(std::basic_ostream<E, T> &os, shared_ptr<Y> const &p) {
    os << p.get();
    return os;
  }

  // get_deleter

  namespace detail {

    template <class D, class T>
    D *basic_get_deleter(shared_ptr<T> const &p) noexcept {
      return static_cast<D *>(p._internal_get_deleter(typeid(D)));
    }

    class esft2_deleter_wrapper {
    private:
      shared_ptr<void const volatile> deleter_;

    public:
      esft2_deleter_wrapper() {
      }

      template <class T>
      void set_deleter(shared_ptr<T> const &deleter) {
        deleter_ = deleter;
      }

      template <typename D>
      D *get_deleter() const noexcept {
        return boost::detail::basic_get_deleter<D>(deleter_);
      }

      template <class T>
      void operator()(T *) {
        assert(deleter_.use_count() <= 1);
        deleter_.reset();
      }
    };

  } // namespace detail

  template <class D, class T>
  D *get_deleter(shared_ptr<T> const &p) noexcept {
    D *del = boost::detail::basic_get_deleter<D>(p);

    if (del == 0) {
      boost::detail::esft2_deleter_wrapper *del_wrapper
          = boost::detail::basic_get_deleter<boost::detail::esft2_deleter_wrapper>(p);
      // The following get_deleter method call is fully qualified because
      // older versions of gcc (2.95, 3.2.3) fail to compile it when written
      // del_wrapper->get_deleter<D>()
      if (del_wrapper)
        del = del_wrapper->::boost::detail::esft2_deleter_wrapper::get_deleter<D>();
    }

    return del;
  }

  // hash_value

  template <class T>
  struct hash;

  template <class T>
  std::size_t hash_value(boost::shared_ptr<T> const &p) noexcept {
    return boost::hash<T *>()(p.get());
  }

} // namespace boost

#endif // #ifndef BOOST_SHARED_PTR_HPP_INCLUDED
