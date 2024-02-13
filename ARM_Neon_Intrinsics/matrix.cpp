# include <iostream>
# include <arm_neon.h>

void print_results(float res[8][8]){

    int i, j;
    for(i = 0; i < 8; i++){
       for(j = 0; j < 8; j++){
            std::cout<<res[i][j]<<" ";
       }
       std::cout<<"\n";
    }

}

void Vector_matrix_multiply_32x4(float mat1[8][8] , float mat2[8][8] , float result1[8][8] , int size){

    int i , j , k;  
    float32x4_t a , b, sum;

    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            sum = vdupq_n_f32(0.0);
            for(k = 0; k < size; k++){
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

    Vector_matrix_multiply_32x4(mat1, mat2, result1, size);
    print_results(result1);

    return 0;
}


/*void print_output(float out[], int size){

    int i;
    for(i = 0; i < size; i++){
        std::cout<<out[i]<<" ";
    }
    std::cout<<"\n";
}*/