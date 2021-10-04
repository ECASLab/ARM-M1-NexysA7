#include <ARMCM1.h>
#include <core_cm1.h>
#include "xil_printf.h"
#include "xil_types.h"
#include "xstatus.h"
#include "uart.h"
#include "gpio.h"

int main(void)
{
    int status;
    // CPU ID register
    volatile u32 *pCPUId = (u32 *)0xE000ED00;
    volatile u32 CPUId;
    int          CPU_part;
    int          CPU_rev;
    int          CPU_var;
    char         CPU_name[20];

    status=InitialiseUART();
    if(status != XST_SUCCESS){
		//add print error message
		return XST_FAILURE;
	}    
    // Enable the UART interrupt
    NVIC_EnableIRQ(UART0_IRQn);
    EnableUARTInterrupts();
	/* Initialize the GPIO driver */
	status=InitialiseGPIO();
	if(status != XST_SUCCESS){
		//add print error message
		return XST_FAILURE;
	}  
    // Enable the GPIO interrupt
    NVIC_EnableIRQ(GPIO0_IRQn);
    NVIC_EnableIRQ(GPIO1_IRQn);
    // Enable GPIO Interrupts
    EnableGPIOInterrupts();
    CPUId    = *pCPUId;
    CPU_var  = ((CPUId & 0x00F00000) >> 20);
    CPU_part = ((CPUId & 0x0000FFF0) >> 4);
    CPU_rev  = CPUId & (0x0000000F);
    switch (CPU_part)
    {
        case 0xC21 : strcpy(  CPU_name, "Cortex-M1" ); break;
        case 0xC23 : strcpy(  CPU_name, "Cortex-M3" ); break;
        default    :
					 strcpy(  CPU_name, "Unknown: ");
                	 strcat(  CPU_name, (char *)CPU_part);
                	 break;
    }
#ifndef SIM_BUILD    
    // Use Xilinx version print command
    print ("************************************\r\n");
    xil_printf ("Arm %s Revision %i Variant %i\r\n\n", CPU_name, CPU_rev, CPU_var );
    print ("Example design for Digilent A7 board\r\n");
    print (" Version 1.1\r\n");
    print ("************************************\r\n");
#else
    xil_printf ("Arm %s Revision %i Variant %i\r\n\n", CPU_name, CPU_rev, CPU_var );
#endif
    print("Hello World: platform m1_nexys_a7_io_hw\n\r");
    while(1)
    {
        
    }
	return 0;
}