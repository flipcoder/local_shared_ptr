//
//  weak_ptr_move_test.cpp
//
//  Copyright (c) 2007 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#include <kit/local_shared_ptr.hpp>
#include <kit/local_weak_ptr.hpp>
#include "lightweight_test.hpp"
#include <utility>

namespace weak_ptr_move_test {

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
        kit::local_shared_ptr<X> p_( new X );
        kit::local_weak_ptr<X> p( p_ );
        KIT_TEST( X::instances == 1 );
        KIT_TEST( p.use_count() == 1 );

        kit::local_weak_ptr<X> p2( std::move( p ) );
        KIT_TEST( X::instances == 1 );
        KIT_TEST( p2.use_count() == 1 );
        KIT_TEST( p.expired() );

        kit::local_weak_ptr<void> p3( std::move( p2 ) );
        KIT_TEST( X::instances == 1 );
        KIT_TEST( p3.use_count() == 1 );
        KIT_TEST( p2.expired() );

        p_.reset();
        KIT_TEST( X::instances == 0 );
        KIT_TEST( p3.expired() );
    }

    {
        kit::local_shared_ptr<X> p_( new X );
        kit::local_weak_ptr<X> p( p_ );
        KIT_TEST( X::instances == 1 );
        KIT_TEST( p.use_count() == 1 );

        kit::local_weak_ptr<X> p2;
        p2 = static_cast< kit::local_weak_ptr<X> && >( p );
        KIT_TEST( X::instances == 1 );
        KIT_TEST( p2.use_count() == 1 );
        KIT_TEST( p.expired() );

        kit::local_weak_ptr<void> p3;
        p3 = std::move( p2 );
        KIT_TEST( X::instances == 1 );
        KIT_TEST( p3.use_count() == 1 );
        KIT_TEST( p2.expired() );

        p_.reset();
        KIT_TEST( X::instances == 0 );
        KIT_TEST( p3.expired() );
    }

    {
        kit::local_shared_ptr<X> p_( new X );
        kit::local_weak_ptr<X> p( p_ );
        KIT_TEST( X::instances == 1 );
        KIT_TEST( p.use_count() == 1 );

        kit::local_shared_ptr<X> p_2( new X );
        kit::local_weak_ptr<X> p2( p_2 );
        KIT_TEST( X::instances == 2 );
        p2 = std::move( p );
        KIT_TEST( X::instances == 2 );
        KIT_TEST( p2.use_count() == 1 );
        KIT_TEST( p.expired() );
        KIT_TEST( p2.lock() != p_2 );

        kit::local_shared_ptr<void> p_3( new X );
        kit::local_weak_ptr<void> p3( p_3 );
        KIT_TEST( X::instances == 3 );
        p3 = std::move( p2 );
        KIT_TEST( X::instances == 3 );
        KIT_TEST( p3.use_count() == 1 );
        KIT_TEST( p2.expired() );
        KIT_TEST( p3.lock() != p_3 );
    }

    return kit::report_errors();
}

}
