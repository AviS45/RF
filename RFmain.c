#include "RF_Toggle_LED_Demo.h"
#include "rfOps.h"
#define PACKET_LEN (0x05)
#define RSSI_IDX (PACKET_LEN+1)
#define CRC_LQI_IDX (PACKET_LEN+2)
#define CRC_OK (BIT7)
#define PATABLE_VAL (0xC0)
//#define PATABLE_VAL (0xC0)
#define PATABLE_ARRAY_SZ	8
static const unsigned char PATABLE_ARR[PATABLE_ARRAY_SZ]={0xC0,0,0,0,0,0,0,0};
extern RF_SETTINGS rfSettings;
unsigned char packetReceived;
unsigned char packetTransmit;
unsigned char RxBuffer1[6]={PACKET_LEN, 0xFF,0X00,PACKET_LEN, 0xFF, 0xFF};
unsigned char RxBufferLength = 0;
unsigned char RxBuffer[6];
const unsigned char TxBuffer3[3]= {PACKET_LEN, 0xFF, 0xAA};
const unsigned int i = 0;
void main(void)
{
/// Stop watchdog timer to prevent time out reset
WDTCTL = WDTPW + WDTHOLD;
P3OUT &= ~BIT6;

SetVCore(2);
ResetRadioCore();
RfOps_InitRadio(&rfSettings,(unsigned char*)PATABLE_ARR,PATABLE_ARRAY_SZ);
InitButtonLeds();
RfOps_ReceiveOn();

while (1)
{
__bis_SR_register(LPM3_bits + GIE);
__no_operation();
if (RfOps_HasPacketReceived())
{

RxBufferLength = ReadSingleReg(RXBYTES);
ReadBurstReg(RF_RXFIFORD, RxBuffer, RxBufferLength);

__no_operation();

if (RxBuffer[CRC_LQI_IDX] & CRC_OK)
   if (RxBuffer1[0] == RxBuffer[0])
	   if (RxBuffer1[1] == RxBuffer[1])
	      if (RxBuffer1[2] == RxBuffer[2])
	         if (RxBuffer1[3] == RxBuffer[3])
	             if (RxBuffer1[4] == RxBuffer[4])
	            	 if (RxBuffer1[5] == RxBuffer[5])
	            		 {
	            		 P3OUT ^= BIT6;
RfOps_ReceiveOff();
RfOps_Transmit((unsigned char*) TxBuffer3, sizeof TxBuffer3);

RfOps_ReceiveOn();
RfOps_ClearPacketReceivedFlag();
}
}
}
}
void InitButtonLeds(void)
{

P1DIR &= ~BIT7;
P1REN |= BIT7;
P1IES &= BIT7;
P1IFG = 0;
P1OUT |= BIT7;
P1IE |= BIT7;
PJOUT = 0x00;
PJDIR = 0xFF;
P1OUT &= ~BIT0;
P1DIR |= BIT0;
P3OUT &= ~BIT6;
P3DIR |= BIT6;
}
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
switch (__even_in_range(P1IV, 16))
{
case 0:
break;
case 2:
break;
case 4:
break;
case 6:
break;
case 8:
break;
case 10:
break;
case 12:
break;
case 14:
break;
case 16:
P1IE = 0;
//buttonPressed = TRUE;

__bic_SR_register_on_exit(LPM3_bits);
break;
}
}
