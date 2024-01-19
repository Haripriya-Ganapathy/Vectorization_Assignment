# include <iostream>
# include <chrono>
# include <immintrin.h>
# include <iomanip>

void Scalar_Multiplication (int arr1[] , int arr2[] , int result1[] , int size ) {

   for( int i = 0; i < size ; i++ ){
       // Multiply 2 arrays element wise
       result1 [i] = arr1 [i] * arr2 [i] ;
   }

}

void Vector_Multiplication_128 (int arr1[] , int arr2[] , int result2[] , int size ) {

    // Process 4 elements at a time using AVX2 intrinsics
    for (int i = 0; i < size - 3 ; i += 4) {
        // Load 128-bit vectors from arr1 and arr2
        __m128i vector1 = _mm_loadu_si128((__m128i*)(arr1 + i));
        __m128i vector2 = _mm_loadu_si128((__m128i*)(arr2 + i));

        // Perform 128-bit integer multiplication
        __m128i product = _mm_mullo_epi32(vector1, vector2);

        // Store the result back to memory
        _mm_storeu_si128((__m128i*)(result2 + i), product);
    }

    // Handle the remaining elements using scalar operations
    for (int i = size - (size % 4); i < size; ++i) {
        result2[i] = arr1[i] * arr2[i];
    }

}

void Vector_Multiplication_256 (int arr1[] , int arr2[] , int result3[] , int size ) {

    // Process 8 elements at a time using AVX2 intrinsics
    for ( int i = 0; i < size - 7 ; i += 8) {
        // Load 128-bit vectors from arr1 and arr2
        __m256i vector1 = _mm256_loadu_si256((__m256i*)(arr1 + i));
        __m256i vector2 = _mm256_loadu_si256((__m256i*)(arr2 + i));

        // Perform 128-bit integer multiplication
        __m256i product = _mm256_mullo_epi32(vector1, vector2);

        // Store the result back to memory
        _mm256_storeu_si256((__m256i*)(result3 + i) , product);
    }

    // Handle the remaining elements using scalar operations
    for ( int i = size - (size % 8); i < size; ++i) {
        result3[i] = arr1[i] * arr2[i];
    }

}

void Compare_Results(int result1[] , int result2[] , int result3[] , int size){

    for(int  i = 0; i < size ; i++){
        if(result1[i] != result2[i] && (result1[i] != result3[i])){
            std::cout<<"Both scalar and vector multiplication results are not equal!\n";
            return;
        }
    }
    std::cout<<"\nBoth scalar and vector multiplication results are equal!\n\n";

}

int main(){

    int size = 17;
    int arr1[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17} , arr2[] = {18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34} , result1[size] , result2[size] , result3[size];

    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    std::chrono::duration<double> scalar_time(0), vector_time(0) , avg_scalar_time , avg_vector_time;
 
    // Scalar Multiplication
    for (int i = 0; i < 1000000; ++i) {
       // This loop is just to calculate the small timing durations 
       start = std::chrono::high_resolution_clock::now();
       Scalar_Multiplication(arr1 , arr2 , result1 , size);
       end = std::chrono::high_resolution_clock::now();
       scalar_time += end - start;
    }
    
    // Execution Time for Scalar Multiplication
    avg_scalar_time = scalar_time / 1000000;
    std::cout<<"\nExecution Time for Scalar Integer Multiplication\t : "<< std::setprecision(4) << avg_scalar_time.count()<<" microseconds\n" <<std::endl;

    // Vector 128-bit Integer Multiplication
    for (int i = 0; i < 1000000; ++i) {
       // This loop is just to calculate the small timing durations
       start = std::chrono::high_resolution_clock::now(); 
       Vector_Multiplication_128(arr1 , arr2 , result2 , size);
       end = std::chrono::high_resolution_clock::now();
       vector_time += end - start;
    }

    // Execution Time for Vector 128-bit Integer Multiplication
    avg_vector_time = vector_time / 1000000;
    double performance1 = ((avg_scalar_time.count()) / (avg_vector_time.count()) * 100) ;
    std::cout<<"Execution Time for Vector 128-bit Integer Multiplication : "<< std::setprecision(4) << avg_vector_time.count()<<" microseconds\n" <<std::endl;

    // Vector 256-bit Integer Multiplication
    for (int i = 0; i < 1000000; ++i) {
       // This loop is just to calculate the small timing durations
       start = std::chrono::high_resolution_clock::now(); 
       Vector_Multiplication_256(arr1 , arr2 , result3 , size);
       end = std::chrono::high_resolution_clock::now();
       vector_time += end - start;
    }
    
    // Execution Time for Vector Multiplication
    avg_vector_time = vector_time / 1000000;
    double performance2 = ((avg_scalar_time.count()) / (avg_vector_time.count()) * 100) ;
    std::cout<<"Execution Time for Vector 256-bit Integer Multiplication : "<< std::setprecision(4) << avg_vector_time.count()<<" microseconds\n" <<std::endl;
    
    std::cout <<"Vector 128-bit Integer Multiplication is " << performance1 <<" \% faster than scalar\n";
    std::cout <<"\nVector 256-bit Integer Multiplication is " << performance2 <<" \% faster than scalar\n";

    // Validating the accuracy of Scalar and Vector Multiplication
    Compare_Results(result1 , result2 , result3 , size);
    
    return 0;
}
