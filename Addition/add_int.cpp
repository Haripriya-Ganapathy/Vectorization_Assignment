# include <iostream>
# include <chrono>
# include <iomanip>
# include <immintrin.h>

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

    int size = 17;
    int arr1[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17} , arr2[] = {18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34} , result1[size] , result2[size] , result3[size];
    
    //std::chrono::time_point<std::chrono::high_resolution_clock > start, end;
    std::chrono::duration<double> scalar_time(0), vector_time1(0) , vector_time2(0) ,avg_scalar_time , avg_vector_time ;
    
    // Scalar Addition
    for (int i = 0; i < 1000000; ++i) {
        // This loop is just to calculate the small timing durations
        auto start = std::chrono::high_resolution_clock ::now(); 
        Scalar_Addition(arr1 , arr2 , result1 , size);
        auto end = std::chrono::high_resolution_clock ::now();
        scalar_time += (end - start);
    }
    
    // Execution Time for Scalar Addition
    avg_scalar_time = scalar_time / 1000000 ;
    std::cout << "\nExecution Time for Scalar Integer Addition\t   : " <<  std::setprecision(4) << avg_scalar_time.count() << " microseconds\n" << std::endl;

    // Vector 128-bit Integer Addition
    for (int i = 0; i < 1000000; ++i) {
        // This loop is just to calculate the small timing durations 
        auto start = std::chrono::high_resolution_clock ::now();
        Vector_Addition_128(arr1 , arr2 , result2 , size);
        auto end = std::chrono::high_resolution_clock ::now();
        vector_time1 += end - start;
    }
    
    // Execution Time for Vector 128-bit Integer Addition
    avg_vector_time = vector_time1 / 100000;
    double performance1 = ((avg_scalar_time.count()) / (avg_vector_time.count()) * 100) ;
    std::cout << "Execution Time for Vector 128-bit Integer Addition : " << std::setprecision(4) << avg_vector_time.count() << " microseconds\n" << std::endl;

    // Vector 256-bit Integer Addition
    for (int i = 0; i < 1000000; ++i) {
        // This loop is just to calculate the small timing durations 
        auto start = std::chrono::high_resolution_clock ::now();
        Vector_Addition_256(arr1 , arr2 , result3 , size);
        auto end = std::chrono::high_resolution_clock ::now();
        vector_time2 += end - start;
    }

    // Execution Time for Vector 256-bit Addition
    avg_vector_time = vector_time2 / 100000;
    double performance2 = ((avg_scalar_time.count()) / (avg_vector_time.count()) * 100) ;
    std::cout << "Execution Time for Vector 256-bit Integer Addition : " << std::setprecision(4) << avg_vector_time.count() << " microseconds\n" << std::endl;
    
    std::cout <<"Vector 128-bit Integer Addition is " << performance1 <<" \% faster than scalar\n";
    std::cout <<"\nVector 256-bit Integer Addition is " << performance2 <<" \% faster than scalar\n";

    // Validating the accuracy of Scalar and Vector Addition
    Compare_Results(result1 , result2 , result3 , size);
    
    return 0;
}
