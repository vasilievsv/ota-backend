#include "minunit.h"

static int rc = 0;
static char *buff = NULL;

MU_TEST(test_reference_error){

}


MU_TEST(test_wrong_delimiter){

}

MU_TEST(test_wrong_size){

}

int main()
{
    MU_RUN_TEST(test_reference_error);
    MU_RUN_TEST(test_wrong_delimiter);
    MU_RUN_TEST(test_wrong_size);
    MU_REPORT();
    
    return MU_EXIT_CODE;
}