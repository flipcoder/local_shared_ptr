//
//  sp_nullptr_test.cpp
//
//  Copyright 2012 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <kit/local_shared_ptr.hpp>
#include "lightweight_test.hpp"
#include <cstddef>
#include <memory>

namespace sp_nullptr_test {

struct X
{
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

    X( X const & );
    X & operator=( X const & );
};

int X::instances = 0;

void f( std::nullptr_t )
{
}

int main()
{
    {
        kit::local_shared_ptr<void> p( nullptr );

        KIT_TEST( p.get() == 0 );
        KIT_TEST( p.use_count() == 0 );

        KIT_TEST( p == nullptr );
        KIT_TEST( nullptr == p );
        KIT_TEST( !( p != nullptr ) );
        KIT_TEST( !( nullptr != p ) );
    }

    {
        kit::local_shared_ptr<int> p( nullptr, f );

        KIT_TEST( p.get() == 0 );
        KIT_TEST( p.use_count() == 1 );

        KIT_TEST( p == nullptr );
        KIT_TEST( nullptr == p );
        KIT_TEST( !( p != nullptr ) );
        KIT_TEST( !( nullptr != p ) );
    }

    {
        kit::local_shared_ptr<int> p( nullptr, f, std::allocator<int>() );

        KIT_TEST( p.get() == 0 );
        KIT_TEST( p.use_count() == 1 );

        KIT_TEST( p == nullptr );
        KIT_TEST( nullptr == p );
        KIT_TEST( !( p != nullptr ) );
        KIT_TEST( !( nullptr != p ) );
    }

    {
        kit::local_shared_ptr<int> p( new int );

        KIT_TEST( p.get() != 0 );
        KIT_TEST( p.use_count() == 1 );

        KIT_TEST( p != nullptr );
        KIT_TEST( nullptr != p );
        KIT_TEST( !( p == nullptr ) );
        KIT_TEST( !( nullptr == p ) );

        p = nullptr;

        KIT_TEST( p.get() == 0 );
        KIT_TEST( p.use_count() == 0 );

        KIT_TEST( p == nullptr );
        KIT_TEST( nullptr == p );
        KIT_TEST( !( p != nullptr ) );
        KIT_TEST( !( nullptr != p ) );
    }

    {
        KIT_TEST( X::instances == 0 );

        kit::local_shared_ptr<X> p( new X );
        KIT_TEST( X::instances == 1 );

        KIT_TEST( p.get() != 0 );
        KIT_TEST( p.use_count() == 1 );

        KIT_TEST( p != nullptr );
        KIT_TEST( nullptr != p );
        KIT_TEST( !( p == nullptr ) );
        KIT_TEST( !( nullptr == p ) );

        p = nullptr;
        KIT_TEST( X::instances == 0 );

        KIT_TEST( p.get() == 0 );
        KIT_TEST( p.use_count() == 0 );

        KIT_TEST( p == nullptr );
        KIT_TEST( nullptr == p );
        KIT_TEST( !( p != nullptr ) );
        KIT_TEST( !( nullptr != p ) );
    }

    return kit::report_errors();
}

}
