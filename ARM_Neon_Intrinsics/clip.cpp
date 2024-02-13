# include <iostream>
# include <arm_neon.h>

void Print_Output_Arrays(int out[], int size)
{
    int i;
    for(i = 0; i < size; i++){
        std::cout << out[i] << "\t";
    }
    std::cout << "\n";
}

void Print_Char_Output_Arrays(int vec[], unsigned char out[], int size)
{
    int i;
    for(i = 0; i < size; i++){
        out[i] = vec[i];
        std::cout << (int) (out[i]) << "\t";
    }
    std::cout << "\n";
}

void av_clip_vectorize_32x4(int a[], int amin[], int amax[] , int vec1[], int size){

    int i;
    for ( i = 0; i < size - 3; i += 4) {
        // Load the integers into separate  vector 
        int32x4_t var = vld1q_s32(a + i);
        int32x4_t min = vld1q_s32(amin + i);
        int32x4_t max = vld1q_s32(amax + i);
    
        // Perform the operation
        int32x4_t res = vmaxq_s32(vminq_s32(var, max), min);

        // Store the result into integer array
        vst1q_s32(vec1 + i, res);
    }

    for (i = size - (size % 4); i < size; ++i) {
        vec1[i] = std:: max(std:: min(a[i], amax[i]), amin[i]);
    }

}

void av_clip_uint8_vectorize_32x4(int a[], int vec2[], int size){
    
    int i;
    //Performing ~0xFF operation
    int32x4_t Val = vmvnq_s32(vdupq_n_s32(0xFF)); 
    
    for ( i = 0; i < size - 3; i += 4) {
       
       // Load 'a' into a 32x4 register
       int32x4_t aVec = vld1q_s32(a + i);  

       //Perform Logical AND Operation(a & ~0xFF)
       int32x4_t checkMask1 = vandq_s32(aVec, Val);

       //To check whether checkMask1 values are zero or not i.e whether it comes under 8 bit representation or not using compare function
       uint32x4_t checkMask2 = vceqzq_s32(checkMask1);

       // Use  right shift operation to get the result
       int32x4_t resultVec = vshrq_n_s32(-aVec, 31); 

       //Bitwise Select operation to choose elements based on the values of checkMask2
       int32x4_t finalResult = vbslq_s32(checkMask2, aVec, resultVec); 
    
       // Store the result into integer array
       vst1q_s32(vec2 + i , finalResult);
    }

    for (i = size - (size % 4); i < size; ++i) {
        vec2[i] = (a[i] & (~0xFF)) ? (-a[i]) >> 31 : a[i];
    }
   
}

int main(){

   int size = 8;
   int aVal[] = {3, 10, -4, 7, 500, -6, 260, -256} , aMin[] = {-2, -12, -3, -8, -550, -4, -200, -260} , aMax[] = {2, 12, 3, 8, 550, 4, 200, 260} ; 
   int vec1[size], vec2[size];
   unsigned char char_result[size];
   
   std::cout<<"Arrays with a value\t: ";    Print_Output_Arrays(aVal,size);
   std::cout<<"Arrays with aMin value\t: "; Print_Output_Arrays(aMin,size);
   std::cout<<"Arrays with aMax value\t: "; Print_Output_Arrays(aMax,size);
   av_clip_vectorize_32x4(aVal, aMin, aMax, vec1, size);
   std::cout<<"av_clip function result : ";  Print_Output_Arrays(vec1, size);

   av_clip_uint8_vectorize_32x4(aVal, vec2, size);
   std::cout<<"av_clip_uint8 result\t: ";Print_Char_Output_Arrays(vec2, char_result, size);

   return 0;
}
