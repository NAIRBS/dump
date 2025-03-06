/*
 * File:   HC05_Test.c
 * Author: Zhang Qi
 * This program is used to test sending/receiving data via bluetooth module HC-05
 * Hardware: 
 * PIC18F4550, HC-05
 * Connection: RC7(Rx) connect to TXD pin of HC-05, RC6(Tx) connect to RXD pin of HC-05 
 * Created on July 18, 2017, 4:24 PM
 */


#include <xc.h>
#include "delays.h"
void main(void) 
{
    unsigned int t,r;
    unsigned char data_out[] = "Bluetooth send out data\n";
    unsigned char data_in[100];
    t = 0;
    r = 0;
    /* Configure USART module of PIC18*/
    // I/O pin 
    TRISCbits.TRISC7 = 1;       // USART RX, input
    TRISCbits.TRISC6 = 0;       // USART TX, output
    TRISDbits.TRISD0 = 0;       // LED for Tx
    TRISDbits.TRISD1 = 0;       // LED for Rx
    TXSTA = 0x20;
   /* TXSTAbits.TX9 = 0;          // 8-bit transmission
    TXSTAbits.TXEN = 1;         // Enable Transmit
    TXSTAbits.SYNC = 0;         // Asynchronous mode
    TXSTAbits.BRGH = 0;         // Low speed 
    */
    SPBRG = 77;                 // Set baudrate as 9600
    
    RCSTA = 0x90;         // Enable serial port, enable Rx
    
    PIR1bits.TXIF = 0;          // Clear flag
    PIR1bits.RCIF = 0;
    PORTD = 0;
    while(1)
    {
        if(PIR1bits.TXIF == 1)
        {
            TXREG = data_out[t];
            t++;
            PORTDbits.RD0 = 1;
            delay_ms(500);
            PORTDbits.RD0 = 0;
            delay_ms(500);
        }
        if(PIR1bits.RCIF == 1)
        {
            data_in[r] = RCREG;
            r++;
            PORTDbits.RD1 = 1;
            delay_ms(500);
            PORTDbits.RD1 = 0;
            delay_ms(500);

        }
    }
    return;
}
