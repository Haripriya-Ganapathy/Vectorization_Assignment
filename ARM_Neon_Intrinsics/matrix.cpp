# include <iostream>
# include <arm_neon.h>
# include <chrono>

void print_results(float res[8][8]){

    int i, j;
    for(i = 0; i < 8; i++){
       for(j = 0; j < 8; j++){
            std::cout<<res[i][j]<<" ";
       }
       std::cout<<"\n";
    }

}

void Scalar_matrix_multiply(float mat1[8][8] , float mat2[8][8] , float result1[8][8]){

    int i , j , k;

    // Initializing elements of matrix result1 to 0.
    for(i = 0; i < 8; ++i){
        for(j = 0; j < 8; ++j){
            result1[i][j]=0;
        }
    }

    for(i = 0; i < 8; ++i)
        for(j = 0; j < 8; ++j)   
            for(k = 0; k < 8; ++k)
            {
                // Multiplying  row from mat1 and first colum from mat2 and summing up 
                result1[i][j] += mat1[i][k] * mat2[k][j];
            }

}



void Vector_matrix_multiply_32x4(float mat1[8][8] , float mat2[8][8] , float result1[8][8] ){

    int i , j , k;  
    float32x4_t a , b, sum;

    for(i = 0; i < 8; i++){
        for(j = 0; j < 8; j++){
            sum = vdupq_n_f32(0.0);
            for(k = 0; k < 8; k++){
                a = vdupq_n_f32(mat1[i][k]);
                b = vld1q_f32(&mat2[k][j]);
                sum =  vaddq_f32(sum, vmulq_f32(a, b));
            }
            vst1q_f32(&result1[i][j], sum);
        }
    }

}

int main(){
   
    int size = 8;
    // Input arrays
    float mat1[8][8] = {
        {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0},
        {2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0},
        {3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0},
        {4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0},
        {5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0},
        {6.0, 6.0, 6.0, 6.0, 6.0, 6.0, 6.0, 6.0},
        {7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0},
        {8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0}
    };
    float mat2[8][8] = {
        {8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0, 8.0},
        {7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0, 7.0},
        {6.0, 6.0, 6.0, 6.0, 6.0, 6.0, 6.0, 6.0},
        {5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0},
        {4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0, 4.0},
        {3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0},
        {2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0},
        {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}
    };

    float result1[8][8] , result2[8][8]  ;

/* Vectorize custom_abs function */
    std::cout<<"\nCalling custom_abs function";
    auto start = std::chrono::high_resolution_clock::now();
    Scalar_matrix_multiply(mat1 , mat2 , result1);
    print_results(result1);
    auto end = std::chrono::high_resolution_clock::now();


    // Execution Time for Scalar custom_abs Funtion
    std::chrono::duration<double> scalar_time = end - start;
    std::cout<<"\nExecution Time for Scalar custom_abs Funtion\t\t : "<<  scalar_time.count()<<" microseconds\n" <<std::endl;

    start = std::chrono::high_resolution_clock::now();
    Vector_matrix_multiply_32x4(mat1, mat2, result2);
    end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> vector_time = end - start;
    print_results(result2);
    // double v_time = std::chrono::duration_cast<std::chrono::microseconds>(vector_time).count();
    std::cout<<"\nExecution Time for Scalar custom_abs Funtion\t\t : "<<  vector_time.count()<<" microseconds\n" <<std::endl;

    
    double performance1 = ((scalar_time.count() - vector_time.count()) / scalar_time.count()) * 100 ;
    std::cout <<"\nVector 256-bit Integer Addition is " << performance1 <<" \% faster than scalar\n";

    return 0;
}


/*void print_output(float out[], int size){

    int i;
    for(i = 0; i < size; i++){
        std::cout<<out[i]<<" ";
    }
    std::cout<<"\n";
}*/