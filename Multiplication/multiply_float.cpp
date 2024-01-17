# include <iostream>
# include <chrono>
# include <immintrin.h>

void Scalar_Multiplication (float arr1[] , float arr2[] , float result1[] , int size ) {

   int i;
   for( i = 0; i < size ; i++ ){
       // Multiply 2 arrays element wise
       result1 [i] = arr1 [i] * arr2 [i] ;
   }

}

void Vector_Multiplication (float arr1[] , float arr2[] , float result2[] , int size ) {

    int i;
    for ( i = 0; i < size; i += 4) {
       // Load 128-bit vectors from arr1 and arr2
        __m128 vector1 = _mm_loadu_ps(&arr1[i]);
        __m128 vector2 = _mm_loadu_ps(&arr2[i]);

        // Perform 128-bit float multiplication
        __m128 product = _mm_mul_ps(vector1, vector2);

        // Store the result back to memory
        _mm_storeu_ps(&result2[i], product);
    }

}

void Compare_Results(float result1[] , float result2[] , int size){

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
    float arr1[] = {1.21,2.41,3.89,4.76} , arr2[] = {5.33,6.78,7.12,8.23} , result1[size] , result2[size];
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
    std::cout<<"\nExecution Time for Scalar float Multiplication : "<<scalar_time.count()<<" microseconds\n" <<std::endl;

    // Vector Multiplication
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 1000000; ++i){
        // This loop is just to calculate the small timing durations 
        Vector_Multiplication(arr1 , arr2 , result2 , size);
    }
    end = std::chrono::high_resolution_clock::now();
    
    // Execution Time for Vector Multiplication
    std::chrono::duration<double> vector_time = end - start;
    std::cout<<"Execution Time for Vector float Multiplication : "<<vector_time.count()<<" microseconds\n" <<std::endl;

    // Validating the accuracy of Scalar and Vector Multiplication
    Compare_Results(result1 , result2 , size);
    
    return 0;
}