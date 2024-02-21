# include <iostream>
# include <chrono>
# include <iomanip>
# include <immintrin.h>
#include <cstdlib> 

void Scalar_Addition (int arr1[] , int arr2[] , int result1[] , int size ) {

   for( int i = 0; i < size ; i++ ){
       // Add 2 arrays element wise
       result1 [i] = arr1 [i] + arr2 [i] ;
   }

}

void Vector_Addition_128 (int arr1[] , int arr2[] , int result2[] , int size ) {

    // Process 4 elements at a time using AVX2 intrinsics
    for ( int i = 0; i < size - 3; i += 4) {
        __m128i vector1 = _mm_loadu_si128((__m128i*)(arr1 + i));
        __m128i vector2 = _mm_loadu_si128((__m128i*)(arr2 + i));

        // Perform 128-bit integer addition
        __m128i sum = _mm_add_epi32(vector1, vector2);

        // Store the result back to memory
        _mm_storeu_si128((__m128i*)(result2 + i), sum);
    }

    // Handle the remaining elements using scalar operations
    for (int i = size - (size % 4); i < size; ++i) {
        result2[i] = arr1[i] + arr2[i];
    }

}

void Vector_Addition_256 (int arr1[] , int arr2[] , int result3[] , int size ) {

    // Process 8 elements at a time using AVX2 intrinsics
    for ( int i = 0; i < size - 7; i += 8) {
        // Load 256-bit vectors from arr1 and arr2
        __m256i vector1 = _mm256_loadu_si256(( __m256i*)(arr1 + i));
        __m256i vector2 = _mm256_loadu_si256(( __m256i*)(arr2 + i));

        // Perform 256-bit integer addition
        __m256i sum = _mm256_add_epi32(vector1, vector2);

        // Store the result back to memory
        _mm256_storeu_si256(( __m256i*)(result3 + i), sum);
    }
    
    // Handle the remaining elements using scalar operations
    for ( int i = size - (size % 8); i < size; ++i) {
        result3[i] = arr1[i] + arr2[i];
    }
    
}

void Compare_Results(int result1[] , int result2[] , int result3[] , int size){

    for( int i = 0; i < size ; i++){
        if((result1[i] != result2[i]) && (result1[i] != result3[i])){
            std::cout<<"Both scalar and vector addition results are not equal!\n";
            return;
        }
    }
    std::cout<<"\nBoth scalar and vector addition results are equal!\n\n";
}

int main(){

    int size = 10000;
    int arr1[size] , arr2[size]  , result1[size] , result2[size] , result3[size];
    
    std::chrono::duration<double> scalar_time, vector_time1 , vector_time2 ;

    for(int i=0;i<10000;i++){
        arr1[i] = rand();
        arr2[i] = rand();
    }
    
    // Scalar Addition
    auto start = std::chrono::high_resolution_clock ::now(); 
    Scalar_Addition(arr1 , arr2 , result1 , size);
    auto end = std::chrono::high_resolution_clock ::now();
    scalar_time = end - start;
    
    // Execution Time for Scalar Addition
    std::cout << "\nExecution Time for Scalar Integer Addition\t   : " <<  std::setprecision(4) << scalar_time.count() << " microseconds\n" << std::endl;

    // Vector 128-bit Integer Addition
    start = std::chrono::high_resolution_clock ::now();
    Vector_Addition_128(arr1 , arr2 , result2 , size);
    end = std::chrono::high_resolution_clock ::now();
    vector_time1 = end - start;
    
    // Execution Time for Vector 128-bit Integer Addition
    double performance1 = ((scalar_time.count() - vector_time1.count()) / scalar_time.count()) * 100 ;
    std::cout << "Execution Time for Vector 128-bit Integer Addition : " << std::setprecision(4) << vector_time1.count() << " microseconds\n" << std::endl;

    
    start = std::chrono::high_resolution_clock ::now();
    Vector_Addition_256(arr1 , arr2 , result3 , size);
    end = std::chrono::high_resolution_clock ::now();
    vector_time2 = end - start;

    // Execution Time for Vector 256-bit Addition
    double performance2 = ((scalar_time.count() - vector_time2.count()) / scalar_time.count())* 100 ;
    std::cout << "Execution Time for Vector 256-bit Integer Addition : " << std::setprecision(4) << vector_time2.count() << " microseconds\n" << std::endl;
    
    std::cout <<"Vector 128-bit Integer Addition is " << performance1 <<" \% faster than scalar\n";
    std::cout <<"\nVector 256-bit Integer Addition is " << performance2 <<" \% faster than scalar\n";

    // Validating the accuracy of Scalar and Vector Addition
    Compare_Results(result1 , result2 , result3 , size);
    
    return 0;
}