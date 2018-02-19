#ifndef DECODER_H
#define DECODER_H

#include <avr/io.h>


uint8_t Decoder_Byte(uint8_t* byte,uint8_t type,uint8_t* polynom );

#endif /* CRC_H */

//****************************************************************************//
//************************  Decode Byte with custom polynom  *****************//
//****************************************************************************//
/*unsigned char Decode_Byte( unsigned char* pol_h_1, unsigned char* pol_h_0,
                           unsigned char* pol_l_1, unsigned char* pol_l_0,
                           unsigned char byte, unsigned char type )
{
  unsigned long poly = 0;
  unsigned char bit_result = 0;
  
  if( type )
  {
    byte ^= *( pol_h_1 );
  }
  else
  {
    byte ^= *( pol_h_0 );
    
    bit_result = ( *( pol_l_1 ) << 4 )^( *( pol_l_0 ) << 3 );
    bit_result ^= ( *( pol_l_0 ) << 6 );
    bit_result &= 0x80;
    
    poly = ( unsigned long  )(*( pol_h_1 )) << 24;
    poly |= ( unsigned long )(*( pol_h_0 )) << 16;
    poly |= *( pol_l_1 ) << 8;
    poly |= *( pol_l_0 );
    
    poly >>= ( unsigned long )1;  
    poly |= ( unsigned long )bit_result << 24;
    
    *( pol_l_0 ) = ( unsigned char )poly; 
    *( pol_l_1 ) = ( unsigned char )( poly >> 8 ); 
    *( pol_h_0 ) = ( unsigned char )( poly >> 16 ); 
    *( pol_h_1 ) = ( unsigned char )( poly >> 24 );     
  }
  return byte;
}
*/