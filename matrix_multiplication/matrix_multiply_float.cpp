# include <iostream>
# include <chrono>
# include <immintrin.h>
# include <iomanip>
#define ROW1 16
#define COL1 16 //ROW2=COL1
#define COL2 16

void print_results(float res[8][8]){

    int i,j;
    for(i=0;i<8;i++){
       for(j=0;j<8;j++){
            std::cout<<res[i][j]<<" ";
       }
       std::cout<<"\n";
    }

}
void initialize(float *arr, int m, int n) {
  int i, j;
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      *((arr + i * n) + j) = i * 1.0f;
    }
  }
}
void Scalar_matrix_multiply(float mat1[8][8] , float mat2[8][8] , float result1[8][8]){

    int i , j , k;

    // Initializing elements of matrix result1 to 0.
    for(i = 0; i < 8; ++i){
        for(j = 0; j < 8; ++j){
            result1[i][j]=0;
        }
    }

    for(i = 0; i < 8; ++i)
        for(j = 0; j < 8; ++j)   
            for(k = 0; k < 8; ++k)
            {
                // Multiplying  row from mat1 and first colum from mat2 and summing up 
                result1[i][j] += mat1[i][k] * mat2[k][j];
            }

}


void matrix_multiply_16x16(float *A, float *B, float *C) {
    // Load constants into SSE registers
    __m128 a, b, c, d, e;
    __m128 c0, c1, c2, c3;
    int i,j;
    for ( i = 0; i < ROW1; i ++) {
            c0 = _mm_setzero_ps();
            c1 = _mm_setzero_ps();
            c2 = _mm_setzero_ps();
            c3 = _mm_setzero_ps();
        for (j = 0; j < COL2; j ++) {
                // Load the values from the matrices into SSE registers
                a = _mm_set_ps1(*(A + i*COL1 + j));
                b = _mm_loadu_ps(&(*(B + j*COL2 + i)));
                c = _mm_loadu_ps(&(*(B + j*COL2 + i + 4)));
                d = _mm_loadu_ps(&(*(B + j*COL2 + i + 8)));
                e = _mm_loadu_ps(&(*(B + j*COL2 + i + 12)));
              
                // Perform SIMD multiplication
                c0 = _mm_add_ps(c0, _mm_mul_ps(a, b));
                c1 = _mm_add_ps(c1, _mm_mul_ps(a, c));
                c2 = _mm_add_ps(c2, _mm_mul_ps(a, d));
                c3 = _mm_add_ps(c3, _mm_mul_ps(a, e));
            }

            // Store the results back into memory
            _mm_storeu_ps(&(*(C + i*COL2)), c0);
            _mm_storeu_ps(&(*(C + i*COL2 + 4)), c1);
            _mm_storeu_ps(&(*(C + i*COL2 + 8)), c2);
            _mm_storeu_ps(&(*(C + i*COL2 + 12)), c3);
  
    }
}
void check(__m128 out){

    for(int i = 0; i < 4; i++){
        std::cout<<out[i]<<" ";
    }
    std::cout<<"\n";
}

void Vector_matrix_multiply_128(float mat1[8][8] , float mat2[8][8] , float result2[8][8] , int size){

    int i , j , k=4;  
    __m128 a , b , c, sum1, sum2;
    
    for (i = 0; i < size; ++i) {

        // Initializing 128-bit vector sum with all of its elements set to zero
        sum1 = _mm_setzero_ps();
        sum2 = _mm_setzero_ps();
        for (j = 0; j < size; ++j) {

                // Broadcasting the value mat1[i][k] to all four elements of the vector a
                a = _mm_set_ps1(mat1[i][j]);

                // Load the vector b from the specified memory address 
                b = _mm_loadu_ps(&mat2[j][0]);
                c = _mm_loadu_ps(&mat2[j][4]);

                // Multiply the vector a and b and add with vector sum
                sum1 = _mm_add_ps(sum1, _mm_mul_ps(a, b));
                sum2 = _mm_add_ps(sum2, _mm_mul_ps(a, c));
            
        }
            // Vector sum will contain the result of first row so store it in result[i][j]
            _mm_storeu_ps(&result2[i][0], sum1);
            _mm_storeu_ps(&result2[i][4], sum2);
    }
    
}

void Vector_matrix_multiply_256(float mat1[8][8] , float mat2[8][8] , float result3[8][8] , int size){

    int i , j , k;  
    __m256 a , b , sum;
    
    for (i = 0; i < size; ++i) {
        // Initializing 128-bit vector sum with all of its elements set to zero
        sum = _mm256_setzero_ps();
        
        for (j = 0; j < size; ++j) {
                // Broadcasting the value mat1[i][k] to all four elements of the vector a
                a = _mm256_set1_ps(mat1[i][j]);

                // Load the vector b from the specified memory address 
                b =  _mm256_loadu_ps(&mat2[j][0]);

                // Multiply the vector a and b and add with vector sum
                sum = _mm256_add_ps(sum, _mm256_mul_ps(a, b));
            
            }

            // Vector sum will conatain the result of first row so store it in result[i][0]
           _mm256_storeu_ps(&result3[i][0], sum);
        }    
}

int main(){

    int size = 8;
    // Input arrays
    float mat1[8][8] = {
        {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
        {2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0},
        {3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0},
        {4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0},
        {5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0},
        {6.0, 6.0, 6.0, 6.0, 6.0, 6.0, 6.0, 6.0},
        {7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0},
        {8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0}
    };
    float mat2[8][8] = {
        {8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0},
        {7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0},
        {6.0, 6.0, 6.0, 6.0, 6.0, 6.0, 6.0, 6.0},
        {5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0},
        {4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0},
        {3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0},
        {2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0},
        {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}
    };
    
    // Output arrays
    float result1[8][8] , result2[8][8]  , result3[8][8];
    
    // Scalar Float Matrix Multiplication
    auto start = std::chrono::high_resolution_clock::now();
    Scalar_matrix_multiply(mat1 , mat2 , result1);
    auto end = std::chrono::high_resolution_clock::now();
    print_results(result1);

    // Execution Time for Scalar Float Matrix Multiplication
    std::chrono::duration<double>  scalar_time = end - start;
    std::cout<<"\nExecution Time for Scalar Float Matrix Multiplication\t : "<< std::setprecision(4) << scalar_time.count()<<" microseconds\n" <<std::endl;
    
    // Vector Float Matrix Multiplication
    start = std::chrono::high_resolution_clock::now();
    Vector_matrix_multiply_128(mat1 , mat2 , result2 , size);
    end = std::chrono::high_resolution_clock::now();
    print_results(result2);

    // Execution Time for Vector Float Matrix Multiplication
    std::chrono::duration<double> vector_time1 = end - start;
    std::cout<<"\nExecution Time for 128-bit Vector Float Matrix Multiplication\t : "<< std::setprecision(4) << vector_time1.count()<<" microseconds\n" <<std::endl;

    // Vector Float Matrix Multiplication
    start = std::chrono::high_resolution_clock::now();
    Vector_matrix_multiply_256(mat1 , mat2 , result3 , size);
    end = std::chrono::high_resolution_clock::now();
    print_results(result3);

    // Execution Time for Vector Float Matrix Multiplication
    std::chrono::duration<double>  vector_time2 = end - start;
    std::cout<<"\nExecution Time for 256-bit Vector Float Matrix Multiplication\t : "<< std::setprecision(4) << vector_time2.count()<<" microseconds\n" <<std::endl;

    double performance1 = ((scalar_time.count() - vector_time1.count()) / scalar_time.count()) * 100 ;
    double performance2 = ((scalar_time.count() - vector_time2.count()) / scalar_time.count()) * 100 ;

    std::cout <<"Vector 128-bit Integer Addition is " << performance1 <<" \%  faster than scalar\n";
    std::cout <<"\nVector 256-bit Integer Addition is " << performance2 <<" \% faster than scalar\n";

    return 0;
}
