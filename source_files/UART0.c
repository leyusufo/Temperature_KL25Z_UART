#include "UART0.h"
#include "MKL25Z4.h"                    // Device header
#include "GPIO.h"
#include "PORT.h"

//receiver queue variables
#ifdef RXQ
uint8_t RX_queque[RXQ_SIZE];
uint32_t RXQ_head = 0;
uint32_t RXQ_tail = 0;
uint32_t RXQ_size = RXQ_SIZE;
#endif

//transmitter queue variables
#ifdef TXQ
uint8_t TX_queque[TXQ_SIZE];
uint32_t TXQ_head = 0;
uint32_t TXQ_tail = 0;
uint32_t TXQ_size = TXQ_SIZE;
#endif

//configure UART perpheral #0 with the given baud rate
void UART0_HWconfig(uint32_t baud_rate){
    uint16_t sbr = 0;
    
    //clock gating; enable clock to UART0 peripheral
    SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
    
	//choose UART0 clock source: MCGPLLCLK/2 clock =  48MHZ
    SIM_SOPT2 |= SIM_SOPT2_UART0SRC(UART0_CLK_MCGF_MCGP); 
	SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK; 
    
    /*
    Configure system options register for UART0
    */
    SIM_SOPT5 = SIM_SOPT5_UART0TXSRC(UART0_TX_PIN); //regular TX pin 
    SIM_SOPT5 &= ~SIM_SOPT5_UART0RXSRC_MASK;        //regular Rx pin
    SIM_SOPT5 &= ~SIM_SOPT5_UART0ODE_MASK;          //open drain disabled
    
    /*
    disable the tranmitter and receiver before hardware configuration 
    */
    UART0_TED(0);
    UART0_RED(0);
    
    /*
    Enable(1)/disable(0) LIN Break Detect Interrupt 
    UART0_BDH ->LBKDIE
    */
    UART0_BDH &= ~UART0_BDH_LBKDIE_MASK;  
    
    /*
    Enable(1)/disable(0) RX Input Active Edge Interrupt
    UART0_BDH ->RXEDGIE[6]
    */
    UART0_BDH &= ~UART0_BDH_RXEDGIE_MASK; 
    
    /*
    Select one stop bit (0) or 2 stop bit (1) number
    UART0_BDH ->SBNS[5]
    */
    UART0_BDH &= ~UART0_BDH_SBNS_MASK;
    
    /*
    Configure the baud rate
    UART0_BDH ->SBR[4-0]
    UART0_BDL ->SBR[7:0]
    */
    sbr = (uint16_t)(UART0_CLK/(baud_rate * UART0_OSR));
    UART0_BDH &= ~UART0_BDH_SBR_MASK;
    UART0_BDH |= UART0_BDH_SBR(sbr >> 8);
    UART0_BDL |= UART0_BDL_SBR(sbr);
	
    /*
    Select normal operation (0) or single wire mode (1) for RX and TX
    UART0_C1->LOOPS[7]: Loop Mode Select
    */
    UART0_C1 &= ~UART0_C1_LOOPS_MASK;
    
    /*
    Enable(0)/disable(1) UART in wait mode
    UART0_C1->DOZEEN[6]
    */
    UART0_C1 |= UART0_C1_DOZEEN_MASK;
    
    /*
    Select the receiver source based on the value of LOOPS 
    If LOOPS is set: choose 0 to not use RX pins or 1 to use TX pins as transmitter and receiver
    UART0_C1->RSRC[5]
    */
    UART0_C1 &= ~UART0_C1_RSRC_MASK;
    
    /*
    Choose whether to transmit 8-bit(0)/9-bit(1) data characters
    UART0_C1->M[4]
    */
    UART0_C1 &= ~UART0_C1_M_MASK;
    
    /*
    Choose your receiver wakeup method: 0 for idle-line and 1 for address-mark wakeup
    UART0_C1->WAKE[3]
    */
    UART0_C1 &= ~UART0_C1_WAKE_MASK;
    
    /*
    Choose your idle line type: 0 for counting after start bit or 1 for counting after stop bit
    UART0_C1->ILT[2]
    */
    UART0_C1 &= ~UART0_C1_ILT_MASK;
    
    /*
    Enable(1)/disable(0) parity generation and checking
    UART0_C1->PE[1]
    */
    UART0_C1 &= ~UART0_C1_PE_MASK;
    
    /*
    Choose even(0)/odd(1) parity if parity hardware is enabled
    UART0_C1->PT[0]
    */
    UART0_C1 &= ~UART0_C1_PT_MASK;
    
    /*
    Enable(1)/disable(0) hardware transmit interrupt
    UART0_C2->TIE[7]
    */
    UART0_C2 &= ~UART0_C2_TIE_MASK;
    
    /*
    Enable(1)/disable(0) hardware transmission complete interrupt
    UART0_C2->TCIE[6]
    */
    UART0_C2 &= ~UART0_C2_TCIE_MASK;
    
    /*
    Enable(1)/disable(0) hardware receive  interrupt
    UART0_C2->RIE[5]
    */
    UART0_C2 &= ~UART0_C2_RIE_MASK;
    
    /*
    Enable(1)/disable(0) hardware interrupt when idle flag is 1
    UART0_C2->ILIE[4]
    */
    UART0_C2 &= ~UART0_C2_ILIE_MASK;
    
    /*
    Select normal(0) or waiting for wakeup(1) for the receiver wakeup control
    UART0_C2->RWU[1]: Receiver Wakeup Control
    */
    UART0_C2 &= ~UART0_C2_RWU_MASK;
    
    /*
    Choose normal(0) or queque break characters (1) for transmission
    UART0_C2->SBK[0]: Send Break
    */
    UART0_C2 &= ~UART0_C2_SBK_MASK;
    
    /*
    Choose to send/receive data from LSB-MSB(0) or MSB-LSB(1)
    UART0_S2->MSBF[5]
    */
    UART0_S2 &= ~UART0_S2_MSBF_MASK; //LSB first
    
    /*
    Choose whether to invert the received data or not: 0=not inverted; 1=inverted
    UART0_S2->RXINV[4]
    */
    UART0_S2 &= ~UART0_S2_RXINV_MASK;
    
    /*
    Choose whether to allow the idle character that wakes up the receiver to set the IDLE bit: 0= does not set; 1=sets
    UART0_S2->RWUID[3]
    */
    UART0_S2 &= ~UART0_S2_RWUID_MASK;
    
    /*
    Select break character generation length based on the value of M, SBNS, SNBS, M10
    0=Break character is transmitted with length of 10 bit times or 11 or 12 or 13 
    1=Break character is transmitted with length of 13 bit times or 14 or or 16 
    UART0_S2->BRK13[2]
    */
    UART0_S2 &= ~UART0_S2_BRK13_MASK;
    
    /*
    select a longer break character detection length
    0 Break character is detected at length 10 bit times or 11  or 12 or 13 
    1 Break character is detected at length of 11 bit times or 12  or 14 or 15
    UART0_S2->LBKDE[1]
    */
    UART0_S2 &= ~UART0_S2_LBKDE_MASK;
    
    /*
    Enable(1)/disable(0) transmit data reverse
    UART0_C3->TXINV[4]
    */
    UART0_C3 &= ~UART0_C3_TXINV_MASK;
    
    /*
    Enable(1)/disable(0) overrun interrupt
    UART0_C3->ORIE[3]
    */
    UART0_C3 |= UART0_C3_ORIE_MASK;
    
    /*
    Enable(1)/disable(0) noise error interrupt
    UART0_C3->NEIE[2]
    */
    UART0_C3 |= UART0_C3_NEIE_MASK;
    
    /*
    Enable(1)/disable(0) framing error interrupt
    UART0_C3->FEIE[1]
    */
    UART0_C3 |= UART0_C3_FEIE_MASK;
    
    /*
    Enable(1)/disable(0) parity error interrupt
    UART0_C3->PEIE[0]
    */
    UART0_C3 |= UART0_C3_PEIE_MASK;
    
    /*
    Enable/disable match address mode 1
    UART0_C4->MAEN1[7]
    */
    UART0_C4 &= ~UART0_C4_MAEN1_MASK;
    
    /*
    Enable/disable match address mode 2
    UART0_C4->MAEN2[6]
    */
    UART0_C4 &= ~UART0_C4_MAEN2_MASK;
    
    /*
    Enable(1)/disable(0) 10 bit mode data
    UART0_C4->M10[5]
    */
    UART0_C4 &= ~UART0_C4_M10_MASK;
    
    /*
    Configure oversampling ratio for the receiver
    UART0_C4->OSR[4-0]
    */
    UART0_C4 |= UART0_C4_OSR(UART0_OSR-1);
    
    /*
    Enable(1)/disable(0) transmitter DMA request
    UART0_C5->TDMAE[7]
    */
    UART0_C5 &= ~UART0_C5_TDMAE_MASK;
    
    /*
    Enable(1)/disable(0) receiver full DMA request
    UART0_C5->RDMAE[5]
    */
    UART0_C5 &= ~UART0_C5_RDMAE_MASK;
    
    /*
    Enable(1)/disable(0) both edge sampling of the received data: 0=only rising edge; 1=both rising and falling
    UART0_C5->BOTHEDGE[1]
    */
    UART0_C5 &= ~UART0_C5_BOTHEDGE_MASK;
    
    /*
    Enable(0)/disable(1) resynchronization during received data word
    UART0_C5->RESYNCDIS[0]
    */
    UART0_C5 &= ~UART0_C5_RESYNCDIS_MASK;
    
    /*
    clear error flags
    */
    UART0_S1 |= UART0_S1_OR_MASK;
    UART0_S1 |= UART0_S1_NF_MASK;
    UART0_S1 |= UART0_S1_FE_MASK;
    UART0_S1 |= UART0_S1_PF_MASK;
}

/*
To enable/disable tranmitter for UART peripheral #0
enables tranmitter when ted=1; disables tranmitter when ted=0;
*/
void UART0_TED(uint16_t ted){
    if(ted)
        UART0_C2 |= UART0_C2_TE_MASK;
    else
        UART0_C2 &= ~UART0_C2_TE_MASK;
}

/*
To enable/disable tranmitter for interrupts UART peripheral #0
enables tranmitter Interrupts when tied=1; disables tranmitter interrupts when tied=0;
*/
void UART0_TIED(uint16_t tied){
    if(tied){
        UART0_C2 |= UART0_C2_TIE_MASK;
	}
    else{
        UART0_C2 &= ~UART0_C2_TIE_MASK;
	}
}
/*
To check whether the tranmitter interrupt for UART peripheral #0 is enabled 
*/
uint16_t UART0_TIE(){
    if(UART0_C2 & UART0_C2_TIE_MASK)
        return 1;
    return 0;
}

/*
To enable/disable receiver for UART peripheral #0
enables receiver when ted=1; disables receiver when ted=0;
*/
void UART0_RED(uint16_t red){
    if(red)
        UART0_C2 |= UART0_C2_RE_MASK;
    else
        UART0_C2 &= ~UART0_C2_RE_MASK;
}

/*
enqueques a character (data) into the TX queue
*/
void UART_TX_Enqueque(uint8_t data){
    if(UART_TXQ_Full())//returns if the queue is full
        return;
    else{
        TX_queque[TXQ_tail]= data;//enqueues character data at the tail of the queue
        
		//updates the queue tail
        TXQ_tail++;
        if(TXQ_tail >= TXQ_SIZE)
            TXQ_tail=0;
        
		//updates the size of the queue
        TXQ_size--;
    }
}

/*
dequeques a character (data) out of the TX queque
*/
uint8_t UART_TX_Dequeque(){
	uint8_t data;
   
	//gets a character data from the head of the queue
	data = TX_queque[TXQ_head];
   
	//updates the head of the queue
	TXQ_head++;
	if(TXQ_head >= TXQ_SIZE)
		TXQ_head=0;
        
	//updates the size of the queue
	TXQ_size++;
    
	//returns the character data
    return data;
}

/*
checks if the Transmitter queque is empty
returns 1 for empty
*/
uint16_t UART_TXQ_Empty(){
	//empty when the dynamic size of the queue equals the actual size of the queue
    if(TXQ_size == TXQ_SIZE)
        return 1;
    
    return 0;
}

/*
checks if the TX queque is full
returns 1 for full
*/
uint16_t UART_TXQ_Full(){
	//full when the dynamic size of the queue equals zero- there is no space left
    if(TXQ_size == 0)
        return 1;
    
    return 0;
}

/*
configures the Interrupt service routine for UART0
*/
void UART0_IRQconfig(){
    // Configure NVIC to enable IRQ handler for PIT
	NVIC_SetPriority(UART0_IRQn, 3); 
	NVIC_ClearPendingIRQ(UART0_IRQn); 
	NVIC_EnableIRQ(UART0_IRQn);
}

//Interrupt service routine definition
void UART0_IRQHandler(){
	uint8_t ch;
	
    /*
	check if interupt was caused by errors
	*/
	
	//checks for overrun error
    if(UART0_S1 & UART0_S1_OR_MASK){
        UART0_S1 |= UART0_S1_OR_MASK;
		ch = UART0_D;
    }
	
	//checks for noise error
    if(UART0_S1 & UART0_S1_NF_MASK){
        UART0_S1 |= UART0_S1_NF_MASK;
		ch = UART0_D;
    }
	
	//checks for framing error
    if(UART0_S1 & UART0_S1_FE_MASK){
        UART0_S1 |= UART0_S1_NF_MASK;
		ch = UART0_D;
    }
	
	//checks for parity error
    if(UART0_S1 & UART0_S1_PF_MASK){
        UART0_S1 |= UART0_S1_NF_MASK;
		ch = UART0_D;
    }
	
	//checks if interrupt was caused by receiver
    if(UART0_S1 & UART0_S1_RDRF_MASK){
        ch = UART0_D;
    }
    
    //checks if interrupt was caused by Transmitter
    if((UART0_C2 & UART0_C2_TIE_MASK) && (UART0_S1 & UART0_S1_TDRE_MASK)){
        if(!UART_TXQ_Empty()){//if the TX queque is not empty
            UART0_D = UART_TX_Dequeque(); //send a character
			
			//red LED on
			clear_pin(GPIO_B, RED_LED_SHIFT);
        }
        else{//TX queque is empty so disable TX interrupts
            UART0_TIED(0); //disable the Transmitter interrupt
			
			//red LED off
			set_pin(GPIO_B, RED_LED_SHIFT);
        }
    }
}
