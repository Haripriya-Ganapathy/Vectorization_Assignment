.global    chroma_horizontal_vector
.type      chroma_horizontal_vector, "function"
.p2align   2


chroma_horizontal_vector:
    STP X29, X30, [SP, -16]!
    MOV X29, SP
  
    //Loading the input array values
    MOV X6, #-2
    MOV X7, #-1 

    MUL X6, X6, X1
    MUL X7, X7, X1

    ADD X6, X6, X0
    ADD X7, X7, X0
    ADD X8, X1, X0

    LD1 {V0.8B}, [X6]       //X6 is pointing to pu1_src[-2 * src_strd]
    LD1 {V1.8B}, [X7]       //X7 is pointing to pu1_src[-1 * src_strd]
    LD1 {V2.8B}, [X0]       //X0 is pointing to pu1_src[0 * src_strd]
    LD1 {V3.8B}, [X8]       //X8 is pointing to pu1_src[1 * src_strd]
    
    UXTL V0.8H, V0.8B
    UXTL V1.8H, V1.8B
    UXTL V2.8H, V2.8B
    UXTL V3.8H, V3.8B

    SUB V4.8H, V2.8H, V1.8H
    SUB V5.8H, V0.8H, V3.8H
    
    SHL V4.8H, V4.8H, #2

    MOVI V7.8H, #4
    ADD V5.8H, V5.8H, V7.8H

    ADD V6.8H, V4.8H, V5.8H
    SSHR V6.8H, V6.8H, #3       //V6 contains the value of delta before clip operation 

    DUP V8.8H, W2                // V8 contains the value of tc_u  
    DUP V7.8H, W3                // V7 contains the value of tc_v  
    ZIP1 V8.8H, V8.8H, V7.8H     // V8 contains {tc_u,tc_v,tc_u,tc_v,tc_u,tc_v,tc_u,tc_v}
    NEG V9.8H , V8.8H            // V9 contains the negative values of the register V8

    //av_clip operation
    SMIN V10.8H, V6.8H, V8.8H
    SMAX V11.8H, V10.8H, V9.8H   //V11 contains the value of delta after clip operation 
    
    //p0 and q0 values
    ADD V12.8H, V1.8H, V11.8H    //V12(p0) = -1 * src_strd + delta
    SUB V13.8H, V2.8H, V11.8H    //V13(q0) =  0 * src_strd - delta

    MOVI V14.8H, #0  
    MOVI V15.8H, #255

    //av_clip_uint operation
    SMIN V16.8H, V12.8H, V15.8H
    SMAX V17.8H, V16.8H, V14.8H

    //Narrow down the size to 8-bit and then store
    XTN  V17.8B, V17.8H
    ST1 {V17.8B}, [X7]              //p0 values

    //av_clip_uint operation
    SMIN V18.8H, V13.8H, V15.8H
    SMAX V19.8H, V18.8H, V14.8H

    //Narrow down the size to 8-bit and then store
    XTN  V19.8B, V19.8H
    ST1 {V19.8B} , [X0]            //q0 values

    MOV SP, x29
    LDP x29, x30, [SP], 16
    RET
