//  local_shared_ptr_alloc3_test.cpp
//
//  Copyright (c) 2005, 2014 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt


#include "lightweight_test.hpp"
#include <kit/local_shared_ptr.hpp>
#include <memory>
#include <cstddef>

//

namespace local_shared_ptr_alloc3_test {

struct D;

struct X
{
    static int instances;

    X(): deleted_( false )
    {
        ++instances;
    }

    ~X()
    {
        KIT_TEST( deleted_ );
        --instances;
    }

private:

    friend struct D;

    bool deleted_;

    X( X const & );
    X & operator=( X const & );
};

int X::instances = 0;

struct D
{
    void operator()( X * px ) const
    {
        px->deleted_ = true;
        delete px;
    }
};

int main()
{
    KIT_TEST( X::instances == 0 );

    kit::local_shared_ptr<void> pv( new X, D(), std::allocator<X>() );

    KIT_TEST( X::instances == 1 );

    pv.reset();

    KIT_TEST( X::instances == 0 );

    pv.reset( new X, D(), std::allocator<void>() );

    KIT_TEST( X::instances == 1 );

    pv.reset();

    KIT_TEST( X::instances == 0 );

    return kit::report_errors();
}

}
