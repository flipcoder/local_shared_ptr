// allocate_shared_alloc11_test.cpp
//
// allocate_shared with a minimal C++11 allocator
//
// Copyright 2007-2009, 2014 Peter Dimov
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

namespace allocate_shared_alloc11_test {
  
template< class T > class cxx11_allocator
{
public:

    typedef T value_type;

    cxx11_allocator()
    {
    }

    template< class Y > cxx11_allocator( cxx11_allocator<Y> const & )
    {
    }

    T * allocate( std::size_t n )
    {
        return static_cast< T* >( ::operator new( n * sizeof( T ) ) );
    }

    void deallocate( T * p, std::size_t n )
    {
        ::operator delete( p );
    }
};

class X
{
private:

    X( X const & );
    X & operator=( X const & );

    void * operator new( std::size_t n )
    {
        KIT_ERROR( "private X::new called" );
        return ::operator new( n );
    }

    void operator delete( void * p )
    {
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
        kit::local_shared_ptr< int > pi = kit::allocate_shared< int >( cxx11_allocator<int>() );

        KIT_TEST( pi.get() != 0 );
        KIT_TEST( *pi == 0 );
    }

    {
        kit::local_shared_ptr< int > pi = kit::allocate_shared< int >( cxx11_allocator<int>(), 5 );

        KIT_TEST( pi.get() != 0 );
        KIT_TEST( *pi == 5 );
    }

    KIT_TEST( X::instances == 0 );

    {
        kit::local_shared_ptr< X > pi = kit::allocate_shared< X >( cxx11_allocator<void>() );
        kit::local_weak_ptr<X> wp( pi );

        KIT_TEST( X::instances == 1 );
        KIT_TEST( pi.get() != 0 );
        KIT_TEST( pi->v == 0 );

        pi.reset();

        KIT_TEST( X::instances == 0 );
    }

    {
        kit::local_shared_ptr< X > pi = kit::allocate_shared< X >( cxx11_allocator<void>(), 1 );
        kit::local_weak_ptr<X> wp( pi );

        KIT_TEST( X::instances == 1 );
        KIT_TEST( pi.get() != 0 );
        KIT_TEST( pi->v == 1 );

        pi.reset();

        KIT_TEST( X::instances == 0 );
    }

    {
        kit::local_shared_ptr< X > pi = kit::allocate_shared< X >( cxx11_allocator<void>(), 1, 2 );
        kit::local_weak_ptr<X> wp( pi );

        KIT_TEST( X::instances == 1 );
        KIT_TEST( pi.get() != 0 );
        KIT_TEST( pi->v == 1+2 );

        pi.reset();

        KIT_TEST( X::instances == 0 );
    }

    {
        kit::local_shared_ptr< X > pi = kit::allocate_shared< X >( cxx11_allocator<void>(), 1, 2, 3 );
        kit::local_weak_ptr<X> wp( pi );

        KIT_TEST( X::instances == 1 );
        KIT_TEST( pi.get() != 0 );
        KIT_TEST( pi->v == 1+2+3 );

        pi.reset();

        KIT_TEST( X::instances == 0 );
    }

    {
        kit::local_shared_ptr< X > pi = kit::allocate_shared< X >( cxx11_allocator<void>(), 1, 2, 3, 4 );
        kit::local_weak_ptr<X> wp( pi );

        KIT_TEST( X::instances == 1 );
        KIT_TEST( pi.get() != 0 );
        KIT_TEST( pi->v == 1+2+3+4 );

        pi.reset();

        KIT_TEST( X::instances == 0 );
    }

    {
        kit::local_shared_ptr< X > pi = kit::allocate_shared< X >( cxx11_allocator<void>(), 1, 2, 3, 4, 5 );
        kit::local_weak_ptr<X> wp( pi );

        KIT_TEST( X::instances == 1 );
        KIT_TEST( pi.get() != 0 );
        KIT_TEST( pi->v == 1+2+3+4+5 );

        pi.reset();

        KIT_TEST( X::instances == 0 );
    }

    {
        kit::local_shared_ptr< X > pi = kit::allocate_shared< X >( cxx11_allocator<void>(), 1, 2, 3, 4, 5, 6 );
        kit::local_weak_ptr<X> wp( pi );

        KIT_TEST( X::instances == 1 );
        KIT_TEST( pi.get() != 0 );
        KIT_TEST( pi->v == 1+2+3+4+5+6 );

        pi.reset();

        KIT_TEST( X::instances == 0 );
    }

    {
        kit::local_shared_ptr< X > pi = kit::allocate_shared< X >( cxx11_allocator<void>(), 1, 2, 3, 4, 5, 6, 7 );
        kit::local_weak_ptr<X> wp( pi );

        KIT_TEST( X::instances == 1 );
        KIT_TEST( pi.get() != 0 );
        KIT_TEST( pi->v == 1+2+3+4+5+6+7 );

        pi.reset();

        KIT_TEST( X::instances == 0 );
    }

    {
        kit::local_shared_ptr< X > pi = kit::allocate_shared< X >( cxx11_allocator<void>(), 1, 2, 3, 4, 5, 6, 7, 8 );
        kit::local_weak_ptr<X> wp( pi );

        KIT_TEST( X::instances == 1 );
        KIT_TEST( pi.get() != 0 );
        KIT_TEST( pi->v == 1+2+3+4+5+6+7+8 );

        pi.reset();

        KIT_TEST( X::instances == 0 );
    }

    {
        kit::local_shared_ptr< X > pi = kit::allocate_shared< X >( cxx11_allocator<void>(), 1, 2, 3, 4, 5, 6, 7, 8, 9 );
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
