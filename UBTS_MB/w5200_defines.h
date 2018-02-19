
#ifndef DEFINES_W5200_H_
#define DEFINES_W5200_H_


#define COMMON_BASE						0x0000
#define CH_BASE                         (COMMON_BASE + 0x4000)
#define CH_SIZE							0x0100
#define Sn_MR(ch)			(CH_BASE + ch * CH_SIZE + 0x0000)
#define Sn_CR(ch)			(CH_BASE + ch * CH_SIZE + 0x0001)
#define Sn_IR(ch)			(CH_BASE + ch * CH_SIZE + 0x0002)
#define Sn_SR(ch)			(CH_BASE + ch * CH_SIZE + 0x0003)
#define Sn_PORT(ch)			(CH_BASE + ch * CH_SIZE + 0x0004)
#define Sn_MAC(ch)			(CH_BASE + ch * CH_SIZE + 0x0006)
#define Sn_IP(ch)			(CH_BASE + ch * CH_SIZE + 0x000C)
//#define Sn_MSSR0(ch)		(CH_BASE + ch * CH_SIZE + 0x0012)
#define Sn_TX_FSR(ch)		(CH_BASE + ch * CH_SIZE + 0x0020)
#define Sn_TX_RD(ch)		(CH_BASE + ch * CH_SIZE + 0x0022)
#define Sn_TX_WR(ch)		(CH_BASE + ch * CH_SIZE + 0x0024)
#define Sn_RX_RSR(ch)		(CH_BASE + ch * CH_SIZE + 0x0026)
#define Sn_RX_RD(ch)		(CH_BASE + ch * CH_SIZE + 0x0028)
#define Sn_RX_WR(ch)		(CH_BASE + ch * CH_SIZE + 0x002A)
#define RX_BASE_ADDR(ch)		(ch * 2048 + 0xc000)
#define TX_BASE_ADDR(ch)		(ch * 2048 + 0x8000)


#define MR_W5200		0x0000
#define	GATEWAY			0x0001
#define SUBNET			0x0005
#define	MAC				0x0009
#define	IP_source		0x000f
#define IR_W5200		0x0015
#define IMR_W5200		0x0016
#define RTR_W5200		0x0017
#define RCR_W5200		0x0019
#define IR2_W5200		0x0034
#define PHY_STATUS		0x0035
#define IMR2_W5200		0x0036

//#define	SOCKET_CR		0x4001
//#define	SOCKET_IR		0x4002
//#define	SOCKET_SR		0x4003
//#define	SOCKET_MAC_dest	0x4006
//#define	SOCKET_IP_dest	0x400c

//#define	Sn_RX_RSR		0x4026
//#define	Sn_RX_RD		0x4028
//#define	Sn_RX_WR		0x402a
//
//#define	Sn_TX_FSR		0x4020
//#define	Sn_TX_RD		0x4022
//#define	Sn_TX_WR		0x4024


//#define	RX_BASE_ADDR	0xc000
//#define	TX_BASE_ADDR	0x8000

#define	BUFFER_MASK		0x07ff

#define _READ				0x00
#define _WRITE				0x80









#endif /* DEFINES_W5200_H_ */