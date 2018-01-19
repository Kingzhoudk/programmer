#define GPKCON0 (volatile unsigned long*)0x7f008800	//The Memory address of GPK Control Register
#define GPKDAT  (volatile unsigned long*)0x7f008808  //The Memory address of GPK Data  Register

/*  Virtual address
#define GPKCON0 (volatile unsigned long*)0xA0008800	 //The Virtual Memory address of GPK Control Register
#define GPKDAT  (volatile unsigned long*)0xA0008808  //The Virtual Memory address of GPK Data  Register
*/
void led_init()
{
	*(GPKCON0) = 0x11110000;
	
	}

void led_on()
{
	*(GPKDAT)  = 0xa0;
	
	}

void led_off()
{
	*(GPKDAT)  = 0x50;
	}	