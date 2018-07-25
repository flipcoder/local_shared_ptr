// allocate_shared_test.cpp
//
// Copyright 2007-2009 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include "lightweight_test.hpp"
#include <kit/make_local_shared.hpp>
#include <kit/local_shared_ptr.hpp>
#include <kit/local_weak_ptr.hpp>
#include <cstddef>
#include "tests.hpp"

namespace allocate_shared_test {

class X
{
private:

    X( X const & );
    X & operator=( X const & );

    void * operator new( std::size_t n )
    {
        // lack of this definition causes link errors on Comeau C++
        KIT_ERROR( "private X::new called" );
        return ::operator new( n );
    }

    void operator delete( void * p )
    {
        // lack of this definition causes link errors on MSVC
        KIT_ERROR( "private X::delete called" );
        ::operator delete( p );
    }

public:

    static int instances;

    int v;

    explicit X( int a1 = 0, int a2 = 0, int a3 = 0, int a4 = 0, int a5 = 0, int a6 = 0, int a7 = 0, int a8 = 0, int a9 = 0 ): v( a1+a2+a3+a4+a5+a6+a7+a8+a9 )
    {
        ++instances;
    }

    ~X()
    {
        --instances;
    }
};

int X::instances = 0;

int main()
{
    {
        kit::local_shared_ptr< int > pi = kit::allocate_shared< int >( std::allocator<int>() );

        KIT_TEST( pi.get() != 0 );
        KIT_TEST( *pi == 0 );
    }

    {
        kit::local_shared_ptr< int > pi = kit::allocate_shared< int >( std::allocator<int>(), 5 );

        KIT_TEST( pi.get() != 0 );
        KIT_TEST( *pi == 5 );
    }

    KIT_TEST( X::instances == 0 );

    {
        kit::local_shared_ptr< X > pi = kit::allocate_shared< X >( std::allocator<void>() );
        kit::local_weak_ptr<X> wp( pi );

        KIT_TEST( X::instances == 1 );
        KIT_TEST( pi.get() != 0 );
        KIT_TEST( pi->v == 0 );

        pi.reset();

        KIT_TEST( X::instances == 0 );
    }

    {
        kit::local_shared_ptr< X > pi = kit::allocate_shared< X >( std::allocator<void>(), 1 );
        kit::local_weak_ptr<X> wp( pi );

        KIT_TEST( X::instances == 1 );
        KIT_TEST( pi.get() != 0 );
        KIT_TEST( pi->v == 1 );

        pi.reset();

        KIT_TEST( X::instances == 0 );
    }

    {
        kit::local_shared_ptr< X > pi = kit::allocate_shared< X >( std::allocator<void>(), 1, 2 );
        kit::local_weak_ptr<X> wp( pi );

        KIT_TEST( X::instances == 1 );
        KIT_TEST( pi.get() != 0 );
        KIT_TEST( pi->v == 1+2 );

        pi.reset();

        KIT_TEST( X::instances == 0 );
    }

    {
        kit::local_shared_ptr< X > pi = kit::allocate_shared< X >( std::allocator<void>(), 1, 2, 3 );
        kit::local_weak_ptr<X> wp( pi );

        KIT_TEST( X::instances == 1 );
        KIT_TEST( pi.get() != 0 );
        KIT_TEST( pi->v == 1+2+3 );

        pi.reset();

        KIT_TEST( X::instances == 0 );
    }

    {
        kit::local_shared_ptr< X > pi = kit::allocate_shared< X >( std::allocator<void>(), 1, 2, 3, 4 );
        kit::local_weak_ptr<X> wp( pi );

        KIT_TEST( X::instances == 1 );
        KIT_TEST( pi.get() != 0 );
        KIT_TEST( pi->v == 1+2+3+4 );

        pi.reset();

        KIT_TEST( X::instances == 0 );
    }

    {
        kit::local_shared_ptr< X > pi = kit::allocate_shared< X >( std::allocator<void>(), 1, 2, 3, 4, 5 );
        kit::local_weak_ptr<X> wp( pi );

        KIT_TEST( X::instances == 1 );
        KIT_TEST( pi.get() != 0 );
        KIT_TEST( pi->v == 1+2+3+4+5 );

        pi.reset();

        KIT_TEST( X::instances == 0 );
    }

    {
        kit::local_shared_ptr< X > pi = kit::allocate_shared< X >( std::allocator<void>(), 1, 2, 3, 4, 5, 6 );
        kit::local_weak_ptr<X> wp( pi );

        KIT_TEST( X::instances == 1 );
        KIT_TEST( pi.get() != 0 );
        KIT_TEST( pi->v == 1+2+3+4+5+6 );

        pi.reset();

        KIT_TEST( X::instances == 0 );
    }

    {
        kit::local_shared_ptr< X > pi = kit::allocate_shared< X >( std::allocator<void>(), 1, 2, 3, 4, 5, 6, 7 );
        kit::local_weak_ptr<X> wp( pi );

        KIT_TEST( X::instances == 1 );
        KIT_TEST( pi.get() != 0 );
        KIT_TEST( pi->v == 1+2+3+4+5+6+7 );

        pi.reset();

        KIT_TEST( X::instances == 0 );
    }

    {
        kit::local_shared_ptr< X > pi = kit::allocate_shared< X >( std::allocator<void>(), 1, 2, 3, 4, 5, 6, 7, 8 );
        kit::local_weak_ptr<X> wp( pi );

        KIT_TEST( X::instances == 1 );
        KIT_TEST( pi.get() != 0 );
        KIT_TEST( pi->v == 1+2+3+4+5+6+7+8 );

        pi.reset();

        KIT_TEST( X::instances == 0 );
    }

    {
        kit::local_shared_ptr< X > pi = kit::allocate_shared< X >( std::allocator<void>(), 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        kit::local_weak_ptr<X> wp( pi );

        KIT_TEST( X::instances == 1 );
        KIT_TEST( pi.get() != 0 );
        KIT_TEST( pi->v == 1+2+3+4+5+6+7+8+9 );

        pi.reset();

        KIT_TEST( X::instances == 0 );
    }

    return kit::report_errors();
}

}
