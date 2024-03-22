.global    chroma_vertical_vector
.type      chroma_vertical_vector, "function"
.p2align   2


chroma_vertical_vector:
    STP X29, X30, [SP, -16]!
    MOV X29, SP
  
    //Move to the base address (pu1_src - 4) for loading the data
    MOV X6, #-4
    
    //Move to the base address (pu1_src - 2)for storing the computed values when all computations are over
    MOV X5, #-2
    
    ADD X6, X6, X0
    ADD X5, X5, X0

    //Loading the input array values
    LD1 {V0.8B}, [X6]       //X6 is pointing to row 1
    ADD X6, X6, X1

    LD1 {V1.8B}, [X6]       //X6 is pointing to row 2
    ADD X6, X6, X1
  
    LD1 {V2.8B}, [X6]       //X6 is pointing to row 3
    ADD X6, X6, X1

    LD1 {V3.8B}, [X6]       //X6 is pointing to row 4
    
    UXTL V0.8H, V0.8B
    UXTL V1.8H, V1.8B
    UXTL V2.8H, V2.8B
    UXTL V3.8H, V3.8B

    ZIP2 V4.8H, V0.8H, V2.8H
    ZIP2 V5.8H, V1.8H, V3.8H

    ZIP1 V6.8H, V4.8H, V5.8H   //V6 contains pu1_src[0] & pu1_src[1]
    ZIP2 V7.8H, V4.8H, V5.8H   //V7 contains pu1_src[2] & pu1_src[3]

    ZIP1 V8.8H, V0.8H, V2.8H
    ZIP1 V9.8H, V1.8H, V3.8H

    ZIP1 V10.8H, V8.8H, V9.8H   //V10 contains pu1_src[-4] & pu1_src[-3]
    ZIP2 V11.8H, V8.8H, V9.8H   //V11 contains pu1_src[-2] & pu1_src[-1]

    SUB V12.8H, V6.8H, V11.8H   //V12 contains pu1_src[0] - pu1_src[-2] & pu1_src[1] - pu1_src[-1]
    SUB V13.8H, V10.8H, V7.8H   //V13 contains pu1_src[-4] - pu1_src[2] & pu1_src[-3] - pu1_src[3]

    SHL V12.8H, V12.8H, #2       //V12 << 2
    
    MOVI V14.8H, #4
    ADD V13.8H, V13.8H, V14.8H   //V13 + 4
 
    ADD V15.8H, V12.8H, V13.8H   //V15 = V12 + V13
    SSHR V15.8H, V15.8H, #3      //V15 = V15 >> 3

    DUP V4.4H, W2                // V4 contains the value of tc_u  
    DUP V8.4H, W3                // V8 contains the value of tc_v  

    //V4 contains tc_u as first 4 elements and tc_v as next 4 elements 
    MOV V4.D[1], V8.D[0]         //V4 contains {tc_u,tc_u,tc_u,tc_u,  tc_v,tc_v,tc_v,tc_v}
                                  
    NEG V5.8H, V4.8H             // V5 contains the negative values of V4 register

    //av_clip operation for delta_u and delta_v 
    SMIN V16.8H, V15.8H, V4.8H
    SMAX V16.8H, V16.8H, V5.8H   //V16 delta_u followed by delta_v value after clip operation 

    MOVI V8.8H, #0  
    MOVI V9.8H, #255
    
    //tmp_p0_u and tmp_p0_v values
    ADD V10.8H, V11.8H, V16.8H    //V10 contains (pu1_src[-2], pu1_src[-1]) + V16

    //av_clip_uint operation for tmp_p0_u and tmp_p0_v values
    SMIN V17.8H, V10.8H, V9.8H
    SMAX V18.8H, V17.8H, V8.8H    //V18 contains tmp_p0_u and tmp_p0_v values

    //tmp_q0_u and tmp_q0_v values
    SUB V10.8H, V6.8H, V16.8H     //V10 contains (pu1_src[0], pu1_src[1]) + V16

    //av_clip_uint operation for tmp_q0_u and tmp_q0_v values
    SMIN V19.8H, V10.8H, V9.8H
    SMAX V20.8H, V19.8H, V8.8H    //V20 contains tmp_q0_u and tmp_q0_v values

    //Interleaving the elements of V18 and V20
    ZIP1 V21.8H, V18.8H, V20.8H
    ZIP2 V22.8H, V18.8H, V20.8H
    ZIP1 V18.8H, V21.8H, V22.8H   //V18 contains the values of row1 and row2  
    ZIP2 V20.8H, V21.8H, V22.8H   //V20 contains the values of row2 and row4

    //Narrow down the size to 8-bit and then store
    XTN  V18.8B, V18.8H
    XTN  V20.8B, V20.8H
    
    /*Store four, 8-bit element into register 
    pointing to base address and post increment with X1 
    to point to the next address
    */
    
    ST1 {V18.S}[0], [X5], X1
    ST1 {V18.S}[1], [X5], X1

    ST1 {V20.S}[0], [X5], X1
    ST1 {V20.S}[1], [X5], X1
    
    MOV SP, x29
    LDP x29, x30, [SP], 16
    RET
