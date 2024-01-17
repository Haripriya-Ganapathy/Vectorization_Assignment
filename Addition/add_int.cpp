# include <iostream>
# include <chrono>
# include <immintrin.h>

void Scalar_Addition (int arr1[] , int arr2[] , int result1[] , int size ) {

   int i;
   for( i = 0; i < size ; i++ ){
       // Add 2 arrays element wise
       result1 [i] = arr1 [i] + arr2 [i] ;
   }

}

void Vector_Addition (int arr1[] , int arr2[] , int result2[] , int size ) {

    int i;
    for ( i = 0; i < size; i += 4) {
        __m128i vector1 = _mm_loadu_si128((__m128i*)&arr1[i]);
        __m128i vector2 = _mm_loadu_si128((__m128i*)&arr2[i]);

        // Perform 128-bit integer addition
        __m128i sum = _mm_add_epi32(vector1, vector2);

        // Store the result back to memory
        _mm_storeu_si128((__m128i*)&result2[i], sum);
    }

}

void Compare_Results(int result1[] , int result2[] , int size){

    int i;
    for( i = 0; i < size ; i++){
        if(result1[i] != result2[i]){
            std::cout<<"Both scalar and vector addition results are not equal!\n";
            return;
        }
    }
    std::cout<<"Both scalar and vector addition results are equal!\n";

}

int main(){

    int size = 4, i;
    int arr1[] = {1,2,3,4} , arr2[] = {5,6,7,8} , result1[size] , result2[size];
    std::chrono::time_point<std::chrono::high_resolution_clock > start, end;

    // Scalar Addition
    start = std::chrono::high_resolution_clock ::now();
    for (int i = 0; i < 1000000; ++i) {
        // This loop is just to calculate the small timing durations 
        Scalar_Addition(arr1 , arr2 , result1 , size);
    }
    end = std::chrono::high_resolution_clock ::now();
    
    // Execution Time for Scalar Addition
    std::chrono::duration<double> scalar_time = end - start;
    std::cout << "Execution Time for Scalar Addition : " << scalar_time.count() << " microseconds\n" << std::endl;

    // Vector Addition
    start = std::chrono::high_resolution_clock ::now();
    for (int i = 0; i < 1000000; ++i) {
        // This loop is just to calculate the small timing durations 
        Vector_Addition(arr1 , arr2 , result2 , size);
    }
    end = std::chrono::high_resolution_clock ::now();
    
    // Execution Time for Vector Addition
    std::chrono::duration<double> vector_time = end - start;
    std::cout << "Execution Time for Vector Addition : " << vector_time.count() << " microseconds\n" << std::endl;

    // Validating the accuracy of Scalar and Vector Addition
    Compare_Results(result1 , result2 , size);
    
    return 0;
}
