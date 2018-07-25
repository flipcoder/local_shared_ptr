#ifndef KIT_BAD_WEAK_PTR_HPP_INCLUDED
#define KIT_BAD_WEAK_PTR_HPP_INCLUDED

//
//  boost/smart_ptr/bad_local_weak_ptr.hpp
//
//  Copyright (c) 2001, 2002, 2003 Peter Dimov and Multi Media Ltd.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <exception>

namespace kit {

  class bad_local_weak_ptr : public std::exception {
  public:
    virtual char const *what() const throw() {
      return "kit::bad_local_weak_ptr";
    }
  };

} // namespace kit

#endif // #ifndef KIT_BAD_WEAK_PTR_HPP_INCLUDED
