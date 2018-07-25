//
//  weak_from_this_test.cpp
//
//  Copyright (c) 2002, 2003, 2015 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//


#include <kit/enable_shared_from_this.hpp>
#include <kit/local_shared_ptr.hpp>
#include <kit/local_weak_ptr.hpp>
#include "lightweight_test.hpp"

namespace weak_from_this_test {

class V: public kit::enable_shared_from_this<V>
{
};

void test()
{
    kit::local_shared_ptr<V> p( new V );

    kit::local_weak_ptr<V> q = p;
    KIT_TEST( !q.expired() );

    kit::local_weak_ptr<V> q2 = p->weak_from_this();
    KIT_TEST( !q2.expired() );
    KIT_TEST( !(q < q2) && !(q2 < q) );

    V v2( *p );

    kit::local_weak_ptr<V> q3 = v2.weak_from_this();
    KIT_TEST( q3.expired() );

    *p = V();

    kit::local_weak_ptr<V> q4 = p->shared_from_this();
    KIT_TEST( !q4.expired() );
    KIT_TEST( !(q < q4) && !(q4 < q) );
    KIT_TEST( !(q2 < q4) && !(q4 < q2) );
}

int main()
{
    test();
    return kit::report_errors();
}

}
