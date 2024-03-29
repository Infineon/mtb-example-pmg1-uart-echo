/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PMG1 Uart Echo Example
*              for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2021-2023, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/
#include "cy_pdl.h"
#include "cybsp.h"
#include <stdio.h>
#include <inttypes.h>

/*******************************************************************************
* Macros
*******************************************************************************/
#define CY_ASSERT_FAILED          (0u)

/* Debug print macro to enable UART print */
#define DEBUG_PRINT               (0u)

/*******************************************************************************
* Global Variables
*******************************************************************************/
#if DEBUG_PRINT
/* Variable used for tracking the print status */
volatile bool ENTER_LOOP = true;
#endif

/* Structure for UART Context */
cy_stc_scb_uart_context_t CYBSP_UART_context;

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
*  System entrance point. This function performs
*  - initial setup of device
*  - configure the SCB block as UART interface
*  - echoes the characters entered on the keyboard and displays on UART terminal
*
* Parameters:
*  none
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;
    uint32_t rdata;
    cy_en_scb_uart_status_t initstatus;

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(CY_ASSERT_FAILED);
    }

    /* Configure and enable the UART peripheral */
    initstatus = Cy_SCB_UART_Init(CYBSP_UART_HW, &CYBSP_UART_config, &CYBSP_UART_context);

    /* Initialization failed. Halt the program */
    if(initstatus!=CY_SCB_UART_SUCCESS)
    {
        CY_ASSERT(CY_ASSERT_FAILED);
    }

    Cy_SCB_UART_Enable(CYBSP_UART_HW);

    /* Enable global interrupts */
    __enable_irq();

     /* Sequence to clear screen */
     Cy_SCB_UART_PutString(CYBSP_UART_HW, "\x1b[2J\x1b[;H");
     /* Print "Uart Echo" */
     Cy_SCB_UART_PutString(CYBSP_UART_HW, "**********************************\r\n");
     Cy_SCB_UART_PutString(CYBSP_UART_HW, "PMG1 MCU UART transmit and receive\r\n");
     Cy_SCB_UART_PutString(CYBSP_UART_HW, "**********************************\r\n\n");
     Cy_SCB_UART_PutString(CYBSP_UART_HW, ">>Transmit a character to see the receive echo on the screen \r\n\n");

    for(;;)
    {
        /* Check if there is a character received from the terminal; */
        if (0UL != Cy_SCB_UART_GetNumInRxFifo(CYBSP_UART_HW))
        {
            /* Re-transmit whatever the user types */
            rdata=Cy_SCB_UART_Get(CYBSP_UART_HW);
            while(Cy_SCB_UART_Put(CYBSP_UART_HW, rdata) == 0)
            {

            }
        }

#if DEBUG_PRINT
        if (ENTER_LOOP)
        {
            Cy_SCB_UART_PutString(CYBSP_UART_HW, "Entered for loop \r\n");
            ENTER_LOOP = false;
        }
#endif
    }
}

/* [] END OF FILE */

