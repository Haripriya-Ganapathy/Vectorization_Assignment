# include <iostream>
# include <arm_neon.h>
# include <chrono>
# include <iomanip>

void print_output(int out[], int size){

    int i;
    for(i = 0; i < size; i++){
        std::cout<<out[i]<<" ";
    }
    std::cout<<"\n";
}

void Scalar_Addition (int arr1[] , int arr2[] , int result1[] , int size ) {

   for( int i = 0; i < size ; i++ ){
       // Add 2 arrays element wise
       result1 [i] = arr1 [i] + arr2 [i] ;
   }

}

void Vector_Addition_32x4 (int arr1[] , int arr2[] , int result1[] , int size ){

    int i;
    for ( i = 0; i < size - 3; i += 4) {

        int32x4_t vector1 = vld1q_s32(arr1 + i);
        int32x4_t vector2 = vld1q_s32(arr2 + i);

        int32x4_t add = vaddq_s32(vector1, vector2);

        vst1q_s32(result1 + i, add);
    }
    // Handle the remaining elements using scalar operations
    for (i = size - (size % 4); i < size; ++i) {
        result1[i] = arr1[i] + arr2[i];
    }
    //print_output(result1, size);

}

void Vector_Multiplication_32x4 (int arr1[] , int arr2[] , int result2[] , int size ){

    int i;
    for ( i = 0; i < size - 3; i += 4) {

        int32x4_t vector1 = vld1q_s32(arr1 + i);
        int32x4_t vector2 = vld1q_s32(arr2 + i);

        int32x4_t multiply = vmulq_s32(vector1, vector2);

        vst1q_s32(result2 + i, multiply);
    }
    // Handle the remaining elements using scalar operations
    for (i = size - (size % 4); i < size; ++i) {
        result2[i] = arr1[i] * arr2[i];
    }
    //print_output(result2, size);

}

int main(){

    int size = 170;
    int arr1[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17} , arr2[] = {18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34} ;
    int result1[size] , result2[size] , result3[size];
    
// Scalar Addition
    auto start = std::chrono::high_resolution_clock ::now(); 
    Scalar_Addition(arr1 , arr2 , result1 , size);
    auto end = std::chrono::high_resolution_clock ::now();
    
    
    // Execution Time for Scalar Addition
    std::chrono::duration<double> scalar_time = end - start;
    // double s_time = std::chrono::duration_cast<std::chrono::microseconds>(scalar_time).count();
    std::cout << "\nExecution Time for Scalar Integer Addition\t   : "  << std::setprecision(4) <<scalar_time.count() << " microseconds\n" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    Vector_Addition_32x4(arr1, arr2, result1, size);
    end = std::chrono::high_resolution_clock::now();
    
    // Execution Time for Scalar custom_abs Funtion
    std::chrono::duration<double> vector_time = end - start;
    // double v_time = std::chrono::duration_cast<std::chrono::microseconds>(vector_time).count();
    std::cout<<"\nExecution Time for Scalar custom_abs Funtion\t\t : "<<  std::setprecision(4) <<vector_time.count()<<" microseconds\n" <<std::endl;

    //Vector_Multiplication_32x4(arr1, arr2, result2, size);

   double performance1 = ((scalar_time.count() - vector_time.count()) / scalar_time.count()) * 100 ;
    std::cout <<"\nVector 256-bit Integer Addition is " << performance1 <<" \% faster than scalar\n";

}