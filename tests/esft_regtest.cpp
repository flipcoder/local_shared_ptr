//
//  esft_regtest.cpp
//
//  A regression test for enable_shared_from_this
//
//  Copyright (c) 2008 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#include "lightweight_test.hpp"
#include <kit/enable_shared_from_this.hpp>
#include <kit/local_shared_ptr.hpp>
#include <kit/local_weak_ptr.hpp>
#include <memory>
#include <string>

namespace esft_regtest {

class X: public kit::enable_shared_from_this< X >
{
private:

    int destroyed_;
    int deleted_;
    int expected_;

private:

    X( X const& );
    X& operator=( X const& );

public:

    static int instances;

public:

    explicit X( int expected ): destroyed_( 0 ), deleted_( 0 ), expected_( expected )
    {
        ++instances;
    }

    ~X()
    {
        KIT_TEST( deleted_ == expected_ );
        KIT_TEST( destroyed_ == 0 );
        ++destroyed_;
        --instances;
    }

    typedef void (*deleter_type)( X* );

    static void deleter( X * px )
    {
        ++px->deleted_;
    }

    static void deleter2( X * px )
    {
        ++px->deleted_;
        delete px;
    }
};

int X::instances = 0;

void test()
{
    KIT_TEST( X::instances == 0 );

    {
        X x( 0 );
        KIT_TEST( X::instances == 1 );
    }

    KIT_TEST( X::instances == 0 );

//    {
//        std::auto_ptr<X> px( new X( 0 ) );
//        KIT_TEST( X::instances == 1 );
//    }

    KIT_TEST( X::instances == 0 );

    {
        kit::local_shared_ptr<X> px( new X( 0 ) );
        KIT_TEST( X::instances == 1 );

        kit::local_weak_ptr<X> wp( px );
        KIT_TEST( !wp.expired() );

        px.reset();

        KIT_TEST( wp.expired() );
    }

    KIT_TEST( X::instances == 0 );

    {
        X x( 1 );
        kit::local_shared_ptr<X> px( &x, X::deleter );
        KIT_TEST( X::instances == 1 );

        X::deleter_type * pd = kit::get_deleter<X::deleter_type>( px );
        KIT_TEST( pd != 0 && *pd == X::deleter );

        kit::local_weak_ptr<X> wp( px );
        KIT_TEST( !wp.expired() );

        px.reset();

        KIT_TEST( wp.expired() );
    }

    KIT_TEST( X::instances == 0 );

    {
        kit::local_shared_ptr<X> px( new X( 1 ), X::deleter2 );
        KIT_TEST( X::instances == 1 );

        X::deleter_type * pd = kit::get_deleter<X::deleter_type>( px );
        KIT_TEST( pd != 0 && *pd == X::deleter2 );

        kit::local_weak_ptr<X> wp( px );
        KIT_TEST( !wp.expired() );

        px.reset();

        KIT_TEST( wp.expired() );
    }

    KIT_TEST( X::instances == 0 );
}

struct V: public kit::enable_shared_from_this<V>
{
    virtual ~V() {}
    std::string m_;
};

struct V2
{
    virtual ~V2() {}
    std::string m2_;
};

struct W: V2, V
{
};

void test2()
{
    kit::local_shared_ptr<W> p( new W );
}

void test3()
{
    V * p = new W;
    kit::local_shared_ptr<void> pv( p );
    KIT_TEST( pv.get() == p );
    KIT_TEST( pv.use_count() == 1 );
}

struct null_deleter
{
    void operator()( void const* ) const {}
};

void test4()
{
    kit::local_shared_ptr<V> pv( new V );
    kit::local_shared_ptr<V> pv2( pv.get(), null_deleter() );
    KIT_TEST( pv2.get() == pv.get() );
    KIT_TEST( pv2.use_count() == 1 );
}

void test5()
{
    V v;

    kit::local_shared_ptr<V> p1( &v, null_deleter() );
    KIT_TEST( p1.get() == &v );
    KIT_TEST( p1.use_count() == 1 );

    try
    {
        p1->shared_from_this();
    }
    catch( ... )
    {
        KIT_ERROR( "p1->shared_from_this() failed" );
    }

    p1.reset();

    kit::local_shared_ptr<V> p2( &v, null_deleter() );
    KIT_TEST( p2.get() == &v );
    KIT_TEST( p2.use_count() == 1 );

    try
    {
        p2->shared_from_this();
    }
    catch( ... )
    {
        KIT_ERROR( "p2->shared_from_this() failed" );
    }
}

int main()
{
    test();
    test2();
    test3();
    test4();
    test5();

    return kit::report_errors();
}

}
