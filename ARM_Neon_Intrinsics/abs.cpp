# include <iostream>
# include <arm_neon.h>
# include <iomanip>
# include <chrono>

void Print_Output_Arrays(int out[], int size)
{
    int i;
    for(i = 0; i < size; i++){
        std::cout << out[i] << "\t";
    }
    std::cout << "\n";
}

static  int custom_abs(int a)
{
    if (a < 0) return -a;
    else    return a;
}

void custom_abs_vectorize_32x4(int a[], int result[], int size)
{
    int i;
    unsigned int v[size];
    for ( i = 0; i < size - 3; i += 4) {
        // Load the integers into separate  vector 
        int32x4_t var = vld1q_s32(a + i);
        
        // Compare if the element is less than 0 or not 
        uint32x4_t negativeMask = vcleq_s32(var, vdupq_n_s32(0));
        
        //Type casted unsigned int to int type
        int32x4_t negativeMaskCasted = vreinterpretq_s32_u32(negativeMask);
        
        // Perform xor operation and subtract with negativeMaskCasted
        int32x4_t negated = vsubq_s32(veorq_s32(var, negativeMaskCasted), negativeMaskCasted);

        // Store the result into integer array
        vst1q_s32(result + i, negated); 
    }

    for (i = size - (size % 4); i < size; ++i) {
        result[i] =  (a[i] < 0) ?  (-a[i]) : a[i];
    }

}

int main(){

    int size = 10;
    int input[] = {3, -4, 50, -17, 40, -2, 0, -10, -99, 2} , result[size];

    /* Vectorize custom_abs function */
    std::cout<<"\nCalling custom_abs function";
    auto start = std::chrono::high_resolution_clock::now();
    int res = custom_abs(-300);
    res = custom_abs(-300);
    res = custom_abs(-300);
    res = custom_abs(-300);
    res = custom_abs(-300);
    res = custom_abs(-300);
    res = custom_abs(-300);
    res = custom_abs(-300);
    res = custom_abs(-300);
    res = custom_abs(-300);
    // res = custom_abs(-300);
    
    // std::cout<<"\nScalar Result for -300 : "<< res;
    auto end = std::chrono::high_resolution_clock::now();

    // Execution Time for Scalar custom_abs Funtion
    std::chrono::duration<double> scalar_time = end - start;
    std::cout<<"\nExecution Time for Scalar custom_abs Funtion\t\t : "<< std::setprecision(3) << scalar_time.count()<<" microseconds\n" <<std::endl;

    start = std::chrono::high_resolution_clock::now();
    custom_abs_vectorize_32x4(input, result, size);
    end = std::chrono::high_resolution_clock::now();

    // Execution Time for Scalar custom_abs Funtion
    std::chrono::duration<double> vector_time = end - start;
    // double v_time = std::chrono::duration_cast<std::chrono::microseconds>(vector_time).count();
    std::cout<<"\nExecution Time for Scalar custom_abs Funtion\t\t : "<< std::setprecision(3) << vector_time.count()<<" microseconds\n" <<std::endl;

    double performance1 = ((scalar_time.count() - vector_time.count()) / scalar_time.count()) * 100 ;
    std::cout <<"\nVector 256-bit Integer Addition is " << performance1 <<" \% faster than scalar\n";

    Print_Output_Arrays(result, size);

    return 0;
}