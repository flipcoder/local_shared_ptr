//
//  sp_unary_addr_test.cpp
//
//  Copyright (c) 2007 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <kit/local_shared_ptr.hpp>
#include "lightweight_test.hpp"
#include <memory>

namespace sp_unary_addr_test {

struct deleter
{
private:

    void operator& ();
    void operator& () const;

public:

    int data;

    deleter(): data( 17041 )
    {
    }

    void operator()( void * )
    {
    }
};

struct X
{
};

int main()
{
    X x;

    {
        kit::local_shared_ptr<X> p( &x, deleter() );

        deleter * q = kit::get_deleter<deleter>( p );

        KIT_TEST( q != 0 );
        KIT_TEST( q != 0 && q->data == 17041 );
    }

    {
        kit::local_shared_ptr<X> p( &x, deleter(), std::allocator<X>() );

        deleter * q = kit::get_deleter<deleter>( p );

        KIT_TEST( q != 0 );
        KIT_TEST( q != 0 && q->data == 17041 );
    }

    return kit::report_errors();
}

}
