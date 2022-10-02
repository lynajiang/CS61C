#include "CUnit/CUnit.h"
#include "CUnit/Basic.h"
#include "../src/matrix.h"
#include <stdio.h>

/* Test Suite setup and cleanup functions: */
int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/************* Test case functions ****************/
 
// custom test
void alloc_custom_fail_test(void) {
  matrix *mat = NULL;
  CU_ASSERT_EQUAL(allocate_matrix(&mat, 0, 0), -1);
}

void get_test(void) {
  matrix *mat = NULL;
  allocate_matrix(&mat, 2, 2);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      set(mat, i, j, i * 2 + j);
    }
  }
  CU_ASSERT_EQUAL(get(mat, 0, 0), 0);
  CU_ASSERT_EQUAL(get(mat, 0, 1), 1);
  CU_ASSERT_EQUAL(get(mat, 1, 0), 2);
  CU_ASSERT_EQUAL(get(mat, 1, 1), 3);
  deallocate_matrix(mat);
}

void set_test(void) {
  matrix *mat = NULL;
  allocate_matrix(&mat, 2, 2);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      set(mat, i, j, i * 2 + j);
    }
  }
  CU_ASSERT_EQUAL(get(mat, 0, 0), 0);
  CU_ASSERT_EQUAL(get(mat, 0, 1), 1);
  CU_ASSERT_EQUAL(get(mat, 1, 0), 2);
  CU_ASSERT_EQUAL(get(mat, 1, 1), 3);
  deallocate_matrix(mat);
}

void transpose_test(void) {
  matrix *mat = NULL;
  allocate_matrix(&mat, 4, 3);

  matrix *result = NULL;
  allocate_matrix(&result, 3, 4);

  set(mat, 0, 0, 1);
  set(mat, 0, 1, 2);
  set(mat, 0, 2, 3);
  set(mat, 1, 0, 4);
  set(mat, 1, 1, 5);
  set(mat, 1, 2, 6);
  set(mat, 2, 0, 7);
  set(mat, 2, 1, 8);
  set(mat, 2, 2, 9);
  set(mat, 3, 0, 10);
  set(mat, 3, 1, 11);
  set(mat, 3, 2, 12);

  transpose_matrix(result, mat);

  CU_ASSERT_EQUAL(get(result, 0, 0), 1);
  CU_ASSERT_EQUAL(get(result, 1, 0), 2);
  CU_ASSERT_EQUAL(get(result, 2, 0), 3);
  CU_ASSERT_EQUAL(get(result, 0, 1), 4);
  CU_ASSERT_EQUAL(get(result, 1, 1), 5);
  CU_ASSERT_EQUAL(get(result, 2, 1), 6);
  CU_ASSERT_EQUAL(get(result, 0, 2), 7);
  CU_ASSERT_EQUAL(get(result, 1, 2), 8);
  CU_ASSERT_EQUAL(get(result, 2, 2), 9);
  CU_ASSERT_EQUAL(get(result, 0, 3), 10);
  CU_ASSERT_EQUAL(get(result, 1, 3), 11);
  CU_ASSERT_EQUAL(get(result, 2, 3), 12);
  deallocate_matrix(mat);
  deallocate_matrix(result);
}
void fill_test(void) {
  matrix *mat = NULL;
  allocate_matrix(&mat, 2, 2);
  fill_matrix(mat, 5);
  CU_ASSERT_EQUAL(get(mat, 0, 0), 5);
  CU_ASSERT_EQUAL(get(mat, 0, 1), 5);
  CU_ASSERT_EQUAL(get(mat, 1, 0), 5);
  CU_ASSERT_EQUAL(get(mat, 1, 1), 5);
  deallocate_matrix(mat);
}

void alloc_fail_test(void) {
  matrix *mat = NULL;
  CU_ASSERT_EQUAL(allocate_matrix(&mat, 0, 0), -1);
  CU_ASSERT_EQUAL(allocate_matrix(&mat, 0, 1), -1);
  CU_ASSERT_EQUAL(allocate_matrix(&mat, 1, 0), -1);
}

void alloc_success_test(void) {
  matrix *mat = NULL;
  CU_ASSERT_EQUAL(allocate_matrix(&mat, 3, 2), 0);
  CU_ASSERT_EQUAL(mat->parent, NULL);
  CU_ASSERT_EQUAL(mat->ref_cnt, 1);
  CU_ASSERT_EQUAL(mat->rows, 3);
  CU_ASSERT_EQUAL(mat->cols, 2);
  CU_ASSERT_NOT_EQUAL(mat->data, NULL);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 2; j++) {
      CU_ASSERT_EQUAL(get(mat, i, j), 0);
    }
  }
  deallocate_matrix(mat);
}

void alloc_ref_fail_test(void) {
  matrix *mat = NULL;
  matrix *from = NULL;
  CU_ASSERT_EQUAL(allocate_matrix_ref(&mat, from, 0, 0, 0), -1);
  CU_ASSERT_EQUAL(allocate_matrix_ref(&mat, from, 0, 0, 1), -1);
  CU_ASSERT_EQUAL(allocate_matrix_ref(&mat, from, 0, 1, 0), -1);
}

void alloc_ref_success_test(void) {
  matrix *mat = NULL;
  matrix *from = NULL;
  allocate_matrix(&from, 3, 2);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 2; j++) {
      set(from, i, j, i * 2 + j);
    }
  }
  CU_ASSERT_EQUAL(allocate_matrix_ref(&mat, from, 2, 2, 2), 0);
  CU_ASSERT_PTR_EQUAL(mat->data, from->data + 2);
  CU_ASSERT_PTR_EQUAL(mat->parent, from);
  CU_ASSERT_EQUAL(mat->parent->ref_cnt, 2);
  CU_ASSERT_EQUAL(mat->rows, 2);
  CU_ASSERT_EQUAL(mat->cols, 2);
  deallocate_matrix(from);
  deallocate_matrix(mat);
}

void dealloc_null_test(void) {
  matrix *mat = NULL;
  deallocate_matrix(mat); // Test the null case doesn't crash
}

void add_test(void) {
  matrix *result = NULL;
  matrix *mat1 = NULL;
  matrix *mat2 = NULL;
  CU_ASSERT_EQUAL(allocate_matrix(&result, 2, 2), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat1, 2, 2), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat2, 2, 2), 0);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      set(mat1, i, j, i * 2 + j);
      set(mat2, i, j, i * 2 + j);
    }
  }
  add_matrix(result, mat1, mat2);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      CU_ASSERT_EQUAL(get(result, i, j), 2 * (i * 2 + j));
    }
  }
  deallocate_matrix(result);
  deallocate_matrix(mat1);
  deallocate_matrix(mat2);
}

// (OPTIONAL) Uncomment the following sub_test if you have decided to implement it in matrix.c.
void sub_test(void) {
  matrix *result = NULL;
  matrix *mat1 = NULL;
  matrix *mat2 = NULL;
  CU_ASSERT_EQUAL(allocate_matrix(&result, 2, 2), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat1, 2, 2), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat2, 2, 2), 0);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      set(mat1, i, j, i * 2 + j);
      set(mat2, i, j, (i * 2 + j) * 3);
    }
  }
  sub_matrix(result, mat1, mat2);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      CU_ASSERT_EQUAL(get(result, i, j), (-2) * (i * 2 + j));
    }
  }
  deallocate_matrix(result);
  deallocate_matrix(mat1);
  deallocate_matrix(mat2);
}

// (OPTIONAL) Uncomment the following neg_test if you have decided to implement it in matrix.c.
void neg_test(void) {
  matrix *result = NULL;
  matrix *mat = NULL;
  CU_ASSERT_EQUAL(allocate_matrix(&result, 2, 2), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat, 2, 2), 0);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      set(mat, i, j, i * 2 + j);
    }
  }
  neg_matrix(result, mat);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      CU_ASSERT_EQUAL(get(result, i, j), -(i * 2 + j));
    }
  }
  deallocate_matrix(result);
  deallocate_matrix(mat);
} 

void abs_test(void) {
  matrix *result = NULL;
  matrix *mat = NULL;
  CU_ASSERT_EQUAL(allocate_matrix(&result, 2, 2), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat, 2, 2), 0);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      if (j % 2 == 0)
        set(mat, i, j, i * 2 + j);
      else
        set(mat, i, j, -(i * 2 + j));
    }
  }
  abs_matrix(result, mat);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      CU_ASSERT_EQUAL(get(result, i, j), i * 2 + j);
    }
  }
  deallocate_matrix(result);
  deallocate_matrix(mat);
}

void abs_test_1(void) {
  matrix *result = NULL;
  matrix *mat = NULL;
  CU_ASSERT_EQUAL(allocate_matrix(&result, 3, 3), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat, 3, 3), 0);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (j % 2 == 0)
        set(mat, i, j, i * 2 + j);
      else
        set(mat, i, j, -(i * 2 + j));
    }
  }
  abs_matrix(result, mat);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      CU_ASSERT_EQUAL(get(result, i, j), i * 2 + j);
    }
  }
  deallocate_matrix(result);
  deallocate_matrix(mat);
}

void mul_square_test(void) {
  matrix *result = NULL;
  matrix *mat1 = NULL;
  matrix *mat2 = NULL;
  CU_ASSERT_EQUAL(allocate_matrix(&result, 3, 3), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat1, 3, 3), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat2, 3, 3), 0);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      set(mat1, i, j, i * 3 + j + 1);
      set(mat2, i, j, i * 3 + j + 1);
    }
  }
  mul_matrix(result, mat1, mat2);
  CU_ASSERT_EQUAL(get(result, 0, 0), 30);
  CU_ASSERT_EQUAL(get(result, 0, 1), 36);
  CU_ASSERT_EQUAL(get(result, 0, 2), 42);
  CU_ASSERT_EQUAL(get(result, 1, 0), 66);
  CU_ASSERT_EQUAL(get(result, 1, 1), 81);
  CU_ASSERT_EQUAL(get(result, 1, 2), 96);
  CU_ASSERT_EQUAL(get(result, 2, 0), 102);
  CU_ASSERT_EQUAL(get(result, 2, 1), 126);
  CU_ASSERT_EQUAL(get(result, 2, 2), 150);
  deallocate_matrix(result);
  deallocate_matrix(mat1);
  deallocate_matrix(mat2);
}

void mul_square_test_4x4(void) {
  matrix *result = NULL;
  matrix *mat1 = NULL;
  matrix *mat2 = NULL;
  CU_ASSERT_EQUAL(allocate_matrix(&result, 4, 4), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat1, 4, 4), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat2, 4, 4), 0);
  set(mat1, 0, 0, 1);
  set(mat1, 0, 1, 2);
  set(mat1, 0, 2, 3);
  set(mat1, 0, 3, 4);
  set(mat1, 1, 0, 5);
  set(mat1, 1, 1, 6);
  set(mat1, 1, 2, 7);
  set(mat1, 1, 3, 8);
  set(mat1, 2, 0, 9);
  set(mat1, 2, 1, 10);
  set(mat1, 2, 2, 11);
  set(mat1, 2, 3, 12);
  set(mat1, 3, 0, 13);
  set(mat1, 3, 1, 14);
  set(mat1, 3, 2, 15);
  set(mat1, 3, 3, 16);

  set(mat2, 0, 0, 1);
  set(mat2, 0, 1, 2);
  set(mat2, 0, 2, 3);
  set(mat2, 0, 3, 4);
  set(mat2, 1, 0, 4);
  set(mat2, 1, 1, 3);
  set(mat2, 1, 2, 2);
  set(mat2, 1, 3, 1);
  set(mat2, 2, 0, 9);
  set(mat2, 2, 1, 10);
  set(mat2, 2, 2, 11);
  set(mat2, 2, 3, 12);
  set(mat2, 3, 0, 12);
  set(mat2, 3, 1, 11);
  set(mat2, 3, 2, 10);
  set(mat2, 3, 3, 9);
  mul_matrix(result, mat1, mat2);
  CU_ASSERT_EQUAL(get(result, 0, 0), 84);
  CU_ASSERT_EQUAL(get(result, 0, 1), 82);
  CU_ASSERT_EQUAL(get(result, 0, 2), 80);
  CU_ASSERT_EQUAL(get(result, 0, 3), 78);
  CU_ASSERT_EQUAL(get(result, 1, 0), 188);
  CU_ASSERT_EQUAL(get(result, 1, 1), 186);
  CU_ASSERT_EQUAL(get(result, 1, 2), 184);
  CU_ASSERT_EQUAL(get(result, 1, 3), 182);
  CU_ASSERT_EQUAL(get(result, 2, 0), 292);
  CU_ASSERT_EQUAL(get(result, 2, 1), 290);
  CU_ASSERT_EQUAL(get(result, 2, 2), 288);
  CU_ASSERT_EQUAL(get(result, 2, 3), 286);
  CU_ASSERT_EQUAL(get(result, 3, 0), 396);
  CU_ASSERT_EQUAL(get(result, 3, 1), 394);
  CU_ASSERT_EQUAL(get(result, 3, 2), 392);
  CU_ASSERT_EQUAL(get(result, 3, 3), 390);
  deallocate_matrix(result);
  deallocate_matrix(mat1);
  deallocate_matrix(mat2);
}

// void mul_square_test_simd(void) {
//   matrix *result = NULL;
//   matrix *mat1 = NULL;
//   matrix *mat2 = NULL;
//   CU_ASSERT_EQUAL(allocate_matrix(&result, 2, 2), 0);
//   CU_ASSERT_EQUAL(allocate_matrix(&mat1, 2, 2), 0);
//   CU_ASSERT_EQUAL(allocate_matrix(&mat2, 2, 2), 0);
//   double array[4] = {0.8401877171547095, 0.39438292681909304, 0.7830992237586059, 0.7984400334760733};
//   set(mat1, 0, 0, array[0]);
//   set(mat1, 0, 1, array[1]);
//   set(mat1, 1, 0, array[2]);
//   set(mat1, 1, 1, array[3]);
//   set(mat2, 0, 0, array[0]);
//   set(mat2, 0, 1, array[1]);
//   set(mat2, 1, 0, array[2]);
//   set(mat2, 1, 1, array[3]);
//   mul_matrix(result, mat1, mat2);
//   printf("%f\n", get(result, 0, 0));
//   CU_ASSERT_EQUAL(get(result, 0, 0), 1.0147563639133209685);
//   CU_ASSERT_EQUAL(get(result, 0, 1), 0.64624680826075505202);
//   CU_ASSERT_EQUAL(get(result, 1, 0), 1.28320811954827645);
//   CU_ASSERT_EQUAL(get(result, 1, 1), 0.94634745091295188828);
//   deallocate_matrix(result);
//   deallocate_matrix(mat1);
//   deallocate_matrix(mat2);
// }

void mul_non_square_test(void) {
  matrix *result = NULL;
  matrix *mat1 = NULL;
  matrix *mat2 = NULL;
  CU_ASSERT_EQUAL(allocate_matrix(&result, 3, 3), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat1, 3, 2), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat2, 2, 3), 0);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 2; j++) {
      set(mat1, i, j, i * 2 + j + 1);
    }
  }
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      set(mat2, i, j, i * 3 + j + 1);
    }
  }
  mul_matrix(result, mat1, mat2);
  CU_ASSERT_EQUAL(get(result, 0, 0), 9);
  CU_ASSERT_EQUAL(get(result, 0, 1), 12);
  CU_ASSERT_EQUAL(get(result, 0, 2), 15);
  CU_ASSERT_EQUAL(get(result, 1, 0), 19);
  CU_ASSERT_EQUAL(get(result, 1, 1), 26);
  CU_ASSERT_EQUAL(get(result, 1, 2), 33);
  CU_ASSERT_EQUAL(get(result, 2, 0), 29);
  CU_ASSERT_EQUAL(get(result, 2, 1), 40);
  CU_ASSERT_EQUAL(get(result, 2, 2), 51);
  deallocate_matrix(result);
  deallocate_matrix(mat1);
  deallocate_matrix(mat2);
}

void pow_test(void) {
  matrix *result = NULL;
  matrix *mat = NULL;
  CU_ASSERT_EQUAL(allocate_matrix(&result, 2, 2), 0);
  CU_ASSERT_EQUAL(allocate_matrix(&mat, 2, 2), 0);
  set(mat, 0, 0, 1);
  set(mat, 0, 1, 1);
  set(mat, 1, 0, 1);
  set(mat, 1, 1, 0);
  pow_matrix(result, mat, 2);
  CU_ASSERT_EQUAL(get(result, 0, 0), 2);
  CU_ASSERT_EQUAL(get(result, 0, 1), 1);
  CU_ASSERT_EQUAL(get(result, 1, 0), 1);
  CU_ASSERT_EQUAL(get(result, 1, 1), 1);
  pow_matrix(result, mat, 3);
  CU_ASSERT_EQUAL(get(result, 0, 0), 3);
  CU_ASSERT_EQUAL(get(result, 0, 1), 2);
  CU_ASSERT_EQUAL(get(result, 1, 0), 2);
  CU_ASSERT_EQUAL(get(result, 1, 1), 1);
  pow_matrix(result, mat, 4);
  CU_ASSERT_EQUAL(get(result, 0, 0), 5);
  CU_ASSERT_EQUAL(get(result, 0, 1), 3);
  CU_ASSERT_EQUAL(get(result, 1, 0), 3);
  CU_ASSERT_EQUAL(get(result, 1, 1), 2);
  pow_matrix(result, mat, 5);
  CU_ASSERT_EQUAL(get(result, 0, 0), 8);
  CU_ASSERT_EQUAL(get(result, 0, 1), 5);
  CU_ASSERT_EQUAL(get(result, 1, 0), 5);
  CU_ASSERT_EQUAL(get(result, 1, 1), 3);
  pow_matrix(result, mat, 6);
  CU_ASSERT_EQUAL(get(result, 0, 0), 13); // 6
  CU_ASSERT_EQUAL(get(result, 0, 1), 8);
  CU_ASSERT_EQUAL(get(result, 1, 0), 8);
  CU_ASSERT_EQUAL(get(result, 1, 1), 5);
  pow_matrix(result, mat, 7);
  CU_ASSERT_EQUAL(get(result, 0, 0), 21);
  CU_ASSERT_EQUAL(get(result, 0, 1), 13);
  CU_ASSERT_EQUAL(get(result, 1, 0), 13);
  CU_ASSERT_EQUAL(get(result, 1, 1), 8);
  pow_matrix(result, mat, 8);
  CU_ASSERT_EQUAL(get(result, 0, 0), 34);
  CU_ASSERT_EQUAL(get(result, 0, 1), 21);
  CU_ASSERT_EQUAL(get(result, 1, 0), 21);
  CU_ASSERT_EQUAL(get(result, 1, 1), 13);
  pow_matrix(result, mat, 9);
  CU_ASSERT_EQUAL(get(result, 0, 0), 55);
  CU_ASSERT_EQUAL(get(result, 0, 1), 34);
  CU_ASSERT_EQUAL(get(result, 1, 0), 34);
  CU_ASSERT_EQUAL(get(result, 1, 1), 21);
  pow_matrix(result, mat, 10);
  CU_ASSERT_EQUAL(get(result, 0, 0), 89);
  CU_ASSERT_EQUAL(get(result, 0, 1), 55);
  CU_ASSERT_EQUAL(get(result, 1, 0), 55);
  CU_ASSERT_EQUAL(get(result, 1, 1), 34);
  pow_matrix(result, mat, 11);
  CU_ASSERT_EQUAL(get(result, 0, 0), 144);
  CU_ASSERT_EQUAL(get(result, 0, 1), 89);
  CU_ASSERT_EQUAL(get(result, 1, 0), 89);
  CU_ASSERT_EQUAL(get(result, 1, 1), 55);
  pow_matrix(result, mat, 12);
  CU_ASSERT_EQUAL(get(result, 0, 0), 233);
  CU_ASSERT_EQUAL(get(result, 0, 1), 144);
  CU_ASSERT_EQUAL(get(result, 1, 0), 144);
  CU_ASSERT_EQUAL(get(result, 1, 1), 89);
  pow_matrix(result, mat, 13);
  CU_ASSERT_EQUAL(get(result, 0, 0), 377);
  CU_ASSERT_EQUAL(get(result, 0, 1), 233);
  CU_ASSERT_EQUAL(get(result, 1, 0), 233);
  CU_ASSERT_EQUAL(get(result, 1, 1), 144);
  pow_matrix(result, mat, 14);
  CU_ASSERT_EQUAL(get(result, 0, 0), 610);
  CU_ASSERT_EQUAL(get(result, 0, 1), 377);
  CU_ASSERT_EQUAL(get(result, 1, 0), 377);
  CU_ASSERT_EQUAL(get(result, 1, 1), 233);
  deallocate_matrix(result);
  deallocate_matrix(mat);
}

/************* Test Runner Code goes here **************/

int main (void)
{
  Py_Initialize(); // Need to call this so that Python.h functions won't segfault
  CU_pSuite pSuite = NULL;

  /* initialize the CUnit test registry */
  if (CU_initialize_registry() != CUE_SUCCESS)
    return CU_get_error();

  /* add a suite to the registry */
  pSuite = CU_add_suite("mat_test_suite", init_suite, clean_suite);
  if (pSuite == NULL) {
    CU_cleanup_registry();
    return CU_get_error();
  }

   /* add the tests to the suite */
   if ((CU_add_test(pSuite, "add_test", add_test) == NULL) ||
        // (OPTIONAL) Uncomment the following lines if you have implemented sub_matrix and neg_matrix.
        (CU_add_test(pSuite, "sub_test", sub_test) == NULL) ||
        (CU_add_test(pSuite, "neg_test", neg_test) == NULL) ||
        (CU_add_test(pSuite, "fill_test", fill_test) == NULL) ||
        (CU_add_test(pSuite, "abs_test_1", abs_test_1) == NULL) ||
        (CU_add_test(pSuite, "transpose_test", transpose_test) == NULL) ||
        // (CU_add_test(pSuite, "mul_square_test_simd", mul_square_test_simd) == NULL) ||
        (CU_add_test(pSuite, "mul_square_test_4x4", mul_square_test_4x4) == NULL) ||
        (CU_add_test(pSuite, "mul_square_test", mul_square_test) == NULL) ||
        (CU_add_test(pSuite, "mul_non_square_test", mul_non_square_test) == NULL) ||
        (CU_add_test(pSuite, "abs_test", abs_test) == NULL) ||
        (CU_add_test(pSuite, "pow_test", pow_test) == NULL) ||
        (CU_add_test(pSuite, "alloc_fail_test", alloc_fail_test) == NULL) ||
        (CU_add_test(pSuite, "alloc_success_test", alloc_success_test) == NULL) ||
        (CU_add_test(pSuite, "alloc_ref_fail_test", alloc_ref_fail_test) == NULL) ||
        (CU_add_test(pSuite, "alloc_ref_success_test", alloc_ref_success_test) == NULL) ||
        (CU_add_test(pSuite, "dealloc_null_test", dealloc_null_test) == NULL) ||
        (CU_add_test(pSuite, "get_test", get_test) == NULL) ||
        (CU_add_test(pSuite, "set_test", set_test) == NULL)
     )
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

  // Run all tests using the basic interface
  CU_basic_set_mode(CU_BRM_NORMAL);
  // CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  printf("\n");
  CU_basic_show_failures(CU_get_failure_list());
  printf("\n\n");

  /* Clean up registry and return */
  CU_cleanup_registry();
  return CU_get_error();
}
