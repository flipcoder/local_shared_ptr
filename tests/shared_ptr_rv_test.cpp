//
//  local_shared_ptr_rv_test.cpp
//
//  Copyright (c) 2007 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#include <kit/local_shared_ptr.hpp>
#include "lightweight_test.hpp"

namespace local_shared_ptr_rv_test {

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

    static kit::local_shared_ptr<X> create()
    {
        return kit::local_shared_ptr<X>( new X );
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
        kit::local_shared_ptr<X> p( X::create() );
        KIT_TEST( X::instances == 1 );

        p = X::create();
        KIT_TEST( X::instances == 1 );

        p.reset();
        KIT_TEST( X::instances == 0 );

        p = X::create();
        KIT_TEST( X::instances == 1 );
    }

    KIT_TEST( X::instances == 0 );

    {
        kit::local_shared_ptr<X const> p( X::create() );
        KIT_TEST( X::instances == 1 );

        p = X::create();
        KIT_TEST( X::instances == 1 );

        p.reset();
        KIT_TEST( X::instances == 0 );

        p = X::create();
        KIT_TEST( X::instances == 1 );
    }

    KIT_TEST( X::instances == 0 );

    {
        kit::local_shared_ptr<void> p( X::create() );
        KIT_TEST( X::instances == 1 );

        p = X::create();
        KIT_TEST( X::instances == 1 );

        p.reset();
        KIT_TEST( X::instances == 0 );

        p = X::create();
        KIT_TEST( X::instances == 1 );
    }

    KIT_TEST( X::instances == 0 );

    {
        kit::local_shared_ptr<void const> p( X::create() );
        KIT_TEST( X::instances == 1 );

        p = X::create();
        KIT_TEST( X::instances == 1 );

        p.reset();
        KIT_TEST( X::instances == 0 );

        p = X::create();
        KIT_TEST( X::instances == 1 );
    }

    KIT_TEST( X::instances == 0 );

    return kit::report_errors();
}

}
