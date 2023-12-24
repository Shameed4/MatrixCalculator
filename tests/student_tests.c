#include "unit_tests.h"
#include "hw6.h"

TestSuite(student_tests, .timeout=TEST_TIMEOUT); 
TestSuite(student_tests_valgrind, .timeout=TEST_TIMEOUT, .disabled=false);

/* execute_script_sf tests */
// 1. call execute_script_sf() to check for correctness()
// 2. call execute_script_sf() via valgrind to check for memory errors
Test(student_tests, a0) { run_script_without_valgrind("a0"); } // extra line
Test(student_tests_valgrind, a0) { expect_no_valgrind_errors(run_script_with_valgrind("a0")); }
Test(student_tests, a1) { run_script_without_valgrind("a1"); } // unusual spacing
Test(student_tests_valgrind, a1) { expect_no_valgrind_errors(run_script_with_valgrind("a1")); }
Test(student_tests, a2) { run_script_without_valgrind("a2"); } // different letters
Test(student_tests_valgrind, a2) { expect_no_valgrind_errors(run_script_with_valgrind("a2")); }
Test(student_tests, a3) { run_script_without_valgrind("a3"); } // large storage needed
Test(student_tests_valgrind, a3) { expect_no_valgrind_errors(run_script_with_valgrind("a3")); }