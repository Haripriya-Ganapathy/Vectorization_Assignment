# include <iostream>
# include <arm_neon.h>

void print_output(float out[], int size){

    int i;
    for(i = 0; i < size; i++){
        std::cout<<out[i]<<" ";
    }
    std::cout<<"\n";
}

void Vector_Complex_Multiply_32x4(float v1[] ,float v2[] ,float final_result[]){

    float32x4_t vector1 = vld1q_f32(v1);
    float32x4_t vector2 = vld1q_f32(v2);

    float32x4_t real1 = vcopyq_laneq_f32(vector1, 1, vector1, 2);
    float32x4_t imag1 = vcopyq_laneq_f32(vcopyq_laneq_f32(vector1, 0, vector1, 1), 1, vector1, 3);
    float32x4_t real2 = vcopyq_laneq_f32(vector2, 1, vector2, 2);
    float32x4_t imag2 = vcopyq_laneq_f32(vcopyq_laneq_f32(vector2, 0, vector2, 1), 1, vector2, 3);

    float32x4_t result_real = vsubq_f32(vmulq_f32(real1, real2), vmulq_f32(imag1, imag2));
    float32x4_t result_imag = vaddq_f32(vmulq_f32(real1, imag2), vmulq_f32(imag1, real2));

    float32x4_t result_comb1 = vcopyq_laneq_f32(result_real, 1, result_imag, 0);
    float32x4_t result_comb2 = vcopyq_laneq_f32(vcopyq_laneq_f32(result_real, 2, result_real, 1), 3, result_imag, 1);
 
    float32x2_t result_high = vget_low_f32(result_comb1);
    float32x2_t result_low = vget_high_f32(result_comb2);
    
    float32x4_t result = vcombine_f32(result_high, result_low);

    vst1q_f32(final_result, result);
    
}

int main(){

    float  a[] = {2.0,1.0,4.0,3.0} , b[] = {4.0,3.0,2.0,1.0} , c[] = {1.0,2.0,3.0,4.0} , d[] = {3.0,4.0,1.0,2.0};
    float  e[] = {4.0,3.0,2.0,1.0} , f[] = {2.0,1.0,4.0,3.0} , g[] = {3.0,4.0,1.0,2.0} , h[] = {1.0,2.0,3.0,4.0};

    float out1[4] , out2[4] , out3[4] , out4[4] ;
    float vec1[4] , vec2[4] , vec3[4] , vec4[4] ;

    Vector_Complex_Multiply_32x4(a, b, vec1);
    print_output(vec1, 4);


    return 0;
}