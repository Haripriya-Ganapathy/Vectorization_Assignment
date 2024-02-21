# include <iostream>
# include <arm_neon.h>
# include <iomanip>
# include <chrono>


void Print_Output_Arrays(float out[]){

    int i;
    for(i = 0 ; i < 4 ; i++){
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


    /* Scalar Complex Multiplication */ 
    std::cout<<"Scalar Results:\n-------------------\n";
    
    // Complex Multiply a and b 
    auto start = std::chrono::high_resolution_clock::now();
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
    auto end = std::chrono::high_resolution_clock::now();
    // auto scalar_time = end - start;

std::chrono::duration<double> scalar_time = end - start;
    std::cout<<"\nExecution Time for Scalar custom_abs Funtion\t\t : "<< std::setprecision(3) << scalar_time.count()<<" microseconds\n" <<std::endl;
   
    start = std::chrono::high_resolution_clock::now();
    Vector_Complex_Multiply_32x4(a, b, vec1);
    Print_Output_Arrays(vec1);

    Vector_Complex_Multiply_32x4(c, d, vec2);
    Print_Output_Arrays(vec2);

    Vector_Complex_Multiply_32x4(e, f, vec3);
    Print_Output_Arrays(vec3);

    Vector_Complex_Multiply_32x4(g, h, vec4);
    Print_Output_Arrays(vec4);

     end = std::chrono::high_resolution_clock::now();
    
std::chrono::duration<double> vector_time = end - start;
    // double v_time = std::chrono::duration_cast<std::chrono::microseconds>(vector_time).count();
    std::cout<<"\nExecution Time for Scalar custom_abs Funtion\t\t : "<< std::setprecision(3) << vector_time.count()<<" microseconds\n" <<std::endl;


    double performance1 = ((scalar_time.count() - vector_time.count()) / scalar_time.count()) * 100 ;
std::cout <<"Vector 128-bit Integer Addition is " << performance1 <<" \% faster than scalar\n";

    return 0;
}