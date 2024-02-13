# include <iostream>
# include <arm_neon.h>

void print_output(int out[], int size){

    int i;
    for(i = 0; i < size; i++){
        std::cout<<out[i]<<" ";
    }
    std::cout<<"\n";
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
    print_output(result1, size);

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
    print_output(result2, size);

}

int main(){

    int size = 17;
    int arr1[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17} , arr2[] = {18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34} ;
    int result1[size] , result2[size] , result3[size];
    
    Vector_Addition_32x4(arr1, arr2, result1, size);
    Vector_Multiplication_32x4(arr1, arr2, result2, size);


}