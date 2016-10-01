#include "msp430g2553.h"
 
#define TXLED BIT0
#define RXLED BIT6
#define TXD BIT2
#define RXD BIT1
 
const char string[] = { "Simple message\n" };
unsigned int i; 
 
int main(void)
{
   WDTCTL = WDTPW + WDTHOLD; 
   DCOCTL = 0; 
   BCSCTL1 = CALBC1_1MHZ;
   DCOCTL = CALDCO_1MHZ;
   P2DIR |= 0xFF; 
   P2OUT &= 0x00; 
   P1SEL |= RXD + TXD ; 
   P1SEL2 |= RXD + TXD ; 
   P1DIR |= RXLED + TXLED;
   P1OUT &= 0x00;

   // SMCLK
   UCA0CTL1 |= UCSSEL_2; 
   UCA0BR0 = 0x08; 
   UCA0BR1 = 0x00; 
   
   UCA0MCTL = UCBRS2 + UCBRS0; 
   UCA0CTL1 &= ~UCSWRST; 
   UC0IE |= UCA0RXIE; 
   __bis_SR_register(CPUOFF + GIE); 
   for(;;){}
}
 
#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void){
   P1OUT |= TXLED; 
     UCA0TXBUF = string[i++]; 
    if (i == sizeof string - 1) 
       UC0IE &= ~UCA0TXIE;
    P1OUT &= ~TXLED; 
} 
  
#pragma vector=USCIAB0RX_VECTOR 
__interrupt void USCI0RX_ISR(void) { 
   P1OUT |= RXLED; 
    if (UCA0RXBUF == 'q') 
    { 
		i = 0; 
		UC0IE |= UCA0TXIE; 
		UCA0TXBUF = string[i++]; 
    } 
    P1OUT &= ~RXLED;
}