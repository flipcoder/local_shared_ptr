//
//  sp_zero_compare_test.cpp - == 0, != 0
//
//  Copyright 2012 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <kit/local_shared_ptr.hpp>
#include "lightweight_test.hpp"

namespace sp_zero_compare_test {

struct W
{
};

void intrusive_ptr_add_ref( W* )
{
}

void intrusive_ptr_release( W* )
{
}

int main()
{
//    {
//        kit::scoped_ptr<int> p;
//
//        KIT_TEST( p == 0 );
//        KIT_TEST( 0 == p );
//        KIT_TEST( !( p != 0 ) );
//        KIT_TEST( !( 0 != p ) );
//    }
//
//    {
//        kit::scoped_array<int> p;
//
//        KIT_TEST( p == 0 );
//        KIT_TEST( 0 == p );
//        KIT_TEST( !( p != 0 ) );
//        KIT_TEST( !( 0 != p ) );
//    }

    {
        kit::local_shared_ptr<int> p;

        KIT_TEST( p == 0 );
        KIT_TEST( 0 == p );
        KIT_TEST( !( p != 0 ) );
        KIT_TEST( !( 0 != p ) );
    }

//    {
//        kit::shared_array<int> p;
//
//        KIT_TEST( p == 0 );
//        KIT_TEST( 0 == p );
//        KIT_TEST( !( p != 0 ) );
//        KIT_TEST( !( 0 != p ) );
//    }
//
//    {
//        kit::intrusive_ptr<W> p;
//
//        KIT_TEST( p == 0 );
//        KIT_TEST( 0 == p );
//        KIT_TEST( !( p != 0 ) );
//        KIT_TEST( !( 0 != p ) );
//    }

    return kit::report_errors();
}

}
