#include <iostream>
#include <cassert>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <vector>
#include <map>
#include "tests.hpp"

int main(int argc, const char * argv[]) {
  allocate_shared_alloc11_test::main();
  allocate_shared_construct11_test::main();
  allocate_shared_esft_test::main();
  allocate_shared_test::main();
  esft_regtest::main();
  esft_second_ptr_test::main();
  esft_void_test::main();
  get_deleter_test::main();
  make_shared_esft_test::main();
  make_shared_perfect_forwarding_test::main();
  make_shared_test::main();
  shared_from_this_test::main();
  shared_ptr_alias_test::main();
  shared_ptr_alloc2_test::main();
  shared_ptr_alloc3_test::main();
  shared_ptr_alloc11_test::main();
  shared_ptr_basic_test::main();
  shared_ptr_convertible_test::main();
  shared_ptr_move_test::main();
  shared_ptr_rv_test::main();
  shared_ptr_test::main();
  smart_ptr_test::main();
  sp_nullptr_test::main();
  sp_recursive_assign_rv_test::main();
  sp_recursive_assign_test::main();
  sp_recursive_assign2_rv_test::main();
  sp_recursive_assign2_test::main();
  sp_unary_addr_test::main();
  sp_unique_ptr_test::main();
  sp_zero_compare_test::main();
  weak_from_this_test::main();
  weak_from_this_test2::main();
  weak_ptr_move_test::main();
  weak_ptr_test::main();
  wp_convertible_test::main();
}
