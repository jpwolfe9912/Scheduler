/** @file 		drv_serial.c
 *  @brief
 *  	This file enables reading of usart data
 *  	for use with printf.
 *
 *  Based on which device you are using (Nucleo or Autodrone PCB),
 *  enable the corresponding Include in the preprocessor
 *
 *  @author 	Jeremy Wolfe
 *  @date 		23 FEB 2022
 */

/* Includes */
#include "main.h"

/* Global Variables */

#ifdef USE_NUCLEO

/** @brief Initializes the low level uart registers in order to use printf
 *
 *  @return Void.
 */
void serialInit(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;  // enable the clock for port D
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN; // enable the clock for UART3

    GPIOD->AFR[1] |= (0x7 << (4 * 0U)); // set pin A2 as alternate function
    GPIOD->AFR[1] |= (0x7 << (4 * 1U)); // set pin A3 as alternate function

    GPIOD->MODER &= ~(GPIO_MODER_MODER8 | GPIO_MODER_MODER9);
    GPIOD->MODER |= GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1; // set PD8,9 as alternate function

    NVIC_SetPriority(USART3_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(USART3_IRQn);

    USART3->BRR = 0x1D5;                                       // set baud rate to 115200
    USART3->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE; // enable the receiver, transmitter, and USART
}

/** @brief Uses polling to write data to the transmit buffer.
 *
 *  @param ch The character to send.
 *  @return Void.
 */
void serialWrite(uint8_t ch)
{
    while (!(USART3->ISR & USART_ISR_TXE))
    {
    }                 // waits for TX buffer to become empty
    USART3->TDR = ch; // transfers the value of the data register into ch
}

#endif

#ifdef USE_AUTODRONE

/** @brief Initializes the low level uart registers in order to use printf.
 *
 *  @return Void.
 */
void serialInit(void)
{

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN |
                    RCC_AHB1ENR_GPIODEN; // enable the clock for port C, D
    RCC->APB1ENR |= RCC_APB1ENR_UART5EN; // enable the clock for UART5

    GPIOC->AFR[1] |= (0x8 << (4 * 4U)); // set pin A2 as alternate function
    GPIOD->AFR[0] |= (0x8 << (4 * 2U)); // set pin A3 as alternate function

    GPIOC->MODER &= ~(GPIO_MODER_MODER12);
    GPIOD->MODER &= ~(GPIO_MODER_MODER2);
    GPIOC->MODER |= GPIO_MODER_MODER12_1; // set PC12 as alternate function
    GPIOD->MODER |= GPIO_MODER_MODER2_1;  // set PD2 as alternate function

    NVIC_SetPriority(UART5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(UART5_IRQn);

    UART5->BRR = 0x1D5;                                       // set baud rate to 115200
    UART5->CR1 |= USART_CR1_RE | USART_CR1_TE | USART_CR1_UE; // enable the receiver, transmitter, and USART								// enable UART5 interrupts on the NVIC (nested vector interrupt controller)
}

/** @brief Uses polling to write data to the transmit buffer.
 *
 *  @param ch The character to send.
 *  @return Void.
 */
void serialWrite(uint8_t ch)
{
    while (!(UART5->ISR & USART_ISR_TXE))
    {
    }                // waits for TX buffer to become empty
    UART5->TDR = ch; // transfers the value of the data register into ch
}

/** @brief Uses interrupts to read uint8 data to the receive buffer.
 *
 *  @param uint8_t *num Pointer to the location you want to store the received number
 *  @return Void.
 */
void serialRead8(uint8_t *num)
{
    temp = '\0';
    UART5->CR1 |= USART_CR1_RXNEIE;
    while (!temp)
        ;

    *num = (uint8_t)temp - 48;
    UART5->CR1 &= ~USART_CR1_RXNEIE;
}

/** @brief Uses interrupts to read a string of PID values.
 *
 *  @param * Pointer to the location you want to store the received number
 *  @return Void.
 */
void serialReadPID(float *P, float *I, float *D)
{
    serialIndex = 0;
    memset(serialBuf, '\0', sizeof(serialBuf));

    endOfString = false;
    UART5->CR1 |= USART_CR1_RXNEIE;
    while (!endOfString)
        ;

    sscanf((char *)serialBuf, "%f, %f, %f", P, I, D);

    UART5->CR1 &= ~USART_CR1_RXNEIE;
    memset(serialBuf, '\0', sizeof(serialBuf));
}

/** @brief Waits for a character.
 *
 *  @param wait Character to wait for.
 *  @return bool True or False based on whether or not the character.
 *  received is the input to the function.
 */
bool serialWaitFor(char wait)
{
    temp = '\0';
    serialIndex = 0;

    UART5->CR1 |= USART_CR1_RXNEIE;
    while (!temp)
        ;
    if (temp == wait)
    {
        temp = '\0';
        UART5->CR1 &= ~USART_CR1_RXNEIE;
        return true;
    }
    else
    {
        temp = '\0';
        UART5->CR1 &= ~USART_CR1_RXNEIE;
        return false;
    }
}

/* Interrupt Handlers */

/**
 * @brief This function handles UART5 global interrupt.
 */
void UART5_IRQHandler(void)
{
    if ((UART5->ISR & USART_ISR_RXNE) && (UART5->CR1 & USART_CR1_RXNEIE))
    {
        temp = UART5->RDR;
        UART5->TDR = temp;
        if (temp == '\r')
        {
            endOfString = true;
            serialIndex = 0;
        }
        else
        {
            serialBuf[serialIndex] = temp;
            serialIndex++;
        }
    }
    if (UART5->ISR & USART_ISR_ORE)
        UART5->ICR |= USART_ICR_ORECF;
}

#endif

/*	This is required to use printf											*/
/*	This basically tells the compiler what to do when it encounters printf	*/
/*	I honestly can't fully explain what is going on but it works			*/
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//	#define GETCHAR_PROTOTYPE int __io_getchar (void)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//	#define GETCHAR_PROTOTYPE int fgetc(FILE * f)
#endif

PUTCHAR_PROTOTYPE
{
    serialWrite(ch);
    return ch;
}
