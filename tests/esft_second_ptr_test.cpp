//
//  esft_second_ptr_test.cpp
//
//  This test has been extracted from a real
//  scenario that occurs in Boost.Python
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

namespace esft_second_ptr_test {

class X: public kit::enable_shared_from_this<X>
{
};

void null_deleter( void const* )
{
}

int main()
{
    kit::local_shared_ptr<X> px( new X );

    {
        kit::local_shared_ptr<X> px2( px.get(), null_deleter );
        KIT_TEST( px == px2 );
    }

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
