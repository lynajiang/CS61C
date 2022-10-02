#include "matrix.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <immintrin.h>
#include <x86intrin.h>
#endif

/* Below are some intel intrinsics that might be useful
 * void _mm256_storeu_pd (double * mem_addr, __m256d a)
 * __m256d _mm256_set1_pd (double a)
 * __m256d _mm256_set_pd (double e3, double e2, double e1, double e0)
 * __m256d _mm256_loadu_pd (double const * mem_addr)
 * __m256d _mm256_add_pd (__m256d a, __m256d b)
 * __m256d _mm256_sub_pd (__m256d a, __m256d b)
 * __m256d _mm256_fmadd_pd (__m256d a, __m256d b, __m256d c)
 * __m256d _mm256_mul_pd (__m256d a, __m256d b)
 * __m256d _mm256_cmp_pd (__m256d a, __m256d b, const int imm8)
 * __m256d _mm256_and_pd (__m256d a, __m256d b)
 * __m256d _mm256_max_pd (__m256d a, __m256d b)
 */

/* Generates a random double between low and high */
double rand_double(double low, double high)
{
    double range = (high - low);
    double div = RAND_MAX / range;
    return low + (rand() / div);
}

/* Generates a random matrix */
void rand_matrix(matrix *result, unsigned int seed, double low, double high)
{
    srand(seed);
    for (int i = 0; i < result->rows; i++)
    {
        for (int j = 0; j < result->cols; j++)
        {
            set(result, i, j, rand_double(low, high));
        }
    }
}

/*
 * Returns the double value of the matrix at the given row and column.
 * You may assume `row` and `col` are valid. Note that the matrix is in row-major order.
 */
double get(matrix *mat, int row, int col)
{
    // Task 1.1 TODO
    int col_len = mat->cols;
    int to_index = ((row)*col_len) + col;
    return mat->data[to_index];
}

/*
 * Sets the value at the given row and column to val. You may assume `row` and
 * `col` are valid. Note that the matrix is in row-major order.
 */
void set(matrix *mat, int row, int col, double val)
{
    // Task 1.1 TODO
    int col_len = mat->cols;
    int to_index = ((row)*col_len) + col;
    mat->data[to_index] = val;
}

/*
 * Allocates space for a matrix struct pointed to by the double pointer mat with
 * `rows` rows and `cols` columns. You should also allocate memory for the data array
 * and initialize all entries to be zeros. `parent` should be set to NULL to indicate that
 * this matrix is not a slice. You should also set `ref_cnt` to 1.
 * You should return -1 if either `rows` or `cols` or both have invalid values. Return -2 if any
 * call to allocate memory in this function fails.
 * Return 0 upon success.
 */
int allocate_matrix(matrix **mat, int rows, int cols)
{
    // Task 1.2 TODO
    // HINTS: Follow these steps.
    // 1. Check if the dimensions are valid. Return -1 if either dimension is not positive.
    if (rows < 1 || cols < 1)
    {
        return -1;
    }
    // 2. Allocate space for the new matrix struct. Return -2 if allocating memory failed.
    matrix *allocd_mat = (matrix *)malloc(sizeof(matrix));
    if (allocd_mat == NULL)
    {
        return -2;
    }
    // 3. Allocate space for the matrix data, initializing all entries to be 0. Return -2 if allocating memory failed.
    int size_of_mat = rows * cols;
    // allocd_mat->data = (double*) malloc(size_of_mat * sizeof(double));
    allocd_mat->data = (double *)calloc(size_of_mat, sizeof(double));
    if (allocd_mat->data == NULL)
    {
        return -2;
    }
    // 4. Set the number of rows and columns in the matrix struct according to the arguments provided.
    allocd_mat->rows = rows;
    allocd_mat->cols = cols;
    // 5. Set the `parent` field to NULL, since this matrix was not created from a slice.
    allocd_mat->parent = NULL;
    // 6. Set the `ref_cnt` field to 1.
    allocd_mat->ref_cnt = 1;
    // 7. Store the address of the allocated matrix struct at the location `mat` is pointing at.
    *mat = allocd_mat;
    // 8. Return 0 upon success.
    return 0;
}

/*
 * You need to make sure that you only free `mat->data` if `mat` is not a slice and has no existing slices,
 * or that you free `mat->parent->data` if `mat` is the last existing slice of its parent matrix and its parent
 * matrix has no other references (including itself).
 */
void deallocate_matrix(matrix *mat)
{
    // Task 1.3 TODO
    // HINTS: Follow these steps.
    // 1. If the matrix pointer `mat` is NULL, return.
    if (mat == NULL)
    {
        return;
    }
    // 2. If `mat` has no parent: decrement its `ref_cnt` field by 1. If the `ref_cnt` field becomes 0, then free `mat` and its `data` field.
    if (mat->parent == NULL)
    {
        mat->ref_cnt -= 1;
        if (mat->ref_cnt == 0)
        {
            free(mat->data);
            free(mat);
        }
    }
    // 3. Otherwise, recursively call `deallocate_matrix` on `mat`'s parent, then free `mat`.
    else
    {
        deallocate_matrix(mat->parent);
        free(mat);
    }
}

/*
 * Allocates space for a matrix struct pointed to by `mat` with `rows` rows and `cols` columns.
 * Its data should point to the `offset`th entry of `from`'s data (you do not need to allocate memory)
 * for the data field. `parent` should be set to `from` to indicate this matrix is a slice of `from`
 * and the reference counter for `from` should be incremented. Lastly, do not forget to set the
 * matrix's row and column values as well.
 * You should return -1 if either `rows` or `cols` or both have invalid values. Return -2 if any
 * call to allocate memory in this function fails.
 * Return 0 upon success.
 * NOTE: Here we're allocating a matrix struct that refers to already allocated data, so
 * there is no need to allocate space for matrix data.
 */
int allocate_matrix_ref(matrix **mat, matrix *from, int offset, int rows, int cols)
{
    // Task 1.4 TODO
    // HINTS: Follow these steps.
    // 1. Check if the dimensions are valid. Return -1 if either dimension is not positive.
    if (rows < 1 || cols < 1)
    {
        return -1;
    }
    // 2. Allocate space for the new matrix struct. Return -2 if allocating memory failed.
    matrix *allocd_mat = (matrix *)malloc(sizeof(matrix));
    if (allocd_mat == NULL)
    {
        return -2;
    }
    // 3. Set the `data` field of the new struct to be the `data` field of the `from` struct plus `offset`.
    allocd_mat->data = from->data + offset;
    // 4. Set the number of rows and columns in the new struct according to the arguments provided.
    allocd_mat->rows = rows;
    allocd_mat->cols = cols;
    // 5. Set the `parent` field of the new struct to the `from` struct pointer.
    allocd_mat->parent = from;
    // 6. Increment the `ref_cnt` field of the `from` struct by 1.
    from->ref_cnt += 1;
    // 7. Store the address of the allocated matrix struct at the location `mat` is pointing at.
    *mat = allocd_mat;
    // 8. Return 0 upon success.
    return 0;
}

/*
 * Sets all entries in mat to val. Note that the matrix is in row-major order.
 */
void fill_matrix(matrix *mat, double val)
{
    // Task 1.5 TODO
    int row_len = mat->rows;
    int col_len = mat->cols;
    int total_entries = row_len * col_len;
    
    __m256d val_vec = _mm256_set1_pd(val);
    int i;
    #pragma omp parallel for
    for (i = 0; i < (total_entries / 4) * 4; i += 4)
    {
        _mm256_storeu_pd (mat->data + i, val_vec);
    }
    for (i = (total_entries / 4) * 4; i < total_entries; i++) {
        mat->data[i] = val;
    }
}

/*
 * Store the result of taking the absolute value element-wise to `result`.
 * Return 0 upon success.
 * Note that the matrix is in row-major order.
 */
int abs_matrix(matrix *result, matrix *mat)
{
    // Task 1.5 TODO
    int row_len = mat->rows;
    int col_len = mat->cols;
    int total_entries = row_len * col_len;

    __m256d neg_1 = _mm256_set1_pd(-1);
    
    int i;
    #pragma omp parallel for
    for (i = 0; i < (total_entries / 4) * 4; i += 4)
    {
        __m256d data_vec = _mm256_loadu_pd(mat->data + i);
        __m256d neg_data_vec = _mm256_mul_pd(data_vec, neg_1);
        __m256d result_vec = _mm256_max_pd(data_vec, neg_data_vec);
        _mm256_storeu_pd (result->data + i, result_vec);
    }
    for (i = (total_entries / 4) * 4; i < total_entries; i++) {
        double val = mat->data[i];
        if (val < 0) {
            val = -val;
        }
        result->data[i] = val;
    }
    // deallocate_matrix(temp);
    return 0;
}

/*
 * (OPTIONAL)
 * Store the result of element-wise negating mat's entries to `result`.
 * Return 0 upon success.
 * Note that the matrix is in row-major order.
 */
int neg_matrix(matrix *result, matrix *mat)
{
    // Task 1.5 TODO
    int row_len = mat->rows;
    int col_len = mat->cols;
    int total_entries = row_len * col_len;
    
    int i;
    #pragma omp parallel for
    for (i = 0; i < (total_entries / 4) * 4; i += 4)
    {
        __m256d data_vec =  _mm256_loadu_pd(mat->data + i);
        __m256d neg_vec = _mm256_set1_pd(-1);
        __m256d result_vec = _mm256_mul_pd(data_vec, neg_vec);
        _mm256_storeu_pd (result->data + i, result_vec);
    }
    for (i = (total_entries / 4) * 4; i < total_entries; i++) {
        result->data[i] = -(mat->data[i]);
    }

    return 0;
}

/*
 * Store the result of adding mat1 and mat2 to `result`.
 * Return 0 upon success.
 * You may assume `mat1` and `mat2` have the same dimensions.
 * Note that the matrix is in row-major order.
 */
int add_matrix(matrix *result, matrix *mat1, matrix *mat2)
{
    // Task 1.5 TODO
    int row_len = mat1->rows;
    int col_len = mat1->cols;
    int total_entries = row_len * col_len;

    int i;
    #pragma omp parallel for
    for (i = 0; i < (total_entries / 4) * 4; i += 4)
    {
        __m256d data_vec_a =  _mm256_loadu_pd(mat1->data + i);
        __m256d data_vec_b =  _mm256_loadu_pd(mat2->data + i);
        __m256d result_vec = _mm256_add_pd(data_vec_a, data_vec_b);
        _mm256_storeu_pd (result->data + i, result_vec);
    }
    for (i = (total_entries / 4) * 4; i < total_entries; i++) {
        result->data[i] = mat1->data[i] + mat2->data[i];
    }
    return 0;
}

/*
 * (OPTIONAL)
 * Store the result of subtracting mat2 from mat1 to `result`.
 * Return 0 upon success.
 * You may assume `mat1` and `mat2` have the same dimensions.
 * Note that the matrix is in row-major order.
 */
int sub_matrix(matrix *result, matrix *mat1, matrix *mat2)
{
    // Task 1.5 TODO
    int row_len = mat1->rows;
    int col_len = mat1->cols;
    int total_entries = row_len * col_len;
    
    int i;
    #pragma omp parallel for
    for (i = 0; i < (total_entries / 4) * 4; i += 4)
    {
        __m256d data_vec_a =  _mm256_loadu_pd(mat1->data + i);
        __m256d data_vec_b =  _mm256_loadu_pd(mat2->data + i);
        __m256d result_vec = _mm256_sub_pd(data_vec_a, data_vec_b);
        _mm256_storeu_pd (result->data + i, result_vec);
    }
    for (i = (total_entries / 4) * 4; i < total_entries; i++) {
        result->data[i] = mat1->data[i] - mat2->data[i];
    }

    return 0;
}

/*
 * Takes in a matrix and transposes it.
 * Return 0 upon success.
 */
int transpose_matrix(matrix* result, matrix* mat) {
    int row_len = mat->rows;
    int col_len = mat->cols;
    int total_entries = row_len * col_len;

    matrix* temp = NULL;
    allocate_matrix(&temp, col_len, row_len);

    #pragma omp parallel for
    for (int i = 0; i < col_len; i++) {
        for (int j = 0; j < row_len/ 4 * 4; j+= 4) {
            // temp->data[i* row_len + j] = mat->data[j * col_len + i];
            set(temp, i, j, get(mat, j, i));
            set(temp, i, j + 1, get(mat, j + 1, i));
            set(temp, i, j + 2, get(mat, j + 2, i));
            set(temp, i, j + 3, get(mat, j + 3, i));
        } 

        for (int j = row_len/4 * 4; j < row_len; j++) {
            temp->data[i* row_len + j] = mat->data[j * col_len + i];
            //set(temp, i, j, get(mat, j, i));
        }    
    }
    result->rows = col_len;
    result->cols = row_len;
    memcpy(result->data, temp->data, sizeof(double)*total_entries);
    deallocate_matrix(temp);
    return 0;
}

/*
 * Store the result of multiplying mat1 and mat2 to `result`.
 * Return 0 upon success.
 * Remember that matrix multiplication is not the same as multiplying individual elements.
 * You may assume `mat1`'s number of columns is equal to `mat2`'s number of rows.
 * Note that the matrix is in row-major order.
 */
int mul_matrix(matrix *result, matrix *mat1, matrix *mat2)
{
    // Task 1.6 TODO
    // code taken from Lab7 where we learned how to optimize matmul with loop ordering
    int final_row_len = mat1->rows;
    int middle = mat1->cols;
    int mat2_row = mat2->rows;
    int final_col_len = mat2->cols;

    matrix* temp = NULL;
    allocate_matrix(&temp, final_row_len, final_col_len);

    matrix* transpose_mat = NULL;
    allocate_matrix(&transpose_mat, final_col_len, mat2_row);

    int total_entries = final_row_len * final_col_len; // in result matrix

    transpose_matrix(transpose_mat, mat2);
    double dot_product = 0;
    #pragma omp parallel for reduction(+ : dot_product)
    for (int i = 0; i < final_row_len; i++) {
        for (int k = 0; k < final_col_len; k++) {
            int j;
            dot_product = 0;
            __m256d result_vec = _mm256_set1_pd(0);
            for (j = 0; j < (middle/64) * 64; j+= 64) {
                __m256d mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j);
                __m256d mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);
                
                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 4);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 4);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 8);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 8);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 12);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 12);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 16);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 16);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 20);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 20);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 24);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 24);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 28);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 28);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 32);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 32);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 36);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 36);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 40);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 40);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 44);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 44);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 48);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 48);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 52);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 52);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 56);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 56);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 60);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 60);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);
            }

            for (j = (middle/64) * 64; j < (middle/32) * 32; j+= 32) {
                __m256d mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j);
                __m256d mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);
                
                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 4);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 4);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 8);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 8);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 12);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 12);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 16);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 16);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);
                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 20);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 20);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 24);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 24);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 28);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 28);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);
            }

            for (j = (middle/32) * 32; j < (middle/16) * 16; j+= 16) {
                __m256d mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j);
                __m256d mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);
                
                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 4);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 4);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 8);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 8);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);

                mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j + 12);
                mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j + 12);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);
            } 

            for (j = (middle/16) * 16; j < (middle/4) * 4; j+= 4) {
                __m256d mat1_vec = _mm256_loadu_pd(mat1->data + i * middle + j);
                __m256d mat2_vec = _mm256_loadu_pd(transpose_mat->data + k * middle + j);
                result_vec = _mm256_fmadd_pd(mat1_vec, mat2_vec, result_vec);
            }

            double to_add[4]; 
            _mm256_storeu_pd (to_add, result_vec);
            dot_product = to_add[0] + to_add[1] + to_add[2] + to_add[3];
            for (j = (middle/4) * 4; j < middle; j++) {
                dot_product += mat1->data[i*middle + j] * transpose_mat->data[k*middle + j];
                // dot_product += get(mat1, i, j) * get(transpose_mat, k, j);
            }
        temp->data[i*final_col_len + k] = dot_product;
        // set(temp, i, k, dot_product);
        }  
    }
    memcpy(result->data, temp->data, (final_row_len * final_col_len * sizeof(double)));
    deallocate_matrix(temp);
    deallocate_matrix(transpose_mat);
    return 0; 
}

/*
 * Store the result of raising mat to the (pow)th power to `result`.
 * Return 0 upon success.
 * Remember that pow is defined with matrix multiplication, not element-wise multiplication.
 * You may assume `mat` is a square matrix and `pow` is a non-negative integer.
 * Note that the matrix is in row-major order.
 */
int pow_matrix(matrix *result, matrix *mat, int pow)
{
    // Task 1.6 TODO
    int row_len = mat->rows;
    int col_len = mat->cols;
    int total_entries = row_len * col_len;

    matrix* id_matrix = NULL;
    allocate_matrix(&id_matrix, row_len, col_len); 

    matrix* mat_copy = NULL;
    allocate_matrix(&mat_copy, row_len, col_len);   
    memcpy(mat_copy->data, mat->data, total_entries * sizeof(double));

    for (int i = 0; i < col_len; i++) {
        for (int j = 0; j < row_len; j++) {
            int val_to_set = 0;
            if (i == j) {
                val_to_set = 1;
            }
            set(id_matrix, i, j, val_to_set);
         }
    }

    if (pow == 0) {
        memcpy(result->data, id_matrix->data, sizeof(double) * total_entries);
        return 0;
    }
    else {
        while (pow > 1) {
            if (pow % 2 == 0) {
                mul_matrix(mat_copy, mat_copy, mat_copy);
                pow = pow / 2;
            } else {
                mul_matrix(id_matrix, mat_copy, id_matrix);
                mul_matrix(mat_copy, mat_copy, mat_copy);
                pow = (pow - 1) / 2;
            }
        }       
    }
    mul_matrix(result, mat_copy, id_matrix);
    deallocate_matrix(id_matrix);
    deallocate_matrix(mat_copy);
    return 0;
}
