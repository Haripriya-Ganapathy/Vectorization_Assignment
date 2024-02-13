# include <iostream>
# include <arm_neon.h>

void Print_Output_Arrays(float** out, int row, int col)
{
    int i, j;
    for(i = 0; i < row; i++){
        for(j = 0; j < col; j++){
            std::cout << out[i][j] << "\t";
        }
        std::cout << "\n";
    }
}

float32x4_t exp_approximation_32x4(float32x4_t x) {
    
    //Find elements which are negative 
    uint32x4_t neg = vcgtzq_f32(x);

    //Perform the abs function for the input x
    x = vabsq_f32(x);
    
    //Use Taylor series to calculate e^x
    float32x4_t result = vdupq_n_f32(1.0); // Initialize result to 1
    float32x4_t term = vdupq_n_f32(1.0);   // Initialize term to 1
    
    // Taylor series approximation up to 20 terms
    for (int i = 1; i <= 20; ++i) { 
        
        term = vmulq_f32(term, vdivq_f32(x, vdupq_n_f32(i))); // Compute (term * x / i)
        result = vaddq_f32(result, term); // Add the term to the result
    }

    //Find e^-x using reciprocal function
    float32x4_t neg_result = vrecpeq_f32(result);
    
    //Blend the results based on condition for negative exponent
    float32x4_t final_result = vbslq_f32(neg, result, neg_result);
    
    return final_result;

}

void softmax_vectorize_32x4(float** input, float** result, int row, int col){

    int i, j ;
    float** res = new float*[col];
    
    //Initialize vectors with zero
    float32x4_t max = vdupq_n_f32(0.0);
    float32x4_t sum = vdupq_n_f32(0.0);

    for(i = 0; i < row; i++){
       // Load 2d array row wise into the vector
       float32x4_t a = vld1q_f32(input[i]);

       //Compute the maximum element in each column
       max = vmaxq_f32(max, a);
       
       //Initialize the 2d res array  
       res[i] = new float[col];
        
    }
    
    for(i = 0; i < row; i++){
        //Load 2d array row wise into the vector
        float32x4_t b = vld1q_f32(input[i]);

        //Subtract the maximum value for numerical stability 
        float32x4_t sub = vsubq_f32(b, max);
        
        //Compute exponentials
        float32x4_t exp_val = exp_approximation_32x4(sub);  
        
        //Store the exponentials into a 2d array
        vst1q_f32(res[i], exp_val);

        //Compute the sum of exponentials among the column
        sum = vaddq_f32(sum, exp_val);  
    
    }
    
    for(i = 0; i < row; i++){
        
        //Perform the division between individual exponential values and sum of exponential
        float32x4_t a = vld1q_f32(res[i]);
        float32x4_t div = vdivq_f32(a, sum);
        
        //Store the results 
        vst1q_f32(result[i], div);
    }
}

int main(){

    // Initialize the Input Array
    int row = 2, col = 4 , i, j;
    float **input = new float*[row] , **output = new float*[row] , **result = new float*[row];
    
    for ( i = 0; i < row; ++i) {
        input[i] = new float[col];
        result[i] = new float[col];
        for ( j = 0; j < col; ++j) {
            // Initialize with consecutive numbers
            input[i][j] = i * col + j;  
        }
    }
    
    //Print the Input array
    std::cout<<"Input array: \n";
    Print_Output_Arrays(input, row, col);
    
    //Call the Softmax Function
    softmax_vectorize_32x4(input, result, row, col);

    //Print the Result Array
    std::cout<<"\nResult:\n";
    Print_Output_Arrays(result, row, col);

    return 0;
}