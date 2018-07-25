//
//  weak_from_this_test2.cpp
//
//  Tests weak_from_this in a destructor
//
//  Copyright (c) 2014, 2015 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//

#include <kit/enable_shared_from_this.hpp>
#include <kit/local_weak_ptr.hpp>
#include "lightweight_test.hpp"

namespace weak_from_this_test2 {

class X: public kit::enable_shared_from_this< X >
{
private:

    kit::local_weak_ptr<X> px_;

public:

    X()
    {
        kit::local_weak_ptr<X> p1 = weak_from_this();
        KIT_TEST( p1._empty() );
        KIT_TEST( p1.expired() );
    }

    void check()
    {
        kit::local_weak_ptr<X> p2 = weak_from_this();
        KIT_TEST( !p2.expired() );

        KIT_TEST( p2.lock().get() == this );

        px_ = p2;
    }

    ~X()
    {
        kit::local_weak_ptr<X> p3 = weak_from_this();
        KIT_TEST( p3.expired() );

        KIT_TEST( !(px_ < p3) && !(p3 < px_) );
    }
};

int main()
{
    {
        kit::local_shared_ptr< X > px( new X );
        px->check();
    }

    return kit::report_errors();
}

}
