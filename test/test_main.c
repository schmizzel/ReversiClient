#include <stdio.h>
#include "misc/minunit.h"
#include "./test_utilities.c"

#include "./test_board_data_access.c"
#include "./test_gamesequence.c"
#include "./test_playground.c"


 int tests_run = 0;

 static char * all_tests() {
     mu_run_test_no_count(utilities_tests);
     mu_run_test_no_count(board_data_access_tests);
     mu_run_test_no_count(gamesequence_tests);
     mu_run_test_no_count(playground);
   return 0;
 }
 
 int main() {
     char *result = all_tests();
     if (result != 0) {
         printf("%s\n", result);
     } else {
         printf("ALL TESTS PASSED\n");
     }
     printf("Tests run: %d\n", tests_run);

     return result != 0;
 }