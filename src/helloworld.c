/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "ps7_init.h"
#include "xil_printf.h"
#include "xil_io.h"
#include "xparameters.h"
#include <time.h>
#include "mapping_table.h"
//#define XPAR_PS7_DDR_0_S_AXI_BASEADDR 0x00100000
//#define XPAR_PS7_DDR_0_S_AXI_HIGHADDR 0x1FFFFFFF


/* now I think we could map the address in this file
 *
 * from 32 bits virtual address
 * The physical address: 0x0010000 ----- 0x1FFFFFFF
 * We need to consider the address alignment 0x00100000, 0x00100004, 0x00100008 ... 0x1FFFFFFF
 *
 */
const short ailgnment = 4; // 4 bytes stored in 4 adjcent addresses.

MappingTable ht; //mapping table

//mapping funcyion

int main()
{
    init_platform();

    ps7_post_config();

    const int iter_time = 100;

    unsigned long array[iter_time];

    MappingTableInit(&ht); //initialize mapping table

    for(unsigned int i = 0; i < iter_time; i++){
    	array[i] = rand()%(0xFFFFFFFFFFFFFFFF); //test random
    	MappingTableInsert(&ht, array[i]);
    	//array[i] = i; //test sequence
    }
    xil_printf("\nWrite mode:\n");

    for(unsigned int i = 0; i < iter_time; i++){
       	unsigned int value = rand()%(0xFFFFFFFF);
       	//unsigned int addr_temp = XPAR_PS7_DDR_0_S_AXI_BASEADDR + 4*i;
       	//unsigned int addr_temp = XPAR_PS7_DDR_0_S_AXI_HIGHADDR - 3 - 4*i;


       	if(MappingTableLookup(&ht, array[i])){
       		printf("address: not exist");
       	}

       	unsigned int addr_temp;
        MappingTableRead(&ht, array[i], &addr_temp);

       	xil_printf("number: %d, ", i+1);
       	xil_printf("virtual address: 0x%010x, ", array[i]);
       	xil_printf("physical address: 0x%08x, ", addr_temp);
       	xil_printf("value: 0x%x\n", value);
       	Xil_Out32(addr_temp, value);
       	//xil_printf("value: 0x%x\n", Xil_In32(mapping(addr_temp)));

    }

    xil_printf("\nRead mode:\n");
    for(unsigned int i = 0; i < iter_time; i++){
       	//unsigned int addr_temp = XPAR_PS7_DDR_0_S_AXI_BASEADDR + 4*i;
       	//unsigned int addr_temp = XPAR_PS7_DDR_0_S_AXI_HIGHADDR - 3 - 4*i;
    	if(MappingTableLookup(&ht, array[i])){
    	       		printf("address: not exist");
    	}

    	unsigned int addr_temp;
    	MappingTableRead(&ht, array[i], &addr_temp);

    	xil_printf("number: %d, ", i+1);
    	xil_printf("virtual address: 0x%010x, ", array[i]);
    	xil_printf("physical address: 0x%08x, ", addr_temp);
    	xil_printf("value: 0x%x\n", Xil_In32(addr_temp));
    }

    xil_printf("\nsize: %d\n", sizeof(unsigned long));
    cleanup_platform();
    return 0;
}
