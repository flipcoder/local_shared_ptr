#ifndef KIT_CORE_LIGHTWEIGHT_TEST_HPP
#define KIT_CORE_LIGHTWEIGHT_TEST_HPP

// MS compatible compilers support #pragma once

#if defined(_MSC_VER)
# pragma once
#endif

//
//  boost/core/lightweight_test.hpp - lightweight test library
//
//  Copyright (c) 2002, 2009, 2014 Peter Dimov
//  Copyright (2) Beman Dawes 2010, 2011
//  Copyright (3) Ion Gaztanaga 2013
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//

//#include <kit/assert.hpp>
//#include <kit/current_function.hpp>
//#include <kit/core/no_exceptions_support.hpp>
#include <iostream>
#include <cassert>

#define KIT_CURRENT_FUNCTION __func__
#define KIT_ASSERT(x) assert(x)

//  IDE's like Visual Studio perform better if output goes to std::cout or
//  some other stream, so allow user to configure output stream:
#ifndef KIT_LIGHTWEIGHT_TEST_OSTREAM
# define KIT_LIGHTWEIGHT_TEST_OSTREAM std::cerr
#endif

namespace kit
{

namespace detail
{

struct report_errors_reminder
{
    bool called_report_errors_function;

    report_errors_reminder() : called_report_errors_function(false) {}

    ~report_errors_reminder()
    {
        KIT_ASSERT(called_report_errors_function);  // verify report_errors() was called  
    }
};

inline report_errors_reminder& report_errors_remind()
{
    static report_errors_reminder r;
    return r;
}

inline int & test_errors()
{
    static int x = 0;
    report_errors_remind();
    return x;
}

inline void test_failed_impl(char const * expr, char const * file, int line, char const * function)
{
    KIT_LIGHTWEIGHT_TEST_OSTREAM
      << file << "(" << line << "): test '" << expr << "' failed in function '"
      << function << "'" << std::endl;
    ++test_errors();
}

inline void error_impl(char const * msg, char const * file, int line, char const * function)
{
    KIT_LIGHTWEIGHT_TEST_OSTREAM
      << file << "(" << line << "): " << msg << " in function '"
      << function << "'" << std::endl;
    ++test_errors();
}

inline void throw_failed_impl(char const * excep, char const * file, int line, char const * function)
{
   KIT_LIGHTWEIGHT_TEST_OSTREAM
    << file << "(" << line << "): Exception '" << excep << "' not thrown in function '"
    << function << "'" << std::endl;
   ++test_errors();
}

template<class T, class U> inline void test_eq_impl( char const * expr1, char const * expr2,
  char const * file, int line, char const * function, T const & t, U const & u )
{
    if( t == u )
    {
        report_errors_remind();
    }
    else
    {
        KIT_LIGHTWEIGHT_TEST_OSTREAM
            << file << "(" << line << "): test '" << expr1 << " == " << expr2
            << "' failed in function '" << function << "': "
            << "'" << t << "' != '" << u << "'" << std::endl;
        ++test_errors();
    }
}

template<class T, class U> inline void test_ne_impl( char const * expr1, char const * expr2,
  char const * file, int line, char const * function, T const & t, U const & u )
{
    if( t != u )
    {
        report_errors_remind();
    }
    else
    {
        KIT_LIGHTWEIGHT_TEST_OSTREAM
            << file << "(" << line << "): test '" << expr1 << " != " << expr2
            << "' failed in function '" << function << "': "
            << "'" << t << "' == '" << u << "'" << std::endl;
        ++test_errors();
    }
}

} // namespace detail

inline int report_errors()
{
    kit::detail::report_errors_remind().called_report_errors_function = true;

    int errors = kit::detail::test_errors();

    if( errors == 0 )
    {
        KIT_LIGHTWEIGHT_TEST_OSTREAM
          << "No errors detected." << std::endl;
        return 0;
    }
    else
    {
        KIT_LIGHTWEIGHT_TEST_OSTREAM
          << errors << " error" << (errors == 1? "": "s") << " detected." << std::endl;
        return 1;
    }
}

} // namespace kit

#define KIT_TEST(expr) ((expr)? (void)0: ::kit::detail::test_failed_impl(#expr, __FILE__, __LINE__, KIT_CURRENT_FUNCTION))

#define KIT_ERROR(msg) ( ::kit::detail::error_impl(msg, __FILE__, __LINE__, KIT_CURRENT_FUNCTION) )

#define KIT_TEST_EQ(expr1,expr2) ( ::kit::detail::test_eq_impl(#expr1, #expr2, __FILE__, __LINE__, KIT_CURRENT_FUNCTION, expr1, expr2) )
#define KIT_TEST_NE(expr1,expr2) ( ::kit::detail::test_ne_impl(#expr1, #expr2, __FILE__, __LINE__, KIT_CURRENT_FUNCTION, expr1, expr2) )

#ifndef KIT_NO_EXCEPTIONS
   #define KIT_TEST_THROWS( EXPR, EXCEP )                    \
      try {                                                    \
         EXPR;                                                 \
         ::kit::detail::throw_failed_impl                    \
         (#EXCEP, __FILE__, __LINE__, KIT_CURRENT_FUNCTION); \
      }                                                        \
      catch(EXCEP const&) {                                    \
      }                                                        \
      catch(...) {                                             \
         ::kit::detail::throw_failed_impl                    \
         (#EXCEP, __FILE__, __LINE__, KIT_CURRENT_FUNCTION); \
      }                                                        \
   //
#else
   #define KIT_TEST_THROWS( EXPR, EXCEP )
#endif

#endif // #ifndef KIT_CORE_LIGHTWEIGHT_TEST_HPP
