//
//  local_shared_ptr_test.cpp
//
//  Copyright (c) 2002, 2003 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "lightweight_test.hpp"
#include <kit/local_shared_ptr.hpp>
#include <kit/local_weak_ptr.hpp>
#include <map>
#include <vector>

//

namespace local_shared_ptr_test
{

namespace n_element_type
{

void f(int &)
{
}

void test()
{
    typedef kit::local_shared_ptr<int>::element_type T;
    T t;
    f(t);
}

} // namespace n_element_type

namespace n_constructors
{

class incomplete;

void default_constructor()
{
    {
        kit::local_shared_ptr<int> pi;
        KIT_TEST(pi? false: true);
        KIT_TEST(!pi);
        KIT_TEST(pi.get() == 0);
        KIT_TEST(pi.use_count() == 0);
    }

    {
        kit::local_shared_ptr<void> pv;
        KIT_TEST(pv? false: true);
        KIT_TEST(!pv);
        KIT_TEST(pv.get() == 0);
        KIT_TEST(pv.use_count() == 0);
    }

    {
        kit::local_shared_ptr<incomplete> px;
        KIT_TEST(px? false: true);
        KIT_TEST(!px);
        KIT_TEST(px.get() == 0);
        KIT_TEST(px.use_count() == 0);
    }
}

struct A
{
    int dummy;
};

struct X
{
    static long instances;

    X()
    {
        ++instances;
    }

    ~X()
    {
        --instances;
    }

private:

    X(X const &);
    X & operator= (X const &);
};

long X::instances = 0;

// virtual inheritance stresses the implementation

struct Y: public A, public virtual X
{
    static long instances;

    Y()
    {
        ++instances;
    }

    ~Y()
    {
        --instances;
    }

private:

    Y(Y const &);
    Y & operator= (Y const &);
};

long Y::instances = 0;

template<class T> void pc0_test(T * p)
{
    KIT_TEST(p == 0);
    kit::local_shared_ptr<T> pt(p);
    KIT_TEST(pt? false: true);
    KIT_TEST(!pt);
    KIT_TEST(pt.get() == 0);
    KIT_TEST(pt.use_count() == 1);
    KIT_TEST(pt.unique());
}

void pointer_constructor()
{
    pc0_test(static_cast<int*>(0));

#if !defined(KIT_MSVC) || (KIT_MSVC > 1300)

    pc0_test(static_cast<int const*>(0));
    pc0_test(static_cast<int volatile*>(0));
    pc0_test(static_cast<int const volatile*>(0));

#endif

    {
        kit::local_shared_ptr<int const> pi(static_cast<int*>(0));
        KIT_TEST(pi? false: true);
        KIT_TEST(!pi);
        KIT_TEST(pi.get() == 0);
        KIT_TEST(pi.use_count() == 1);
        KIT_TEST(pi.unique());
    }

    {
        kit::local_shared_ptr<int volatile> pi(static_cast<int*>(0));
        KIT_TEST(pi? false: true);
        KIT_TEST(!pi);
        KIT_TEST(pi.get() == 0);
        KIT_TEST(pi.use_count() == 1);
        KIT_TEST(pi.unique());
    }

    {
        kit::local_shared_ptr<void> pv(static_cast<int*>(0));
        KIT_TEST(pv? false: true);
        KIT_TEST(!pv);
        KIT_TEST(pv.get() == 0);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());
    }

    {
        kit::local_shared_ptr<void const> pv(static_cast<int*>(0));
        KIT_TEST(pv? false: true);
        KIT_TEST(!pv);
        KIT_TEST(pv.get() == 0);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());
    }

    pc0_test(static_cast<X*>(0));
    pc0_test(static_cast<X const*>(0));
    pc0_test(static_cast<X volatile*>(0));
    pc0_test(static_cast<X const volatile*>(0));

    {
        kit::local_shared_ptr<X const> px(static_cast<X*>(0));
        KIT_TEST(px? false: true);
        KIT_TEST(!px);
        KIT_TEST(px.get() == 0);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());
    }

    {
        kit::local_shared_ptr<X> px(static_cast<Y*>(0));
        KIT_TEST(px? false: true);
        KIT_TEST(!px);
        KIT_TEST(px.get() == 0);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());
    }

    {
        kit::local_shared_ptr<X const> px(static_cast<Y*>(0));
        KIT_TEST(px? false: true);
        KIT_TEST(!px);
        KIT_TEST(px.get() == 0);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());
    }

    {
        kit::local_shared_ptr<void> pv(static_cast<X*>(0));
        KIT_TEST(pv? false: true);
        KIT_TEST(!pv);
        KIT_TEST(pv.get() == 0);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());
    }

    {
        kit::local_shared_ptr<void const> pv(static_cast<X*>(0));
        KIT_TEST(pv? false: true);
        KIT_TEST(!pv);
        KIT_TEST(pv.get() == 0);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());
    }

    {
        int * p = new int(7);
        kit::local_shared_ptr<int> pi(p);
        KIT_TEST(pi? true: false);
        KIT_TEST(!!pi);
        KIT_TEST(pi.get() == p);
        KIT_TEST(pi.use_count() == 1);
        KIT_TEST(pi.unique());
        KIT_TEST(*pi == 7);
    }

    {
        int * p = new int(7);
        kit::local_shared_ptr<int const> pi(p);
        KIT_TEST(pi? true: false);
        KIT_TEST(!!pi);
        KIT_TEST(pi.get() == p);
        KIT_TEST(pi.use_count() == 1);
        KIT_TEST(pi.unique());
        KIT_TEST(*pi == 7);
    }

    {
        int * p = new int(7);
        kit::local_shared_ptr<void> pv(p);
        KIT_TEST(pv? true: false);
        KIT_TEST(!!pv);
        KIT_TEST(pv.get() == p);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());
    }

    {
        int * p = new int(7);
        kit::local_shared_ptr<void const> pv(p);
        KIT_TEST(pv? true: false);
        KIT_TEST(!!pv);
        KIT_TEST(pv.get() == p);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());
    }

    KIT_TEST(X::instances == 0);

    {
        X * p = new X;
        kit::local_shared_ptr<X> px(p);
        KIT_TEST(px? true: false);
        KIT_TEST(!!px);
        KIT_TEST(px.get() == p);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());
        KIT_TEST(X::instances == 1);
    }

    KIT_TEST(X::instances == 0);

    {
        X * p = new X;
        kit::local_shared_ptr<X const> px(p);
        KIT_TEST(px? true: false);
        KIT_TEST(!!px);
        KIT_TEST(px.get() == p);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());
        KIT_TEST(X::instances == 1);
    }

    KIT_TEST(X::instances == 0);

    {
        X * p = new X;
        kit::local_shared_ptr<void> pv(p);
        KIT_TEST(pv? true: false);
        KIT_TEST(!!pv);
        KIT_TEST(pv.get() == p);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());
        KIT_TEST(X::instances == 1);
    }

    KIT_TEST(X::instances == 0);

    {
        X * p = new X;
        kit::local_shared_ptr<void const> pv(p);
        KIT_TEST(pv? true: false);
        KIT_TEST(!!pv);
        KIT_TEST(pv.get() == p);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());
        KIT_TEST(X::instances == 1);
    }

    KIT_TEST(X::instances == 0);
    KIT_TEST(Y::instances == 0);

    {
        Y * p = new Y;
        kit::local_shared_ptr<X> px(p);
        KIT_TEST(px? true: false);
        KIT_TEST(!!px);
        KIT_TEST(px.get() == p);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());
        KIT_TEST(X::instances == 1);
        KIT_TEST(Y::instances == 1);
    }

    KIT_TEST(X::instances == 0);
    KIT_TEST(Y::instances == 0);

    {
        Y * p = new Y;
        kit::local_shared_ptr<X const> px(p);
        KIT_TEST(px? true: false);
        KIT_TEST(!!px);
        KIT_TEST(px.get() == p);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());
        KIT_TEST(X::instances == 1);
        KIT_TEST(Y::instances == 1);
    }

    KIT_TEST(X::instances == 0);
    KIT_TEST(Y::instances == 0);
}

int m = 0;

void deleter(int * p)
{
    KIT_TEST(p == 0);
}

void deleter2(int * p)
{
    KIT_TEST(p == &m);
    ++*p;
}

struct deleter3
{
    void operator()(incomplete * p)
    {
        KIT_TEST(p == 0);
    }
};

// Borland C++ 5.5.1 fails on static_cast<incomplete*>(0)

incomplete * p0 = 0;

void deleter_constructor()
{
    {
        kit::local_shared_ptr<int> pi(static_cast<int*>(0), deleter);
        KIT_TEST(pi? false: true);
        KIT_TEST(!pi);
        KIT_TEST(pi.get() == 0);
        KIT_TEST(pi.use_count() == 1);
        KIT_TEST(pi.unique());
    }

    {
        kit::local_shared_ptr<void> pv(static_cast<int*>(0), &deleter);
        KIT_TEST(pv? false: true);
        KIT_TEST(!pv);
        KIT_TEST(pv.get() == 0);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());
    }

    {
        kit::local_shared_ptr<void const> pv(static_cast<int*>(0), deleter);
        KIT_TEST(pv? false: true);
        KIT_TEST(!pv);
        KIT_TEST(pv.get() == 0);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());
    }

    {
        kit::local_shared_ptr<incomplete> px(p0, deleter3());
        KIT_TEST(px? false: true);
        KIT_TEST(!px);
        KIT_TEST(px.get() == 0);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());
    }

    {
        kit::local_shared_ptr<void> pv(p0, deleter3());
        KIT_TEST(pv? false: true);
        KIT_TEST(!pv);
        KIT_TEST(pv.get() == 0);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());
    }

    {
        kit::local_shared_ptr<void const> pv(p0, deleter3());
        KIT_TEST(pv? false: true);
        KIT_TEST(!pv);
        KIT_TEST(pv.get() == 0);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());
    }

    KIT_TEST(m == 0);

    {
        kit::local_shared_ptr<int> pi(&m, deleter2);
        KIT_TEST(pi? true: false);
        KIT_TEST(!!pi);
        KIT_TEST(pi.get() == &m);
        KIT_TEST(pi.use_count() == 1);
        KIT_TEST(pi.unique());
    }

    KIT_TEST(m == 1);

    {
        kit::local_shared_ptr<int const> pi(&m, &deleter2);
        KIT_TEST(pi? true: false);
        KIT_TEST(!!pi);
        KIT_TEST(pi.get() == &m);
        KIT_TEST(pi.use_count() == 1);
        KIT_TEST(pi.unique());
    }

    KIT_TEST(m == 2);

    {
        kit::local_shared_ptr<void> pv(&m, deleter2);
        KIT_TEST(pv? true: false);
        KIT_TEST(!!pv);
        KIT_TEST(pv.get() == &m);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());
    }

    KIT_TEST(m == 3);

    {
        kit::local_shared_ptr<void const> pv(&m, &deleter2);
        KIT_TEST(pv? true: false);
        KIT_TEST(!!pv);
        KIT_TEST(pv.get() == &m);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());
    }

    KIT_TEST(m == 4);
}

void copy_constructor()
{
    {
        kit::local_shared_ptr<int> pi;

        kit::local_shared_ptr<int> pi2(pi);
        KIT_TEST(pi2 == pi);
        KIT_TEST(pi2? false: true);
        KIT_TEST(!pi2);
        KIT_TEST(pi2.get() == 0);
        KIT_TEST(pi2.use_count() == pi.use_count());

        kit::local_shared_ptr<void> pi3(pi);
        KIT_TEST(pi3 == pi);
        KIT_TEST(pi3? false: true);
        KIT_TEST(!pi3);
        KIT_TEST(pi3.get() == 0);
        KIT_TEST(pi3.use_count() == pi.use_count());

        kit::local_shared_ptr<void> pi4(pi3);
        KIT_TEST(pi4 == pi3);
        KIT_TEST(pi4? false: true);
        KIT_TEST(!pi4);
        KIT_TEST(pi4.get() == 0);
        KIT_TEST(pi4.use_count() == pi3.use_count());
    }

    {
        kit::local_shared_ptr<void> pv;

        kit::local_shared_ptr<void> pv2(pv);
        KIT_TEST(pv2 == pv);
        KIT_TEST(pv2? false: true);
        KIT_TEST(!pv2);
        KIT_TEST(pv2.get() == 0);
        KIT_TEST(pv2.use_count() == pv.use_count());
    }

    {
        kit::local_shared_ptr<incomplete> px;

        kit::local_shared_ptr<incomplete> px2(px);
        KIT_TEST(px2 == px);
        KIT_TEST(px2? false: true);
        KIT_TEST(!px2);
        KIT_TEST(px2.get() == 0);
        KIT_TEST(px2.use_count() == px.use_count());

        kit::local_shared_ptr<void> px3(px);
        KIT_TEST(px3 == px);
        KIT_TEST(px3? false: true);
        KIT_TEST(!px3);
        KIT_TEST(px3.get() == 0);
        KIT_TEST(px3.use_count() == px.use_count());
    }

    {
        kit::local_shared_ptr<int> pi(static_cast<int*>(0));

        kit::local_shared_ptr<int> pi2(pi);
        KIT_TEST(pi2 == pi);
        KIT_TEST(pi2? false: true);
        KIT_TEST(!pi2);
        KIT_TEST(pi2.get() == 0);
        KIT_TEST(pi2.use_count() == 2);
        KIT_TEST(!pi2.unique());
        KIT_TEST(pi2.use_count() == pi.use_count());
        KIT_TEST(!(pi < pi2 || pi2 < pi)); // shared ownership test

        kit::local_shared_ptr<void> pi3(pi);
        KIT_TEST(pi3 == pi);
        KIT_TEST(pi3? false: true);
        KIT_TEST(!pi3);
        KIT_TEST(pi3.get() == 0);
        KIT_TEST(pi3.use_count() == 3);
        KIT_TEST(!pi3.unique());
        KIT_TEST(pi3.use_count() == pi.use_count());
        KIT_TEST(!(pi < pi3 || pi3 < pi)); // shared ownership test

        kit::local_shared_ptr<void> pi4(pi2);
        KIT_TEST(pi4 == pi2);
        KIT_TEST(pi4? false: true);
        KIT_TEST(!pi4);
        KIT_TEST(pi4.get() == 0);
        KIT_TEST(pi4.use_count() == 4);
        KIT_TEST(!pi4.unique());
        KIT_TEST(pi4.use_count() == pi2.use_count());
        KIT_TEST(!(pi2 < pi4 || pi4 < pi2)); // shared ownership test

        KIT_TEST(pi3.use_count() == pi4.use_count());
        KIT_TEST(!(pi3 < pi4 || pi4 < pi3)); // shared ownership test
    }

    {
        kit::local_shared_ptr<X> px(static_cast<X*>(0));

        kit::local_shared_ptr<X> px2(px);
        KIT_TEST(px2 == px);
        KIT_TEST(px2? false: true);
        KIT_TEST(!px2);
        KIT_TEST(px2.get() == 0);
        KIT_TEST(px2.use_count() == 2);
        KIT_TEST(!px2.unique());
        KIT_TEST(px2.use_count() == px.use_count());
        KIT_TEST(!(px < px2 || px2 < px)); // shared ownership test

        kit::local_shared_ptr<void> px3(px);
        KIT_TEST(px3 == px);
        KIT_TEST(px3? false: true);
        KIT_TEST(!px3);
        KIT_TEST(px3.get() == 0);
        KIT_TEST(px3.use_count() == 3);
        KIT_TEST(!px3.unique());
        KIT_TEST(px3.use_count() == px.use_count());
        KIT_TEST(!(px < px3 || px3 < px)); // shared ownership test

        kit::local_shared_ptr<void> px4(px2);
        KIT_TEST(px4 == px2);
        KIT_TEST(px4? false: true);
        KIT_TEST(!px4);
        KIT_TEST(px4.get() == 0);
        KIT_TEST(px4.use_count() == 4);
        KIT_TEST(!px4.unique());
        KIT_TEST(px4.use_count() == px2.use_count());
        KIT_TEST(!(px2 < px4 || px4 < px2)); // shared ownership test

        KIT_TEST(px3.use_count() == px4.use_count());
        KIT_TEST(!(px3 < px4 || px4 < px3)); // shared ownership test
    }

    {
        int * p = new int(7);
        kit::local_shared_ptr<int> pi(p);

        kit::local_shared_ptr<int> pi2(pi);
        KIT_TEST(pi2 == pi);
        KIT_TEST(pi2? true: false);
        KIT_TEST(!!pi2);
        KIT_TEST(pi2.get() == p);
        KIT_TEST(pi2.use_count() == 2);
        KIT_TEST(!pi2.unique());
        KIT_TEST(*pi2 == 7);
        KIT_TEST(pi2.use_count() == pi.use_count());
        KIT_TEST(!(pi < pi2 || pi2 < pi)); // shared ownership test
    }

    {
        int * p = new int(7);
        kit::local_shared_ptr<void> pv(p);
        KIT_TEST(pv.get() == p);

        kit::local_shared_ptr<void> pv2(pv);
        KIT_TEST(pv2 == pv);
        KIT_TEST(pv2? true: false);
        KIT_TEST(!!pv2);
        KIT_TEST(pv2.get() == p);
        KIT_TEST(pv2.use_count() == 2);
        KIT_TEST(!pv2.unique());
        KIT_TEST(pv2.use_count() == pv.use_count());
        KIT_TEST(!(pv < pv2 || pv2 < pv)); // shared ownership test
    }

    KIT_TEST(X::instances == 0);

    {
        X * p = new X;
        kit::local_shared_ptr<X> px(p);
        KIT_TEST(px.get() == p);

        kit::local_shared_ptr<X> px2(px);
        KIT_TEST(px2 == px);
        KIT_TEST(px2? true: false);
        KIT_TEST(!!px2);
        KIT_TEST(px2.get() == p);
        KIT_TEST(px2.use_count() == 2);
        KIT_TEST(!px2.unique());

        KIT_TEST(X::instances == 1);

        KIT_TEST(px2.use_count() == px.use_count());
        KIT_TEST(!(px < px2 || px2 < px)); // shared ownership test

        kit::local_shared_ptr<void> px3(px);
        KIT_TEST(px3 == px);
        KIT_TEST(px3? true: false);
        KIT_TEST(!!px3);
        KIT_TEST(px3.get() == p);
        KIT_TEST(px3.use_count() == 3);
        KIT_TEST(!px3.unique());
        KIT_TEST(px3.use_count() == px.use_count());
        KIT_TEST(!(px < px3 || px3 < px)); // shared ownership test

        kit::local_shared_ptr<void> px4(px2);
        KIT_TEST(px4 == px2);
        KIT_TEST(px4? true: false);
        KIT_TEST(!!px4);
        KIT_TEST(px4.get() == p);
        KIT_TEST(px4.use_count() == 4);
        KIT_TEST(!px4.unique());
        KIT_TEST(px4.use_count() == px2.use_count());
        KIT_TEST(!(px2 < px4 || px4 < px2)); // shared ownership test

        KIT_TEST(px3.use_count() == px4.use_count());
        KIT_TEST(!(px3 < px4 || px4 < px3)); // shared ownership test
    }

    KIT_TEST(X::instances == 0);
    KIT_TEST(Y::instances == 0);

    {
        Y * p = new Y;
        kit::local_shared_ptr<Y> py(p);
        KIT_TEST(py.get() == p);

        kit::local_shared_ptr<X> px(py);
        KIT_TEST(px == py);
        KIT_TEST(px? true: false);
        KIT_TEST(!!px);
        KIT_TEST(px.get() == p);
        KIT_TEST(px.use_count() == 2);
        KIT_TEST(!px.unique());
        KIT_TEST(px.use_count() == py.use_count());
        KIT_TEST(!(px < py || py < px)); // shared ownership test

        KIT_TEST(X::instances == 1);
        KIT_TEST(Y::instances == 1);

        kit::local_shared_ptr<void const> pv(px);
        KIT_TEST(pv == px);
        KIT_TEST(pv? true: false);
        KIT_TEST(!!pv);
        KIT_TEST(pv.get() == px.get());
        KIT_TEST(pv.use_count() == 3);
        KIT_TEST(!pv.unique());
        KIT_TEST(pv.use_count() == px.use_count());
        KIT_TEST(!(px < pv || pv < px)); // shared ownership test

        kit::local_shared_ptr<void const> pv2(py);
        KIT_TEST(pv2 == py);
        KIT_TEST(pv2? true: false);
        KIT_TEST(!!pv2);
        KIT_TEST(pv2.get() == py.get());
        KIT_TEST(pv2.use_count() == 4);
        KIT_TEST(!pv2.unique());
        KIT_TEST(pv2.use_count() == py.use_count());
        KIT_TEST(!(py < pv2 || pv2 < py)); // shared ownership test

        KIT_TEST(pv.use_count() == pv2.use_count());
        KIT_TEST(!(pv < pv2 || pv2 < pv)); // shared ownership test
    }

    KIT_TEST(X::instances == 0);
    KIT_TEST(Y::instances == 0);
}

void weak_ptr_constructor()
{
    {
        kit::local_weak_ptr<Y> wp;
        KIT_TEST(wp.use_count() == 0);

        try
        {
            kit::local_shared_ptr<Y> p2(wp);
            KIT_ERROR("shared_ptr<Y> p2(wp) failed to throw");
        }
        catch(kit::bad_local_weak_ptr)
        {
        }

        try
        {
            kit::local_shared_ptr<X> p3(wp);
            KIT_ERROR("shared_ptr<X> p3(wp) failed to throw");
        }
        catch(kit::bad_local_weak_ptr)
        {
        }
    }

    {
        kit::local_shared_ptr<Y> p;
        kit::local_weak_ptr<Y> wp(p);

        if(wp.use_count() != 0) // 0 allowed but not required
        {
            kit::local_shared_ptr<Y> p2(wp);
            KIT_TEST(p2.use_count() == wp.use_count());
            KIT_TEST(p2.get() == 0);

            kit::local_shared_ptr<X> p3(wp);
            KIT_TEST(p3.use_count() == wp.use_count());
            KIT_TEST(p3.get() == 0);
        }
    }

    {
        kit::local_shared_ptr<Y> p(new Y);
        kit::local_weak_ptr<Y> wp(p);

        {
            kit::local_shared_ptr<Y> p2(wp);
            KIT_TEST(p2? true: false);
            KIT_TEST(!!p2);
            KIT_TEST(p2.get() == p.get());
            KIT_TEST(p2.use_count() == 2);
            KIT_TEST(!p2.unique());
            KIT_TEST(p2.use_count() == wp.use_count());

            KIT_TEST(p.use_count() == p2.use_count());
            KIT_TEST(!(p < p2 || p2 < p)); // shared ownership test

            kit::local_shared_ptr<X> p3(wp);
            KIT_TEST(p3? true: false);
            KIT_TEST(!!p3);
            KIT_TEST(p3.get() == p.get());
            KIT_TEST(p3.use_count() == 3);
            KIT_TEST(!p3.unique());
            KIT_TEST(p3.use_count() == wp.use_count());

            KIT_TEST(p.use_count() == p3.use_count());
        }

        p.reset();
        KIT_TEST(wp.use_count() == 0);

        try
        {
            kit::local_shared_ptr<Y> p2(wp);
            KIT_ERROR("shared_ptr<Y> p2(wp) failed to throw");
        }
        catch(kit::bad_local_weak_ptr)
        {
        }

        try
        {
            kit::local_shared_ptr<X> p3(wp);
            KIT_ERROR("shared_ptr<X> p3(wp) failed to throw");
        }
        catch(kit::bad_local_weak_ptr)
        {
        }
    }
}

#if defined(KIT_DINKUMWARE_STDLIB) && (KIT_DINKUMWARE_STDLIB < 306)
#  define KIT_OLD_AUTO_PTR
#endif

//void auto_ptr_constructor()
//{
//    {
//        std::auto_ptr<int> p;
//        kit::local_shared_ptr<int> pi(p);
//        KIT_TEST(pi? false: true);
//        KIT_TEST(!pi);
//        KIT_TEST(pi.get() == 0);
//        KIT_TEST(pi.use_count() == 1);
//        KIT_TEST(pi.unique());
//        KIT_TEST(p.get() == 0);
//    }
//
//    {
//        std::auto_ptr<int> p;
//        kit::local_shared_ptr<int const> pi(p);
//        KIT_TEST(pi? false: true);
//        KIT_TEST(!pi);
//        KIT_TEST(pi.get() == 0);
//        KIT_TEST(pi.use_count() == 1);
//        KIT_TEST(pi.unique());
//        KIT_TEST(p.get() == 0);
//    }
//
//    {
//        std::auto_ptr<int> p;
//        kit::local_shared_ptr<void> pv(p);
//        KIT_TEST(pv? false: true);
//        KIT_TEST(!pv);
//        KIT_TEST(pv.get() == 0);
//        KIT_TEST(pv.use_count() == 1);
//        KIT_TEST(pv.unique());
//        KIT_TEST(p.get() == 0);
//    }
//
//    {
//        std::auto_ptr<int> p;
//        kit::local_shared_ptr<void const> pv(p);
//        KIT_TEST(pv? false: true);
//        KIT_TEST(!pv);
//        KIT_TEST(pv.get() == 0);
//        KIT_TEST(pv.use_count() == 1);
//        KIT_TEST(pv.unique());
//        KIT_TEST(p.get() == 0);
//    }
//
//    {
//        std::auto_ptr<X> p;
//        kit::local_shared_ptr<X> px(p);
//        KIT_TEST(px? false: true);
//        KIT_TEST(!px);
//        KIT_TEST(px.get() == 0);
//        KIT_TEST(px.use_count() == 1);
//        KIT_TEST(px.unique());
//        KIT_TEST(p.get() == 0);
//    }
//
//    {
//        std::auto_ptr<X> p;
//        kit::local_shared_ptr<X const> px(p);
//        KIT_TEST(px? false: true);
//        KIT_TEST(!px);
//        KIT_TEST(px.get() == 0);
//        KIT_TEST(px.use_count() == 1);
//        KIT_TEST(px.unique());
//        KIT_TEST(p.get() == 0);
//    }
//
//    {
//        std::auto_ptr<Y> p;
//        kit::local_shared_ptr<X> px(p);
//        KIT_TEST(px? false: true);
//        KIT_TEST(!px);
//        KIT_TEST(px.get() == 0);
//        KIT_TEST(px.use_count() == 1);
//        KIT_TEST(px.unique());
//        KIT_TEST(p.get() == 0);
//    }
//
//    {
//        std::auto_ptr<Y> p;
//        kit::local_shared_ptr<X const> px(p);
//        KIT_TEST(px? false: true);
//        KIT_TEST(!px);
//        KIT_TEST(px.get() == 0);
//        KIT_TEST(px.use_count() == 1);
//        KIT_TEST(px.unique());
//        KIT_TEST(p.get() == 0);
//    }
//
//    {
//        std::auto_ptr<Y> p;
//        kit::local_shared_ptr<void> pv(p);
//        KIT_TEST(pv? false: true);
//        KIT_TEST(!pv);
//        KIT_TEST(pv.get() == 0);
//        KIT_TEST(pv.use_count() == 1);
//        KIT_TEST(pv.unique());
//        KIT_TEST(p.get() == 0);
//    }
//
//    {
//        std::auto_ptr<Y> p;
//        kit::local_shared_ptr<void const> pv(p);
//        KIT_TEST(pv? false: true);
//        KIT_TEST(!pv);
//        KIT_TEST(pv.get() == 0);
//        KIT_TEST(pv.use_count() == 1);
//        KIT_TEST(pv.unique());
//        KIT_TEST(p.get() == 0);
//    }
//
//    {
//        std::auto_ptr<int> p(new int(7));
//        int * q = p.get();
//        kit::local_shared_ptr<int> pi(p);
//        KIT_TEST(pi? true: false);
//        KIT_TEST(!!pi);
//        KIT_TEST(pi.get() == q);
//        KIT_TEST(pi.use_count() == 1);
//        KIT_TEST(pi.unique());
//        KIT_TEST(*pi == 7);
//
//#if !defined(KIT_OLD_AUTO_PTR)
//        KIT_TEST(p.get() == 0);
//#endif
//    }
//
//    {
//        std::auto_ptr<int> p(new int(7));
//        int * q = p.get();
//        kit::local_shared_ptr<int const> pi(p);
//        KIT_TEST(pi? true: false);
//        KIT_TEST(!!pi);
//        KIT_TEST(pi.get() == q);
//        KIT_TEST(pi.use_count() == 1);
//        KIT_TEST(pi.unique());
//        KIT_TEST(*pi == 7);
//
//#if !defined(KIT_OLD_AUTO_PTR)
//        KIT_TEST(p.get() == 0);
//#endif
//    }
//
//    {
//        std::auto_ptr<int> p(new int(7));
//        int * q = p.get();
//        kit::local_shared_ptr<void> pv(p);
//        KIT_TEST(pv? true: false);
//        KIT_TEST(!!pv);
//        KIT_TEST(pv.get() == q);
//        KIT_TEST(pv.use_count() == 1);
//        KIT_TEST(pv.unique());
//
//#if !defined(KIT_OLD_AUTO_PTR)
//        KIT_TEST(p.get() == 0);
//#endif
//    }
//
//    {
//        std::auto_ptr<int> p(new int(7));
//        int * q = p.get();
//        kit::local_shared_ptr<void const> pv(p);
//        KIT_TEST(pv? true: false);
//        KIT_TEST(!!pv);
//        KIT_TEST(pv.get() == q);
//        KIT_TEST(pv.use_count() == 1);
//        KIT_TEST(pv.unique());
//
//#if !defined(KIT_OLD_AUTO_PTR)
//        KIT_TEST(p.get() == 0);
//#endif
//    }
//
//    KIT_TEST(X::instances == 0);
//
//    {
//        std::auto_ptr<X> p(new X);
//        X * q = p.get();
//        kit::local_shared_ptr<X> px(p);
//        KIT_TEST(px? true: false);
//        KIT_TEST(!!px);
//        KIT_TEST(px.get() == q);
//        KIT_TEST(px.use_count() == 1);
//        KIT_TEST(px.unique());
//        KIT_TEST(X::instances == 1);
//
//#if !defined(KIT_OLD_AUTO_PTR)
//        KIT_TEST(p.get() == 0);
//#endif
//    }
//
//    KIT_TEST(X::instances == 0);
//
//    {
//        std::auto_ptr<X> p(new X);
//        X * q = p.get();
//        kit::local_shared_ptr<X const> px(p);
//        KIT_TEST(px? true: false);
//        KIT_TEST(!!px);
//        KIT_TEST(px.get() == q);
//        KIT_TEST(px.use_count() == 1);
//        KIT_TEST(px.unique());
//        KIT_TEST(X::instances == 1);
//
//#if !defined(KIT_OLD_AUTO_PTR)
//        KIT_TEST(p.get() == 0);
//#endif
//    }
//
//    KIT_TEST(X::instances == 0);
//
//    {
//        std::auto_ptr<X> p(new X);
//        X * q = p.get();
//        kit::local_shared_ptr<void> pv(p);
//        KIT_TEST(pv? true: false);
//        KIT_TEST(!!pv);
//        KIT_TEST(pv.get() == q);
//        KIT_TEST(pv.use_count() == 1);
//        KIT_TEST(pv.unique());
//        KIT_TEST(X::instances == 1);
//
//#if !defined(KIT_OLD_AUTO_PTR)
//        KIT_TEST(p.get() == 0);
//#endif
//    }
//
//    KIT_TEST(X::instances == 0);
//
//    {
//        std::auto_ptr<X> p(new X);
//        X * q = p.get();
//        kit::local_shared_ptr<void const> pv(p);
//        KIT_TEST(pv? true: false);
//        KIT_TEST(!!pv);
//        KIT_TEST(pv.get() == q);
//        KIT_TEST(pv.use_count() == 1);
//        KIT_TEST(pv.unique());
//        KIT_TEST(X::instances == 1);
//
//#if !defined(KIT_OLD_AUTO_PTR)
//        KIT_TEST(p.get() == 0);
//#endif
//    }
//
//    KIT_TEST(X::instances == 0);
//    KIT_TEST(Y::instances == 0);
//
//    {
//        std::auto_ptr<Y> p(new Y);
//        Y * q = p.get();
//        kit::local_shared_ptr<X> px(p);
//        KIT_TEST(px? true: false);
//        KIT_TEST(!!px);
//        KIT_TEST(px.get() == q);
//        KIT_TEST(px.use_count() == 1);
//        KIT_TEST(px.unique());
//        KIT_TEST(X::instances == 1);
//        KIT_TEST(Y::instances == 1);
//
//#if !defined(KIT_OLD_AUTO_PTR)
//        KIT_TEST(p.get() == 0);
//#endif
//    }
//
//    KIT_TEST(X::instances == 0);
//    KIT_TEST(Y::instances == 0);
//
//    {
//        std::auto_ptr<Y> p(new Y);
//        Y * q = p.get();
//        kit::local_shared_ptr<X const> px(p);
//        KIT_TEST(px? true: false);
//        KIT_TEST(!!px);
//        KIT_TEST(px.get() == q);
//        KIT_TEST(px.use_count() == 1);
//        KIT_TEST(px.unique());
//        KIT_TEST(X::instances == 1);
//        KIT_TEST(Y::instances == 1);
//
//#if !defined(KIT_OLD_AUTO_PTR)
//        KIT_TEST(p.get() == 0);
//#endif
//    }
//
//    KIT_TEST(X::instances == 0);
//    KIT_TEST(Y::instances == 0);
//}

void test()
{
    default_constructor();
    pointer_constructor();
    deleter_constructor();
    copy_constructor();
    weak_ptr_constructor();
//    auto_ptr_constructor();
}

} // namespace n_constructors

namespace n_assignment
{

class incomplete;

struct A
{
    int dummy;
};

struct X
{
    static long instances;

    X()
    {
        ++instances;
    }

    ~X()
    {
        --instances;
    }

private:

    X(X const &);
    X & operator= (X const &);
};

long X::instances = 0;

struct Y: public A, public virtual X
{
    static long instances;

    Y()
    {
        ++instances;
    }

    ~Y()
    {
        --instances;
    }

private:

    Y(Y const &);
    Y & operator= (Y const &);
};

long Y::instances = 0;

void copy_assignment()
{
    {
        kit::local_shared_ptr<incomplete> p1;

        p1 = p1;

        KIT_TEST(p1 == p1);
        KIT_TEST(p1? false: true);
        KIT_TEST(!p1);
        KIT_TEST(p1.get() == 0);

        kit::local_shared_ptr<incomplete> p2;

        p1 = p2;

        KIT_TEST(p1 == p2);
        KIT_TEST(p1? false: true);
        KIT_TEST(!p1);
        KIT_TEST(p1.get() == 0);

        kit::local_shared_ptr<incomplete> p3(p1);

        p1 = p3;

        KIT_TEST(p1 == p3);
        KIT_TEST(p1? false: true);
        KIT_TEST(!p1);
        KIT_TEST(p1.get() == 0);
    }

    {
        kit::local_shared_ptr<void> p1;

        p1 = p1;

        KIT_TEST(p1 == p1);
        KIT_TEST(p1? false: true);
        KIT_TEST(!p1);
        KIT_TEST(p1.get() == 0);

        kit::local_shared_ptr<void> p2;

        p1 = p2;

        KIT_TEST(p1 == p2);
        KIT_TEST(p1? false: true);
        KIT_TEST(!p1);
        KIT_TEST(p1.get() == 0);

        kit::local_shared_ptr<void> p3(p1);

        p1 = p3;

        KIT_TEST(p1 == p3);
        KIT_TEST(p1? false: true);
        KIT_TEST(!p1);
        KIT_TEST(p1.get() == 0);

        kit::local_shared_ptr<void> p4(new int);
        KIT_TEST(p4.use_count() == 1);

        p1 = p4;

        KIT_TEST(p1 == p4);
        KIT_TEST(!(p1 < p4 || p4 < p1));
        KIT_TEST(p1.use_count() == 2);
        KIT_TEST(p4.use_count() == 2);

        p1 = p3;

        KIT_TEST(p1 == p3);
        KIT_TEST(p4.use_count() == 1);
    }

    {
        kit::local_shared_ptr<X> p1;

        p1 = p1;

        KIT_TEST(p1 == p1);
        KIT_TEST(p1? false: true);
        KIT_TEST(!p1);
        KIT_TEST(p1.get() == 0);

        kit::local_shared_ptr<X> p2;

        p1 = p2;

        KIT_TEST(p1 == p2);
        KIT_TEST(p1? false: true);
        KIT_TEST(!p1);
        KIT_TEST(p1.get() == 0);

        kit::local_shared_ptr<X> p3(p1);

        p1 = p3;

        KIT_TEST(p1 == p3);
        KIT_TEST(p1? false: true);
        KIT_TEST(!p1);
        KIT_TEST(p1.get() == 0);

        KIT_TEST(X::instances == 0);

        kit::local_shared_ptr<X> p4(new X);

        KIT_TEST(X::instances == 1);

        p1 = p4;

        KIT_TEST(X::instances == 1);

        KIT_TEST(p1 == p4);
        KIT_TEST(!(p1 < p4 || p4 < p1));

        KIT_TEST(p1.use_count() == 2);

        p1 = p2;

        KIT_TEST(p1 == p2);
        KIT_TEST(X::instances == 1);

        p4 = p3;

        KIT_TEST(p4 == p3);
        KIT_TEST(X::instances == 0);
    }
}

void conversion_assignment()
{
    {
        kit::local_shared_ptr<void> p1;

        kit::local_shared_ptr<incomplete> p2;

        p1 = p2;

        KIT_TEST(p1 == p2);
        KIT_TEST(p1? false: true);
        KIT_TEST(!p1);
        KIT_TEST(p1.get() == 0);

        kit::local_shared_ptr<int> p4(new int);
        KIT_TEST(p4.use_count() == 1);

        kit::local_shared_ptr<void> p5(p4);
        KIT_TEST(p4.use_count() == 2);

        p1 = p4;

        KIT_TEST(p1 == p4);
        KIT_TEST(!(p1 < p5 || p5 < p1));
        KIT_TEST(p1.use_count() == 3);
        KIT_TEST(p4.use_count() == 3);

        p1 = p2;

        KIT_TEST(p1 == p2);
        KIT_TEST(p4.use_count() == 2);
    }

    {
        kit::local_shared_ptr<X> p1;

        kit::local_shared_ptr<Y> p2;

        p1 = p2;

        KIT_TEST(p1 == p2);
        KIT_TEST(p1? false: true);
        KIT_TEST(!p1);
        KIT_TEST(p1.get() == 0);

        KIT_TEST(X::instances == 0);
        KIT_TEST(Y::instances == 0);

        kit::local_shared_ptr<Y> p4(new Y);

        KIT_TEST(X::instances == 1);
        KIT_TEST(Y::instances == 1);
        KIT_TEST(p4.use_count() == 1);

        kit::local_shared_ptr<X> p5(p4);
        KIT_TEST(p4.use_count() == 2);

        p1 = p4;

        KIT_TEST(X::instances == 1);
        KIT_TEST(Y::instances == 1);

        KIT_TEST(p1 == p4);
        KIT_TEST(!(p1 < p5 || p5 < p1));

        KIT_TEST(p1.use_count() == 3);
        KIT_TEST(p4.use_count() == 3);

        p1 = p2;

        KIT_TEST(p1 == p2);
        KIT_TEST(X::instances == 1);
        KIT_TEST(Y::instances == 1);
        KIT_TEST(p4.use_count() == 2);

        p4 = p2;
        p5 = p2;

        KIT_TEST(p4 == p2);
        KIT_TEST(X::instances == 0);
        KIT_TEST(Y::instances == 0);
    }
}

//void auto_ptr_assignment()
//{
//    {
//        kit::local_shared_ptr<int> p1;
//
//        std::auto_ptr<int> p2;
//
//        p1 = p2;
//        KIT_TEST(p1? false: true);
//        KIT_TEST(!p1);
//        KIT_TEST(p1.get() == 0);
//        KIT_TEST(p1.use_count() == 1);
//
//        int * p = new int;
//        std::auto_ptr<int> p3(p);
//
//        p1 = p3;
//        KIT_TEST(p1.get() == p);
//        KIT_TEST(p1.use_count() == 1);
//
//#if !defined(KIT_OLD_AUTO_PTR)
//        KIT_TEST(p3.get() == 0);
//#endif
//
//        p1 = p2;
//        KIT_TEST(p1? false: true);
//        KIT_TEST(!p1);
//        KIT_TEST(p1.get() == 0);
//        KIT_TEST(p1.use_count() == 1);
//    }
//
//    {
//        kit::local_shared_ptr<void> p1;
//
//        std::auto_ptr<int> p2;
//
//        p1 = p2;
//        KIT_TEST(p1? false: true);
//        KIT_TEST(!p1);
//        KIT_TEST(p1.get() == 0);
//        KIT_TEST(p1.use_count() == 1);
//
//        int * p = new int;
//        std::auto_ptr<int> p3(p);
//
//        p1 = p3;
//        KIT_TEST(p1.get() == p);
//        KIT_TEST(p1.use_count() == 1);
//
//#if !defined(KIT_OLD_AUTO_PTR)
//        KIT_TEST(p3.get() == 0);
//#endif
//
//        p1 = p2;
//        KIT_TEST(p1? false: true);
//        KIT_TEST(!p1);
//        KIT_TEST(p1.get() == 0);
//        KIT_TEST(p1.use_count() == 1);
//    }
//
//
//    {
//        kit::local_shared_ptr<X> p1;
//
//        std::auto_ptr<Y> p2;
//
//        p1 = p2;
//        KIT_TEST(p1? false: true);
//        KIT_TEST(!p1);
//        KIT_TEST(p1.get() == 0);
//        KIT_TEST(p1.use_count() == 1);
//        KIT_TEST(X::instances == 0);
//        KIT_TEST(Y::instances == 0);
//
//        Y * p = new Y;
//        std::auto_ptr<Y> p3(p);
//
//        KIT_TEST(X::instances == 1);
//        KIT_TEST(Y::instances == 1);
//
//        p1 = p3;
//        KIT_TEST(p1.get() == p);
//        KIT_TEST(p1.use_count() == 1);
//        KIT_TEST(X::instances == 1);
//        KIT_TEST(Y::instances == 1);
//
//#if !defined(KIT_OLD_AUTO_PTR)
//        KIT_TEST(p3.get() == 0);
//#endif
//
//        p1 = p2;
//        KIT_TEST(p1? false: true);
//        KIT_TEST(!p1);
//        KIT_TEST(p1.get() == 0);
//        KIT_TEST(p1.use_count() == 1);
//        KIT_TEST(X::instances == 0);
//        KIT_TEST(Y::instances == 0);
//    }
//}

void test()
{
    copy_assignment();
    conversion_assignment();
//    auto_ptr_assignment();
}

} // namespace n_assignment

namespace n_reset
{

class incomplete;

incomplete * p0 = 0;

void deleter(incomplete *)
{
}

struct X
{
    static long instances;

    X()
    {
        ++instances;
    }

    ~X()
    {
        --instances;
    }

private:

    X(X const &);
    X & operator= (X const &);
};

long X::instances = 0;

void plain_reset()
{
    {
        kit::local_shared_ptr<int> pi;
        pi.reset();
        KIT_TEST(pi? false: true);
        KIT_TEST(!pi);
        KIT_TEST(pi.get() == 0);
        KIT_TEST(pi.use_count() == 0);
    }

    {
        kit::local_shared_ptr<int> pi(static_cast<int*>(0));
        pi.reset();
        KIT_TEST(pi? false: true);
        KIT_TEST(!pi);
        KIT_TEST(pi.get() == 0);
        KIT_TEST(pi.use_count() == 0);
    }

    {
        kit::local_shared_ptr<int> pi(new int);
        pi.reset();
        KIT_TEST(pi? false: true);
        KIT_TEST(!pi);
        KIT_TEST(pi.get() == 0);
        KIT_TEST(pi.use_count() == 0);
    }

    {
        kit::local_shared_ptr<incomplete> px;
        px.reset();
        KIT_TEST(px? false: true);
        KIT_TEST(!px);
        KIT_TEST(px.get() == 0);
        KIT_TEST(px.use_count() == 0);
    }

    {
        kit::local_shared_ptr<incomplete> px(p0, deleter);
        px.reset();
        KIT_TEST(px? false: true);
        KIT_TEST(!px);
        KIT_TEST(px.get() == 0);
        KIT_TEST(px.use_count() == 0);
    }

    {
        kit::local_shared_ptr<X> px;
        px.reset();
        KIT_TEST(px? false: true);
        KIT_TEST(!px);
        KIT_TEST(px.get() == 0);
        KIT_TEST(px.use_count() == 0);
    }

    {
        KIT_TEST(X::instances == 0);
        kit::local_shared_ptr<X> px(new X);
        KIT_TEST(X::instances == 1);
        px.reset();
        KIT_TEST(px? false: true);
        KIT_TEST(!px);
        KIT_TEST(px.get() == 0);
        KIT_TEST(px.use_count() == 0);
        KIT_TEST(X::instances == 0);
    }

    {
        kit::local_shared_ptr<void> pv;
        pv.reset();
        KIT_TEST(pv? false: true);
        KIT_TEST(!pv);
        KIT_TEST(pv.get() == 0);
        KIT_TEST(pv.use_count() == 0);
    }

    {
        KIT_TEST(X::instances == 0);
        kit::local_shared_ptr<void> pv(new X);
        KIT_TEST(X::instances == 1);
        pv.reset();
        KIT_TEST(pv? false: true);
        KIT_TEST(!pv);
        KIT_TEST(pv.get() == 0);
        KIT_TEST(pv.use_count() == 0);
        KIT_TEST(X::instances == 0);
    }
}

struct A
{
    int dummy;
};

struct Y: public A, public virtual X
{
    static long instances;

    Y()
    {
        ++instances;
    }

    ~Y()
    {
        --instances;
    }

private:

    Y(Y const &);
    Y & operator= (Y const &);
};

long Y::instances = 0;

void pointer_reset()
{
    {
        kit::local_shared_ptr<int> pi;

        pi.reset(static_cast<int*>(0));
        KIT_TEST(pi? false: true);
        KIT_TEST(!pi);
        KIT_TEST(pi.get() == 0);
        KIT_TEST(pi.use_count() == 1);
        KIT_TEST(pi.unique());

        int * p = new int;
        pi.reset(p);
        KIT_TEST(pi? true: false);
        KIT_TEST(!!pi);
        KIT_TEST(pi.get() == p);
        KIT_TEST(pi.use_count() == 1);
        KIT_TEST(pi.unique());

        pi.reset(static_cast<int*>(0));
        KIT_TEST(pi? false: true);
        KIT_TEST(!pi);
        KIT_TEST(pi.get() == 0);
        KIT_TEST(pi.use_count() == 1);
        KIT_TEST(pi.unique());
    }

    {
        kit::local_shared_ptr<X> px;

        px.reset(static_cast<X*>(0));
        KIT_TEST(px? false: true);
        KIT_TEST(!px);
        KIT_TEST(px.get() == 0);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());
        KIT_TEST(X::instances == 0);

        X * p = new X;
        px.reset(p);
        KIT_TEST(px? true: false);
        KIT_TEST(!!px);
        KIT_TEST(px.get() == p);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());
        KIT_TEST(X::instances == 1);

        px.reset(static_cast<X*>(0));
        KIT_TEST(px? false: true);
        KIT_TEST(!px);
        KIT_TEST(px.get() == 0);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());
        KIT_TEST(X::instances == 0);
        KIT_TEST(Y::instances == 0);

        Y * q = new Y;
        px.reset(q);
        KIT_TEST(px? true: false);
        KIT_TEST(!!px);
        KIT_TEST(px.get() == q);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());
        KIT_TEST(X::instances == 1);
        KIT_TEST(Y::instances == 1);

        px.reset(static_cast<Y*>(0));
        KIT_TEST(px? false: true);
        KIT_TEST(!px);
        KIT_TEST(px.get() == 0);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());
        KIT_TEST(X::instances == 0);
        KIT_TEST(Y::instances == 0);
    }

    {
        kit::local_shared_ptr<void> pv;

        pv.reset(static_cast<X*>(0));
        KIT_TEST(pv? false: true);
        KIT_TEST(!pv);
        KIT_TEST(pv.get() == 0);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());
        KIT_TEST(X::instances == 0);

        X * p = new X;
        pv.reset(p);
        KIT_TEST(pv? true: false);
        KIT_TEST(!!pv);
        KIT_TEST(pv.get() == p);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());
        KIT_TEST(X::instances == 1);

        pv.reset(static_cast<X*>(0));
        KIT_TEST(pv? false: true);
        KIT_TEST(!pv);
        KIT_TEST(pv.get() == 0);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());
        KIT_TEST(X::instances == 0);
        KIT_TEST(Y::instances == 0);

        Y * q = new Y;
        pv.reset(q);
        KIT_TEST(pv? true: false);
        KIT_TEST(!!pv);
        KIT_TEST(pv.get() == q);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());
        KIT_TEST(X::instances == 1);
        KIT_TEST(Y::instances == 1);

        pv.reset(static_cast<Y*>(0));
        KIT_TEST(pv? false: true);
        KIT_TEST(!pv);
        KIT_TEST(pv.get() == 0);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());
        KIT_TEST(X::instances == 0);
        KIT_TEST(Y::instances == 0);
    }
}

void * deleted = 0;

void deleter2(void * p)
{
    deleted = p;
}

void deleter_reset()
{
    {
        kit::local_shared_ptr<int> pi;

        pi.reset(static_cast<int*>(0), deleter2);
        KIT_TEST(pi? false: true);
        KIT_TEST(!pi);
        KIT_TEST(pi.get() == 0);
        KIT_TEST(pi.use_count() == 1);
        KIT_TEST(pi.unique());

        deleted = &pi;

        int m = 0;
        pi.reset(&m, deleter2);
        KIT_TEST(deleted == 0);
        KIT_TEST(pi? true: false);
        KIT_TEST(!!pi);
        KIT_TEST(pi.get() == &m);
        KIT_TEST(pi.use_count() == 1);
        KIT_TEST(pi.unique());

        pi.reset(static_cast<int*>(0), deleter2);
        KIT_TEST(deleted == &m);
        KIT_TEST(pi? false: true);
        KIT_TEST(!pi);
        KIT_TEST(pi.get() == 0);
        KIT_TEST(pi.use_count() == 1);
        KIT_TEST(pi.unique());

        pi.reset();
        KIT_TEST(deleted == 0);
    }

    {
        kit::local_shared_ptr<X> px;

        px.reset(static_cast<X*>(0), deleter2);
        KIT_TEST(px? false: true);
        KIT_TEST(!px);
        KIT_TEST(px.get() == 0);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());

        deleted = &px;

        X x;
        px.reset(&x, deleter2);
        KIT_TEST(deleted == 0);
        KIT_TEST(px? true: false);
        KIT_TEST(!!px);
        KIT_TEST(px.get() == &x);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());

        px.reset(static_cast<X*>(0), deleter2);
        KIT_TEST(deleted == &x);
        KIT_TEST(px? false: true);
        KIT_TEST(!px);
        KIT_TEST(px.get() == 0);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());

        Y y;
        px.reset(&y, deleter2);
        KIT_TEST(deleted == 0);
        KIT_TEST(px? true: false);
        KIT_TEST(!!px);
        KIT_TEST(px.get() == &y);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());

        px.reset(static_cast<Y*>(0), deleter2);
        KIT_TEST(deleted == &y);
        KIT_TEST(px? false: true);
        KIT_TEST(!px);
        KIT_TEST(px.get() == 0);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());

        px.reset();
        KIT_TEST(deleted == 0);
    }

    {
        kit::local_shared_ptr<void> pv;

        pv.reset(static_cast<X*>(0), deleter2);
        KIT_TEST(pv? false: true);
        KIT_TEST(!pv);
        KIT_TEST(pv.get() == 0);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());

        deleted = &pv;

        X x;
        pv.reset(&x, deleter2);
        KIT_TEST(deleted == 0);
        KIT_TEST(pv? true: false);
        KIT_TEST(!!pv);
        KIT_TEST(pv.get() == &x);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());

        pv.reset(static_cast<X*>(0), deleter2);
        KIT_TEST(deleted == &x);
        KIT_TEST(pv? false: true);
        KIT_TEST(!pv);
        KIT_TEST(pv.get() == 0);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());

        Y y;
        pv.reset(&y, deleter2);
        KIT_TEST(deleted == 0);
        KIT_TEST(pv? true: false);
        KIT_TEST(!!pv);
        KIT_TEST(pv.get() == &y);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());

        pv.reset(static_cast<Y*>(0), deleter2);
        KIT_TEST(deleted == &y);
        KIT_TEST(pv? false: true);
        KIT_TEST(!pv);
        KIT_TEST(pv.get() == 0);
        KIT_TEST(pv.use_count() == 1);
        KIT_TEST(pv.unique());

        pv.reset();
        KIT_TEST(deleted == 0);
    }

    {
        kit::local_shared_ptr<incomplete> px;

        px.reset(p0, deleter2);
        KIT_TEST(px? false: true);
        KIT_TEST(!px);
        KIT_TEST(px.get() == 0);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());

        deleted = &px;
        px.reset(p0, deleter2);
        KIT_TEST(deleted == 0);
    }
}

void test()
{
    plain_reset();
    pointer_reset();
    deleter_reset();
}

} // namespace n_reset

namespace n_access
{

struct X
{
};

void test()
{
    {
        kit::local_shared_ptr<X> px;
        KIT_TEST(px.get() == 0);
        KIT_TEST(px? false: true);
        KIT_TEST(!px);

#if defined(KIT_NO_ARGUMENT_DEPENDENT_LOOKUP)
        using kit::get_pointer;
#endif

        KIT_TEST(get_pointer(px) == px.get());
    }

    {
        kit::local_shared_ptr<X> px(static_cast<X*>(0));
        KIT_TEST(px.get() == 0);
        KIT_TEST(px? false: true);
        KIT_TEST(!px);

#if defined(KIT_NO_ARGUMENT_DEPENDENT_LOOKUP)
        using kit::get_pointer;
#endif

        KIT_TEST(get_pointer(px) == px.get());
    }

    {
        kit::local_shared_ptr<X> px(static_cast<X*>(0), kit::checked_deleter<X>());
        KIT_TEST(px.get() == 0);
        KIT_TEST(px? false: true);
        KIT_TEST(!px);

#if defined(KIT_NO_ARGUMENT_DEPENDENT_LOOKUP)
        using kit::get_pointer;
#endif

        KIT_TEST(get_pointer(px) == px.get());
    }

    {
        X * p = new X;
        kit::local_shared_ptr<X> px(p);
        KIT_TEST(px.get() == p);
        KIT_TEST(px? true: false);
        KIT_TEST(!!px);
        KIT_TEST(&*px == px.get());
        KIT_TEST(px.operator ->() == px.get());

#if defined(KIT_NO_ARGUMENT_DEPENDENT_LOOKUP)
        using kit::get_pointer;
#endif

        KIT_TEST(get_pointer(px) == px.get());
    }

    {
        X * p = new X;
        kit::local_shared_ptr<X> px(p, kit::checked_deleter<X>());
        KIT_TEST(px.get() == p);
        KIT_TEST(px? true: false);
        KIT_TEST(!!px);
        KIT_TEST(&*px == px.get());
        KIT_TEST(px.operator ->() == px.get());

#if defined(KIT_NO_ARGUMENT_DEPENDENT_LOOKUP)
        using kit::get_pointer;
#endif

        KIT_TEST(get_pointer(px) == px.get());
    }
}

} // namespace n_access

namespace n_use_count
{

struct X
{
};

void test()
{
    {
        kit::local_shared_ptr<X> px(static_cast<X*>(0));
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());

        kit::local_shared_ptr<X> px2(px);
        KIT_TEST(px2.use_count() == 2);
        KIT_TEST(!px2.unique());
        KIT_TEST(px.use_count() == 2);
        KIT_TEST(!px.unique());
    }

    {
        kit::local_shared_ptr<X> px(new X);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());

        kit::local_shared_ptr<X> px2(px);
        KIT_TEST(px2.use_count() == 2);
        KIT_TEST(!px2.unique());
        KIT_TEST(px.use_count() == 2);
        KIT_TEST(!px.unique());
    }

    {
        kit::local_shared_ptr<X> px(new X, kit::checked_deleter<X>());
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px.unique());

        kit::local_shared_ptr<X> px2(px);
        KIT_TEST(px2.use_count() == 2);
        KIT_TEST(!px2.unique());
        KIT_TEST(px.use_count() == 2);
        KIT_TEST(!px.unique());
    }
}

} // namespace n_use_count

namespace n_swap
{

struct X
{
};

void test()
{
    {
        kit::local_shared_ptr<X> px;
        kit::local_shared_ptr<X> px2;

        px.swap(px2);

        KIT_TEST(px.get() == 0);
        KIT_TEST(px2.get() == 0);

        using std::swap;
        swap(px, px2);

        KIT_TEST(px.get() == 0);
        KIT_TEST(px2.get() == 0);
    }

    {
        X * p = new X;
        kit::local_shared_ptr<X> px;
        kit::local_shared_ptr<X> px2(p);
        kit::local_shared_ptr<X> px3(px2);

        px.swap(px2);

        KIT_TEST(px.get() == p);
        KIT_TEST(px.use_count() == 2);
        KIT_TEST(px2.get() == 0);
        KIT_TEST(px3.get() == p);
        KIT_TEST(px3.use_count() == 2);

        using std::swap;
        swap(px, px2);

        KIT_TEST(px.get() == 0);
        KIT_TEST(px2.get() == p);
        KIT_TEST(px2.use_count() == 2);
        KIT_TEST(px3.get() == p);
        KIT_TEST(px3.use_count() == 2);
    }

    {
        X * p1 = new X;
        X * p2 = new X;
        kit::local_shared_ptr<X> px(p1);
        kit::local_shared_ptr<X> px2(p2);
        kit::local_shared_ptr<X> px3(px2);

        px.swap(px2);

        KIT_TEST(px.get() == p2);
        KIT_TEST(px.use_count() == 2);
        KIT_TEST(px2.get() == p1);
        KIT_TEST(px2.use_count() == 1);
        KIT_TEST(px3.get() == p2);
        KIT_TEST(px3.use_count() == 2);

        using std::swap;
        swap(px, px2);

        KIT_TEST(px.get() == p1);
        KIT_TEST(px.use_count() == 1);
        KIT_TEST(px2.get() == p2);
        KIT_TEST(px2.use_count() == 2);
        KIT_TEST(px3.get() == p2);
        KIT_TEST(px3.use_count() == 2);
    }
}

} // namespace n_swap

namespace n_comparison
{

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

void test()
{
    {
        kit::local_shared_ptr<X> px;
        KIT_TEST(px == px);
        KIT_TEST(!(px != px));
        KIT_TEST(!(px < px));

        kit::local_shared_ptr<X> px2;

        KIT_TEST(px.get() == px2.get());
        KIT_TEST(px == px2);
        KIT_TEST(!(px != px2));
        KIT_TEST(!(px < px2 && px2 < px));
    }

    {
        kit::local_shared_ptr<X> px;
        kit::local_shared_ptr<X> px2(px);

        KIT_TEST(px2 == px2);
        KIT_TEST(!(px2 != px2));
        KIT_TEST(!(px2 < px2));

        KIT_TEST(px.get() == px2.get());
        KIT_TEST(px == px2);
        KIT_TEST(!(px != px2));
        KIT_TEST(!(px < px2 && px2 < px));
    }

    {
        kit::local_shared_ptr<X> px;
        kit::local_shared_ptr<X> px2(new X);

        KIT_TEST(px2 == px2);
        KIT_TEST(!(px2 != px2));
        KIT_TEST(!(px2 < px2));

        KIT_TEST(px.get() != px2.get());
        KIT_TEST(px != px2);
        KIT_TEST(!(px == px2));
        KIT_TEST(px < px2 || px2 < px);
        KIT_TEST(!(px < px2 && px2 < px));
    }

    {
        kit::local_shared_ptr<X> px(new X);
        kit::local_shared_ptr<X> px2(new X);

        KIT_TEST(px.get() != px2.get());
        KIT_TEST(px != px2);
        KIT_TEST(!(px == px2));
        KIT_TEST(px < px2 || px2 < px);
        KIT_TEST(!(px < px2 && px2 < px));
    }

    {
        kit::local_shared_ptr<X> px(new X);
        kit::local_shared_ptr<X> px2(px);

        KIT_TEST(px2 == px2);
        KIT_TEST(!(px2 != px2));
        KIT_TEST(!(px2 < px2));

        KIT_TEST(px.get() == px2.get());
        KIT_TEST(px == px2);
        KIT_TEST(!(px != px2));
        KIT_TEST(!(px < px2 || px2 < px));
    }

    {
        kit::local_shared_ptr<X> px(new X);
        kit::local_shared_ptr<Y> py(new Y);
        kit::local_shared_ptr<Z> pz(new Z);

        KIT_TEST(px.get() != pz.get());
        KIT_TEST(px != pz);
        KIT_TEST(!(px == pz));

        KIT_TEST(py.get() != pz.get());
        KIT_TEST(py != pz);
        KIT_TEST(!(py == pz));

        KIT_TEST(px < py || py < px);
        KIT_TEST(px < pz || pz < px);
        KIT_TEST(py < pz || pz < py);

        KIT_TEST(!(px < py && py < px));
        KIT_TEST(!(px < pz && pz < px));
        KIT_TEST(!(py < pz && pz < py));

        kit::local_shared_ptr<void> pvx(px);

        KIT_TEST(pvx == pvx);
        KIT_TEST(!(pvx != pvx));
        KIT_TEST(!(pvx < pvx));

        kit::local_shared_ptr<void> pvy(py);
        kit::local_shared_ptr<void> pvz(pz);

        KIT_TEST(pvx < pvy || pvy < pvx);
        KIT_TEST(pvx < pvz || pvz < pvx);
        KIT_TEST(pvy < pvz || pvz < pvy);

        KIT_TEST(!(pvx < pvy && pvy < pvx));
        KIT_TEST(!(pvx < pvz && pvz < pvx));
        KIT_TEST(!(pvy < pvz && pvz < pvy));
    }

    {
        kit::local_shared_ptr<Z> pz(new Z);
        kit::local_shared_ptr<X> px(pz);

        KIT_TEST(px == px);
        KIT_TEST(!(px != px));
        KIT_TEST(!(px < px));

        kit::local_shared_ptr<Y> py(pz);

        KIT_TEST(px.get() == pz.get());
        KIT_TEST(px == pz);
        KIT_TEST(!(px != pz));

        KIT_TEST(py.get() == pz.get());
        KIT_TEST(py == pz);
        KIT_TEST(!(py != pz));

        KIT_TEST(!(px < py || py < px));
        KIT_TEST(!(px < pz || pz < px));
        KIT_TEST(!(py < pz || pz < py));

        kit::local_shared_ptr<void> pvx(px);
        kit::local_shared_ptr<void> pvy(py);
        kit::local_shared_ptr<void> pvz(pz);

        // pvx and pvy aren't equal...
        KIT_TEST(pvx.get() != pvy.get());
        KIT_TEST(pvx != pvy);
        KIT_TEST(!(pvx == pvy));

        // ... but they share ownership ...
        KIT_TEST(!(pvx < pvy || pvy < pvx));

        // ... with pvz
        KIT_TEST(!(pvx < pvz || pvz < pvx));
        KIT_TEST(!(pvy < pvz || pvz < pvy));
    }
}

} // namespace n_comparison

namespace n_static_cast
{

struct X
{
};

struct Y: public X
{
};

void test()
{
    {
        kit::local_shared_ptr<void> pv;

        kit::local_shared_ptr<int> pi = kit::static_pointer_cast<int>(pv);
        KIT_TEST(pi.get() == 0);

        kit::local_shared_ptr<X> px = kit::static_pointer_cast<X>(pv);
        KIT_TEST(px.get() == 0);
    }

    {
        kit::local_shared_ptr<int> pi(new int);
        kit::local_shared_ptr<void> pv(pi);

        kit::local_shared_ptr<int> pi2 = kit::static_pointer_cast<int>(pv);
        KIT_TEST(pi.get() == pi2.get());
        KIT_TEST(!(pi < pi2 || pi2 < pi));
        KIT_TEST(pi.use_count() == 3);
        KIT_TEST(pv.use_count() == 3);
        KIT_TEST(pi2.use_count() == 3);
    }

    {
        kit::local_shared_ptr<X> px(new X);
        kit::local_shared_ptr<void> pv(px);

        kit::local_shared_ptr<X> px2 = kit::static_pointer_cast<X>(pv);
        KIT_TEST(px.get() == px2.get());
        KIT_TEST(!(px < px2 || px2 < px));
        KIT_TEST(px.use_count() == 3);
        KIT_TEST(pv.use_count() == 3);
        KIT_TEST(px2.use_count() == 3);
    }

    {
        kit::local_shared_ptr<X> px(new Y);

        kit::local_shared_ptr<Y> py = kit::static_pointer_cast<Y>(px);
        KIT_TEST(px.get() == py.get());
        KIT_TEST(px.use_count() == 2);
        KIT_TEST(py.use_count() == 2);

        kit::local_shared_ptr<X> px2(py);
        KIT_TEST(!(px < px2 || px2 < px));
    }
}

} // namespace n_static_cast

namespace n_const_cast
{

struct X;

void test()
{
    {
        kit::local_shared_ptr<void const volatile> px;

        kit::local_shared_ptr<void> px2 = kit::const_pointer_cast<void>(px);
        KIT_TEST(px2.get() == 0);
    }

    {
        kit::local_shared_ptr<int const volatile> px;

        kit::local_shared_ptr<int> px2 = kit::const_pointer_cast<int>(px);
        KIT_TEST(px2.get() == 0);
    }

    {
        kit::local_shared_ptr<X const volatile> px;

        kit::local_shared_ptr<X> px2 = kit::const_pointer_cast<X>(px);
        KIT_TEST(px2.get() == 0);
    }

    {
        kit::local_shared_ptr<void const volatile> px(new int);

        kit::local_shared_ptr<void> px2 = kit::const_pointer_cast<void>(px);
        KIT_TEST(px.get() == px2.get());
        KIT_TEST(!(px < px2 || px2 < px));
        KIT_TEST(px.use_count() == 2);
        KIT_TEST(px2.use_count() == 2);
    }

    {
        kit::local_shared_ptr<int const volatile> px(new int);

        kit::local_shared_ptr<int> px2 = kit::const_pointer_cast<int>(px);
        KIT_TEST(px.get() == px2.get());
        KIT_TEST(!(px < px2 || px2 < px));
        KIT_TEST(px.use_count() == 2);
        KIT_TEST(px2.use_count() == 2);
    }
}

} // namespace n_const_cast

#if !defined( KIT_NO_RTTI )

namespace n_dynamic_cast
{

struct V
{
    virtual ~V() {}
};

struct W: public V
{
};

void test()
{
    {
        kit::local_shared_ptr<V> pv;
        kit::local_shared_ptr<W> pw = kit::dynamic_pointer_cast<W>(pv);
        KIT_TEST(pw.get() == 0);
    }

    {
        kit::local_shared_ptr<V> pv(static_cast<V*>(0));

        kit::local_shared_ptr<W> pw = kit::dynamic_pointer_cast<W>(pv);
        KIT_TEST(pw.get() == 0);

        kit::local_shared_ptr<V> pv2(pw);
        KIT_TEST(pv < pv2 || pv2 < pv);
    }

    {
        kit::local_shared_ptr<V> pv(static_cast<W*>(0));

        kit::local_shared_ptr<W> pw = kit::dynamic_pointer_cast<W>(pv);
        KIT_TEST(pw.get() == 0);

        kit::local_shared_ptr<V> pv2(pw);
        KIT_TEST(pv < pv2 || pv2 < pv);
    }

    {
        kit::local_shared_ptr<V> pv(new V);

        kit::local_shared_ptr<W> pw = kit::dynamic_pointer_cast<W>(pv);
        KIT_TEST(pw.get() == 0);

        kit::local_shared_ptr<V> pv2(pw);
        KIT_TEST(pv < pv2 || pv2 < pv);
    }

    {
        kit::local_shared_ptr<V> pv(new W);

        kit::local_shared_ptr<W> pw = kit::dynamic_pointer_cast<W>(pv);
        KIT_TEST(pw.get() == pv.get());
        KIT_TEST(pv.use_count() == 2);
        KIT_TEST(pw.use_count() == 2);

        kit::local_shared_ptr<V> pv2(pw);
        KIT_TEST(!(pv < pv2 || pv2 < pv));
    }
}

} // namespace n_dynamic_cast

#endif

namespace n_map
{

struct X
{
};

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

    std::map< kit::local_shared_ptr<void>, long > m;

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
        for(std::map< kit::local_shared_ptr<void>, long >::iterator i = m.begin(); i != m.end(); ++i)
        {
            KIT_TEST(i->first.use_count() == i->second + 1);
        }
    }
}

} // namespace n_map

namespace n_transitive
{

struct X
{
    X(): next() {}
    kit::local_shared_ptr<X> next;
};

void test()
{
    kit::local_shared_ptr<X> p(new X);
    p->next = kit::local_shared_ptr<X>(new X);
    KIT_TEST(!p->next->next);
    p = p->next;
    KIT_TEST(!p->next);
}

} // namespace n_transitive

namespace n_report_1
{

class foo
{ 
public: 

    foo(): m_self(this)
    {
    } 

    void suicide()
    {
        m_self.reset();
    }

private:

    kit::local_shared_ptr<foo> m_self;
}; 

void test()
{
    foo * foo_ptr = new foo;
    foo_ptr->suicide();
} 

} // namespace n_report_1

// Test case by Per Kristensen
namespace n_report_2
{

class foo
{
public:

    void setWeak(kit::local_shared_ptr<foo> s)
    {
        w = s;
    }

private:

    kit::local_weak_ptr<foo> w;
};
 
class deleter
{
public:

    deleter(): lock(0)
    {
    }

    ~deleter()
    {
        KIT_TEST(lock == 0);
    }

    void operator() (foo * p)
    {
        ++lock;
        delete p;
        --lock;
    }
 
private:

    int lock;
};
 
void test()
{
    kit::local_shared_ptr<foo> s(new foo, deleter());
    s->setWeak(s);
    s.reset();
}

} // namespace n_report_2

namespace n_spt_incomplete
{

class file;

kit::local_shared_ptr<file> fopen(char const * name, char const * mode);
void fread(kit::local_shared_ptr<file> f, void * data, long size);

int file_instances = 0;

void test()
{
    KIT_TEST(file_instances == 0);

    {
        kit::local_shared_ptr<file> pf = fopen("name", "mode");
        KIT_TEST(file_instances == 1);
        fread(pf, 0, 17041);
    }

    KIT_TEST(file_instances == 0);
}

} // namespace n_spt_incomplete

namespace n_spt_pimpl
{

class file
{
private:

    class impl;
    kit::local_shared_ptr<impl> pimpl_;

public:

    file(char const * name, char const * mode);

    // compiler generated members are fine and useful

    void read(void * data, long size);

    long total_size() const;
};

int file_instances = 0;

void test()
{
    KIT_TEST(file_instances == 0);

    {
        file f("name", "mode");
        KIT_TEST(file_instances == 1);
        f.read(0, 152);

        file f2(f);
        KIT_TEST(file_instances == 1);
        f2.read(0, 894);

        KIT_TEST(f.total_size() == 152+894);

        {
            file f3("name2", "mode2");
            KIT_TEST(file_instances == 2);
        }

        KIT_TEST(file_instances == 1);
    }

    KIT_TEST(file_instances == 0);
}

} // namespace n_spt_pimpl

namespace n_spt_abstract
{

class X
{
public:

    virtual void f(int) = 0;
    virtual int g() = 0;

protected:

    ~X() {}
};

kit::local_shared_ptr<X> createX();

int X_instances = 0;

void test()
{
    KIT_TEST(X_instances == 0);

    {
        kit::local_shared_ptr<X> px = createX();

        KIT_TEST(X_instances == 1);

        px->f(18);
        px->f(152);

        KIT_TEST(px->g() == 170);
    }

    KIT_TEST(X_instances == 0);
}

} // namespace n_spt_abstract

namespace n_spt_preventing_delete
{

int X_instances = 0;

class X
{
private:

    X()
    {
        ++X_instances;
    }

    ~X()
    {
        --X_instances;
    }

    class deleter;
    friend class deleter;

    class deleter
    {
    public:

        void operator()(X * p) { delete p; }
    };

public:

    static kit::local_shared_ptr<X> create()
    {
        kit::local_shared_ptr<X> px(new X, X::deleter());
        return px;
    }
};

void test()
{
    KIT_TEST(X_instances == 0);

    {
        kit::local_shared_ptr<X> px = X::create();
        KIT_TEST(X_instances == 1);
    }

    KIT_TEST(X_instances == 0);
}

} // namespace n_spt_preventing_delete

namespace n_spt_array
{

int X_instances = 0;

struct X
{
    X()
    {
        ++X_instances;
    }

    ~X()
    {
        --X_instances;
    }
};

void test()
{
    KIT_TEST(X_instances == 0);

    {
        kit::local_shared_ptr<X> px(new X[4], kit::checked_array_deleter<X>());
        KIT_TEST(X_instances == 4);
    }

    KIT_TEST(X_instances == 0);
}

} // namespace n_spt_array

namespace n_spt_static
{

class X
{
public:

    X()
    {
    }

private:

    void operator delete(void *)
    {
        // Comeau 4.3.0.1 wants a definition
        KIT_ERROR("n_spt_static::X::operator delete() called.");
    }
};

struct null_deleter
{
    void operator()(void const *) const
    {
    }
};

static X x;

void test()
{
    kit::local_shared_ptr<X> px(&x, null_deleter());
}

} // namespace n_spt_static

namespace n_spt_intrusive
{

int X_instances = 0;

struct X
{
    long count;

    X(): count(0)
    {
        ++X_instances;
    }

    ~X()
    {
        --X_instances;
    }
};

void intrusive_ptr_add_ref(X * p)
{
    ++p->count;
}

void intrusive_ptr_release(X * p)
{
    if(--p->count == 0) delete p;
}

template<class T> struct intrusive_deleter
{
    void operator()(T * p)
    {
        if(p != 0) intrusive_ptr_release(p);
    }
};

kit::local_shared_ptr<X> make_local_shared_from_intrusive(X * p)
{
    if(p != 0) intrusive_ptr_add_ref(p);
    kit::local_shared_ptr<X> px(p, intrusive_deleter<X>());
    return px;
}

void test()
{
    KIT_TEST(X_instances == 0);

    {
        X * p = new X;
        KIT_TEST(X_instances == 1);
        KIT_TEST(p->count == 0);
        kit::local_shared_ptr<X> px = make_local_shared_from_intrusive(p);
        KIT_TEST(px.get() == p);
        KIT_TEST(p->count == 1);
        kit::local_shared_ptr<X> px2(px);
        KIT_TEST(px2.get() == p);
        KIT_TEST(p->count == 1);
    }

    KIT_TEST(X_instances == 0);
}

} // namespace n_spt_intrusive

namespace n_spt_another_sp
{

template<class T> class another_ptr: private kit::local_shared_ptr<T>
{
private:

    typedef kit::local_shared_ptr<T> base_type;

public:

    explicit another_ptr(T * p = 0): base_type(p)
    {
    }

    void reset()
    {
        base_type::reset();
    }

    T * get() const
    {
        return base_type::get();
    }
};

class event_handler
{
public:

    virtual ~event_handler() {}
    virtual void begin() = 0;
    virtual void handle(int event) = 0;
    virtual void end() = 0;
};

int begin_called = 0;
int handle_called = 0;
int end_called = 0;

class event_handler_impl: public event_handler
{
public:

    virtual void begin()
    {
        ++begin_called;
    }

    virtual void handle(int event)
    {
        handle_called = event;
    }

    virtual void end()
    {
        ++end_called;
    }
};

another_ptr<event_handler> get_event_handler()
{
    another_ptr<event_handler> p(new event_handler_impl);
    return p;
}

kit::local_shared_ptr<event_handler> current_handler;

void install_event_handler(kit::local_shared_ptr<event_handler> p)
{
    p->begin();
    current_handler = p;
}

void handle_event(int event)
{
    current_handler->handle(event);
}

void remove_event_handler()
{
    current_handler->end();
    current_handler.reset();
}

template<class P> class smart_pointer_deleter
{
private:

    P p_;

public:

    smart_pointer_deleter(P const & p): p_(p)
    {
    }

    void operator()(void const *)
    {
        p_.reset();
    }
};

void test()
{
    another_ptr<event_handler> p = get_event_handler();

    kit::local_shared_ptr<event_handler> q(p.get(), smart_pointer_deleter< another_ptr<event_handler> >(p));

    p.reset();

    KIT_TEST(begin_called == 0);

    install_event_handler(q);

    KIT_TEST(begin_called == 1);

    KIT_TEST(handle_called == 0);

    handle_event(17041);

    KIT_TEST(handle_called == 17041);

    KIT_TEST(end_called == 0);

    remove_event_handler();

    KIT_TEST(end_called == 1);
}

} // namespace n_spt_another_sp

namespace n_spt_shared_from_this
{

class X
{
public:

    virtual void f() = 0;

protected:

    ~X() {}
};

class Y
{
public:

    virtual kit::local_shared_ptr<X> getX() = 0;

protected:

    ~Y() {}
};

class impl: public X, public Y
{
private:

    kit::local_weak_ptr<impl> weak_this;

    impl(impl const &);
    impl & operator=(impl const &);

    impl() {}

public:

    static kit::local_shared_ptr<impl> create()
    {
        kit::local_shared_ptr<impl> pi(new impl);
        pi->weak_this = pi;
        return pi;
    }

    virtual void f() {}

    virtual kit::local_shared_ptr<X> getX()
    {
        kit::local_shared_ptr<X> px = weak_this.lock();
        return px;
    }
};

void test()
{
    kit::local_shared_ptr<Y> py = impl::create();
    KIT_TEST(py.get() != 0);
    KIT_TEST(py.use_count() == 1);

    kit::local_shared_ptr<X> px = py->getX();
    KIT_TEST(px.get() != 0);
    KIT_TEST(py.use_count() == 2);

#if !defined( KIT_NO_RTTI )
    kit::local_shared_ptr<Y> py2 = kit::dynamic_pointer_cast<Y>(px);
    KIT_TEST(py.get() == py2.get());
    KIT_TEST(!(py < py2 || py2 < py));
    KIT_TEST(py.use_count() == 3);
#endif
}

} // namespace n_spt_shared_from_this

namespace n_spt_wrap
{

void test()
{
}

} // namespace n_spt_wrap

int main()
{
    n_element_type::test();
    n_constructors::test();
    n_assignment::test();
    n_reset::test();
    n_access::test();
    n_use_count::test();
    n_swap::test();
    n_comparison::test();
    n_static_cast::test();
    n_const_cast::test();
#if !defined( KIT_NO_RTTI )
    n_dynamic_cast::test();
#endif

    n_map::test();

    n_transitive::test();
    n_report_1::test();
    n_report_2::test();

    n_spt_incomplete::test();
    n_spt_pimpl::test();
    n_spt_abstract::test();
    n_spt_preventing_delete::test();
    n_spt_array::test();
    n_spt_static::test();
    n_spt_intrusive::test();
    n_spt_another_sp::test();
    n_spt_shared_from_this::test();
    n_spt_wrap::test();

    return kit::report_errors();
}

namespace n_spt_incomplete
{

class file
{
public:

    file(): fread_called(false)
    {
        ++file_instances;
    }

    ~file()
    {
        KIT_TEST(fread_called);
        --file_instances;
    }

    bool fread_called;
};

kit::local_shared_ptr<file> fopen(char const *, char const *)
{
    kit::local_shared_ptr<file> pf(new file);
    return pf;
}

void fread(kit::local_shared_ptr<file> pf, void *, long)
{
    pf->fread_called = true;
}

} // namespace n_spt_incomplete

namespace n_spt_pimpl
{

class file::impl
{
private:

    impl(impl const &);
    impl & operator=(impl const &);

    long total_size_;

public:

    impl(char const *, char const *): total_size_(0)
    {
        ++file_instances;
    }

    ~impl()
    {
        --file_instances;
    }

    void read(void *, long size)
    {
        total_size_ += size;
    }

    long total_size() const
    {
        return total_size_;
    }
};

file::file(char const * name, char const * mode): pimpl_(new impl(name, mode))
{
}

void file::read(void * data, long size)
{
    pimpl_->read(data, size);
}

long file::total_size() const
{
    return pimpl_->total_size();
}

} // namespace n_spt_pimpl

namespace n_spt_abstract
{

class X_impl: public X
{
private:

    X_impl(X_impl const &);
    X_impl & operator=(X_impl const &);

    int n_;

public:

    X_impl(): n_(0)
    {
        ++X_instances;
    }

    ~X_impl()
    {
        --X_instances;
    }

    virtual void f(int n)
    {
        n_ += n;
    }

    virtual int g()
    {
        return n_;
    }
};

kit::local_shared_ptr<X> createX()
{
    kit::local_shared_ptr<X> px(new X_impl);
    return px;
}

} // namespace n_spt_abstract

}
