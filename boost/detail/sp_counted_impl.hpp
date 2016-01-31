#ifndef BOOST_SMART_PTR_DETAIL_SP_COUNTED_IMPL_HPP_INCLUDED
#define BOOST_SMART_PTR_DETAIL_SP_COUNTED_IMPL_HPP_INCLUDED

//
//  detail/sp_counted_impl.hpp
//
//  Copyright (c) 2001, 2002, 2003 Peter Dimov and Multi Media Ltd.
//  Copyright 2004-2005 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/detail/checked_delete.hpp>
#include <boost/detail/sp_counted_base_nt.hpp>
#include <cstddef> // std::size_t
#include <typeinfo>

namespace boost {

  namespace detail {

    template <class X>
    class sp_counted_impl_p : public sp_counted_base {
    private:
      X *px_;

      sp_counted_impl_p(sp_counted_impl_p const &);
      sp_counted_impl_p &operator=(sp_counted_impl_p const &);

      typedef sp_counted_impl_p<X> this_type;

    public:
      explicit sp_counted_impl_p(X *px)
          : px_(px) {
      }

      virtual void dispose() // nothrow
      {
        boost::checked_delete(px_);
      }

      virtual void *get_deleter(std::type_info const &) {
        return 0;
      }

      virtual void *get_untyped_deleter() {
        return 0;
      }
    };

    template <class P, class D>
    class sp_counted_impl_pd : public sp_counted_base {
    private:
      P ptr; // copy constructor must not throw
      D del; // copy constructor must not throw

      sp_counted_impl_pd(sp_counted_impl_pd const &);
      sp_counted_impl_pd &operator=(sp_counted_impl_pd const &);

      typedef sp_counted_impl_pd<P, D> this_type;

    public:
      // pre: d(p) must not throw

      sp_counted_impl_pd(P p, D &d)
          : ptr(p)
          , del(d) {
      }

      sp_counted_impl_pd(P p)
          : ptr(p)
          , del() {
      }

      virtual void dispose() // nothrow
      {
        del(ptr);
      }

      virtual void *get_deleter(std::type_info const &ti) {
        return ti == typeid(D) ? &reinterpret_cast<char &>(del) : 0;
      }

      virtual void *get_untyped_deleter() {
        return &reinterpret_cast<char &>(del);
      }
    };

    template <class P, class D, class A>
    class sp_counted_impl_pda : public sp_counted_base {
    private:
      P p_; // copy constructor must not throw
      D d_; // copy constructor must not throw
      A a_; // copy constructor must not throw

      sp_counted_impl_pda(sp_counted_impl_pda const &);
      sp_counted_impl_pda &operator=(sp_counted_impl_pda const &);

      typedef sp_counted_impl_pda<P, D, A> this_type;

    public:
      // pre: d( p ) must not throw

      sp_counted_impl_pda(P p, D &d, A a)
          : p_(p)
          , d_(d)
          , a_(a) {
      }

      sp_counted_impl_pda(P p, A a)
          : p_(p)
          , d_(a)
          , a_(a) {
      }

      virtual void dispose() // nothrow
      {
        d_(p_);
      }

      virtual void destroy() // nothrow
      {
        typedef typename std::allocator_traits<A>::template rebind_alloc<this_type> A2;

        A2 a2(a_);

        std::allocator_traits<A2>::destroy(a2, this);

        a2.deallocate(this, 1);
      }

      virtual void *get_deleter(std::type_info const &ti) {
        return ti == typeid(D) ? &reinterpret_cast<char &>(d_) : 0;
      }

      virtual void *get_untyped_deleter() {
        return &reinterpret_cast<char &>(d_);
      }
    };

  } // namespace detail

} // namespace boost

#endif // #ifndef BOOST_SMART_PTR_DETAIL_SP_COUNTED_IMPL_HPP_INCLUDED
