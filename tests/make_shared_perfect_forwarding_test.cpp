// make_local_shared_perfect_forwarding_test.cpp - a test of make_local_shared
//   perfect forwarding of constructor arguments when using a C++0x
//   compiler.
//
// Copyright 2009 Frank Mori Hess
//
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt

#include "lightweight_test.hpp"
#include <kit/make_local_shared.hpp>
#include <kit/local_shared_ptr.hpp>

namespace make_local_shared_perfect_forwarding_test {

class myarg
{
public:
    myarg()
    {}
private:
    myarg(myarg && other)
    {}
    myarg& operator=(myarg && other)
    {
        return *this;
    }
    myarg(const myarg & other)
    {}
    myarg& operator=(const myarg & other)
    {
        return *this;
    }
};

class X
{
public:
    enum constructor_id
    {
        move_constructor,
        const_ref_constructor,
        ref_constructor
    };

    X(myarg &&arg): constructed_by_(move_constructor)
    {}
    X(const myarg &arg): constructed_by_(const_ref_constructor)
    {}
    X(myarg &arg): constructed_by_(ref_constructor)
    {}

    constructor_id constructed_by_;
};

struct Y
{
    Y(int &value): ref(value)
    {}
    int &ref;
};

int main()
{
    {
        myarg a;
        kit::local_shared_ptr< X > x = kit::make_local_shared< X >(a);
        KIT_TEST( x->constructed_by_ == X::ref_constructor);
    }
    {
        const myarg ca;
        kit::local_shared_ptr< X > x = kit::make_local_shared< X >(ca);
        KIT_TEST( x->constructed_by_ == X::const_ref_constructor);
    }
    {
        kit::local_shared_ptr< X > x = kit::make_local_shared< X >(myarg());
        KIT_TEST( x->constructed_by_ == X::move_constructor);
    }
    {
        int value = 1;
        kit::local_shared_ptr< Y > y = kit::make_local_shared< Y >(value);
        KIT_TEST( y->ref == 1 && value == y->ref );
        ++y->ref;
        KIT_TEST( value == y->ref );
    }

    return kit::report_errors();
}

}
