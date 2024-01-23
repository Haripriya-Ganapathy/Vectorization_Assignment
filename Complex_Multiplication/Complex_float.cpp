# include <iostream>
# include <chrono>
# include <immintrin.h>
# include <iomanip>

void Print_Output_Arrays(float out[]){

    int i;
    for(i = 0 ; i < 4 ; i++){
        //Printing the output array
        std::cout << out[i] <<" ";
    }   
    std::cout << "\n";

}

void Print_Vector_Output_Arrays(float out[]){

    int i;
    for(i = 0 ; i < 8 ; i++){
        //Printing the output array
        std::cout << out[i] <<" ";
    }   
    std::cout << "\n";

}

void Scalar_Complex_Multiply(float x[] , float y[] , float output[]) {

    /*
      x = {a,b,d,e} i.e a + bi and d + ei
      y = {c,d,f,g} i.e c + di and f + gi
      output = {ac - bd , ad + bc , df - eg , gf + ef}

    */
    int i;
    for(i = 0 ; i < 4 ; i += 2){ // Incrementing i by 2 times
        //Real part
        output[i] = (x[i] * y[i]) - (x[i+1] * y[i+1]);

        // Imaginary part
        output[i+1] = (x[i] * y[i+1]) + (x[i+1] * y[i]);
    }
}

void Vector_Complex_Multiply_128(float v1[] ,float v2[] ,float final_result[]){

    // Load 128-bit vectors from v1 and v2
    __m128 vector1 = _mm_loadu_ps(v1);
    __m128 vector2 = _mm_loadu_ps(v2);

    // Unpack the complex numbers into separate real and imaginary parts
    __m128 real1 = _mm_shuffle_ps(vector1, vector1, _MM_SHUFFLE(0, 2, 0, 2));
    __m128 imag1 = _mm_shuffle_ps(vector1, vector1, _MM_SHUFFLE(1, 3, 1, 3));
    __m128 real2 = _mm_shuffle_ps(vector2, vector2, _MM_SHUFFLE(0, 2, 0, 2));
    __m128 imag2 = _mm_shuffle_ps(vector2, vector2, _MM_SHUFFLE(1, 3, 1, 3));

    // Perform complex number multiplication
    __m128 result_real = _mm_sub_ps(_mm_mul_ps(real1, real2), _mm_mul_ps(imag1, imag2));
    __m128 result_imag = _mm_add_ps(_mm_mul_ps(real1, imag2), _mm_mul_ps(imag1, real2));

    // Pack the result into [real, imag, real, imag] format
    __m128 result_combined1 = _mm_shuffle_ps(result_real, result_imag, _MM_SHUFFLE(0, 1, 0, 1));
    __m128 result_combined2 = _mm_shuffle_ps(result_combined1, result_combined1, _MM_SHUFFLE(3, 1, 2, 0));

    // Store the result
    _mm_storeu_ps(final_result, result_combined2);

}

void Vector_Complex_Multiply_256(float v1[] , float v2[] , float v3[] , float v4[] , float final_result[]){

    // Load 128-bit vectors from v1 and v3
    __m128 vector1 = _mm_loadu_ps(v1);
    __m128 vector2 = _mm_loadu_ps(v3);

    // Combine the two vectors into a single 256-bit vector
    __m256 vector_comb1 = _mm256_insertf128_ps(_mm256_castps128_ps256(vector1), vector2, 1);

    // Load 128-bit vectors from v2 and v4
    __m128 vector3 = _mm_loadu_ps(v2);
    __m128 vector4 = _mm_loadu_ps(v4);

    // Combine the two vectors into a single 256-bit vector
    __m256 vector_comb2 = _mm256_insertf128_ps(_mm256_castps128_ps256(vector3), vector4, 1);

    // Unpack the complex numbers into separate real and imaginary parts
    __m256 real1 = _mm256_shuffle_ps(vector_comb1, vector_comb1, _MM_SHUFFLE(0, 2, 0, 2));
    __m256 imag1 = _mm256_shuffle_ps(vector_comb1, vector_comb1, _MM_SHUFFLE(1, 3, 1, 3));
    __m256 real2 = _mm256_shuffle_ps(vector_comb2, vector_comb2, _MM_SHUFFLE(0, 2, 0, 2));
    __m256 imag2 = _mm256_shuffle_ps(vector_comb2, vector_comb2, _MM_SHUFFLE(1, 3, 1, 3));

    // Perform complex number multiplication
    __m256 result_real = _mm256_sub_ps(_mm256_mul_ps(real1, real2), _mm256_mul_ps(imag1, imag2));
    __m256 result_imag = _mm256_add_ps(_mm256_mul_ps(real1, imag2), _mm256_mul_ps(imag1, real2));

    // Pack the result into [real, imag, real, imag] format
    __m256 result_combined1 = _mm256_shuffle_ps(result_real, result_imag, _MM_SHUFFLE(0, 1, 0, 1));
    __m256 result_combined2 = _mm256_shuffle_ps(result_combined1, result_combined1, _MM_SHUFFLE(3, 1, 2, 0));

    // Store the result
    _mm256_storeu_ps(final_result, result_combined2);

}

int main(){
    
    //Input Arrays
    float  a[] = {2.0,1.0,4.0,3.0} , b[] = {4.0,3.0,2.0,1.0} , c[] = {1.0,2.0,3.0,4.0} , d[] = {3.0,4.0,1.0,2.0};
    float  e[] = {4.0,3.0,2.0,1.0} , f[] = {2.0,1.0,4.0,3.0} , g[] = {3.0,4.0,1.0,2.0} , h[] = {1.0,2.0,3.0,4.0};
    
    //Output Arrays
    float out1[4] , out2[4] , out3[4] , out4[4] ;
    float vec1[4] , vec2[4] , vec3[4] , vec4[4] ;
    float vect1[8] , vect2[8];

    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    std::chrono::duration<double> scalar_time, vector_time1 , vector_time2 ;

    /* Scalar Complex Multiplication */ 
    std::cout<<"Scalar Results:\n-------------------\n";
    
    // Complex Multiply a and b 
    start = std::chrono::high_resolution_clock::now();
    Scalar_Complex_Multiply(a , b , out1);
    Print_Output_Arrays(out1);

    // Complex Multiply c and d 
    Scalar_Complex_Multiply(c , d , out2);
    Print_Output_Arrays(out2);

    // Complex Multiply e and f 
    Scalar_Complex_Multiply(e , f , out3);
    Print_Output_Arrays(out3);

    // Complex Multiply g and h 
    Scalar_Complex_Multiply(g , h , out4);
    Print_Output_Arrays(out4);

    std::cout<<"-------------------\n\n";
    end = std::chrono::high_resolution_clock::now();
    scalar_time = end - start;

    /* Vector Complex 128-bit Multiplication */
    std::cout<<"Vector Results for 128-bit :\n-------------------\n";

    // Vector Complex Multiply a and b 
    start = std::chrono::high_resolution_clock::now();
    Vector_Complex_Multiply_128(a , b , vec1);
    Print_Output_Arrays(vec1);
   
    // Vector Complex Multiply c and d 
    Vector_Complex_Multiply_128(c , d , vec2);
    Print_Output_Arrays(vec2);
   
    // Vector Complex Multiply e and f 
    Vector_Complex_Multiply_128(e , f , vec3);
    Print_Output_Arrays(vec3);
   
    // Vector Complex Multiply g and h 
    Vector_Complex_Multiply_128(g , h , vec4);
    Print_Output_Arrays(vec4);

    std::cout<<"-------------------\n\n";    
    end = std::chrono::high_resolution_clock::now();
    vector_time1 = end - start;

    /* Vector Complex 256-bit Multiplication */
    std::cout<<"Vector Results for 256-bit :\n-------------------\n";

    // Complex Multiply a & b and c & d
    start = std::chrono::high_resolution_clock::now();
    Vector_Complex_Multiply_256(a , b , c , d, vect1);
    Print_Vector_Output_Arrays(vect1);

    // Complex Multiply e & f and g & h
    Vector_Complex_Multiply_256(e , f , g , h ,vect2);
    Print_Vector_Output_Arrays(vect2);

    std::cout<<"-------------------\n";
    end = std::chrono::high_resolution_clock::now();
    vector_time2 = end - start;

    // Execution Time
    std::cout<<"\nExecution Time for Scalar float Multiplication         : "<< std::setprecision(4) << scalar_time.count()<<" microseconds\n" <<std::endl;
    std::cout<<"Execution Time for Vector 128-bit float Multiplication : "<< std::setprecision(4) << vector_time1.count()<<" microseconds\n" <<std::endl;
    std::cout<<"Execution Time for Vector 256-bit float Multiplication : "<< std::setprecision(4) << vector_time2.count()<<" microseconds\n" <<std::endl;    

    return 0;
}