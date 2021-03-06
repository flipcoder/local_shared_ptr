//
//  sp_recursive_assign_test.cpp
//
//  Copyright 2009 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//


#include <kit/local_shared_ptr.hpp>
#include "lightweight_test.hpp"

//

namespace sp_recursive_assign_test {

class X
{
public:

    static int instances;

    X()
    {
        ++instances;
    }

    ~X()
    {
        --instances;
    }

private:

    X( X const& );
};

int X::instances = 0;

class Y
{
public:

    static int instances;

    Y()
    {
        ++instances;
    }

    ~Y()
    {
        --instances;
    }

private:

    Y( Y const& );
};

int Y::instances = 0;

static kit::local_shared_ptr<void> s_pv;

class Z
{
public:

    static int instances;

    Z()
    {
        ++instances;
    }

    ~Z()
    {
        --instances;

        kit::local_shared_ptr<void> pv( new Y );
        s_pv = pv;
    }

private:

    Z( Z const& );
};

int Z::instances = 0;

int main()
{
    KIT_TEST( X::instances == 0 );
    KIT_TEST( Y::instances == 0 );
    KIT_TEST( Z::instances == 0 );

    {
        kit::local_shared_ptr<void> pv( new Z );
        s_pv = pv;
    }

    KIT_TEST( X::instances == 0 );
    KIT_TEST( Y::instances == 0 );
    KIT_TEST( Z::instances == 1 );

    {
        kit::local_shared_ptr<void> pv( new X );
        s_pv = pv;
    }

    KIT_TEST( X::instances == 0 );
    KIT_TEST( Y::instances == 1 );
    KIT_TEST( Z::instances == 0 );

    s_pv.reset();

    KIT_TEST( X::instances == 0 );
    KIT_TEST( Y::instances == 0 );
    KIT_TEST( Z::instances == 0 );

    return kit::report_errors();
}

}
