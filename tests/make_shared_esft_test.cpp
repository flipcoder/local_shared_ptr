//  make_local_shared_esft_test.cpp
//
//  Copyright 2007-2009 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

#include "lightweight_test.hpp"
#include <kit/make_local_shared.hpp>
#include <kit/local_shared_ptr.hpp>
#include <kit/enable_shared_from_this.hpp>

namespace make_local_shared_esft_test {

class X: public kit::enable_shared_from_this<X>
{
private:

    X( X const & );
    X & operator=( X const & );

public:

    static int instances;

    explicit X( int = 0, int = 0, int = 0, int = 0, int = 0, int = 0, int = 0, int = 0, int = 0 )
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
    KIT_TEST( X::instances == 0 );

    {
        kit::local_shared_ptr< X > px = kit::make_local_shared< X >();
        KIT_TEST( X::instances == 1 );

        try
        {
            kit::local_shared_ptr< X > qx = px->shared_from_this();

            KIT_TEST( px == qx );
            KIT_TEST( !( px < qx ) && !( qx < px ) );

            px.reset();
            KIT_TEST( X::instances == 1 );
        }
        catch( kit::bad_local_weak_ptr const& )
        {
            KIT_ERROR( "px->shared_from_this() failed" );
        }
    }

    KIT_TEST( X::instances == 0 );

    {
        kit::local_shared_ptr< X > px = kit::make_local_shared< X >( 1 );
        KIT_TEST( X::instances == 1 );

        try
        {
            kit::local_shared_ptr< X > qx = px->shared_from_this();

            KIT_TEST( px == qx );
            KIT_TEST( !( px < qx ) && !( qx < px ) );

            px.reset();
            KIT_TEST( X::instances == 1 );
        }
        catch( kit::bad_local_weak_ptr const& )
        {
            KIT_ERROR( "px->shared_from_this() failed" );
        }
    }

    KIT_TEST( X::instances == 0 );

    {
        kit::local_shared_ptr< X > px = kit::make_local_shared< X >( 1, 2 );
        KIT_TEST( X::instances == 1 );

        try
        {
            kit::local_shared_ptr< X > qx = px->shared_from_this();

            KIT_TEST( px == qx );
            KIT_TEST( !( px < qx ) && !( qx < px ) );

            px.reset();
            KIT_TEST( X::instances == 1 );
        }
        catch( kit::bad_local_weak_ptr const& )
        {
            KIT_ERROR( "px->shared_from_this() failed" );
        }
    }

    KIT_TEST( X::instances == 0 );

    {
        kit::local_shared_ptr< X > px = kit::make_local_shared< X >( 1, 2, 3 );
        KIT_TEST( X::instances == 1 );

        try
        {
            kit::local_shared_ptr< X > qx = px->shared_from_this();

            KIT_TEST( px == qx );
            KIT_TEST( !( px < qx ) && !( qx < px ) );

            px.reset();
            KIT_TEST( X::instances == 1 );
        }
        catch( kit::bad_local_weak_ptr const& )
        {
            KIT_ERROR( "px->shared_from_this() failed" );
        }
    }

    KIT_TEST( X::instances == 0 );

    {
        kit::local_shared_ptr< X > px = kit::make_local_shared< X >( 1, 2, 3, 4 );
        KIT_TEST( X::instances == 1 );

        try
        {
            kit::local_shared_ptr< X > qx = px->shared_from_this();

            KIT_TEST( px == qx );
            KIT_TEST( !( px < qx ) && !( qx < px ) );

            px.reset();
            KIT_TEST( X::instances == 1 );
        }
        catch( kit::bad_local_weak_ptr const& )
        {
            KIT_ERROR( "px->shared_from_this() failed" );
        }
    }

    KIT_TEST( X::instances == 0 );

    {
        kit::local_shared_ptr< X > px = kit::make_local_shared< X >( 1, 2, 3, 4, 5 );
        KIT_TEST( X::instances == 1 );

        try
        {
            kit::local_shared_ptr< X > qx = px->shared_from_this();

            KIT_TEST( px == qx );
            KIT_TEST( !( px < qx ) && !( qx < px ) );

            px.reset();
            KIT_TEST( X::instances == 1 );
        }
        catch( kit::bad_local_weak_ptr const& )
        {
            KIT_ERROR( "px->shared_from_this() failed" );
        }
    }

    KIT_TEST( X::instances == 0 );

    {
        kit::local_shared_ptr< X > px = kit::make_local_shared< X >( 1, 2, 3, 4, 5, 6 );
        KIT_TEST( X::instances == 1 );

        try
        {
            kit::local_shared_ptr< X > qx = px->shared_from_this();

            KIT_TEST( px == qx );
            KIT_TEST( !( px < qx ) && !( qx < px ) );

            px.reset();
            KIT_TEST( X::instances == 1 );
        }
        catch( kit::bad_local_weak_ptr const& )
        {
            KIT_ERROR( "px->shared_from_this() failed" );
        }
    }

    KIT_TEST( X::instances == 0 );

    {
        kit::local_shared_ptr< X > px = kit::make_local_shared< X >( 1, 2, 3, 4, 5, 6, 7 );
        KIT_TEST( X::instances == 1 );

        try
        {
            kit::local_shared_ptr< X > qx = px->shared_from_this();

            KIT_TEST( px == qx );
            KIT_TEST( !( px < qx ) && !( qx < px ) );

            px.reset();
            KIT_TEST( X::instances == 1 );
        }
        catch( kit::bad_local_weak_ptr const& )
        {
            KIT_ERROR( "px->shared_from_this() failed" );
        }
    }

    KIT_TEST( X::instances == 0 );

    {
        kit::local_shared_ptr< X > px = kit::make_local_shared< X >( 1, 2, 3, 4, 5, 6, 7, 8 );
        KIT_TEST( X::instances == 1 );

        try
        {
            kit::local_shared_ptr< X > qx = px->shared_from_this();

            KIT_TEST( px == qx );
            KIT_TEST( !( px < qx ) && !( qx < px ) );

            px.reset();
            KIT_TEST( X::instances == 1 );
        }
        catch( kit::bad_local_weak_ptr const& )
        {
            KIT_ERROR( "px->shared_from_this() failed" );
        }
    }

    KIT_TEST( X::instances == 0 );

    {
        kit::local_shared_ptr< X > px = kit::make_local_shared< X >( 1, 2, 3, 4, 5, 6, 7, 8, 9 );
        KIT_TEST( X::instances == 1 );

        try
        {
            kit::local_shared_ptr< X > qx = px->shared_from_this();

            KIT_TEST( px == qx );
            KIT_TEST( !( px < qx ) && !( qx < px ) );

            px.reset();
            KIT_TEST( X::instances == 1 );
        }
        catch( kit::bad_local_weak_ptr const& )
        {
            KIT_ERROR( "px->shared_from_this() failed" );
        }
    }

    KIT_TEST( X::instances == 0 );

    return kit::report_errors();
}

}
