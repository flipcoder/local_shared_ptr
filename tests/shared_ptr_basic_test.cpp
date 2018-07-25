//
//  local_shared_ptr_basic_test.cpp
//
//  Copyright (c) 2001, 2002 Peter Dimov and Multi Media Ltd.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include "lightweight_test.hpp"

#include <kit/local_shared_ptr.hpp>
#include <kit/local_weak_ptr.hpp>

namespace local_shared_ptr_basic_test {

int cnt = 0;

struct X
{
    X()
    {
        ++cnt;
    }

    ~X() // virtual destructor deliberately omitted
    {
        --cnt;
    }

    virtual int id() const
    {
        return 1;
    }

private:

    X(X const &);
    X & operator= (X const &);
};

struct Y: public X
{
    Y()
    {
        ++cnt;
    }

    ~Y()
    {
        --cnt;
    }

    virtual int id() const
    {
        return 2;
    }

private:

    Y(Y const &);
    Y & operator= (Y const &);
};

int * get_object()
{
    ++cnt;
    return &cnt;
}

void release_object(int * p)
{
    KIT_TEST(p == &cnt);
    --cnt;
}

template<class T> void test_is_X(kit::local_shared_ptr<T> const & p)
{
    KIT_TEST(p->id() == 1);
    KIT_TEST((*p).id() == 1);
}

template<class T> void test_is_X(kit::local_weak_ptr<T> const & p)
{
    KIT_TEST(p.get() != 0);
    KIT_TEST(p.get()->id() == 1);
}

template<class T> void test_is_Y(kit::local_shared_ptr<T> const & p)
{
    KIT_TEST(p->id() == 2);
    KIT_TEST((*p).id() == 2);
}

template<class T> void test_is_Y(kit::local_weak_ptr<T> const & p)
{
    kit::local_shared_ptr<T> q = p.lock();
    KIT_TEST(q.get() != 0);
    KIT_TEST(q->id() == 2);
}

template<class T> void test_eq(T const & a, T const & b)
{
    KIT_TEST(a == b);
    KIT_TEST(!(a != b));
    KIT_TEST(!(a < b));
    KIT_TEST(!(b < a));
}

template<class T> void test_ne(T const & a, T const & b)
{
    KIT_TEST(!(a == b));
    KIT_TEST(a != b);
    KIT_TEST(a < b || b < a);
    KIT_TEST(!(a < b && b < a));
}

template<class T, class U> void test_shared(kit::local_weak_ptr<T> const & a, kit::local_weak_ptr<U> const & b)
{
    KIT_TEST(!(a < b));
    KIT_TEST(!(b < a));
}

template<class T, class U> void test_nonshared(kit::local_weak_ptr<T> const & a, kit::local_weak_ptr<U> const & b)
{
    KIT_TEST(a < b || b < a);
    KIT_TEST(!(a < b && b < a));
}

template<class T, class U> void test_eq2(T const & a, U const & b)
{
    KIT_TEST(a == b);
    KIT_TEST(!(a != b));
}

template<class T, class U> void test_ne2(T const & a, U const & b)
{
    KIT_TEST(!(a == b));
    KIT_TEST(a != b);
}

template<class T> void test_is_zero(kit::local_shared_ptr<T> const & p)
{
    KIT_TEST(!p);
    KIT_TEST(p.get() == 0);
}

template<class T> void test_is_nonzero(kit::local_shared_ptr<T> const & p)
{
    // p? true: false is used to test p in a boolean context.
    // KIT_TEST(p) is not guaranteed to test the conversion,
    // as the macro might test !!p instead.
    KIT_TEST(p? true: false);
    KIT_TEST(p.get() != 0);
}

int main()
{
    using namespace kit;

    {
        local_shared_ptr<X> p(new Y);
        local_shared_ptr<X> p2(new X);

        test_is_nonzero(p);
        test_is_nonzero(p2);
        test_is_Y(p);
        test_is_X(p2);
        test_ne(p, p2);

        {
            local_shared_ptr<X> q(p);
            test_eq(p, q);
        }

#if !defined( KIT_NO_RTTI )
        local_shared_ptr<Y> p3 = dynamic_pointer_cast<Y>(p);
        local_shared_ptr<Y> p4 = dynamic_pointer_cast<Y>(p2);

        test_is_nonzero(p3);
        test_is_zero(p4);

        KIT_TEST(p.use_count() == 2);
        KIT_TEST(p2.use_count() == 1);
        KIT_TEST(p3.use_count() == 2);

        test_is_Y(p3);
        test_eq2(p, p3);
        test_ne2(p2, p4);
#endif

        local_shared_ptr<void> p5(p);

        test_is_nonzero(p5);
        test_eq2(p, p5);

        local_weak_ptr<X> wp1(p2);

        KIT_TEST(!wp1.expired());
        KIT_TEST(wp1.use_count() != 0);

        p.reset();
        p2.reset();
#if !defined( KIT_NO_RTTI )
        p3.reset();
        p4.reset();
#endif

        test_is_zero(p);
        test_is_zero(p2);
#if !defined( KIT_NO_RTTI )
        test_is_zero(p3);
        test_is_zero(p4);
#endif

        KIT_TEST(p5.use_count() == 1);

        KIT_TEST(wp1.expired());
        KIT_TEST(wp1.use_count() == 0);

        try
        {
            local_shared_ptr<X> sp1(wp1);
            KIT_ERROR("shared_ptr<X> sp1(wp1) failed to throw");
        }
        catch(kit::bad_local_weak_ptr const &)
        {
        }

        test_is_zero(wp1.lock());

        local_weak_ptr<X> wp2 = static_pointer_cast<X>(p5);

        KIT_TEST(wp2.use_count() == 1);
        test_is_Y(wp2);
        test_nonshared(wp1, wp2);

        // Scoped to not affect the subsequent use_count() tests.
        {
            local_shared_ptr<X> sp2(wp2);
            test_is_nonzero(wp2.lock());
        }

#if !defined( KIT_NO_RTTI )
        local_weak_ptr<Y> wp3 = dynamic_pointer_cast<Y>(wp2.lock());

        KIT_TEST(wp3.use_count() == 1);
        test_shared(wp2, wp3);

        local_weak_ptr<X> wp4(wp3);

        KIT_TEST(wp4.use_count() == 1);
        test_shared(wp2, wp4);
#endif

        wp1 = p2;
        test_is_zero(wp1.lock());

#if !defined( KIT_NO_RTTI )
        wp1 = p4;
        wp1 = wp3;
#endif
        wp1 = wp2;

        KIT_TEST(wp1.use_count() == 1);
        test_shared(wp1, wp2);

        local_weak_ptr<X> wp5;

        bool b1 = wp1 < wp5;
        bool b2 = wp5 < wp1;

        p5.reset();

        KIT_TEST(wp1.use_count() == 0);
        KIT_TEST(wp2.use_count() == 0);
#if !defined( KIT_NO_RTTI )
        KIT_TEST(wp3.use_count() == 0);
#endif

        // Test operator< stability for std::set< local_weak_ptr<> >
        // Thanks to Joe Gottman for pointing this out

        KIT_TEST(b1 == (wp1 < wp5));
        KIT_TEST(b2 == (wp5 < wp1));

        {
            // note that both get_object and release_object deal with int*
            local_shared_ptr<void> p6(get_object(), release_object);
        }
    }

    KIT_TEST(cnt == 0);

    return kit::report_errors();
}

}
