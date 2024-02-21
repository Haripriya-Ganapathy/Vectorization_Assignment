# include <iostream>
# include <chrono>
# include <immintrin.h>
# include <iomanip>

void print_results(float res[8][8]){

    int i,j;
    for(i=0;i<8;i++){
       for(j=0;j<8;j++){
            std::cout<<res[i][j]<<" ";
       }
       std::cout<<"\n";
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

void Vector_matrix_multiply_128(float mat1[8][8] , float mat2[8][8] , float result2[8][8] , int size){

    int i , j , k;  
    __m128 a , b , sum;
    
    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
            // Initializing 128-bit vector sum with all of its elements set to zero
            sum = _mm_setzero_ps();

            for(k = 0; k < size; ++k) {
                // Broadcasting the value mat1[i][k] to all four elements of the vector a
                a = _mm_set_ps1(mat1[i][k]);

                // Load the vector b from the specified memory address 
                b = _mm_loadu_ps(&mat2[k][j]);

                // Multiply the vector a and b and add with vector sum
                sum = _mm_add_ps(sum, _mm_mul_ps(a, b));
            
            }

            // Vector sum will conatain the result of first element so store it in result[i][j]
            _mm_storeu_ps(&result2[i][j], sum);
        }
    }
    
}

void Vector_matrix_multiply_256(float mat1[8][8] , float mat2[8][8] , float result3[8][8] , int size){

    int i , j , k;  
    __m256 a , b , sum;
    
    for (i = 0; i < size; ++i) {
        for (j = 0; j < size; ++j) {
            // Initializing 128-bit vector sum with all of its elements set to zero
            sum = _mm256_setzero_ps();

            for(k = 0; k < size; ++k) {
                // Broadcasting the value mat1[i][k] to all four elements of the vector a
                a = _mm256_set1_ps(mat1[i][k]);

                // Load the vector b from the specified memory address 
                b =  _mm256_loadu_ps(&mat2[k][j]);

                // Multiply the vector a and b and add with vector sum
                sum = _mm256_add_ps(sum, _mm256_mul_ps(a, b));
            
            }

            // Vector sum will conatain the result of first element so store it in result[i][j]
           _mm256_storeu_ps(&result3[i][j], sum);
        }
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
    // double s_time = std::chrono::duration_cast<std::chrono::microseconds>(scalar_time).count();
    std::cout<<"\nExecution Time for Scalar Float Matrix Multiplication\t : "<< std::setprecision(4) << scalar_time.count()<<" microseconds\n" <<std::endl;
    
    // Vector Float Matrix Multiplication
    start = std::chrono::high_resolution_clock::now();
    Vector_matrix_multiply_128(mat1 , mat2 , result2 , size);
    // std::cout<<"\n";
    
    end = std::chrono::high_resolution_clock::now();
print_results(result2);
    // Execution Time for Vector Float Matrix Multiplication
    std::chrono::duration<double> vector_time1 = end - start;
    // double v_time1 = std::chrono::duration_cast<std::chrono::microseconds>(vector_time1).count();
    std::cout<<"\nExecution Time for 128-bit Vector Float Matrix Multiplication\t : "<< std::setprecision(4) << vector_time1.count()<<" microseconds\n" <<std::endl;

    // Vector Float Matrix Multiplication
    start = std::chrono::high_resolution_clock::now();
    Vector_matrix_multiply_256(mat1 , mat2 , result3 , size);
    // std::cout<<"\n";
   
    end = std::chrono::high_resolution_clock::now();
 print_results(result3);
    // Execution Time for Vector Float Matrix Multiplication
     std::chrono::duration<double>  vector_time2 = end - start;
    // double v_time2 = std::chrono::duration_cast<std::chrono::microseconds>(vector_time2).count();
    std::cout<<"\nExecution Time for 128-bit Vector Float Matrix Multiplication\t : "<< std::setprecision(4) << vector_time2.count()<<" microseconds\n" <<std::endl;

       double performance1 = ((scalar_time.count() - vector_time1.count()) / scalar_time.count()) * 100 ;
    double performance2 = ((scalar_time.count() - vector_time2.count()) / scalar_time.count()) * 100 ;

    std::cout <<"Vector 128-bit Integer Addition is " << performance1 <<" \%  faster than scalar\n";
    std::cout <<"\nVector 256-bit Integer Addition is " << performance2 <<" \% faster than scalar\n";

    return 0;
}
