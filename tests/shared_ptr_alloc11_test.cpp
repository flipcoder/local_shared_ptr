//  local_shared_ptr_alloc11_test.cpp
//
//  Test the allocator constructor with a C++11 minimal allocator
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

namespace local_shared_ptr_alloc11_test {

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

//

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

    kit::local_shared_ptr<void> pv( new X, D(), cxx11_allocator<X>() );

    KIT_TEST( X::instances == 1 );

    pv.reset();

    KIT_TEST( X::instances == 0 );

    pv.reset( new X, D(), cxx11_allocator<void>() );

    KIT_TEST( X::instances == 1 );

    pv.reset();

    KIT_TEST( X::instances == 0 );

    return kit::report_errors();
}

}
