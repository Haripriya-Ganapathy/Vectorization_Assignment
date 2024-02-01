# include <iostream>
# include <immintrin.h>
# include <chrono>
# include <algorithm>
# include <iomanip>

void Print_Output_Arrays(int out[], int size)
{
    int i;
    for(i = 0; i < size; i++){
        std::cout<<out[i]<<"\t";
    }
    std::cout<<"\n";
}

void Print_Char_Arrays(unsigned char out[], int size)
{
    int i;
    for(i = 0; i < size; i++){
        std::cout<<(int)out[i]<<"\t";
    }
    std::cout<<"\n";
}

void Print_Char_Output_Arrays(int vec[], unsigned char out[], int size)
{
    int i;
    for(i = 0; i < size; i++){
        out[i] = vec[i];
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

void av_clip_vectorize_128(int a[], int amin[], int amax[] , int vec1[], int size)
{
    int i;
    for ( i = 0; i < size - 3; i += 4) {
    // Load the integers into separate  vector 
    __m128i var = _mm_load_si128((__m128i*)(a + i));
    __m128i min = _mm_load_si128((__m128i*)(amin + i));
    __m128i max = _mm_load_si128((__m128i*)(amax + i));

    // Perform the operation
    __m128i res = _mm_max_epi32(_mm_min_epi32(var, max), min);
    
    // Store the result into integer array
    _mm_storeu_si128((__m128i*)(vec1 + i), res);
    }

    for (i = size - (size % 4); i < size; ++i) {
        vec1[i] = std::max(std::min(a[i], amax[i]), amin[i]);
    }

}

void av_clip_vectorize_256(int a[], int amin[], int amax[] , int vec2[], int size)
{
    int i;
    for ( i = 0; i < size - 7; i += 8) {
    // Load the integers into separate  vector 
    __m256i var =  _mm256_loadu_si256((__m256i*)(a + i));
    __m256i min =  _mm256_loadu_si256((__m256i*)(amin + i));
    __m256i max =  _mm256_loadu_si256((__m256i*)(amax + i));

    // Perform the operation
    __m256i res = _mm256_max_epi32(_mm256_min_epi32(var, max), min);
    
    // Store the result into integer array 
    _mm256_storeu_si256((__m256i*)(vec2 + i), res);
    }
    
    for (i = size - (size % 8); i < size; ++i) {
        vec2[i] =  std::max(std::min(a[i], amax[i]), amin[i]);
    }

}

static unsigned char av_clip_uint8(int a)
{
    if (a & (~0xFF))    return (-a) >> 31;
    else                return a;
}

void av_clip_uint8_vectorize_128(int a[] , int vec3[], int size)
{
    int i;
    for ( i = 0; i < size - 3; i += 4) {
    // Load 'a' into a SIMD register
    __m128i aVec = _mm_load_si128((__m128i*)(a + i));  
    
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
    _mm_storeu_si128((__m128i*)(vec3 + i), finalResult);   
    }

    for (i = size - (size % 4); i < size; ++i) {
        vec3[i] = (a[i] & (~0xFF)) ? (-a[i]) >> 31 : a[i];
    }

}

void av_clip_uint8_vectorize_128_opt(int a[] , int vec3[], int size)
{
    int i;
    for ( i = 0; i < size - 3; i += 4) {
    // Load 'a' into a SIMD register
    __m128i aVec = _mm_load_si128((__m128i*)(a + i));

    __m128i aMin = _mm_min_epi32(aVec, _mm_set1_epi32(255));
    __m128i aRes = _mm_max_epi32(aMin, _mm_setzero_si128()); 
    
    // Extract the result from the SIMD register
    _mm_storeu_si128((__m128i*)(vec3 + i), aRes);   
    }

    for (i = size - (size % 4); i < size; ++i) {
        vec3[i] = (a[i] & (~0xFF)) ? (-a[i]) >> 31 : a[i];
    }

}
void av_clip_uint8_vectorize_256_opt(int a[] , int vec3[], int size)
{
    int i;
    for ( i = 0; i < size - 7; i += 8) {
    // Load 'a' into a SIMD register
    __m256i aVec = _mm256_loadu_si256((__m256i*)(a + i));  

    __m256i aMin = _mm256_min_epi32(aVec, _mm256_set1_epi32(255));
    __m256i aRes = _mm256_max_epi32(aMin,  _mm256_setzero_si256()); 
    
    // Extract the result from the SIMD register
    _mm256_storeu_si256((__m256i*)(vec3 + i), aRes);   
    }
    for (i = size - (size % 8); i < size; ++i) {
        vec3[i] = (a[i] & (~0xFF)) ? (-a[i]) >> 31 : a[i];
    }

}
void av_clip_uint8_vectorize_256(int a[] , int vec4[] , int size)
{
    int i;
    for ( i = 0; i < size - 7; i += 8) {
    // Load 'a' into a SIMD register
    __m256i aVec = _mm256_loadu_si256((__m256i*)(a));  
    
    // Set a mask with all bits set to 1 for the lowest byte
    __m256i Val  = _mm256_set1_epi32(0xFF); 
    
    // Use SIMD bitwise AND to check if any bits outside the least significant 8 bits are set
    __m256i checkMask1 = _mm256_andnot_si256(Val, aVec); 

    //To check whether checkMask1 values are non zero or not
    __m256i checkMask2 = _mm256_cmpeq_epi32(checkMask1, _mm256_setzero_si256());

    // Use SIMD right shift operation to get the result based on the sign of 'a'
    __m256i resultVec =_mm256_srai_epi32(-aVec, 31); 
    
    // Use SIMD blend to select values based on the condition
    __m256i finalResult = _mm256_blendv_epi8(resultVec, aVec,  checkMask2); 
    
    // Extract the result from the SIMD register
    _mm256_storeu_si256((__m256i*)(vec4), finalResult);   
    }

    for (i = size - (size % 8); i < size; ++i) {
        vec4[i] = (a[i] & (~0xFF)) ? (-a[i]) >> 31 : a[i];
    }

}

int main()
{
    int size = 10;
    unsigned char char_result1 , char_result2[size] ,char_result3[size],c[size];
    int aVal[] = {3, 10, -4, 7, 500, -6, 260, -256, 50, 0} , aMin[] = {-2, -12, -3, -8, -550, -4, -200, -260, -20, -1} , aMax[] = {2, 12, 3, 8, 550, 4, 200, 260, 20, 1} ; 
    int result, vec1[size], vec2[size], vec3[size], vec4[size];

    /* Vectorize av_clip function */
    std::cout<<"\nCalling av_clip function";
    auto start = std::chrono::high_resolution_clock::now();
    result = av_clip(3,-2,2);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout<<"\nScalar Result: "<< result;

    // Execution Time for Scalar av_clip Funtion
    std::chrono::duration<double> scalar_time = end - start;
    std::cout<<"\nExecution Time for Scalar av_clip Funtion\t      : "<< std::setprecision(3) << scalar_time.count()<<" microseconds\n" <<std::endl;

    std::cout<<"Calling av_clip 128-bit Vectorized function\n\n";
    start = std::chrono::high_resolution_clock::now();
    av_clip_vectorize_128(aVal, aMin, aMax, vec1, size);
    end = std::chrono::high_resolution_clock::now();

    std::cout<<"Arrays with a value\t: ";    Print_Output_Arrays(aVal,size);
    std::cout<<"Arrays with aMin value\t: "; Print_Output_Arrays(aMin,size);
    std::cout<<"Arrays with aMax value\t: "; Print_Output_Arrays(aMax,size);
    std::cout<<"Vector result\t\t: ";        Print_Output_Arrays(vec1,size);

    // Execution Time for Vectorized 128-bit av_clip Function
    std::chrono::duration<double> vector_time1 = end - start;
    double performance1 = ((scalar_time.count()) / (vector_time1.count())) * 100 ;
    std::cout<<"Execution Time for Vectorized 128-bit av_clip Funtion : "<< std::setprecision(3) << vector_time1.count()<<" microseconds\n" <<std::endl;
    
    std::cout<<"Calling av_clip 256-bit Vectorized function\n\n";
    start = std::chrono::high_resolution_clock::now();
    av_clip_vectorize_256(aVal, aMin, aMax, vec2, size);
    end = std::chrono::high_resolution_clock::now();

    std::cout<<"Arrays with a value\t: ";    Print_Output_Arrays(aVal,size);
    std::cout<<"Arrays with aMin value\t: "; Print_Output_Arrays(aMin,size);
    std::cout<<"Arrays with aMax value\t: "; Print_Output_Arrays(aMax,size);
    std::cout<<"Vector result\t\t: ";        Print_Output_Arrays(vec2,size);

    // Execution Time for Vectorized 256-bit av_clip Function
    std::chrono::duration<double> vector_time2 = end - start;
    double performance2 = ((scalar_time.count()) / (vector_time2.count())) * 100 ;
    std::cout<<"Execution Time for Vectorized 256-bit av_clip Funtion : "<< std::setprecision(3) << vector_time2.count()<<" microseconds\n" <<std::endl;
    
    std::cout <<"Vector 128-bit av_Clip function is " << performance1 <<" \% faster than scalar\n";
    std::cout <<"Vector 256-bit av_Clip function is " << performance2 <<" \% faster than scalar\n";
    
    /* Vectorize av_clip_uint8 function */
    std::cout<<"\n------------------------------\n";
    std::cout<<"\nCalling av_clip_unit8 function\n";
    start = std::chrono::high_resolution_clock::now();
    char_result1 = av_clip_uint8(256);
    std::cout<<"Scalar Result: "<< (int)char_result1;
    end = std::chrono::high_resolution_clock::now();
    
    //Execution Time for Scalar av_clip_uint8 Funtion
    scalar_time = end - start;
    std::cout<<"\nExecution Time for Scalar av_clip_uint8 Funtion\t\t   : "<< std::setprecision(3) << scalar_time.count()<<" microseconds\n" <<std::endl;
    
    std::cout<<"Calling av_clip_uint8 128-bit Vectorized function\n\n";
    start = std::chrono::high_resolution_clock::now();
    av_clip_uint8_vectorize_128(aVal, vec3, size);
    end = std::chrono::high_resolution_clock::now();

    std::cout<<"Arrays with a value\t: ";    Print_Output_Arrays(aVal,size);
    std::cout<<"Vector result\t\t: ";        Print_Char_Output_Arrays(vec3, char_result2, size);//Print_Char_Arrays(c,size);

    //Execution Time for Vectorized 128-bit av_clip_uint8 Function
    vector_time1 = end - start;
    performance1 = ((scalar_time.count()) / (vector_time1.count())) * 100 ;
    std::cout<<"Execution Time for Vectorized 128-bit av_clip_uint8 Funtion : "<< std::setprecision(3) << vector_time1.count()<<" microseconds\n" <<std::endl;
    
    std::cout<<"Calling av_clip_uint8 256-bit Vectorized function\n\n";
    start = std::chrono::high_resolution_clock::now();
    av_clip_uint8_vectorize_128_opt(aVal, vec4, size);
    end = std::chrono::high_resolution_clock::now();

    std::cout<<"Arrays with a value\t: ";    Print_Output_Arrays(aVal,size);
    std::cout<<"Vector result\t\t: ";        Print_Char_Output_Arrays(vec4, char_result3, size);

    //Execution Time for Vectorized 256-bit av_clip_uint8 Function
    vector_time2 = end - start;
    performance2 = ((scalar_time.count()) / (vector_time2.count())) * 100 ;
    std::cout<<"Execution Time for Vectorized 256-bit av_clip_uint8 Funtion : "<< std::setprecision(3) << vector_time2.count()<<" microseconds\n" <<std::endl;
    
    std::cout <<"Vector 128-bit av_Clip_uint8 function is " << performance1 <<" \% faster than scalar\n";
    std::cout <<"Vector 256-bit av_Clip_uint8 function is " << performance2 <<" \% faster than scalar\n\n";
    
    return 0;
}
