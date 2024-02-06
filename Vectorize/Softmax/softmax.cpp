# include <iostream>
# include <immintrin.h>
# include <chrono>
# include <iomanip>
# include <cmath>

void Print_Output_Arrays(float** out, int row, int col)
{
    int i, j;
    for(i = 0; i < row; i++){
        for(j = 0; j < col; j++){
            std::cout<<out[i][j]<<"\t";
        }
        std::cout<<"\n";
    }
}

void softmax(float** input, float **output , int row, int col) {
    
    int i, j;
    // Calculate the maximum value along each column
    float* max_vals = new float[col];
    for (j = 0; j < col; ++j) {
        float max_val = input[0][j];
        for (i = 1; i < row; ++i) {
            max_val = std::max(max_val, input[i][j]);
        }
        max_vals[j] = max_val;
    }
    
    // Subtract the maximum value for numerical stability and compute exponentials
    for (i = 0; i < row; ++i) {
        for (j = 0; j < col; ++j) {
            output[i][j] = exp(input[i][j] - max_vals[j]);
        }
    }
    
    // Compute the sum of exponentials along each column
    float* exp_sums = new float[col];
    for (j = 0; j < col; ++j) {
        exp_sums[j] = 0.0;
        for (i = 0; i < row; ++i) {
            exp_sums[j] += output[i][j];
        }
    }
    
    // Divide each element by the sum of exponentials
    for (i = 0; i < row; ++i) {
        for (j = 0; j < col; ++j) {
           output[i][j] /= exp_sums[j];
        }
    }

}

__m128 exp_approximation_128(__m128 x) {
    
    int i;
    __m128 result = _mm_set1_ps(1.0); // Initialize result to 1
    __m128 term = _mm_set1_ps(1.0);   // Initialize term to 1
    
    for (i = 1; i <= 15; ++i) { // Taylor series approximation up to 15 terms
        
        term = _mm_mul_ps(term, _mm_div_ps(x, _mm_set1_ps(i))); // Compute (term * x / i)
        result = _mm_add_ps(result, term); // Add the term to the result
    }
    return result;

}

__m256 exp_approximation_256(__m256 x) {
    
    int i;
    __m256 result = _mm256_set1_ps(1.0); // Initialize result to 1
    __m256 term = _mm256_set1_ps(1.0);   // Initialize term to 1
    
    for (i = 1; i <= 15; ++i) { // Taylor series approximation up to 15 terms
        
        term = _mm256_mul_ps(term, _mm256_div_ps(x, _mm256_set1_ps(i))); // Compute (term * x / i)
        result = _mm256_add_ps(result, term); // Add the term to the result
    }
    return result;

}

void softmax_vectorize_128(float** input, float** result1, int row, int col){

    int i, j ;
    float** res = new float*[col];
    //Initialize vectors with zero
    __m128 max =_mm_setzero_ps();
    __m128 sum =_mm_setzero_ps();

    for(i = 0; i < row; i++){
       // Load 2d array row wise into the vector
       __m128 a = _mm_loadu_ps(input[i]);

       //Compute the maximum element in each column
       max = _mm_max_ps(max, a);
       
       //Initialize the 2d res array  
       res[i] = new float[col];
        
    }
    for(i = 0; i < row; i++){
        //Load 2d array row wise into the vector
        __m128 b = _mm_loadu_ps(input[i]);

        //Subtract the maximum value for numerical stability 
        __m128 sub = _mm_sub_ps(b, max);

        //Compute exponentials
        __m128 exp_val = exp_approximation_128(sub);  
        
        //Store the exponentials into a 2d array
        _mm_storeu_ps(res[i], exp_val);

        //Compute the sum of exponentials among the column
        sum = _mm_add_ps(sum, exp_val);  
    
    }
   
   for(i = 0; i < row; i++){
    //Perform the division between individual exponential values and sum of exponential
    __m128 a = _mm_loadu_ps(res[i]);
    __m128 div = _mm_div_ps(a, sum);

    //Store the results 
    _mm_storeu_ps(result1[i], div);

   }
}

void softmax_vectorize_256(float** input, float** result2, int row, int col){

    int i, j ;
    float** res = new float*[col];
    
    //Initialize vectors with zero
    __m256 max =_mm256_setzero_ps();
    __m256 sum =_mm256_setzero_ps();

    for(i = 0; i < row; i++){
       // Load 2d array row wise into the vector
       __m256 a = _mm256_loadu_ps(input[i]);

       //Compute the maximum element in each column
       max = _mm256_max_ps(max, a);
       
       //Initialize the 2d res array  
       res[i] = new float[col];
        
    }
    
    for(i = 0; i < row; i++){
        //Load 2d array row wise into the vector
        __m256 b = _mm256_loadu_ps(input[i]);

        //Subtract the maximum value for numerical stability 
        __m256 sub = _mm256_sub_ps(b, max);

        //Compute exponentials
        __m256 exp_val = exp_approximation_256(sub);  
        
        //Store the exponentials into a 2d array
       _mm256_storeu_ps(res[i], exp_val);

        //Compute the sum of exponentials among the column
        sum = _mm256_add_ps(sum, exp_val);  
    
   }
   
   for(i = 0; i < row; i++){
    //Perform the division between individual exponential values and sum of exponential
    __m256 a = _mm256_loadu_ps(res[i]);
    __m256 div = _mm256_div_ps(a, sum);

    //Store the results 
    _mm256_storeu_ps(result2[i], div);

   }
   
}

int main(){

    // Initialize the Input Array
    int row = 2, col = 4 , i, j;
    float **input = new float*[row] , **output = new float*[row] , **result1 = new float*[row], **result2 = new float*[row];

    for ( i = 0; i < row; ++i) {
        input[i] = new float[col];
        output[i] = new float[col];
        result1[i] = new float[col];
        result2[i] = new float[col];
        for ( j = 0; j < col; ++j) {
            // Initialize with consecutive numbers
            input[i][j] = i * col + j;  
        }
    }
    std::cout<<"Input array: \n";
    Print_Output_Arrays(input, row, col);
    
    /* Vectorize Softmax function */
    auto start = std::chrono::high_resolution_clock::now();
    softmax(input, output , row, col);
    auto end = std::chrono::high_resolution_clock::now();
    
    //Print the Output Array
    std::cout<<"\nScalar Result:\n";
    Print_Output_Arrays(output, row, col);

    // Execution Time for Scalar Softmax Funtion
    std::chrono::duration<double> scalar_time = end - start;
    std::cout<<"\nExecution Time for Scalar Softmax Funtion\t      : "<< std::setprecision(3) << scalar_time.count()<<" microseconds\n" <<std::endl;

    std::cout<<"\nCalling Vectorized 128-bit Softmax function\n";
    start = std::chrono::high_resolution_clock::now();
    softmax_vectorize_128(input, result1, row, col);
    end = std::chrono::high_resolution_clock::now();
    
    //Print the Output Array
    std::cout<<"\nVector Result:\n";
    Print_Output_Arrays(result1, row, col);
    
    // Execution Time for Vectorized 128-bit Softmax Function
    std::chrono::duration<double> vector_time1 = end - start;
    double performance1 = ((scalar_time.count()) / (vector_time1.count())) * 100  ;
    std::cout<<"\nExecution Time for Vectorized 128-bit Softmax Funtion : "<< std::setprecision(3) << vector_time1.count()<<" microseconds\n" <<std::endl;
    
    std::cout<<"\nCalling Vectorized 256-bit Softmax function\n";
    start = std::chrono::high_resolution_clock::now();
    softmax_vectorize_256(input, result2, row, col);
    end = std::chrono::high_resolution_clock::now();
    
    //Print the Output Array
    std::cout<<"\nVector Result:\n";
    Print_Output_Arrays(result2, row, col);
    
    // Execution Time for Vectorized 256-bit Softmax Function
    std::chrono::duration<double> vector_time2 = end - start;
    double performance2 = ((scalar_time.count()) / (vector_time2.count())) * 100  ;
    std::cout<<"\nExecution Time for Vectorized 256-bit Softmax Funtion : "<< std::setprecision(3) << vector_time2.count()<<" microseconds\n" <<std::endl;
    
    std::cout <<"Vector 128-bit Softmax function is " << performance1 <<" \% faster than scalar\n";
    std::cout <<"Vector 256-bit Softmax function is " << performance2 <<" \% faster than scalar\n";

    return 0;
}