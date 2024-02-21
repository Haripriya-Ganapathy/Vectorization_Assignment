# include <iostream>
# include <arm_neon.h>
# include <chrono>

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

static unsigned char av_clip_uint8(int a)
{
    if (a & (~0xFF))    return (-a) >> 31;
    else                return a;
}

static  int av_clip(int a, int amin, int amax)
{
    if (a < amin)      return amin;
    else if (a > amax) return amax;
    else               return a;
}

int main(){

   int size = 8;
   int aVal[] = {3, 10, -4, 7, 500, -6, 260, -256} , aMin[] = {-2, -12, -3, -8, -550, -4, -200, -260} , aMax[] = {2, 12, 3, 8, 550, 4, 200, 260} ; 
   int vec1[size], vec2[size];
   unsigned char char_result[size];
   
   std::cout<<"Arrays with a value\t: ";    Print_Output_Arrays(aVal,size);
   std::cout<<"Arrays with aMin value\t: "; Print_Output_Arrays(aMin,size);
   std::cout<<"Arrays with aMax value\t: "; Print_Output_Arrays(aMax,size);

   std::cout<<"\nCalling av_clip function";
    auto start = std::chrono::high_resolution_clock::now();
    int result = av_clip(3,-2,2);
     result = av_clip(3,-2,2);
     result = av_clip(3,-2,2);
     result = av_clip(3,-2,2);
     result = av_clip(3,-2,2);
     result = av_clip(3,-2,2);
     result = av_clip(3,-2,2);
     result = av_clip(3,-2,2);
     result = av_clip(3,-2,2);
    //  std::cout<<"\n";
    //  std::cout<<"\nScalar Result: "<< result;
    auto end = std::chrono::high_resolution_clock::now();
    
    
   

    // Execution Time for Scalar av_clip Funtion
    std::chrono::duration<double> scalar_time1 = end - start;
    // double s_time = std::chrono::duration_cast<std::chrono::microseconds>(scalar_time).count();
    std::cout<<"\nExecution Time for Scalar av_clip Funtion\t      : "<<  scalar_time1.count()<<" microseconds\n" <<std::endl;

    start = std::chrono::high_resolution_clock::now();
   av_clip_vectorize_32x4(aVal, aMin, aMax, vec1, size);
    end = std::chrono::high_resolution_clock::now();
   std::cout<<"av_clip function result : ";  Print_Output_Arrays(vec1, size);
   std::chrono::duration<double> vector_time1 = end - start;

   std::cout<<"\nExecution Time for Scalar custom_abs Funtion\t\t : "<<  vector_time1.count()<<" microseconds\n" <<std::endl;

start = std::chrono::high_resolution_clock::now();
unsigned char char_result1;
    char_result1 = av_clip_uint8(256);
    char_result1 = av_clip_uint8(256);
    char_result1 = av_clip_uint8(256);
    char_result1 = av_clip_uint8(256);
    char_result1 = av_clip_uint8(256);
    char_result1 = av_clip_uint8(256);
     char_result1 = av_clip_uint8(256);
    char_result1 = av_clip_uint8(256);

    // std::cout<<"Scalar Result: "<< (int)char_result1;
    end = std::chrono::high_resolution_clock::now();
    
    //Execution Time for Scalar av_clip_uint8 Funtion
     std::chrono::duration<double>  scalar_time2 = end - start;
    // s_time = std::chrono::duration_cast<std::chrono::microseconds>(scalar_time).count();
    std::cout<<"\nExecution Time for Scalar av_clip_uint8 Funtion\t\t   : "<< scalar_time2.count()<<" microseconds\n" <<std::endl;
    
    start = std::chrono::high_resolution_clock::now();
   av_clip_uint8_vectorize_32x4(aVal, vec2, size);
    end = std::chrono::high_resolution_clock::now();

   std::cout<<"av_clip_uint8 result\t: ";Print_Char_Output_Arrays(vec2, char_result, size);
     std::chrono::duration<double>  vector_time2 = end - start;
    std::cout<<"\nExecution Time for Scalar custom_abs Funtion\t\t : "<<  vector_time2.count()<<" microseconds\n" <<std::endl;

   double performance1 = ((scalar_time1.count() - vector_time1.count()) / scalar_time1.count()) * 100 ;
   double performance2 = ((scalar_time2.count() - vector_time2.count()) / scalar_time2.count()) * 100 ;

    std::cout <<"\nVector 256-bit Integer Addition is " << performance1 <<" \% faster than scalar\n";
     std::cout <<"\nVector 256-bit Integer Addition is " << performance2 <<" \% faster than scalar\n";

   return 0;
}
