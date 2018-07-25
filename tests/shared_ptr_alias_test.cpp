//  local_shared_ptr_alias_test.cpp
//
//  Copyright (c) 2007 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include "lightweight_test.hpp"
#include <kit/local_shared_ptr.hpp>
#include <memory>
#include <cstddef>

//

namespace local_shared_ptr_alias_test {

class incomplete;

struct X
{
    int v_;

    explicit X( int v ): v_( v )
    {
    }

    ~X()
    {
        v_ = 0;
    }
};

int main()
{
    {
        int m = 0;
        kit::local_shared_ptr< int > p;
        kit::local_shared_ptr< int > p2( p, &m );

        KIT_TEST( p2.get() == &m );
        KIT_TEST( p2? true: false );
        KIT_TEST( !!p2 );
        KIT_TEST( p2.use_count() == p.use_count() );
        KIT_TEST( !( p < p2 ) && !( p2 < p ) );

        p2.reset( p, 0 );

        KIT_TEST( p2.get() == 0 );
        KIT_TEST( p2? false: true );
        KIT_TEST( !p2 );
        KIT_TEST( p2.use_count() == p.use_count() );
        KIT_TEST( !( p < p2 ) && !( p2 < p ) );
    }

    {
        int m = 0;
        kit::local_shared_ptr< int > p( new int );
        kit::local_shared_ptr< int const > p2( p, &m );

        KIT_TEST( p2.get() == &m );
        KIT_TEST( p2? true: false );
        KIT_TEST( !!p2 );
        KIT_TEST( p2.use_count() == p.use_count() );
        KIT_TEST( !( p < p2 ) && !( p2 < p ) );

        kit::local_shared_ptr< int volatile > p3;
        p2.reset( p3, 0 );

        KIT_TEST( p2.get() == 0 );
        KIT_TEST( p2? false: true );
        KIT_TEST( !p2 );
        KIT_TEST( p2.use_count() == p3.use_count() );
        KIT_TEST( !( p3 < p2 ) && !( p2 < p3 ) );
    }

    {
        kit::local_shared_ptr< int > p( new int );
        kit::local_shared_ptr< void const > p2( p, 0 );

        KIT_TEST( p2.get() == 0 );
        KIT_TEST( p2? false: true );
        KIT_TEST( !p2 );
        KIT_TEST( p2.use_count() == p.use_count() );
        KIT_TEST( !( p < p2 ) && !( p2 < p ) );

        int m = 0;
        kit::local_shared_ptr< void volatile > p3;

        p2.reset( p3, &m );

        KIT_TEST( p2.get() == &m );
        KIT_TEST( p2? true: false );
        KIT_TEST( !!p2 );
        KIT_TEST( p2.use_count() == p3.use_count() );
        KIT_TEST( !( p3 < p2 ) && !( p2 < p3 ) );
    }

    {
        kit::local_shared_ptr< incomplete > p;
        kit::local_shared_ptr< incomplete > p2( p, 0 );

        KIT_TEST( p2.get() == 0 );
        KIT_TEST( p2? false: true );
        KIT_TEST( !p2 );
        KIT_TEST( p2.use_count() == p.use_count() );
        KIT_TEST( !( p < p2 ) && !( p2 < p ) );

        p2.reset( p, 0 );

        KIT_TEST( p2.get() == 0 );
        KIT_TEST( p2? false: true );
        KIT_TEST( !p2 );
        KIT_TEST( p2.use_count() == p.use_count() );
        KIT_TEST( !( p < p2 ) && !( p2 < p ) );
    }

    {
        kit::local_shared_ptr< X > p( new X( 5 ) );
        kit::local_shared_ptr< int const > p2( p, &p->v_ );

        KIT_TEST( p2.get() == &p->v_ );
        KIT_TEST( p2? true: false );
        KIT_TEST( !!p2 );
        KIT_TEST( p2.use_count() == p.use_count() );
        KIT_TEST( !( p < p2 ) && !( p2 < p ) );

        p.reset();
        KIT_TEST( *p2 == 5 );

        kit::local_shared_ptr< X const > p3( new X( 8 ) );
        p2.reset( p3, &p3->v_ );

        KIT_TEST( p2.get() == &p3->v_ );
        KIT_TEST( p2? true: false );
        KIT_TEST( !!p2 );
        KIT_TEST( p2.use_count() == p3.use_count() );
        KIT_TEST( !( p3 < p2 ) && !( p2 < p3 ) );

        p3.reset();
        KIT_TEST( *p2 == 8 );
    }

    return kit::report_errors();
}

}
