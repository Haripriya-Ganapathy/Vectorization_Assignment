# include <iostream>
# include <immintrin.h>
# include <chrono>
# include <iomanip>

void Print_Output_Arrays(int out[], int size)
{
    int i;
    for(i = 0; i < size; i++){
        std::cout<<out[i]<<"\t";
    }
    std::cout<<"\n";
}

static  int custom_abs(int a)
{
    if (a < 0) return -a;
    else    return a;
}

void custom_abs_vectorize_128(int a[], int res1[], int size)
{
    int i;
    for ( i = 0; i < size - 3; i += 4) {
        // Load the integers into separate  vector 
        __m128i var = _mm_loadu_si128((__m128i*)(a + i));
        
        // Compare if the element is less than 0 or not
        __m128i negativeMask = _mm_cmplt_epi32(var, _mm_setzero_si128());
        
        // Perform xor operation and subtract with negativeMask
        __m128i negated = _mm_sub_epi32(_mm_xor_si128(var, negativeMask), negativeMask);
        
        // Blend results based on sign
        __m128i result = _mm_blendv_epi8(var, negated, negativeMask);

        _mm_storeu_si128((__m128i*)(res1 + i), result); 
    }

    for (i = size - (size % 4); i < size; ++i) {
        res1[i] =  (a[i] < 0) ?  (-a[i]) : a[i];
    }

}

void custom_abs_vectorize_256(int a[], int res2[], int size)
{
    int i;
    for ( i = 0; i < size - 7; i += 8) {
        // Load the integers into separate  vector 
        __m256i var = _mm256_loadu_si256((__m256i*)(a + i));
        
        // Compare if the element is less than 0 or not
        __m256i negativeMask = _mm256_cmpgt_epi32(_mm256_setzero_si256(), var);
        
        // Perform xor operation and subtract with negativeMask
        __m256i negated = _mm256_sub_epi32(_mm256_xor_si256(var, negativeMask), negativeMask);
        
        // Blend results based on sign
        __m256i result = _mm256_blendv_epi8(var, negated, negativeMask);

        _mm256_storeu_si256((__m256i*)(res2 + i), result); 
    }

    for (i = size - (size % 8); i < size; ++i) {
        res2[i] =  (a[i] < 0) ?  (-a[i]) : a[i];
    }

}

int main()
{
    int result, size = 15;
    int input[] = {3, -4, 50, -17, 40, -2, 0, -10, -3, 100, 234, -200, -11, 39, 15} , res1[size], res2[size];

    /* Vectorize custom_abs function */
    std::cout<<"\nCalling custom_abs function";
    auto start = std::chrono::high_resolution_clock::now();
    result = custom_abs(-300);
    auto end = std::chrono::high_resolution_clock::now();

    // Print the results
    std::cout<<"\nScalar Result for -300 : "<< result;
   
    // Execution Time for Scalar custom_abs Funtion
    std::chrono::duration<double> scalar_time = end - start;
    std::cout<<"\nExecution Time for Scalar custom_abs Funtion\t\t : "<< std::setprecision(3) << scalar_time.count()<<" microseconds\n" <<std::endl;

    std::cout<<"Calling custom_abs 128-bit Vectorized function\n\n";
    start = std::chrono::high_resolution_clock::now();
    custom_abs_vectorize_128(input, res1, size);
    end = std::chrono::high_resolution_clock::now();

    // Print the results
    std::cout<<"Input values\t\t: ";    Print_Output_Arrays(input, size);
    std::cout<<"Vector result\t\t: ";   Print_Output_Arrays(res1, size);

    // Execution Time for Vectorized 128-bit custom_abs Function
    std::chrono::duration<double> vector_time1 = end - start;
    double performance1 = ((scalar_time.count()) / (vector_time1.count())) * 100 ;
    std::cout<<"Execution Time for Vectorized 128-bit custom_abs Funtion : "<< std::setprecision(3) << vector_time1.count()<<" microseconds\n" <<std::endl;
    
    std::cout<<"Calling custom_abs 256-bit Vectorized function\n\n";
    start = std::chrono::high_resolution_clock::now();
    custom_abs_vectorize_256(input, res2, size);
    end = std::chrono::high_resolution_clock::now();
    
    // Print the results
    std::cout<<"Input values\t\t: ";    Print_Output_Arrays(input, size);
    std::cout<<"Vector result\t\t: ";   Print_Output_Arrays(res2, size);
    
    // Execution Time for Vectorized 256-bit custom_abs Function
    std::chrono::duration<double> vector_time2 = end - start;
    double performance2 = ((scalar_time.count()) / (vector_time2.count())) * 100 ;
    std::cout<<"Execution Time for Vectorized 256-bit custom_abs Funtion : "<< std::setprecision(3) << vector_time2.count()<<" microseconds\n" <<std::endl;

    std::cout <<"Vector 128-bit custom_abs function is " << performance1 <<" \% faster than scalar\n";
    std::cout <<"Vector 256-bit custom_abs function is " << performance2 <<" \% faster than scalar\n";
    
    return 0;
}
