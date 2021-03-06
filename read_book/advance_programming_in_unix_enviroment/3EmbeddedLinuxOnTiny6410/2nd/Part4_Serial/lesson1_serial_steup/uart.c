/*
@Platform: Friendly Arm tiny6410
@CPU: S3C6410
@Function: 
@Written by Armstrong Wall
@Start wrirte date: 2016.3.13
@Daily: 		2016.3.10  Finish Nand Flash
*/
#define GPACON	 (*(volatile unsigned long*) 0x7F008000)
//Port A Configuration Register
#define ULCON0	 (*(volatile unsigned long*) 0x7F005000)
//UART channel 0 line control register
#define UCON0    (*(volatile unsigned long*) 0x7F005004)
//UART channel 0 control register
#define UBRDIV0	 (*(volatile unsigned long*) 0x7F005028)
//Baud rate divisior register 0
#define UTRSTAT0 (*(volatile unsigned long*) 0x7F005010)
//UART channel 0 Tx/Rx status register 0x6
#define UTXH0		 (*(volatile unsigned long*) 0x7F005020)
//UART channel 0 transmit buffer register
#define UDIVSLOT0 (*(volatile unsigned long*) 0x7F00502C)
//Baud rate divisior register 0

#define PCLK 66000000
#define Baud 115200

void uart_init()
{
//1.steup pin function
		GPACON |= 0x22;
//GPA0 [3:0]  0010 = UART RXD[0]
//GPA1 [7:4]  0010 = UART TXD[0]
 

//2.date Format
	  ULCON0 = 0x03;
//No parity ,One stop bit per frame , 11 = 8-bit

//3.Work mode
		UCON0 = 0b0101;
//Transmit Mode [3:2] 01 = Interrupt request or polling mode
//Receive Mode [1:0]	01 = Interrupt request or polling mode
		
//4.UART BAUD RATE 
//DIV_VAL =  (PCLK / (bps x 16 ) ) -1  = UBRDIVn + (num of 1��s in UDIVSLOTn)/16
//DIV_VAL = (66M / (115200 x 16 ) ) -1 = 34.8= 34 + 0.8
//so (num of 1��s in UDIVSLOTn)/16 = 0.8 , UDIVSLOTn =[12.8]=12
//according to datasheet P1101,UDIVSLOT0  = 0xDDDD;

		UBRDIV0    = (long)((PCLK/(Baud*16 ))-1); //34
		UDIVSLOT0  = 0xDDDD;
//
		
//DIV_VAL = (PCLK / (bps x 16 ) )-1		
}

//put a char
void put_char(unsigned char c)
{
		while(!(UTRSTAT0 & (1<<2)));
//if TX,FIFO full, wait

	  UTXH0 = c ;
}

//get a char
unsigned char get_char()
{
	unsigned char ret;
	
			while(!(UTRSTAT0 & (1<<0)));
//if RX,FIFO empty,wait
		if((ret == 0x0d) || (ret == 0x0a))
			{
				
					put_char(0x0d);
					put_char(0x0a);
					
			}
		else
			{
				  put_char(ret);
			}
			
	  return ret;
}



