//
//  shared_from_this_test.cpp
//
//  Copyright (c) 2002, 2003 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//


#include <kit/enable_shared_from_this.hpp>
#include <kit/local_shared_ptr.hpp>

#include "lightweight_test.hpp"

//

namespace shared_from_this_test {

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

kit::local_shared_ptr<Y> createY();

void test()
{
    kit::local_shared_ptr<Y> py = createY();
    KIT_TEST(py.get() != 0);
    KIT_TEST(py.use_count() == 1);

    try
    {
        kit::local_shared_ptr<X> px = py->getX();
        KIT_TEST(px.get() != 0);
        KIT_TEST(py.use_count() == 2);

        px->f();

#if !defined( KIT_NO_RTTI )
        kit::local_shared_ptr<Y> py2 = kit::dynamic_pointer_cast<Y>(px);
        KIT_TEST(py.get() == py2.get());
        KIT_TEST(!(py < py2 || py2 < py));
        KIT_TEST(py.use_count() == 3);
#endif
    }
    catch( kit::bad_local_weak_ptr const& )
    {
        KIT_ERROR( "py->getX() failed" );
    }
}

void test2();
void test3();

int main()
{
    test();
    test2();
    test3();
    return kit::report_errors();
}

// virtual inheritance to stress the implementation
// (prevents Y* -> impl*, enable_shared_from_this<impl>* -> impl* casts)

class impl: public X, public virtual Y, public virtual kit::enable_shared_from_this<impl>
{
public:

    virtual void f()
    {
    }

    virtual kit::local_shared_ptr<X> getX()
    {
        kit::local_shared_ptr<impl> pi = shared_from_this();
        KIT_TEST(pi.get() == this);
        return pi;
    }
};

// intermediate impl2 to stress the implementation

class impl2: public impl
{
};

kit::local_shared_ptr<Y> createY()
{
    kit::local_shared_ptr<Y> pi(new impl2);
    return pi;
}

void test2()
{
    kit::local_shared_ptr<Y> pi(static_cast<impl2*>(0));
}

//

struct V: public kit::enable_shared_from_this<V>
{
};

void test3()
{
    kit::local_shared_ptr<V> p(new V);

    try
    {
        kit::local_shared_ptr<V> q = p->shared_from_this();
        KIT_TEST(p == q);
        KIT_TEST(!(p < q) && !(q < p));
    }
    catch( kit::bad_local_weak_ptr const & )
    {
        KIT_ERROR( "p->shared_from_this() failed" );
    }

    V v2(*p);

    try
    {
        kit::local_shared_ptr<V> r = v2.shared_from_this();
        KIT_ERROR("v2.shared_from_this() failed to throw");
    }
    catch( kit::bad_local_weak_ptr const & )
    {
    }

    try
    {
        *p = V();
        kit::local_shared_ptr<V> r = p->shared_from_this();
        KIT_TEST(p == r);
        KIT_TEST(!(p < r) && !(r < p));
    }
    catch( kit::bad_local_weak_ptr const & )
    {
        KIT_ERROR("p->shared_from_this() threw bad_local_weak_ptr after *p = V()");
    }
}

}
