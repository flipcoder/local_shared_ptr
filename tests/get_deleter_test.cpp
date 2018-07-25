//
//  get_deleter_test.cpp
//
//  Copyright (c) 2002 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <kit/local_shared_ptr.hpp>
#include "lightweight_test.hpp"

namespace get_deleter_test {

struct deleter
{
    int data;

    deleter(): data(0)
    {
    }

    void operator()(void *)
    {
        KIT_TEST(data == 17041);
    }
};

struct deleter2
{
};

struct X
{
};

int main()
{
    {
        kit::local_shared_ptr<X> p;

        KIT_TEST(kit::get_deleter<void>(p) == 0);
        KIT_TEST(kit::get_deleter<void const>(p) == 0);
        KIT_TEST(kit::get_deleter<int>(p) == 0);
        KIT_TEST(kit::get_deleter<int const>(p) == 0);
        KIT_TEST(kit::get_deleter<X>(p) == 0);
        KIT_TEST(kit::get_deleter<X const>(p) == 0);
        KIT_TEST(kit::get_deleter<deleter>(p) == 0);
        KIT_TEST(kit::get_deleter<deleter const>(p) == 0);
        KIT_TEST(kit::get_deleter<deleter2>(p) == 0);
        KIT_TEST(kit::get_deleter<deleter2 const>(p) == 0);
    }

    {
        kit::local_shared_ptr<X> p(new X);

        KIT_TEST(kit::get_deleter<void>(p) == 0);
        KIT_TEST(kit::get_deleter<void const>(p) == 0);
        KIT_TEST(kit::get_deleter<int>(p) == 0);
        KIT_TEST(kit::get_deleter<int const>(p) == 0);
        KIT_TEST(kit::get_deleter<X>(p) == 0);
        KIT_TEST(kit::get_deleter<X const>(p) == 0);
        KIT_TEST(kit::get_deleter<deleter>(p) == 0);
        KIT_TEST(kit::get_deleter<deleter const>(p) == 0);
        KIT_TEST(kit::get_deleter<deleter2>(p) == 0);
        KIT_TEST(kit::get_deleter<deleter2 const>(p) == 0);
    }

    {
        X x;
        kit::local_shared_ptr<X> p(&x, deleter());

        KIT_TEST(kit::get_deleter<void>(p) == 0);
        KIT_TEST(kit::get_deleter<void const>(p) == 0);
        KIT_TEST(kit::get_deleter<int>(p) == 0);
        KIT_TEST(kit::get_deleter<int const>(p) == 0);
        KIT_TEST(kit::get_deleter<X>(p) == 0);
        KIT_TEST(kit::get_deleter<X const>(p) == 0);
        KIT_TEST(kit::get_deleter<deleter2>(p) == 0);
        KIT_TEST(kit::get_deleter<deleter2 const>(p) == 0);

        deleter * q = kit::get_deleter<deleter>(p);

        KIT_TEST(q != 0);
        KIT_TEST(q->data == 0);

        q->data = 17041;

        deleter const * r = kit::get_deleter<deleter const>(p);

        KIT_TEST(r == q);
        KIT_TEST(r->data == 17041);
    }

    return kit::report_errors();
}

}
