#include "delay.h"

//=======================  以下是使用DWT重新编写HAL库延迟相关函数，释放SysTick计时器   ===========================//
//=======================  当SysTick被占用时，可以使用此方法来对HAL库中延迟函数重新配置 ===========================//
//=========== The following is to use DWT to rewrite the HAL library delay related functions and release the SysTick timer ============//
//============ When SysTick is occupied, this method can be used to reconfigure the delay function in the HAL library =============//
/*
	DWT使用步骤：
a.先使能DWT外设，由内核调试寄存器DEM_CR的位24控制，写1使能。   DEM_CR地址0xE000EDFC
b.使能CYCCNT寄存器之前，先清0。						 									CYCCNT地址0xE001004
c.使能CYCCNT寄存器，由DWT_CTRL的位0控制，写1使能。		 				DWT_CTRL地址0xE0001000
*/
/*
DWT usage steps:
a. Enable the DWT peripheral first, controlled by bit 24 of the kernel debug register DEM_CR, write 1 to enable. DEM_CR address 0xE000EDFC
b. Before enabling the CYCCNT register, clear it to 0.                                                                     CYCCNT address 0xE001004
c. Enable the CYCCNT register, controlled by bit 0 of DWT_CTRL, write 1 to enable.                        DWT_CTRL address 0xE0001000
*/

//=== 重写HAL库延迟函数后，宏定义  HAL_MAX_DELAY 需要修改成下列式子 === //
//修改完成后需要注释掉下面的语句，防止冲突
//=== After rewriting the HAL library delay function, the macro definition HAL_MAX_DELAY needs to be modified to the following formula === //
//After the modification, the following statement needs to be commented out to prevent conflicts
//#define HAL_MAX_DELAY  4294967295/(HAL_RCC_GetSysClockFreq()/1000)

//DWT延迟微秒因子 DWT delay microsecond factor
uint32_t dwt_us;

//重写HAL_InitTick()，生成函数后此函数会被自动调用，用户不需要自行再次调用
//Rewrite HAL_InitTick(). This function will be called automatically after the function is generated. Users do not need to call it again.
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
	//1.使用DWT前必须使能DGB的系统跟踪（《Cortex-M3权威指南》） DGB system trace must be enabled before using DWT
	DEM_CR |= 1<<24;

	//2.使能CYCCNT计数器前必须先将其清零，CYCCNT计数器是32位的  Before enabling the CYCCNT counter, it must be cleared first. The CYCCNT counter is 32 bits.
	DWT_CYCCNT = (uint32_t)0u;

	//3.使能CYCCNT，开启计时  Enable  CYCCNT and start timing
	DWT_CTRL |= 1<<0;

	//4.计算DWT微秒延迟函数的延迟因子  Calculate the delay factor of the DWT microsecond delay function
	dwt_us = HAL_RCC_GetSysClockFreq()/1000000;

	return HAL_OK;
}

//重写HAL_GetTick() Override HAL_GetTick()
uint32_t HAL_GetTick(void)
{
	//把计数值除以内核频率的1000分之一倍，实现1毫秒返回1
	//以C8T6为例：因为计数到72是1微秒（1÷72MHz）
	//能返回的最大的毫秒值是 2^32 - 1 / 72000 = 59652  (取整数)
	// Divide the count value by 1/1000 of the core frequency to return 1 in 1 millisecond
	// Take C8T6 as an example: because counting to 72 is 1 microsecond (1÷72MHz)
	// The maximum millisecond value that can be returned is 2^32 - 1 / 72000 = 59652 (rounded)
	return ((uint32_t)DWT_CYCCNT/(HAL_RCC_GetSysClockFreq()/1000));
}

//重写HAL_Delay()
//Delay 范围： 0 ~ ( 2^32-1 / （系统时钟÷1000））
//72MHz时钟： 0~59652
//84MHz时钟： 0~51130
//180MHz时钟：0~23860
//400Mhz时钟：0~10737
//Rewrite HAL_Delay()
//Delay range: 0 ~ (2^32-1 / (system clock ÷ 1000))
//72MHz clock: 0 ~ 59652
//84MHz clock: 0 ~ 51130
//180MHz clock: 0 ~ 23860
//400Mhz clock: 0 ~ 10737
void HAL_Delay(uint32_t Delay)
{
  uint32_t tickstart = HAL_GetTick();
  uint32_t wait = Delay;

  /* Add a freq to guarantee minimum wait */
  if (wait < __HAL_MAX_DELAY)
  {
    wait += (uint32_t)(uwTickFreq);
  }

  //HAL_Delay()延迟函数重写后，需要考虑溢出的问题，原函数注释  After the HAL_Delay() delay function is rewritten, the overflow problem needs to be considered. The original function comments
//  while ((HAL_GetTick() - tickstart) < wait)
//  {
//  }

  wait += tickstart;   	   																 //计算所需要的计时时间  Calculate the time required
  if(wait>__HAL_MAX_DELAY) wait = wait - __HAL_MAX_DELAY;  //大于最大计数值则溢出,计算溢出部分  If it is greater than the maximum count value, it overflows and calculates the overflow part

  //计数没有溢出，直接等待到延迟时间即可 If the count does not overflow, just wait until the delay time.
   if(wait>tickstart)
  {
	while(HAL_GetTick()<wait);
  }
  //计数溢出 Count overflow
  else
  {
	while(HAL_GetTick()>wait); //未溢出部分计时 Timing of the non-overflow part
	while(HAL_GetTick()<wait); //溢出部分计时 Overflow Part Timing
  }
}

//返回32位计数器CYCCNT的值 Returns the value of the 32-bit counter CYCCNT
uint32_t DWT_CNT_GET(void)
{
	return((uint32_t)DWT_CYCCNT);
}

//使用DWT提供微秒级延迟函数 Using DWT to provide microsecond delay function
void HAL_Delay_us(uint32_t us)
{
	uint32_t BeginTime,EndTime,delaytime;

	//获取当前时间戳 Get the current timestamp
	BeginTime = DWT_CNT_GET();

	//计算需要延迟多少微秒 Calculate how many microseconds to delay
	delaytime = us*dwt_us;

	//起始+需要延迟的微秒 = 需要等待的时间  Start + microseconds of delay = time to wait
	EndTime = BeginTime+delaytime;

	//计数没有溢出，直接等待到延迟时间即可  If the count does not overflow, just wait until the delay time.
	if(EndTime>BeginTime)
	{
		while(DWT_CNT_GET()<EndTime);
	}

	//计数溢出 Count overflow
	else
	{
		while(DWT_CNT_GET()>EndTime); //未溢出部分计时  Timing of the non-overflow part
		while(DWT_CNT_GET()<EndTime); //溢出部分计时  Overflow Part Timing
	}
}

////使用DWT提供微秒级延迟函数,方法2 Using DWT to provide microsecond delay function, method 2
//void HAL_Delay_us(uint32_t us)
//{
//	//用于保存上一次计数值和当前计数值 Used to save the last count value and the current count value
//	uint32_t last_count,count,WaitTime;
//
//	//计算需要延迟多少个1微秒 Calculate how many 1 microseconds of delay are needed
//	uint32_t delaytime = us*dwt_us;
//
//	last_count = DWT_CNT_GET(); //获取当前的计数值 Get the current count value
//	WaitTime = 0;
//
//	//循环等待延迟时间 Loop wait delay time
//	while(WaitTime<delaytime)
//	{
//		count = DWT_CNT_GET();
//		//计时未溢出 The timer has not overflowed
//		if(count>last_count) WaitTime = count-last_count;
//
//		//计时溢出，加上32位的偏差值 Timing overflow, plus 32-bit deviation value
//		else				 WaitTime = count+0xffffffff - last_count;
//	}
//}


//======================= SysTick被释放，以下是对SysTick重新配置利用 SysTick is released, the following is the SysTick reconfiguration exploit ===========================//

u16 i_us;  //微秒因子 Microsecond Factor
u16 i_ms;  //毫秒因子 Millisecond Factor

//SysTick被释放后，使用SysTick编写延迟函数 After SysTick is released, use SysTick to write a delay function
void delay_init(void)
{
	SysTick->CTRL &= ~(1<<2); 				   //设定Systick时钟源，HCLK/8 Set Systick clock source, HCLK/8
	SysTick->CTRL &= ~(1<<1);				     //关闭由HAL库自带的SysTick中断，减少系统资源浪费 Disable the SysTick interrupt provided by the HAL library to reduce the waste of system resources
	i_us = HAL_RCC_GetSysClockFreq()/8000000;  //计算微秒因子 HCLK/晶振 Calculate the microsecond factor HCLK/crystal
	i_ms = i_us * 1000 ; 					   //计算毫秒因子 Calculating millisecond factors
}

// i_us * us 的值不可超过 2^24 -1 = 16777215
// 以72MHz的F103C8T6为例，i_us = 9 , us = 0~1,864,135
// The value of i_us * us cannot exceed 2^24 -1 = 16777215
// Taking the 72MHz F103C8T6 as an example, i_us = 9, us = 0~1,864,135
void delay_us(u32 us)
{
//	u32 temp;
//	SysTick -> LOAD = i_us * us; //计算需要设定的自动重装值 Calculate the auto-reload value that needs to be set
//	SysTick -> VAL = 0 ;         //清空计数器 Clear counter
//	SysTick -> CTRL |= 1<<0 ;    //开启计时 Start timing
//	do{
//		temp = SysTick -> CTRL; 		  			//读取CTRL寄存器的状态位，目的是获取第0位和最高位 Read the status bits of the CTRL register to get the 0th and highest bits
//	}while((temp&0x01)&&!(temp&(1<<16))); //如果计数器被使能且计时时间未到
//	SysTick->CTRL &= ~(1<<0);  			  		//计时结束，关闭倒数 Timer ends, close countdown
//	SysTick -> VAL = 0 ;       		      	//清空计数器 Clear counter
}

// i_ms * ms 的值不可超过 2^24 -1 = 16777215
// 以72MHz的F103C8T6为例，i_ms = 9000 , us = 0 ~ 1864
// The value of i_ms * ms cannot exceed 2^24 -1 = 16777215
// Taking the 72MHz F103C8T6 as an example, i_ms = 9000, us = 0 ~ 1864
void delay_ms(u16 ms)
{
	u32 temp;
	SysTick -> LOAD = i_ms * ms; //计算需要设定的自动重装值 Calculate the auto-reload value that needs to be set
	SysTick -> VAL = 0 ;         //清空计数器 Clear counter
	SysTick -> CTRL |= 1<<0 ;    //开启计时 Start timing
	do{
		temp = SysTick -> CTRL; 		  			//读取CTRL寄存器的状态位，目的是获取第0位和最高位 Read the status bits of the CTRL register to get the 0th and highest bits
	}while((temp&0x01)&&!(temp&(1<<16))); //如果计数器在使能，且倒数未到，则循环 If the counter is enabled and the countdown has not yet arrived, the loop
	SysTick->CTRL &= ~(1<<0);  			  		//计时结束，关闭倒数 Timer ends, close countdown
	SysTick -> VAL = 0 ;       		      	//清空计数器 Clear counter
}
