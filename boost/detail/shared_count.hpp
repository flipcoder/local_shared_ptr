#ifndef BOOST_SMART_PTR_DETAIL_SHARED_COUNT_HPP_INCLUDED
#define BOOST_SMART_PTR_DETAIL_SHARED_COUNT_HPP_INCLUDED

//
//  detail/shared_count.hpp
//
//  Copyright (c) 2001, 2002, 2003 Peter Dimov and Multi Media Ltd.
//  Copyright 2004-2005 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/detail/checked_delete.hpp>
#include <boost/bad_weak_ptr.hpp>
#include <boost/detail/sp_counted_base_nt.hpp>
#include <boost/detail/sp_counted_impl.hpp>
#include <functional> // std::less
#include <memory>

namespace boost {

  namespace detail {

    struct sp_nothrow_tag {};

    template <class D>
    struct sp_inplace_tag {};

    template <class T>
    class sp_reference_wrapper {
    public:
      explicit sp_reference_wrapper(T &t)
          : t_(std::addressof(t)) {
      }

      template <class Y>
      void operator()(Y *p) const {
        (*t_)(p);
      }

    private:
      T *t_;
    };

    template <class D>
    struct sp_convert_reference {
      typedef D type;
    };

    template <class D>
    struct sp_convert_reference<D &> {
      typedef sp_reference_wrapper<D> type;
    };

    class weak_count;

    class shared_count {
    private:
      sp_counted_base *pi_;

      friend class weak_count;

    public:
      shared_count()
          : pi_(0) // nothrow
      {
      }

      template <class Y>
      explicit shared_count(Y *p)
          : pi_(0) {
        try {
          pi_ = new sp_counted_impl_p<Y>(p);
        } catch (...) {
          boost::checked_delete(p);
          throw;
        }
      }

      template <class P, class D>
      shared_count(P p, D d)
          : pi_(0) {
        try {
          pi_ = new sp_counted_impl_pd<P, D>(p, d);
        } catch (...) {
          d(p); // delete p
          throw;
        }
      }

      template <class P, class D>
      shared_count(P p, sp_inplace_tag<D>)
          : pi_(0) {
        try {
          pi_ = new sp_counted_impl_pd<P, D>(p);
        } catch (...) {
          D::operator_fn(p); // delete p
          throw;
        }
      }

      template <class P, class D, class A>
      shared_count(P p, D d, A a)
          : pi_(0) {
        typedef sp_counted_impl_pda<P, D, A> impl_type;

        typedef typename std::allocator_traits<A>::template rebind_alloc<impl_type> A2;

        A2 a2(a);

        try {
          impl_type *pi = std::allocator_traits<A2>::allocate(a2, 1);
          pi_ = pi;
          std::allocator_traits<A2>::construct(a2, pi, p, d, a);
        } catch (...) {
          d(p);

          if (pi_ != 0) {
            a2.deallocate(static_cast<impl_type *>(pi_), 1);
          }

          throw;
        }
      }

      template <class P, class D, class A>
      shared_count(P p, sp_inplace_tag<D>, A a)
          : pi_(0) {
        typedef sp_counted_impl_pda<P, D, A> impl_type;

        typedef typename std::allocator_traits<A>::template rebind_alloc<impl_type> A2;

        A2 a2(a);

        try {

          impl_type *pi = std::allocator_traits<A2>::allocate(a2, 1);
          pi_ = pi;
          std::allocator_traits<A2>::construct(a2, pi, p, a);

        } catch (...) {
          D::operator_fn(p);

          if (pi_ != 0) {
            a2.deallocate(static_cast<impl_type *>(pi_), 1);
          }

          throw;
        }
      }

      template <class Y, class D>
      explicit shared_count(std::unique_ptr<Y, D> &r)
          : pi_(0) {
        typedef typename sp_convert_reference<D>::type D2;

        D2 d2(r.get_deleter());
        pi_ = new sp_counted_impl_pd<typename std::unique_ptr<Y, D>::pointer, D2>(r.get(), d2);

        r.release();
      }

      ~shared_count() // nothrow
      {
        if (pi_ != 0)
          pi_->release();
      }

      shared_count(shared_count const &r)
          : pi_(r.pi_) // nothrow
      {
        if (pi_ != 0)
          pi_->add_ref_copy();
      }

      shared_count(shared_count &&r)
          : pi_(r.pi_) // nothrow
      {
        r.pi_ = 0;
      }

      explicit shared_count(weak_count const &r); // throws bad_weak_ptr when r.use_count() == 0
      shared_count(weak_count const &r,
                   sp_nothrow_tag); // constructs an empty *this when r.use_count() == 0

      shared_count &operator=(shared_count const &r) // nothrow
      {
        sp_counted_base *tmp = r.pi_;

        if (tmp != pi_) {
          if (tmp != 0)
            tmp->add_ref_copy();
          if (pi_ != 0)
            pi_->release();
          pi_ = tmp;
        }

        return *this;
      }

      void swap(shared_count &r) // nothrow
      {
        sp_counted_base *tmp = r.pi_;
        r.pi_ = pi_;
        pi_ = tmp;
      }

      long use_count() const // nothrow
      {
        return pi_ != 0 ? pi_->use_count() : 0;
      }

      bool unique() const // nothrow
      {
        return use_count() == 1;
      }

      bool empty() const // nothrow
      {
        return pi_ == 0;
      }

      friend inline bool operator==(shared_count const &a, shared_count const &b) {
        return a.pi_ == b.pi_;
      }

      friend inline bool operator<(shared_count const &a, shared_count const &b) {
        return std::less<sp_counted_base *>()(a.pi_, b.pi_);
      }

      void *get_deleter(std::type_info const &ti) const {
        return pi_ ? pi_->get_deleter(ti) : 0;
      }

      void *get_untyped_deleter() const {
        return pi_ ? pi_->get_untyped_deleter() : 0;
      }
    };

    class weak_count {
    private:
      sp_counted_base *pi_;

      friend class shared_count;

    public:
      weak_count()
          : pi_(0) // nothrow
      {
      }

      weak_count(shared_count const &r)
          : pi_(r.pi_) // nothrow
      {
        if (pi_ != 0)
          pi_->weak_add_ref();
      }

      weak_count(weak_count const &r)
          : pi_(r.pi_) // nothrow
      {
        if (pi_ != 0)
          pi_->weak_add_ref();
      }

      // Move support

      weak_count(weak_count &&r)
          : pi_(r.pi_) // nothrow
      {
        r.pi_ = 0;
      }

      ~weak_count() // nothrow
      {
        if (pi_ != 0)
          pi_->weak_release();
      }

      weak_count &operator=(shared_count const &r) // nothrow
      {
        sp_counted_base *tmp = r.pi_;

        if (tmp != pi_) {
          if (tmp != 0)
            tmp->weak_add_ref();
          if (pi_ != 0)
            pi_->weak_release();
          pi_ = tmp;
        }

        return *this;
      }

      weak_count &operator=(weak_count const &r) // nothrow
      {
        sp_counted_base *tmp = r.pi_;

        if (tmp != pi_) {
          if (tmp != 0)
            tmp->weak_add_ref();
          if (pi_ != 0)
            pi_->weak_release();
          pi_ = tmp;
        }

        return *this;
      }

      void swap(weak_count &r) // nothrow
      {
        sp_counted_base *tmp = r.pi_;
        r.pi_ = pi_;
        pi_ = tmp;
      }

      long use_count() const // nothrow
      {
        return pi_ != 0 ? pi_->use_count() : 0;
      }

      bool empty() const // nothrow
      {
        return pi_ == 0;
      }

      friend inline bool operator==(weak_count const &a, weak_count const &b) {
        return a.pi_ == b.pi_;
      }

      friend inline bool operator<(weak_count const &a, weak_count const &b) {
        return std::less<sp_counted_base *>()(a.pi_, b.pi_);
      }
    };

    inline shared_count::shared_count(weak_count const &r)
        : pi_(r.pi_) {
      if (pi_ == 0 || !pi_->add_ref_lock()) {
        throw boost::bad_weak_ptr();
      }
    }

    inline shared_count::shared_count(weak_count const &r, sp_nothrow_tag)
        : pi_(r.pi_) {
      if (pi_ != 0 && !pi_->add_ref_lock()) {
        pi_ = 0;
      }
    }

  } // namespace detail

} // namespace boost

#endif // #ifndef BOOST_SMART_PTR_DETAIL_SHARED_COUNT_HPP_INCLUDED
