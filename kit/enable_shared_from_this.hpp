#ifndef KIT_ENABLE_SHARED_FROM_THIS_HPP_INCLUDED
#define KIT_ENABLE_SHARED_FROM_THIS_HPP_INCLUDED

//
//  enable_shared_from_this.hpp
//
//  Copyright 2002, 2009 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//  http://www.boost.org/libs/smart_ptr/enable_shared_from_this.html
//

#include <kit/local_shared_ptr.hpp>
#include <kit/local_weak_ptr.hpp>
#include <cassert>

namespace kit {

  template <class T>
  class enable_shared_from_this {
  protected:
    enable_shared_from_this() noexcept {
    }

    enable_shared_from_this(enable_shared_from_this const &) noexcept {
    }

    enable_shared_from_this &operator=(enable_shared_from_this const &) noexcept {
      return *this;
    }

    ~enable_shared_from_this() noexcept // ~weak_ptr<T> newer throws, so this call also must not
                                        // throw
    {
    }

  public:
    local_shared_ptr<T> shared_from_this() {
      local_shared_ptr<T> p(weak_this_);
      assert(p.get() == this);
      return p;
    }

    local_shared_ptr<T const> shared_from_this() const {
      local_shared_ptr<T const> p(weak_this_);
      assert(p.get() == this);
      return p;
    }

    local_weak_ptr<T> weak_from_this() noexcept {
      return weak_this_;
    }

    local_weak_ptr<T const> weak_from_this() const noexcept {
      return weak_this_;
    }

  public: // actually private, but avoids compiler template friendship issues
    // Note: invoked automatically by local_shared_ptr; do not call
    template <class X, class Y>
    void _internal_accept_owner(local_shared_ptr<X> const *ppx, Y *py) const {
      if (weak_this_.expired()) {
        weak_this_ = local_shared_ptr<T>(*ppx, py);
      }
    }

  private:
    mutable local_weak_ptr<T> weak_this_;
  };

} // namespace kit

#endif // #ifndef KIT_ENABLE_SHARED_FROM_THIS_HPP_INCLUDED
