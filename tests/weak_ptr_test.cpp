//
//  weak_ptr_test.cpp
//
//  Copyright (c) 2002-2005 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "lightweight_test.hpp"
#include <kit/local_shared_ptr.hpp>
#include <kit/local_weak_ptr.hpp>
#include "tests.hpp"
#include <map>
#include <vector>

//

namespace weak_ptr_test
{

namespace n_element_type
{

void f(int &)
{
}

void test()
{
    typedef kit::local_weak_ptr<int>::element_type T;
    T t;
    f(t);
}

} // namespace n_element_type

class incomplete;

kit::local_shared_ptr<incomplete> create_incomplete();

struct X
{
    int dummy;
};

struct Y
{
    int dummy2;
};

struct Z: public X, public virtual Y
{
};

namespace n_constructors
{

void default_constructor()
{
    {
        kit::local_weak_ptr<int> wp;
        KIT_TEST(wp.use_count() == 0);
    }

    {
        kit::local_weak_ptr<void> wp;
        KIT_TEST(wp.use_count() == 0);
    }

    {
        kit::local_weak_ptr<incomplete> wp;
        KIT_TEST(wp.use_count() == 0);
    }
}

void local_shared_ptr_constructor()
{
    {
        kit::local_shared_ptr<int> sp;

        kit::local_weak_ptr<int> wp(sp);
        KIT_TEST(wp.use_count() == sp.use_count());

        kit::local_weak_ptr<void> wp2(sp);
        KIT_TEST(wp2.use_count() == sp.use_count());
    }

    {
        kit::local_shared_ptr<int> sp(static_cast<int*>(0));

        {
            kit::local_weak_ptr<int> wp(sp);
            KIT_TEST(wp.use_count() == sp.use_count());
            KIT_TEST(wp.use_count() == 1);
            kit::local_shared_ptr<int> sp2(wp);
            KIT_TEST(wp.use_count() == 2);
            KIT_TEST(!(sp < sp2 || sp2 < sp));
        }

        {
            kit::local_weak_ptr<void> wp(sp);
            KIT_TEST(wp.use_count() == sp.use_count());
            KIT_TEST(wp.use_count() == 1);
            kit::local_shared_ptr<void> sp2(wp);
            KIT_TEST(wp.use_count() == 2);
            KIT_TEST(!(sp < sp2 || sp2 < sp));
        }
    }

    {
        kit::local_shared_ptr<int> sp(new int);

        {
            kit::local_weak_ptr<int> wp(sp);
            KIT_TEST(wp.use_count() == sp.use_count());
            KIT_TEST(wp.use_count() == 1);
            kit::local_shared_ptr<int> sp2(wp);
            KIT_TEST(wp.use_count() == 2);
            KIT_TEST(!(sp < sp2 || sp2 < sp));
        }

        {
            kit::local_weak_ptr<void> wp(sp);
            KIT_TEST(wp.use_count() == sp.use_count());
            KIT_TEST(wp.use_count() == 1);
            kit::local_shared_ptr<void> sp2(wp);
            KIT_TEST(wp.use_count() == 2);
            KIT_TEST(!(sp < sp2 || sp2 < sp));
        }
    }

    {
        kit::local_shared_ptr<void> sp;

        kit::local_weak_ptr<void> wp(sp);
        KIT_TEST(wp.use_count() == sp.use_count());
    }

    {
        kit::local_shared_ptr<void> sp(static_cast<int*>(0));

        kit::local_weak_ptr<void> wp(sp);
        KIT_TEST(wp.use_count() == sp.use_count());
        KIT_TEST(wp.use_count() == 1);
        kit::local_shared_ptr<void> sp2(wp);
        KIT_TEST(wp.use_count() == 2);
        KIT_TEST(!(sp < sp2 || sp2 < sp));
    }

    {
        kit::local_shared_ptr<void> sp(new int);

        kit::local_weak_ptr<void> wp(sp);
        KIT_TEST(wp.use_count() == sp.use_count());
        KIT_TEST(wp.use_count() == 1);
        kit::local_shared_ptr<void> sp2(wp);
        KIT_TEST(wp.use_count() == 2);
        KIT_TEST(!(sp < sp2 || sp2 < sp));
    }

    {
        kit::local_shared_ptr<incomplete> sp;

        kit::local_weak_ptr<incomplete> wp(sp);
        KIT_TEST(wp.use_count() == sp.use_count());

        kit::local_weak_ptr<void> wp2(sp);
        KIT_TEST(wp2.use_count() == sp.use_count());
    }

    {
        kit::local_shared_ptr<incomplete> sp = create_incomplete();

        {
            kit::local_weak_ptr<incomplete> wp(sp);
            KIT_TEST(wp.use_count() == sp.use_count());
            KIT_TEST(wp.use_count() == 1);
            kit::local_shared_ptr<incomplete> sp2(wp);
            KIT_TEST(wp.use_count() == 2);
            KIT_TEST(!(sp < sp2 || sp2 < sp));
        }

        {
            kit::local_weak_ptr<void> wp(sp);
            KIT_TEST(wp.use_count() == sp.use_count());
            KIT_TEST(wp.use_count() == 1);
            kit::local_shared_ptr<void> sp2(wp);
            KIT_TEST(wp.use_count() == 2);
            KIT_TEST(!(sp < sp2 || sp2 < sp));
        }
    }

    {
        kit::local_shared_ptr<void> sp = create_incomplete();

        kit::local_weak_ptr<void> wp(sp);
        KIT_TEST(wp.use_count() == sp.use_count());
        KIT_TEST(wp.use_count() == 1);
        kit::local_shared_ptr<void> sp2(wp);
        KIT_TEST(wp.use_count() == 2);
        KIT_TEST(!(sp < sp2 || sp2 < sp));
    }
}

void copy_constructor()
{
    {
        kit::local_weak_ptr<int> wp;
        kit::local_weak_ptr<int> wp2(wp);
        KIT_TEST(wp2.use_count() == wp.use_count());
        KIT_TEST(wp2.use_count() == 0);
    }

    {
        kit::local_weak_ptr<void> wp;
        kit::local_weak_ptr<void> wp2(wp);
        KIT_TEST(wp2.use_count() == wp.use_count());
        KIT_TEST(wp2.use_count() == 0);
    }

    {
        kit::local_weak_ptr<incomplete> wp;
        kit::local_weak_ptr<incomplete> wp2(wp);
        KIT_TEST(wp2.use_count() == wp.use_count());
        KIT_TEST(wp2.use_count() == 0);
    }

    {
        kit::local_shared_ptr<int> sp(static_cast<int*>(0));
        kit::local_weak_ptr<int> wp(sp);

        kit::local_weak_ptr<int> wp2(wp);
        KIT_TEST(wp2.use_count() == wp.use_count());
        KIT_TEST(wp2.use_count() == 1);
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        sp.reset();
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        kit::local_weak_ptr<int> wp3(wp);
        KIT_TEST(wp3.use_count() == wp.use_count());
        KIT_TEST(wp3.use_count() == 0);
        KIT_TEST(!(wp < wp3 || wp3 < wp));
    }

    {
        kit::local_shared_ptr<int> sp(new int);
        kit::local_weak_ptr<int> wp(sp);

        kit::local_weak_ptr<int> wp2(wp);
        KIT_TEST(wp2.use_count() == wp.use_count());
        KIT_TEST(wp2.use_count() == 1);
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        sp.reset();
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        kit::local_weak_ptr<int> wp3(wp);
        KIT_TEST(wp3.use_count() == wp.use_count());
        KIT_TEST(wp3.use_count() == 0);
        KIT_TEST(!(wp < wp3 || wp3 < wp));
    }

    {
        kit::local_shared_ptr<void> sp(static_cast<int*>(0));
        kit::local_weak_ptr<void> wp(sp);

        kit::local_weak_ptr<void> wp2(wp);
        KIT_TEST(wp2.use_count() == wp.use_count());
        KIT_TEST(wp2.use_count() == 1);
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        sp.reset();
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        kit::local_weak_ptr<void> wp3(wp);
        KIT_TEST(wp3.use_count() == wp.use_count());
        KIT_TEST(wp3.use_count() == 0);
        KIT_TEST(!(wp < wp3 || wp3 < wp));
    }

    {
        kit::local_shared_ptr<void> sp(new int);
        kit::local_weak_ptr<void> wp(sp);

        kit::local_weak_ptr<void> wp2(wp);
        KIT_TEST(wp2.use_count() == wp.use_count());
        KIT_TEST(wp2.use_count() == 1);
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        sp.reset();
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        kit::local_weak_ptr<void> wp3(wp);
        KIT_TEST(wp3.use_count() == wp.use_count());
        KIT_TEST(wp3.use_count() == 0);
        KIT_TEST(!(wp < wp3 || wp3 < wp));
    }

    {
        kit::local_shared_ptr<incomplete> sp = create_incomplete();
        kit::local_weak_ptr<incomplete> wp(sp);

        kit::local_weak_ptr<incomplete> wp2(wp);
        KIT_TEST(wp2.use_count() == wp.use_count());
        KIT_TEST(wp2.use_count() == 1);
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        sp.reset();
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        kit::local_weak_ptr<incomplete> wp3(wp);
        KIT_TEST(wp3.use_count() == wp.use_count());
        KIT_TEST(wp3.use_count() == 0);
        KIT_TEST(!(wp < wp3 || wp3 < wp));
    }
}

void conversion_constructor()
{
    {
        kit::local_weak_ptr<int> wp;
        kit::local_weak_ptr<void> wp2(wp);
        KIT_TEST(wp2.use_count() == wp.use_count());
        KIT_TEST(wp2.use_count() == 0);
    }

    {
        kit::local_weak_ptr<incomplete> wp;
        kit::local_weak_ptr<void> wp2(wp);
        KIT_TEST(wp2.use_count() == wp.use_count());
        KIT_TEST(wp2.use_count() == 0);
    }

    {
        kit::local_weak_ptr<Z> wp;

        kit::local_weak_ptr<X> wp2(wp);
        KIT_TEST(wp2.use_count() == wp.use_count());
        KIT_TEST(wp2.use_count() == 0);

        kit::local_weak_ptr<Y> wp3(wp);
        KIT_TEST(wp3.use_count() == wp.use_count());
        KIT_TEST(wp3.use_count() == 0);
    }

    {
        kit::local_shared_ptr<int> sp(static_cast<int*>(0));
        kit::local_weak_ptr<int> wp(sp);

        kit::local_weak_ptr<void> wp2(wp);
        KIT_TEST(wp2.use_count() == wp.use_count());
        KIT_TEST(wp2.use_count() == 1);
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        sp.reset();
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        kit::local_weak_ptr<void> wp3(wp);
        KIT_TEST(wp3.use_count() == wp.use_count());
        KIT_TEST(wp3.use_count() == 0);
        KIT_TEST(!(wp < wp3 || wp3 < wp));
    }

    {
        kit::local_shared_ptr<int> sp(new int);
        kit::local_weak_ptr<int> wp(sp);

        kit::local_weak_ptr<void> wp2(wp);
        KIT_TEST(wp2.use_count() == wp.use_count());
        KIT_TEST(wp2.use_count() == 1);
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        sp.reset();
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        kit::local_weak_ptr<void> wp3(wp);
        KIT_TEST(wp3.use_count() == wp.use_count());
        KIT_TEST(wp3.use_count() == 0);
        KIT_TEST(!(wp < wp3 || wp3 < wp));
    }

    {
        kit::local_shared_ptr<incomplete> sp = create_incomplete();
        kit::local_weak_ptr<incomplete> wp(sp);

        kit::local_weak_ptr<void> wp2(wp);
        KIT_TEST(wp2.use_count() == wp.use_count());
        KIT_TEST(wp2.use_count() == 1);
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        sp.reset();
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        kit::local_weak_ptr<void> wp3(wp);
        KIT_TEST(wp3.use_count() == wp.use_count());
        KIT_TEST(wp3.use_count() == 0);
        KIT_TEST(!(wp < wp3 || wp3 < wp));
    }

    {
        kit::local_shared_ptr<Z> sp(static_cast<Z*>(0));
        kit::local_weak_ptr<Z> wp(sp);

        kit::local_weak_ptr<X> wp2(wp);
        KIT_TEST(wp2.use_count() == wp.use_count());
        KIT_TEST(wp2.use_count() == 1);
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        sp.reset();
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        kit::local_weak_ptr<X> wp3(wp);
        KIT_TEST(wp3.use_count() == wp.use_count());
        KIT_TEST(wp3.use_count() == 0);
        KIT_TEST(!(wp < wp3 || wp3 < wp));
    }

    {
        kit::local_shared_ptr<Z> sp(static_cast<Z*>(0));
        kit::local_weak_ptr<Z> wp(sp);

        kit::local_weak_ptr<Y> wp2(wp);
        KIT_TEST(wp2.use_count() == wp.use_count());
        KIT_TEST(wp2.use_count() == 1);
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        sp.reset();
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        kit::local_weak_ptr<Y> wp3(wp);
        KIT_TEST(wp3.use_count() == wp.use_count());
        KIT_TEST(wp3.use_count() == 0);
        KIT_TEST(!(wp < wp3 || wp3 < wp));
    }

    {
        kit::local_shared_ptr<Z> sp(new Z);
        kit::local_weak_ptr<Z> wp(sp);

        kit::local_weak_ptr<X> wp2(wp);
        KIT_TEST(wp2.use_count() == wp.use_count());
        KIT_TEST(wp2.use_count() == 1);
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        sp.reset();
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        kit::local_weak_ptr<X> wp3(wp);
        KIT_TEST(wp3.use_count() == wp.use_count());
        KIT_TEST(wp3.use_count() == 0);
        KIT_TEST(!(wp < wp3 || wp3 < wp));
    }

    {
        kit::local_shared_ptr<Z> sp(new Z);
        kit::local_weak_ptr<Z> wp(sp);

        kit::local_weak_ptr<Y> wp2(wp);
        KIT_TEST(wp2.use_count() == wp.use_count());
        KIT_TEST(wp2.use_count() == 1);
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        sp.reset();
        KIT_TEST(!(wp < wp2 || wp2 < wp));

        kit::local_weak_ptr<Y> wp3(wp);
        KIT_TEST(wp3.use_count() == wp.use_count());
        KIT_TEST(wp3.use_count() == 0);
        KIT_TEST(!(wp < wp3 || wp3 < wp));
    }
}

void test()
{
    default_constructor();
    local_shared_ptr_constructor();
    copy_constructor();
    conversion_constructor();
}

} // namespace n_constructors

namespace n_assignment
{

template<class T> void copy_assignment(kit::local_shared_ptr<T> & sp)
{
    KIT_TEST(sp.unique());

    kit::local_weak_ptr<T> p1;

    p1 = p1;
    KIT_TEST(p1.use_count() == 0);

    kit::local_weak_ptr<T> p2;

    p1 = p2;
    KIT_TEST(p1.use_count() == 0);

    kit::local_weak_ptr<T> p3(p1);

    p1 = p3;
    KIT_TEST(p1.use_count() == 0);

    kit::local_weak_ptr<T> p4(sp);

    p4 = p4;
    KIT_TEST(p4.use_count() == 1);

    p1 = p4;
    KIT_TEST(p1.use_count() == 1);

    p4 = p2;
    KIT_TEST(p4.use_count() == 0);

    sp.reset();

    p1 = p1;
    KIT_TEST(p1.use_count() == 0);

    p4 = p1;
    KIT_TEST(p4.use_count() == 0);
}

void conversion_assignment()
{
    {
        kit::local_weak_ptr<void> p1;

        kit::local_weak_ptr<incomplete> p2;

        p1 = p2;
        KIT_TEST(p1.use_count() == 0);

        kit::local_shared_ptr<incomplete> sp = create_incomplete();
        kit::local_weak_ptr<incomplete> p3(sp);

        p1 = p3;
        KIT_TEST(p1.use_count() == 1);

        sp.reset();

        p1 = p3;
        KIT_TEST(p1.use_count() == 0);

        p1 = p2;
        KIT_TEST(p1.use_count() == 0);
    }

    {
        kit::local_weak_ptr<X> p1;

        kit::local_weak_ptr<Z> p2;

        p1 = p2;
        KIT_TEST(p1.use_count() == 0);

        kit::local_shared_ptr<Z> sp(new Z);
        kit::local_weak_ptr<Z> p3(sp);

        p1 = p3;
        KIT_TEST(p1.use_count() == 1);

        sp.reset();

        p1 = p3;
        KIT_TEST(p1.use_count() == 0);

        p1 = p2;
        KIT_TEST(p1.use_count() == 0);
    }

    {
        kit::local_weak_ptr<Y> p1;

        kit::local_weak_ptr<Z> p2;

        p1 = p2;
        KIT_TEST(p1.use_count() == 0);

        kit::local_shared_ptr<Z> sp(new Z);
        kit::local_weak_ptr<Z> p3(sp);

        p1 = p3;
        KIT_TEST(p1.use_count() == 1);

        sp.reset();

        p1 = p3;
        KIT_TEST(p1.use_count() == 0);

        p1 = p2;
        KIT_TEST(p1.use_count() == 0);
    }
}

template<class T, class U> void local_shared_ptr_assignment(kit::local_shared_ptr<U> & sp, T * = 0)
{
    KIT_TEST(sp.unique());

    kit::local_weak_ptr<T> p1;
    kit::local_weak_ptr<T> p2(p1);
    kit::local_weak_ptr<T> p3(sp);
    kit::local_weak_ptr<T> p4(p3);

    p1 = sp;
    KIT_TEST(p1.use_count() == 1);

    p2 = sp;
    KIT_TEST(p2.use_count() == 1);

    p3 = sp;
    KIT_TEST(p3.use_count() == 1);

    p4 = sp;
    KIT_TEST(p4.use_count() == 1);

    sp.reset();

    KIT_TEST(p1.use_count() == 0);
    KIT_TEST(p2.use_count() == 0);
    KIT_TEST(p3.use_count() == 0);
    KIT_TEST(p4.use_count() == 0);

    p1 = sp;
}

void test()
{
    {
        kit::local_shared_ptr<int> p( new int );
        copy_assignment( p );
    }

    {
        kit::local_shared_ptr<X> p( new X );
        copy_assignment( p );
    }

    {
        kit::local_shared_ptr<void> p( new int );
        copy_assignment( p );
    }

    {
        kit::local_shared_ptr<incomplete> p = create_incomplete();
        copy_assignment( p );
    }

    conversion_assignment();

    {
        kit::local_shared_ptr<int> p( new int );
        local_shared_ptr_assignment<int>( p );
    }

    {
        kit::local_shared_ptr<int> p( new int );
        local_shared_ptr_assignment<void>( p );
    }

    {
        kit::local_shared_ptr<X> p( new X );
        local_shared_ptr_assignment<X>( p );
    }

    {
        kit::local_shared_ptr<X> p( new X );
        local_shared_ptr_assignment<void>( p );
    }

    {
        kit::local_shared_ptr<void> p( new int );
        local_shared_ptr_assignment<void>( p );
    }

    {
        kit::local_shared_ptr<incomplete> p = create_incomplete();
        local_shared_ptr_assignment<incomplete>( p );
    }

    {
        kit::local_shared_ptr<incomplete> p = create_incomplete();
        local_shared_ptr_assignment<void>( p );
    }
}

} // namespace n_assignment

namespace n_reset
{

template<class T, class U> void test2( kit::local_shared_ptr<U> & sp, T * = 0 )
{
    KIT_TEST(sp.unique());

    kit::local_weak_ptr<T> p1;
    kit::local_weak_ptr<T> p2(p1);
    kit::local_weak_ptr<T> p3(sp);
    kit::local_weak_ptr<T> p4(p3);
    kit::local_weak_ptr<T> p5(sp);
    kit::local_weak_ptr<T> p6(p5);

    p1.reset();
    KIT_TEST(p1.use_count() == 0);

    p2.reset();
    KIT_TEST(p2.use_count() == 0);

    p3.reset();
    KIT_TEST(p3.use_count() == 0);

    p4.reset();
    KIT_TEST(p4.use_count() == 0);

    sp.reset();

    p5.reset();
    KIT_TEST(p5.use_count() == 0);

    p6.reset();
    KIT_TEST(p6.use_count() == 0);
}

void test()
{
    {
        kit::local_shared_ptr<int> p( new int );
        test2<int>( p );
    }

    {
        kit::local_shared_ptr<int> p( new int );
        test2<void>( p );
    }

    {
        kit::local_shared_ptr<X> p( new X );
        test2<X>( p );
    }

    {
        kit::local_shared_ptr<X> p( new X );
        test2<void>( p );
    }

    {
        kit::local_shared_ptr<void> p( new int );
        test2<void>( p );
    }

    {
        kit::local_shared_ptr<incomplete> p = create_incomplete();
        test2<incomplete>( p );
    }

    {
        kit::local_shared_ptr<incomplete> p = create_incomplete();
        test2<void>( p );
    }
}

} // namespace n_reset

namespace n_use_count
{

void test()
{
    {
        kit::local_weak_ptr<X> wp;
        KIT_TEST(wp.use_count() == 0);
        KIT_TEST(wp.expired());

        kit::local_weak_ptr<X> wp2;
        KIT_TEST(wp.use_count() == 0);
        KIT_TEST(wp.expired());

        kit::local_weak_ptr<X> wp3(wp);
        KIT_TEST(wp.use_count() == 0);
        KIT_TEST(wp.expired());
        KIT_TEST(wp3.use_count() == 0);
        KIT_TEST(wp3.expired());
    }

    {
        kit::local_shared_ptr<X> sp(static_cast<X*>(0));

        kit::local_weak_ptr<X> wp(sp);
        KIT_TEST(wp.use_count() == 1);
        KIT_TEST(!wp.expired());

        kit::local_weak_ptr<X> wp2(sp);
        KIT_TEST(wp.use_count() == 1);
        KIT_TEST(!wp.expired());

        kit::local_weak_ptr<X> wp3(wp);
        KIT_TEST(wp.use_count() == 1);
        KIT_TEST(!wp.expired());
        KIT_TEST(wp3.use_count() == 1);
        KIT_TEST(!wp3.expired());

        kit::local_shared_ptr<X> sp2(sp);

        KIT_TEST(wp.use_count() == 2);
        KIT_TEST(!wp.expired());
        KIT_TEST(wp2.use_count() == 2);
        KIT_TEST(!wp2.expired());
        KIT_TEST(wp3.use_count() == 2);
        KIT_TEST(!wp3.expired());

        kit::local_shared_ptr<void> sp3(sp);

        KIT_TEST(wp.use_count() == 3);
        KIT_TEST(!wp.expired());
        KIT_TEST(wp2.use_count() == 3);
        KIT_TEST(!wp2.expired());
        KIT_TEST(wp3.use_count() == 3);
        KIT_TEST(!wp3.expired());

        sp.reset();

        KIT_TEST(wp.use_count() == 2);
        KIT_TEST(!wp.expired());
        KIT_TEST(wp2.use_count() == 2);
        KIT_TEST(!wp2.expired());
        KIT_TEST(wp3.use_count() == 2);
        KIT_TEST(!wp3.expired());

        sp2.reset();

        KIT_TEST(wp.use_count() == 1);
        KIT_TEST(!wp.expired());
        KIT_TEST(wp2.use_count() == 1);
        KIT_TEST(!wp2.expired());
        KIT_TEST(wp3.use_count() == 1);
        KIT_TEST(!wp3.expired());

        sp3.reset();

        KIT_TEST(wp.use_count() == 0);
        KIT_TEST(wp.expired());
        KIT_TEST(wp2.use_count() == 0);
        KIT_TEST(wp2.expired());
        KIT_TEST(wp3.use_count() == 0);
        KIT_TEST(wp3.expired());
    }
}

} // namespace n_use_count

namespace n_swap
{

void test()
{
    {
        kit::local_weak_ptr<X> wp;
        kit::local_weak_ptr<X> wp2;

        wp.swap(wp2);

        KIT_TEST(wp.use_count() == 0);
        KIT_TEST(wp2.use_count() == 0);

        using std::swap;
        swap(wp, wp2);

        KIT_TEST(wp.use_count() == 0);
        KIT_TEST(wp2.use_count() == 0);
    }

    {
        kit::local_shared_ptr<X> sp(new X);
        kit::local_weak_ptr<X> wp;
        kit::local_weak_ptr<X> wp2(sp);
        kit::local_weak_ptr<X> wp3(sp);

        wp.swap(wp2);

        KIT_TEST(wp.use_count() == 1);
        KIT_TEST(wp2.use_count() == 0);
        KIT_TEST(!(wp < wp3 || wp3 < wp));

        using std::swap;
        swap(wp, wp2);

        KIT_TEST(wp.use_count() == 0);
        KIT_TEST(wp2.use_count() == 1);
        KIT_TEST(!(wp2 < wp3 || wp3 < wp2));

        sp.reset();

        wp.swap(wp2);

        KIT_TEST(wp.use_count() == 0);
        KIT_TEST(wp2.use_count() == 0);
        KIT_TEST(!(wp < wp3 || wp3 < wp));

        swap(wp, wp2);

        KIT_TEST(wp.use_count() == 0);
        KIT_TEST(wp2.use_count() == 0);
        KIT_TEST(!(wp2 < wp3 || wp3 < wp2));
    }

    {
        kit::local_shared_ptr<X> sp(new X);
        kit::local_shared_ptr<X> sp2(new X);
        kit::local_weak_ptr<X> wp(sp);
        kit::local_weak_ptr<X> wp2(sp2);
        kit::local_weak_ptr<X> wp3(sp2);

        wp.swap(wp2);

        KIT_TEST(wp.use_count() == 1);
        KIT_TEST(wp2.use_count() == 1);
        KIT_TEST(!(wp < wp3 || wp3 < wp));

        using std::swap;
        swap(wp, wp2);

        KIT_TEST(wp.use_count() == 1);
        KIT_TEST(wp2.use_count() == 1);
        KIT_TEST(!(wp2 < wp3 || wp3 < wp2));

        sp.reset();

        wp.swap(wp2);

        KIT_TEST(wp.use_count() == 1);
        KIT_TEST(wp2.use_count() == 0);
        KIT_TEST(!(wp < wp3 || wp3 < wp));

        swap(wp, wp2);

        KIT_TEST(wp.use_count() == 0);
        KIT_TEST(wp2.use_count() == 1);
        KIT_TEST(!(wp2 < wp3 || wp3 < wp2));

        sp2.reset();

        wp.swap(wp2);

        KIT_TEST(wp.use_count() == 0);
        KIT_TEST(wp2.use_count() == 0);
        KIT_TEST(!(wp < wp3 || wp3 < wp));

        swap(wp, wp2);

        KIT_TEST(wp.use_count() == 0);
        KIT_TEST(wp2.use_count() == 0);
        KIT_TEST(!(wp2 < wp3 || wp3 < wp2));
    }
}

} // namespace n_swap

namespace n_comparison
{

void test()
{
    {
        kit::local_weak_ptr<X> wp;
        KIT_TEST(!(wp < wp));

        kit::local_weak_ptr<X> wp2;
        KIT_TEST(!(wp < wp2 && wp2 < wp));

        kit::local_weak_ptr<X> wp3(wp);
        KIT_TEST(!(wp3 < wp3));
        KIT_TEST(!(wp < wp3 && wp3 < wp));
    }

    {
        kit::local_shared_ptr<X> sp(new X);

        kit::local_weak_ptr<X> wp(sp);
        KIT_TEST(!(wp < wp));

        kit::local_weak_ptr<X> wp2;
        KIT_TEST(wp < wp2 || wp2 < wp);
        KIT_TEST(!(wp < wp2 && wp2 < wp));

        bool b1 = wp < wp2;
        bool b2 = wp2 < wp;

        {
            kit::local_weak_ptr<X> wp3(wp);

            KIT_TEST(!(wp < wp3 || wp3 < wp));
            KIT_TEST(!(wp < wp3 && wp3 < wp));

            KIT_TEST(wp2 < wp3 || wp3 < wp2);
            KIT_TEST(!(wp2 < wp3 && wp3 < wp2));

            kit::local_weak_ptr<X> wp4(wp2);

            KIT_TEST(wp4 < wp3 || wp3 < wp4);
            KIT_TEST(!(wp4 < wp3 && wp3 < wp4));
        }

        sp.reset();

        KIT_TEST(b1 == (wp < wp2));
        KIT_TEST(b2 == (wp2 < wp));

        {
            kit::local_weak_ptr<X> wp3(wp);

            KIT_TEST(!(wp < wp3 || wp3 < wp));
            KIT_TEST(!(wp < wp3 && wp3 < wp));

            KIT_TEST(wp2 < wp3 || wp3 < wp2);
            KIT_TEST(!(wp2 < wp3 && wp3 < wp2));

            kit::local_weak_ptr<X> wp4(wp2);

            KIT_TEST(wp4 < wp3 || wp3 < wp4);
            KIT_TEST(!(wp4 < wp3 && wp3 < wp4));
        }
    }

    {
        kit::local_shared_ptr<X> sp(new X);
        kit::local_shared_ptr<X> sp2(new X);

        kit::local_weak_ptr<X> wp(sp);
        kit::local_weak_ptr<X> wp2(sp2);

        KIT_TEST(wp < wp2 || wp2 < wp);
        KIT_TEST(!(wp < wp2 && wp2 < wp));

        bool b1 = wp < wp2;
        bool b2 = wp2 < wp;

        {
            kit::local_weak_ptr<X> wp3(wp);

            KIT_TEST(!(wp < wp3 || wp3 < wp));
            KIT_TEST(!(wp < wp3 && wp3 < wp));

            KIT_TEST(wp2 < wp3 || wp3 < wp2);
            KIT_TEST(!(wp2 < wp3 && wp3 < wp2));

            kit::local_weak_ptr<X> wp4(wp2);

            KIT_TEST(wp4 < wp3 || wp3 < wp4);
            KIT_TEST(!(wp4 < wp3 && wp3 < wp4));
        }

        sp.reset();

        KIT_TEST(b1 == (wp < wp2));
        KIT_TEST(b2 == (wp2 < wp));

        {
            kit::local_weak_ptr<X> wp3(wp);

            KIT_TEST(!(wp < wp3 || wp3 < wp));
            KIT_TEST(!(wp < wp3 && wp3 < wp));

            KIT_TEST(wp2 < wp3 || wp3 < wp2);
            KIT_TEST(!(wp2 < wp3 && wp3 < wp2));

            kit::local_weak_ptr<X> wp4(wp2);

            KIT_TEST(wp4 < wp3 || wp3 < wp4);
            KIT_TEST(!(wp4 < wp3 && wp3 < wp4));
        }

        sp2.reset();

        KIT_TEST(b1 == (wp < wp2));
        KIT_TEST(b2 == (wp2 < wp));

        {
            kit::local_weak_ptr<X> wp3(wp);

            KIT_TEST(!(wp < wp3 || wp3 < wp));
            KIT_TEST(!(wp < wp3 && wp3 < wp));

            KIT_TEST(wp2 < wp3 || wp3 < wp2);
            KIT_TEST(!(wp2 < wp3 && wp3 < wp2));

            kit::local_weak_ptr<X> wp4(wp2);

            KIT_TEST(wp4 < wp3 || wp3 < wp4);
            KIT_TEST(!(wp4 < wp3 && wp3 < wp4));
        }
    }

    {
        kit::local_shared_ptr<X> sp(new X);
        kit::local_shared_ptr<X> sp2(sp);

        kit::local_weak_ptr<X> wp(sp);
        kit::local_weak_ptr<X> wp2(sp2);

        KIT_TEST(!(wp < wp2 || wp2 < wp));
        KIT_TEST(!(wp < wp2 && wp2 < wp));

        bool b1 = wp < wp2;
        bool b2 = wp2 < wp;

        {
            kit::local_weak_ptr<X> wp3(wp);

            KIT_TEST(!(wp < wp3 || wp3 < wp));
            KIT_TEST(!(wp < wp3 && wp3 < wp));

            KIT_TEST(!(wp2 < wp3 || wp3 < wp2));
            KIT_TEST(!(wp2 < wp3 && wp3 < wp2));

            kit::local_weak_ptr<X> wp4(wp2);

            KIT_TEST(!(wp4 < wp3 || wp3 < wp4));
            KIT_TEST(!(wp4 < wp3 && wp3 < wp4));
        }

        sp.reset();
        sp2.reset();

        KIT_TEST(b1 == (wp < wp2));
        KIT_TEST(b2 == (wp2 < wp));

        {
            kit::local_weak_ptr<X> wp3(wp);

            KIT_TEST(!(wp < wp3 || wp3 < wp));
            KIT_TEST(!(wp < wp3 && wp3 < wp));

            KIT_TEST(!(wp2 < wp3 || wp3 < wp2));
            KIT_TEST(!(wp2 < wp3 && wp3 < wp2));

            kit::local_weak_ptr<X> wp4(wp2);

            KIT_TEST(!(wp4 < wp3 || wp3 < wp4));
            KIT_TEST(!(wp4 < wp3 && wp3 < wp4));
        }
    }

    {
        kit::local_shared_ptr<X> spx(new X);
        kit::local_shared_ptr<Y> spy(new Y);
        kit::local_shared_ptr<Z> spz(new Z);

        kit::local_weak_ptr<X> px(spx);
        kit::local_weak_ptr<Y> py(spy);
        kit::local_weak_ptr<Z> pz(spz);

        KIT_TEST(px < py || py < px);
        KIT_TEST(px < pz || pz < px);
        KIT_TEST(py < pz || pz < py);

        KIT_TEST(!(px < py && py < px));
        KIT_TEST(!(px < pz && pz < px));
        KIT_TEST(!(py < pz && pz < py));

        kit::local_weak_ptr<void> pvx(px);
        KIT_TEST(!(pvx < pvx));

        kit::local_weak_ptr<void> pvy(py);
        KIT_TEST(!(pvy < pvy));

        kit::local_weak_ptr<void> pvz(pz);
        KIT_TEST(!(pvz < pvz));

        KIT_TEST(pvx < pvy || pvy < pvx);
        KIT_TEST(pvx < pvz || pvz < pvx);
        KIT_TEST(pvy < pvz || pvz < pvy);

        KIT_TEST(!(pvx < pvy && pvy < pvx));
        KIT_TEST(!(pvx < pvz && pvz < pvx));
        KIT_TEST(!(pvy < pvz && pvz < pvy));

        spx.reset();
        spy.reset();
        spz.reset();

        KIT_TEST(px < py || py < px);
        KIT_TEST(px < pz || pz < px);
        KIT_TEST(py < pz || pz < py);

        KIT_TEST(!(px < py && py < px));
        KIT_TEST(!(px < pz && pz < px));
        KIT_TEST(!(py < pz && pz < py));

        KIT_TEST(!(pvx < pvx));
        KIT_TEST(!(pvy < pvy));
        KIT_TEST(!(pvz < pvz));

        KIT_TEST(pvx < pvy || pvy < pvx);
        KIT_TEST(pvx < pvz || pvz < pvx);
        KIT_TEST(pvy < pvz || pvz < pvy);

        KIT_TEST(!(pvx < pvy && pvy < pvx));
        KIT_TEST(!(pvx < pvz && pvz < pvx));
        KIT_TEST(!(pvy < pvz && pvz < pvy));
    }

    {
        kit::local_shared_ptr<Z> spz(new Z);
        kit::local_shared_ptr<X> spx(spz);

        kit::local_weak_ptr<Z> pz(spz);
        kit::local_weak_ptr<X> px(spx);
        kit::local_weak_ptr<Y> py(spz);

        KIT_TEST(!(px < px));
        KIT_TEST(!(py < py));

        KIT_TEST(!(px < py || py < px));
        KIT_TEST(!(px < pz || pz < px));
        KIT_TEST(!(py < pz || pz < py));

        kit::local_weak_ptr<void> pvx(px);
        kit::local_weak_ptr<void> pvy(py);
        kit::local_weak_ptr<void> pvz(pz);

        KIT_TEST(!(pvx < pvy || pvy < pvx));
        KIT_TEST(!(pvx < pvz || pvz < pvx));
        KIT_TEST(!(pvy < pvz || pvz < pvy));

        spx.reset();
        spz.reset();

        KIT_TEST(!(px < px));
        KIT_TEST(!(py < py));

        KIT_TEST(!(px < py || py < px));
        KIT_TEST(!(px < pz || pz < px));
        KIT_TEST(!(py < pz || pz < py));

        KIT_TEST(!(pvx < pvy || pvy < pvx));
        KIT_TEST(!(pvx < pvz || pvz < pvx));
        KIT_TEST(!(pvy < pvz || pvz < pvy));
    }
}

} // namespace n_comparison

namespace n_lock
{

void test()
{
}

} // namespace n_lock

namespace n_map
{

void test()
{
    std::vector< kit::local_shared_ptr<int> > vi;

    {
        kit::local_shared_ptr<int> pi1(new int);
        kit::local_shared_ptr<int> pi2(new int);
        kit::local_shared_ptr<int> pi3(new int);

        vi.push_back(pi1);
        vi.push_back(pi1);
        vi.push_back(pi1);
        vi.push_back(pi2);
        vi.push_back(pi1);
        vi.push_back(pi2);
        vi.push_back(pi1);
        vi.push_back(pi3);
        vi.push_back(pi3);
        vi.push_back(pi2);
        vi.push_back(pi1);
    }

    std::vector< kit::local_shared_ptr<X> > vx;

    {
        kit::local_shared_ptr<X> px1(new X);
        kit::local_shared_ptr<X> px2(new X);
        kit::local_shared_ptr<X> px3(new X);

        vx.push_back(px2);
        vx.push_back(px2);
        vx.push_back(px1);
        vx.push_back(px2);
        vx.push_back(px1);
        vx.push_back(px1);
        vx.push_back(px1);
        vx.push_back(px2);
        vx.push_back(px1);
        vx.push_back(px3);
        vx.push_back(px2);
    }

    std::map< kit::local_weak_ptr<void>, long > m;

    {
        for(std::vector< kit::local_shared_ptr<int> >::iterator i = vi.begin(); i != vi.end(); ++i)
        {
            ++m[*i];
        }
    }

    {
        for(std::vector< kit::local_shared_ptr<X> >::iterator i = vx.begin(); i != vx.end(); ++i)
        {
            ++m[*i];
        }
    }

    {
        for(std::map< kit::local_weak_ptr<void>, long >::iterator i = m.begin(); i != m.end(); ++i)
        {
            KIT_TEST(i->first.use_count() == i->second);
        }
    }
}

} // namespace n_map

int main()
{
    n_element_type::test();
    n_constructors::test();
    n_assignment::test();
    n_reset::test();
    n_use_count::test();
    n_swap::test();
    n_comparison::test();
    n_lock::test();

    n_map::test();

//    return kit::report_errors();
  return 0;
}

class incomplete
{
};

kit::local_shared_ptr<incomplete> create_incomplete()
{
    kit::local_shared_ptr<incomplete> px(new incomplete);
    return px;
}

}
