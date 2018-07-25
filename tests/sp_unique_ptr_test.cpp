//
//  sp_unique_ptr_test.cpp
//
//  Copyright (c) 2012 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt
//

#include <kit/local_shared_ptr.hpp>
#include <kit/enable_shared_from_this.hpp>
#include "lightweight_test.hpp"
#include <memory>
#include <utility>

namespace sp_unique_ptr_test {

struct X: public kit::enable_shared_from_this< X >
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

struct Y
{
    static int instances;

    bool deleted_;

    Y(): deleted_( false )
    {
        ++instances;
    }

    ~Y()
    {
        KIT_TEST( deleted_ );
        --instances;
    }

private:

    Y( Y const & );
    Y & operator=( Y const & );
};

int Y::instances = 0;

struct YD
{
    void operator()( Y* p ) const
    {
        p->deleted_ = true;
        delete p;
    }
};

int main()
{
    {
        KIT_TEST( X::instances == 0 );

        std::unique_ptr<X> p( new X );
        KIT_TEST( X::instances == 1 );

        kit::local_shared_ptr<X> p2( std::move( p ) );
        KIT_TEST( X::instances == 1 );
        KIT_TEST( p.get() == 0 );

        kit::local_shared_ptr<X> p3 = p2->shared_from_this();
        KIT_TEST( p2 == p3 );
        KIT_TEST( !(p2 < p3) && !(p3 < p2) );

        p2.reset();
        p3.reset();
        KIT_TEST( X::instances == 0 );

        p2 = std::unique_ptr<X>( new X );
        KIT_TEST( X::instances == 1 );

        p2 = std::unique_ptr<X>( new X );
        KIT_TEST( X::instances == 1 );

        p2.reset();
        KIT_TEST( X::instances == 0 );
    }

    {
        KIT_TEST( X::instances == 0 );

        std::unique_ptr<X> p( new X );
        KIT_TEST( X::instances == 1 );

        kit::local_shared_ptr<X const> p2( std::move( p ) );
        KIT_TEST( X::instances == 1 );
        KIT_TEST( p.get() == 0 );

        kit::local_shared_ptr<X const> p3 = p2->shared_from_this();
        KIT_TEST( p2 == p3 );
        KIT_TEST( !(p2 < p3) && !(p3 < p2) );

        p2.reset();
        p3.reset();
        KIT_TEST( X::instances == 0 );

        p2 = std::unique_ptr<X>( new X );
        KIT_TEST( X::instances == 1 );

        p2 = std::unique_ptr<X>( new X );
        KIT_TEST( X::instances == 1 );

        p2.reset();
        KIT_TEST( X::instances == 0 );
    }

    {
        KIT_TEST( X::instances == 0 );

        std::unique_ptr<X> p( new X );
        KIT_TEST( X::instances == 1 );

        kit::local_shared_ptr<void> p2( std::move( p ) );
        KIT_TEST( X::instances == 1 );
        KIT_TEST( p.get() == 0 );

        p2.reset();
        KIT_TEST( X::instances == 0 );

        p2 = std::unique_ptr<X>( new X );
        KIT_TEST( X::instances == 1 );

        p2 = std::unique_ptr<X>( new X );
        KIT_TEST( X::instances == 1 );

        p2.reset();
        KIT_TEST( X::instances == 0 );
    }

    {
        KIT_TEST( Y::instances == 0 );

        std::unique_ptr<Y, YD> p( new Y, YD() );
        KIT_TEST( Y::instances == 1 );

        kit::local_shared_ptr<Y> p2( std::move( p ) );
        KIT_TEST( Y::instances == 1 );
        KIT_TEST( p.get() == 0 );

        p2.reset();
        KIT_TEST( Y::instances == 0 );

        p2 = std::unique_ptr<Y, YD>( new Y, YD() );
        KIT_TEST( Y::instances == 1 );

        p2 = std::unique_ptr<Y, YD>( new Y, YD() );
        KIT_TEST( Y::instances == 1 );

        p2.reset();
        KIT_TEST( Y::instances == 0 );
    }

    {
        KIT_TEST( Y::instances == 0 );

        YD yd;

        std::unique_ptr<Y, YD&> p( new Y, yd );
        KIT_TEST( Y::instances == 1 );

        kit::local_shared_ptr<Y> p2( std::move( p ) );
        KIT_TEST( Y::instances == 1 );
        KIT_TEST( p.get() == 0 );

        p2.reset();
        KIT_TEST( Y::instances == 0 );

        p2 = std::unique_ptr<Y, YD&>( new Y, yd );
        KIT_TEST( Y::instances == 1 );

        p2 = std::unique_ptr<Y, YD&>( new Y, yd );
        KIT_TEST( Y::instances == 1 );

        p2.reset();
        KIT_TEST( Y::instances == 0 );
    }

    {
        KIT_TEST( Y::instances == 0 );

        YD yd;

        std::unique_ptr<Y, YD const&> p( new Y, yd );
        KIT_TEST( Y::instances == 1 );

        kit::local_shared_ptr<Y> p2( std::move( p ) );
        KIT_TEST( Y::instances == 1 );
        KIT_TEST( p.get() == 0 );

        p2.reset();
        KIT_TEST( Y::instances == 0 );

        p2 = std::unique_ptr<Y, YD const&>( new Y, yd );
        KIT_TEST( Y::instances == 1 );

        p2 = std::unique_ptr<Y, YD const&>( new Y, yd );
        KIT_TEST( Y::instances == 1 );

        p2.reset();
        KIT_TEST( Y::instances == 0 );
    }

    return kit::report_errors();
}

}
