#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

#include "list.hpp"


// Hilfsfunktionen, um indirekt auf die Member der Klasse List zuzugreifen (nur für die Tests!)
template <typename T>
size_t get_size(List<T> const& list_to_test) {return list_to_test.size_;}

template <typename T>
ListNode<T>* get_first_pointer(List<T> const& list_to_test) {return list_to_test.first_;}

template <typename T>
ListNode<T>* get_last_pointer(List<T> const& list_to_test) {return list_to_test.last_;}


//test case definitions for default constructor of list as well as empty and size for empty list
#include "sub_tests/default_constructed_list.test"

//test case definitions for push methods of list
#include "sub_tests/push_front.test"
#include "sub_tests/push_back.test"

//test case definitions for pop methods of list
#include "sub_tests/pop_front.test"
#include "sub_tests/pop_back.test"

//test case definitions for 

//test cases for element access of list
#include "sub_tests/front.test"
#include "sub_tests/back.test"

//test cases for method clear
#include "sub_tests/clear.test"

//test cases for (deep) copy-constructor
#include "sub_tests/copy_constructor.test"

//test cases for reverse() method and reverse() free function
#include "sub_tests/reverse.test"

//test cases for operator== and operator!=
#include "sub_tests/equality.test" 

//test cases for retrieving iterators
#include "sub_tests/begin.test"
#include "sub_tests/end.test"

//iterator tests
#include "sub_tests/iterators/operator_star.test"
#include "sub_tests/iterators/operator_arrow.test"
#include "sub_tests/iterators/operator_equals.test"
#include "sub_tests/iterators/operator_does_not_equal.test"
#include "sub_tests/iterators/operator_iterate_forward.test" 

//insert tests
#include "sub_tests/insert.test"

//erase tests
#include "sub_tests/erase.test"

//test has_same_content method for list and vector
#include "sub_tests/has_same_content.test"

//test the move constructor
#include "sub_tests/move_constructor.test"

//test the initializer list constructor
#include "sub_tests/ini_list_constructor.test"

//test the free function operator+ for lists
#include "sub_tests/concatenate_lists.test"


int main(int argc, char *argv[])
{
  return Catch::Session().run(argc, argv);
}




