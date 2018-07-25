//
//  local_shared_ptr_move_test.cpp
//
//  Copyright (c) 2007 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#include <kit/local_shared_ptr.hpp>
#include "lightweight_test.hpp"
#include <utility>

namespace local_shared_ptr_move_test {

struct X
{
    static long instances;

    X()
    {
        ++instances;
    }

    ~X()
    {
        --instances;
    }

private:

    X( X const & );
    X & operator=( X const & );
};

long X::instances = 0;

int main()
{
    KIT_TEST( X::instances == 0 );

    {
        kit::local_shared_ptr<X> p( new X );
        KIT_TEST( X::instances == 1 );

        kit::local_shared_ptr<X> p2( std::move( p ) );
        KIT_TEST( X::instances == 1 );
        KIT_TEST( p.get() == 0 );

        kit::local_shared_ptr<void> p3( std::move( p2 ) );
        KIT_TEST( X::instances == 1 );
        KIT_TEST( p2.get() == 0 );

        p3.reset();
        KIT_TEST( X::instances == 0 );
    }

    {
        kit::local_shared_ptr<X> p( new X );
        KIT_TEST( X::instances == 1 );

        kit::local_shared_ptr<X> p2;
        p2 = std::move( p );
        KIT_TEST( X::instances == 1 );
        KIT_TEST( p.get() == 0 );

        kit::local_shared_ptr<void> p3;
        p3 = std::move( p2 );
        KIT_TEST( X::instances == 1 );
        KIT_TEST( p2.get() == 0 );

        p3.reset();
        KIT_TEST( X::instances == 0 );
    }

    {
        kit::local_shared_ptr<X> p( new X );
        KIT_TEST( X::instances == 1 );

        kit::local_shared_ptr<X> p2( new X );
        KIT_TEST( X::instances == 2 );
        p2 = std::move( p );
        KIT_TEST( X::instances == 1 );
        KIT_TEST( p.get() == 0 );

        kit::local_shared_ptr<void> p3( new X );
        KIT_TEST( X::instances == 2 );
        p3 = std::move( p2 );
        KIT_TEST( X::instances == 1 );
        KIT_TEST( p2.get() == 0 );

        p3.reset();
        KIT_TEST( X::instances == 0 );
    }

    return kit::report_errors();
}

}
