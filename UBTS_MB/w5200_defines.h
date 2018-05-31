
#ifndef DEFINES_W5200_H_
#define DEFINES_W5200_H_


#define	RX_MASK							0x07ff			//2048 - 1
#define CH_BASE                         0x4000
#define CH_SIZE							0x0100
#define Sn_MR(ch)			(CH_BASE + ch * CH_SIZE + 0x0000)
#define Sn_CR(ch)			(CH_BASE + ch * CH_SIZE + 0x0001)
#define Sn_IR(ch)			(CH_BASE + ch * CH_SIZE + 0x0002)
#define Sn_SR(ch)			(CH_BASE + ch * CH_SIZE + 0x0003)
#define Sn_PORT(ch)			(CH_BASE + ch * CH_SIZE + 0x0004)

#define Sn_DHAR0(ch)			(CH_BASE + ch * CH_SIZE + 0x0006)
//#define Sn_DHAR1(ch)			(CH_BASE + ch * CH_SIZE + 0x0007)
//#define Sn_DHAR2(ch)			(CH_BASE + ch * CH_SIZE + 0x0008)
//#define Sn_DHAR3(ch)			(CH_BASE + ch * CH_SIZE + 0x0009)
//#define Sn_DHAR4(ch)			(CH_BASE + ch * CH_SIZE + 0x000a)
//#define Sn_DHAR5(ch)			(CH_BASE + ch * CH_SIZE + 0x000b)

#define Sn_DIPR0(ch)			(CH_BASE + ch * CH_SIZE + 0x000c)
//#define Sn_DIRP1(ch)			(CH_BASE + ch * CH_SIZE + 0x000d)
//#define Sn_DIRP2(ch)			(CH_BASE + ch * CH_SIZE + 0x000e)
//#define Sn_DIRP3(ch)			(CH_BASE + ch * CH_SIZE + 0x000f)

#define Sn_DPORT0(ch)			(CH_BASE + ch * CH_SIZE + 0x0010)
//#define Sn_DPORT1(ch)			(CH_BASE + ch * CH_SIZE + 0x0011)


//#define Sn_MSSR0(ch)		(CH_BASE + ch * CH_SIZE + 0x0012)
#define Sn_TX_FSR(ch)		(CH_BASE + ch * CH_SIZE + 0x0020)
#define Sn_TX_RD(ch)		(CH_BASE + ch * CH_SIZE + 0x0022)
#define Sn_TX_WR(ch)		(CH_BASE + ch * CH_SIZE + 0x0024)
#define Sn_RX_RSR(ch)		(CH_BASE + ch * CH_SIZE + 0x0026)
#define Sn_RX_RD(ch)		(CH_BASE + ch * CH_SIZE + 0x0028)
#define Sn_RX_WR(ch)		(CH_BASE + ch * CH_SIZE + 0x002A)
#define RX_BASE(ch)		(ch * (RX_MASK + 1) + 0xc000)
#define TX_BASE(ch)		(ch * (RX_MASK + 1) + 0x8000)


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
#define INT_W5200		0x3031
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

//#define	BUFFER_MASK		0x07ff

#define _READ				0x00
#define _WRITE				0x80



#define _OPEN_COMMAND			0x01
#define _LISTEN_COMMAND			0x02
//#define _CONNECT_COMMAND		0x04
#define _DISCONNECT_COMMAND		0x08
#define _CLOSE_COMMAND			0x10
#define _SEND_COMMAND			0x20
#define _RECV_COMMAND			0x40


#define _SOCK_CLOSED					0x00
#define _SOCK_INIT					0x13
#define _SOCK_LISTEN				0x14
#define _SOCK_ESTABL				0x17
#define _SOCK_UDP					0x22
#define _SOCK_CLOSE_WAIT			0x1c





#endif /* DEFINES_W5200_H_ */