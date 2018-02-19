//****************************************************************************//
//************************  Calc CRC of 2 Word    ****************************//
//****************************************************************************//
MODULE _Calc_CRC
PUBLIC Calc_CRC
RSEG CODE
#include <avr\io.h>

        #define    counter       r19 //count
        #define    temp_reg      r22 //temp

        #define    data          r16 // data
//#define    P_CRC_l       r21:r20 //crc_low

//#define    p_CRC_h       r19:r17 //crc_high

        #define    CRC_l         r17
        #define    CRC_h         r18


    Calc_CRC:
//      in R16 - data
//      in R17 - CRC high
//      in R18 - CRC Low

       // push    data
//        push    counter
        push    temp_reg
        push    ZH
        push    ZL
        push    CRC_h
        push    CRC_l
        
        ldi     counter, 8 // load counter 8
//        mov     Zl, low(p_CRC_l)
        movw	ZH:ZL, r21:r20  // Load Z with point adress.
        ld      CRC_l, Z+
        ld      CRC_h, Z
        
        
        

    next_iter:

        bst     CRC_l, 1
        bld     temp_reg, 0
        
        eor     temp_reg, CRC_l
        eor     temp_reg, data
        
        clc
        
        sbrs    temp_reg, 0
        rjmp    shift_CRC
        
        sec

    shift_CRC:        
        ror     CRC_h
        ror     CRC_l
        
        lsr     data
        
        dec     counter
        
        brne    next_iter
        
        st      Z, CRC_h 
        st      -Z, CRC_l               
        
       // pop    data
       // pop    counter
        pop    temp_reg
        pop    ZH
        pop    ZL
        pop    CRC_h
        pop    CRC_l
        ret
ENDMOD
END
