//  wp_convertible_test.cpp
//
//  Copyright (c) 2008 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "lightweight_test.hpp"
#include <kit/local_weak_ptr.hpp>

//

namespace wp_convertible_test {

class incomplete;

struct X
{
};

struct Y
{
};

struct Z: public X
{
};

int f( kit::local_weak_ptr<void const> )
{
    return 1;
}

int f( kit::local_weak_ptr<int> )
{
    return 2;
}

int f( kit::local_weak_ptr<incomplete> )
{
    return 3;
}

int g( kit::local_weak_ptr<X> )
{
    return 4;
}

int g( kit::local_weak_ptr<Y> )
{
    return 5;
}

int g( kit::local_weak_ptr<incomplete> )
{
    return 6;
}

int main()
{
    KIT_TEST( 1 == f( kit::local_weak_ptr<double>() ) );
    KIT_TEST( 1 == f( kit::local_shared_ptr<double>() ) );
    KIT_TEST( 4 == g( kit::local_weak_ptr<Z>() ) );
    KIT_TEST( 4 == g( kit::local_shared_ptr<Z>() ) );

    return kit::report_errors();
}

}
