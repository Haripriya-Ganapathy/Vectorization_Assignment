# include <iostream>
# include <chrono>
# include <immintrin.h>

void Scalar_Multiplication (int arr1[] , int arr2[] , int result1[] , int size ) {

   int i;
   for( i = 0; i < size ; i++ ){
       // Multiply 2 arrays element wise
       result1 [i] = arr1 [i] * arr2 [i] ;
   }

}

void Vector_Multiplication (int arr1[] , int arr2[] , int result2[] , int size ) {

    int i;
    for ( i = 0; i < size; i += 4) {
        // Load 128-bit vectors from arr1 and arr2
        __m128i vector1 = _mm_loadu_si128((__m128i*)&arr1[i]);
        __m128i vector2 = _mm_loadu_si128((__m128i*)&arr2[i]);

        // Perform 128-bit integer multiplication
        __m128i product = _mm_mullo_epi32(vector1, vector2);

        // Store the result back to memory
        _mm_storeu_si128((__m128i*)&result2[i], product);
    }

}

void Compare_Results(int result1[] , int result2[] , int size){

    int i;
    for( i = 0; i < size ; i++){
        if(result1[i] != result2[i]){
            std::cout<<"Both scalar and vector multiplication results are not equal!\n";
            return;
        }
    }
    std::cout<<"Both scalar and vector multiplication results are equal!\n";

}

int main(){

    int size = 4, i;
    int arr1[] = {1,2,3,4} , arr2[] = {5,6,7,8} , result1[size] , result2[size];
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
 
    // Scalar Multiplication
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000; ++i) {
       // This loop is just to calculate the small timing durations 
       Scalar_Multiplication(arr1 , arr2 , result1 , size);
    }
    end = std::chrono::high_resolution_clock::now();
    
    // Execution Time for Scalar Multiplication
    std::chrono::duration<double> scalar_time = end - start;
    std::cout<<"\nExecution Time for Scalar Multiplication : "<<scalar_time.count()<<" microseconds\n" <<std::endl;

    // Vector Multiplication
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000; ++i) {
       // This loop is just to calculate the small timing durations 
       Vector_Multiplication(arr1 , arr2 , result2 , size);
    }
    end = std::chrono::high_resolution_clock::now();
    
    // Execution Time for Vector Multiplication
    std::chrono::duration<double> vector_time = end - start;
    std::cout<<"Execution Time for Vector Multiplication : "<<vector_time.count()<<" microseconds\n" <<std::endl;

    // Validating the accuracy of Scalar and Vector Multiplication
    Compare_Results(result1 , result2 , size);
    
    return 0;
}
