#include<iostream>
#include<arm_neon.h>
#include <chrono>
#define ROW1 5
#define COL1 8 //ROW2=COL1
#define COL2 3

// void check(float32x4_t out){

//   for(int i=0;i<4;i++){
//     std::cout<<out[i]<<" ";
//   }
//   std::cout<<std::endl;
// }

void print(float *C,int m, int n){
    for(int i=0;i<m;i++){
    for(int j=0;j<n;j++){
        std::cout<<"\t"<<C[i * n + j];
    }
    std::cout<<std::endl;
  }
}


void initialize(float *arr, int m, int n) {
  int i, j;
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      *((arr + i * n) + j) = i * 1.0f;
    }
  }
}

void initialize_Zero(float *arr, int m, int n) {
  int i, j;
  for (i = 0; i < m; i++) {
    for (j = 0; j < n; j++) {
      *((arr + i * n) + j) = 0.0f;
    }
  }
}

void matmul_scalar(float *A, float *B, float *result) {
  for (int i = 0; i < ROW1; i++) {
    for (int j = 0; j < COL2; j++) {
      *((result + i * COL2) + j) = 0.0f;
      for (int k = 0; k < COL1; k++) {
        *((result + i * COL2) + j) +=
            (*((A + i * COL1) + k) * *((B + k * COL2) + j));
      }
    }
  }
}

void matmul_intrinsic(float *A, float *B, float *C) {

    float32x4_t a, b,c0;
    int i,j,k;
    for ( i = 0; i < ROW1; i ++) {
            
        for (j = 0; j <= COL2 - 3; j +=4) {
                c0 = vdupq_n_f32(0.0);

                for(k = 0; k < COL1; k++){
                    // Load the values from the matrices into SSE registers
                    a = vdupq_n_f32(*(A + i*COL1 + k));
                    b = vld1q_f32(&(*(B + k*COL2 + j)));
                
                    // check(a);
                    // check(b);

                    // Perform SIMD multiplication
                    c0 = vaddq_f32(c0, vmulq_f32(a,b));
                    // check(c0);
                }
                // Store the results back into memory
                vst1q_f32(&(*(C + i*COL2 + j)), c0);
        }

        // To handle remaining elements where matrix size is not a multiple of 4
        while(j < COL2){
           for(k = 0 ;k < COL1; k++){
              *(C + i*COL2 + j) += (*(A + i*COL1 + k) * (*(B + k*COL2 + j)));
           }
          j++;
        }
        
    }

}
int main(){

  float *A = (float *)malloc(sizeof(float) * ROW1 * COL1);
  float *B = (float *)malloc(sizeof(float) * COL1 * COL2);
  float *C = (float *)malloc(sizeof(float) * ROW1 * COL2);
  float *D = (float *)malloc(sizeof(float) * ROW1 * COL2);

  std::chrono::duration<double> scalar_time, vector_time;

  initialize(A, ROW1, COL1);
  initialize(B, COL1, COL2);
  initialize_Zero(C, ROW1, COL2);
  initialize_Zero(D, ROW1, COL2);

  print(A,ROW1,COL1);
  std::cout<<std::endl;

  print(B,COL1,COL2);
  std::cout<<std::endl;

  auto start = std::chrono::high_resolution_clock::now();
  matmul_scalar(A,B,C);
  auto end = std::chrono::high_resolution_clock::now();
  scalar_time = end - start;

  print(C,ROW1,COL2);
  std::cout<<std::endl;

  std::cout<<scalar_time.count()<<std::endl;

  start = std::chrono::high_resolution_clock::now();
  matmul_intrinsic(A, B, D);
  end = std::chrono::high_resolution_clock::now();
  vector_time = end - start;

  print(D,ROW1,COL2);
  std::cout<<std::endl;

  std::cout<<vector_time.count()<<std::endl;
  
  return 0;

}