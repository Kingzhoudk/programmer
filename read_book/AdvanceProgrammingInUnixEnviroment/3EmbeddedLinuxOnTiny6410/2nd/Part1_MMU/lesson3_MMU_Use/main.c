/*
@Platform: Friendly Arm tiny6410
@CPU: S3C6410
@Function: Bootloader0 ,init cpu 
@Written by Armstrong Wall
@Start wrirte date: 2016.3.3
@Daily: 		2016.3.6  Finish init MMU
*/

#define GPKCON0 (volatile unsigned long*)0xA0008800	 //The Memory address of GPK Control Register
#define GPKDAT  (volatile unsigned long*)0xA0008808  //The Memory address of GPK Data  Register


/* 
 * 用于段描述符的一些宏定义
 */ 
#define MMU_FULL_ACCESS     (3<<10)/* 访问权限 */
#define MMU_DOMAIN		      (0<<5)/* 属于哪个域 */
#define MMU_SPECIAL         (1<<4)/* 必须是1 */
#define MMU_CACHEABLE       (1<<3)/* cacheable */
#define MMU_BUFFERABLE      (1<<2)/* bufferable */
#define MMU_SECTION         (2<<0)/* 表示这是段描述符 */

#define SECDISC             MMU_SECTION|MMU_SPECIAL|MMU_DOMAIN|MMU_FULL_ACCESS
#define SECDISC_WB          MMU_SECTION|MMU_SPECIAL|MMU_DOMAIN|MMU_FULL_ACCESS|MMU_CACHEABLE|MMU_BUFFERABLE


void create_page_table()
{
	unsigned long *ttb = (unsigned long*) 0x50000000;		//定义一个指针来指向TTB表,地址是内存的首地址
	unsigned long vaddr,paddr;													//虚拟地址 和物理地址
	
	
	/*对GPIO进行映射*/
	vaddr = 0xA0000000;
	paddr = 0x7f000000;
	
	*(ttb + (vaddr>>20))= (paddr&0xfff00000)|SECDISC;			//取出虚拟地址的高12位（base）,将这个地址的    
	//首先实现表项高12位的物理基地址,后面的是通过宏拼接起来的，有待加注释
	
	
	/*对内存进行相应的映射*/
	vaddr = 0x50000000;
	paddr = 0x50000000;
	
	while(vaddr < 0x54000000)//设置64MB的内存映射，都是原地址映射
	{
		*(ttb + (vaddr>>20))= (paddr&0xfff00000)|SECDISC_WB;
		
		vaddr += 0x100000;//加一兆的地址
		paddr += 0x100000;//加一兆的地址
		}
	
}

void mmu_init()
{
	__asm__(
	
	/*把制定好的TTB写入到cp15的C2*/
	"ldr r0, =0x50000000\n"
	"mcr p15,0,r0,c2,c0,0\n"  //把TTB装入到cp15的C2寄存器中
	
	
	/*访问控制机制(Access Permission )的设置*/
	"mvn r0,#0\n"
	"mcr p15,0,r0,c3,c0,0\n"  //设置段的访问权限，他是属于一个domain（域）决定的，设为可读可写
	
	/*使能MMU*/
	"mrc p15,0,r0,c1,c0,0\n"  //把cp15的c1寄存器的值读到r0中
	"orr r0,r0,#0x0001\n"
	"mcr p15,0,r0,c1,c0,0\n"  //使能MMU,这里曾经出了错，把mcr写成了mrc
	:
	:
	);


}


int gboot_main()
{
	//1.建立页表
	create_page_table();
	
	//2.写入TTB
	
	
  //3.使能MMU	
  mmu_init();
  
	*(GPKCON0) = 0x11110000;
	*(GPKDAT)  = 0xa0;
	
	
	return 0;
}


/*
int gboot_main()
{
	unsigned long a=500000;
	*(GPKCON0) = 0x11110000;
	
	while(1)
	{
		light_led();
		
	  *(GPKDAT)  = 0xb0;
	  while(a--);
	  a=500000;
	  
	  
	  *(GPKDAT)  = 0x70;
	  while(a--);
	  a=500000;
	  
	  *(GPKDAT)  = 0xd0;
	  while(a--);
	  a=500000;
	  
	  *(GPKDAT)  = 0xe0;
	  while(a--);
	  a=500000;
	  
	  
		
		}
	
	
	
	return 0;
}
*/
