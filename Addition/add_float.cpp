# include <iostream>
# include <chrono>
# include <iomanip>
# include <immintrin.h>

void Scalar_Addition (float arr1[] , float arr2[] , float result1[] , int size ) {

   for( int i = 0; i < size ; i++ ){
       // Add 2 arrays element wise
       result1 [i] = arr1 [i] + arr2 [i] ;
   }

}

void Vector_Addition_128 (float arr1[] , float arr2[] , float result2[] , int size ) {

    // Process 4 elements at a time using AVX2 intrinsics
    for ( int i = 0; i < size - 3 ; i += 4) {
        // Load 128-bit vectors from arr1 and arr2
        __m128 vector1 = _mm_loadu_ps(arr1 + i);
        __m128 vector2 = _mm_loadu_ps(arr2 + i);

        // Perform 128-bit float addition
        __m128 sum = _mm_add_ps(vector1, vector2);

        // Store the result back to memory
        _mm_storeu_ps((result2 + i), sum);
    }

    // Handle the remaining elements using scalar operations
    for (int i = size - (size % 4); i < size; ++i) {
        result2[i] = arr1[i] + arr2[i];
    }

}

void Vector_Addition_256 (float arr1[] , float arr2[] , float result3[] , int size ) {

    // Process 8 elements at a time using AVX2 intrinsics
    for ( int i = 0; i < size - 7; i += 8) {
        // Load 256-bit vectors from arr1 and arr2
        __m256 vector1 = _mm256_loadu_ps(arr1 + i);
        __m256 vector2 = _mm256_loadu_ps(arr2 + i);

        // Perform 256-bit float addition
        __m256 sum =  _mm256_add_ps (vector1, vector2);

        // Store the result back to memory
        _mm256_storeu_ps(result3 + i , sum);
    }

    // Handle the remaining elements using scalar operations
    for (int i = size - (size % 8); i < size; ++i) {
        result3[i] = arr1[i] + arr2[i];
    }

}

void Compare_Results(float result1[] , float result2[] , float result3[] , int size){

    for( int i = 0; i < size ; i++){
        if(result1[i] != result2[i] && (result1[i] != result3[i])){
            std::cout<<"Both scalar and vector float addition results are not equal!\n";
            return;
        }
    }
    std::cout<<"\nBoth scalar and vector float addition results are equal!\n\n";

}

int main(){

    int size = 17;
    float arr1[] = {1.2,2.3,3.4,4.5,5.6,6.7,7.8,8.9,9.0,10.1,11.2,12.3,13.4,14.5,15.6,16.7,17.8} , arr2[] = {18.9,19.0,20.1,21.2,22.3,23.4,24.5,25.6,26.7,27.8,28.9,29.0,30.1,31.2,32.3,33.4,34.5} , result1[size] , result2[size] , result3[size];
    
    std::chrono::time_point<std::chrono::high_resolution_clock > start, end;
    std::chrono::duration<double> scalar_time(0), vector_time(0) , avg_scalar_time , avg_vector_time;
 
    // Scalar Float Addition
    for (int i = 0; i < 1000000; ++i) {
        // This loop is just to calculate the small timing durations 
        start = std::chrono::high_resolution_clock ::now();
        Scalar_Addition(arr1 , arr2 , result1 , size);
        end = std::chrono::high_resolution_clock ::now();
        scalar_time += (end - start);
    }
    
    // Execution Time for Scalar Float Addition
    avg_scalar_time = scalar_time / 1000000;
    std::cout<<"\nExecution Time for Scalar float Addition\t : "<< std::setprecision(4) << avg_scalar_time.count()<<" microseconds\n" <<std::endl;

    // Vector 128-bit Float Addition
    for (int i = 0; i < 1000000; ++i) {
        // This loop is just to calculate the small timing durations 
        start = std::chrono::high_resolution_clock ::now();
        Vector_Addition_128(arr1 , arr2 , result2 , size);
        end = std::chrono::high_resolution_clock ::now();
        vector_time += (end - start);
    }

    // Execution Time for Vector 128-bit Float Addition
    avg_vector_time = vector_time / 1000000;
    double performance1 = ((avg_scalar_time.count()) / (avg_vector_time.count()) * 100) ;
    std::cout<<"Execution Time for Vector 128-bit float Addition : "<< std::setprecision(4) << avg_vector_time.count()<<" microseconds\n" <<std::endl;

    // Vector 256-bit Float Addition
    for (int i = 0; i < 1000000; ++i) {
        // This loop is just to calculate the small timing durations 
        start = std::chrono::high_resolution_clock ::now();
        Vector_Addition_256(arr1 , arr2 , result3 , size);
        end = std::chrono::high_resolution_clock ::now();
        vector_time += (end - start);
    }

    // Execution Time for Vector 256-bit Float Addition
    avg_vector_time = vector_time / 1000000;
    double performance2 = ((avg_scalar_time.count()) / (avg_vector_time.count()) * 100) ;
    std::cout<<"Execution Time for Vector 256-bit float Addition : "<< std::setprecision(4) << avg_vector_time.count()<<" microseconds\n" <<std::endl;

    std::cout <<"Vector 128-bit Integer Addition is " << performance1 <<" \%  faster than scalar\n";
    std::cout <<"\nVector 256-bit Integer Addition is " << performance2 <<" \% faster than scalar\n";

    // Validating the accuracy of Scalar and Vector Float Addition
    Compare_Results(result1 , result2 , result3 , size);
    
    return 0;
}