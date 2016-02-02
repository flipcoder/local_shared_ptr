#ifndef BOOST_DETAIL_SP_FORWARD_HPP_INCLUDED
#define BOOST_DETAIL_SP_FORWARD_HPP_INCLUDED

//  detail/sp_forward.hpp
//
//  Copyright 2008,2012 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

namespace boost {

  namespace detail {

    template <class T>
    T &&sp_forward(T &t) noexcept {
      return static_cast<T &&>(t);
    }

  } // namespace detail

} // namespace boost

#endif // #ifndef BOOST_DETAIL_SP_FORWARD_HPP_INCLUDED
