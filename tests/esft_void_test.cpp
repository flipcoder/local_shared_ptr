//
//  esft_void_test.cpp
//
//  Copyright 2009 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//


#include <kit/enable_shared_from_this.hpp>
#include <kit/local_shared_ptr.hpp>
#include "lightweight_test.hpp"

//

namespace esft_void_test {

class X: public kit::enable_shared_from_this<X>
{
};

int main()
{
    kit::local_shared_ptr< void const volatile > pv( new X );
    kit::local_shared_ptr< void > pv2 = kit::const_pointer_cast< void >( pv );
    kit::local_shared_ptr< X > px = kit::static_pointer_cast< X >( pv2 );

    try
    {
        kit::local_shared_ptr< X > qx = px->shared_from_this();

        KIT_TEST( px == qx );
        KIT_TEST( !( px < qx ) && !( qx < px ) );
    }
    catch( kit::bad_local_weak_ptr const& )
    {
        KIT_ERROR( "px->shared_from_this() failed" );
    }

    return kit::report_errors();
}

}
