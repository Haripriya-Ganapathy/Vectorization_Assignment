# include <iostream>
# include <immintrin.h>
# include <chrono>
# include <iomanip>

void Print_Output_Arrays(int out[])
{
    int i;
    for(i = 0; i < 4; i++){
        std::cout<<out[i]<<"\t";
    }
    std::cout<<"\n";
}

void Print_Char_Output_Arrays(int vec2[], unsigned char out[])
{
    int i;
    for(i = 0; i < 4; i++){
        out[i] = vec2[i];
        std::cout<<(int)(out[i])<<"\t";
    }
    std::cout<<"\n";
}

static  int av_clip(int a, int amin, int amax)
{
    if (a < amin)      return amin;
    else if (a > amax) return amax;
    else               return a;
}

void av_clip_vectorize(int a[], int amin[], int amax[] , int vec1[])
{
    // Load the integers into separate  vector 
    __m128i var = _mm_load_si128((__m128i*)(a));
    __m128i min = _mm_load_si128((__m128i*)(amin));
    __m128i max = _mm_load_si128((__m128i*)(amax));

    // Perform the operation
    __m128i res = _mm_max_epi32(_mm_min_epi32(var, max), min);
    
    // Store the result into integer variable
    _mm_storeu_si128((__m128i*)(vec1), res);

}

static unsigned char av_clip_uint8(int a)
{
    if (a & (~0xFF))    return (-a) >> 31;
    else                return a;
}

void av_clip_uint8_vectorize(int a[] , int vec2[])
{
    // Load 'a' into a SIMD register
    __m128i aVec = _mm_load_si128((__m128i*)(a));  
    
    // Set a mask with all bits set to 1 for the lowest byte
    __m128i Val  = _mm_set1_epi32(0xFF); 
    
    // Use SIMD bitwise AND to check if any bits outside the least significant 8 bits are set
    __m128i checkMask1 = _mm_andnot_si128(Val, aVec); 

    //To check whether checkMask1 values are non zero or not
    __m128i checkMask2 = _mm_cmpeq_epi32(checkMask1, _mm_setzero_si128());

    // Use SIMD right shift operation to get the result based on the sign of 'a'
    __m128i resultVec = _mm_srai_epi32(-aVec, 31); 
    
    // Use SIMD blend to select values based on the condition
    __m128i finalResult = _mm_blendv_epi8(resultVec, aVec,  checkMask2); 
    
    // Extract the result from the SIMD register
    _mm_storeu_si128((__m128i*)(vec2), finalResult);   

}

int main()
{
    unsigned char char_result1 , char_result2[4] ;
    int result, aVal[] = {3, 10, -4, 7} , aMin[] = {-2, -12, -3, -8} , aMax[] = {2, 12, 3, 8} , aChar[] = {500, -6, 260, -256}, vec1[4] , vec2[4]; 
    
    /* Vectorize av_clip function */
    std::cout<<"\nCalling av_clip function";
    auto start1 = std::chrono::high_resolution_clock::now();
    result = av_clip(3,-2,2);
    auto end1 = std::chrono::high_resolution_clock::now();
    std::cout<<"\nScalar Result: "<< result;

    // Execution Time for Scalar av_clip Funtion
    std::chrono::duration<double> scalar_time = end1 - start1;
    std::cout<<"\nExecution Time for Scalar av_clip Funtion\t\t : "<< std::setprecision(3) << scalar_time.count()<<" microseconds\n" <<std::endl;

    std::cout<<"\nCalling av_clip Vectorized function\n";
    auto start2 = std::chrono::high_resolution_clock::now();
    av_clip_vectorize(aVal, aMin, aMax, vec1);
    auto end2 = std::chrono::high_resolution_clock::now();

    std::cout<<"Arrays with a value\t: ";    Print_Output_Arrays(aVal);
    std::cout<<"Arrays with aMin value\t: "; Print_Output_Arrays(aMin);
    std::cout<<"Arrays with aMax value\t: "; Print_Output_Arrays(aMax);
    std::cout<<"Vector result\t\t: ";        Print_Output_Arrays(vec1);

    // Execution Time for Vectorized av_clip Function
    std::chrono::duration<double> vector_time = end2 - start2;
    std::cout<<"Execution Time for Vectorized av_clip Funtion\t\t : "<< std::setprecision(3) << vector_time.count()<<" microseconds\n" <<std::endl;
    
    /* Vectorize av_clip_uint8 function */
    std::cout<<"Calling av_clip_unit8 function\n";
    start1 = std::chrono::high_resolution_clock::now();
    char_result1 = av_clip_uint8(256);
    end1 = std::chrono::high_resolution_clock::now();
    std::cout<<"Scalar Result: "<< (int)char_result1;

    // Execution Time for Scalar av_clip_uint8 Funtion
    scalar_time = end1 - start1;
    std::cout<<"\nExecution Time for Scalar av_clip_uint8 Funtion\t\t : "<< std::setprecision(3) << scalar_time.count()<<" microseconds\n" <<std::endl;

    std::cout<<"\nCalling av_clip_uint8 Vectorized function\n";
    start2 = std::chrono::high_resolution_clock::now();
    av_clip_uint8_vectorize(aChar, vec2);
    end2 = std::chrono::high_resolution_clock::now();

    std::cout<<"Arrays with a value\t: ";    Print_Output_Arrays(aChar);
    std::cout<<"Vector result\t\t: ";        Print_Char_Output_Arrays(vec2, char_result2);

    //Execution Time for Vectorized av_clip_uint8 Function
    vector_time = end2 - start2;
    std::cout<<"Execution Time for Vectorized av_clip_uint8 Funtion\t : "<< std::setprecision(3) << vector_time.count()<<" microseconds\n" <<std::endl;
    
    return 0;
}
