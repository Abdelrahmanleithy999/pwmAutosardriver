/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  PWM.c
 *        \brief  PWM Driver 
 *
 *      \details  Source file for PWM Driver
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "PWM.h"
#include "PWM_Regs.h"
#include "PWM_Cfg.h"
#include "tm4c123gh6pm.h"
/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
STATIC const Pwm_ConfigChannel* PWM_Configuration = NULL_PTR;
//STATIC uint8 PWM_Status = PWM_NOT_INITIALIZED ;  
/****************************** FUNCTIONS ******************************************************/  
static uint8 pin ; 
//static uint32 port ; 
static uint8 port_Num ;  
void Pwm_Init(Pwm_ConfigType* ConfigPtr ) { 
 // PWM_Status = PWM_INITIALIZED;  
 
  PWM_Configuration = ConfigPtr->PWM_Channels;
  for (uint8 i = 0; i < PWM_CONFIGURED_CHANNLES; i++)
  {    
    if (PWM_Configuration[i].DIV == sys_clk_no_div ) {
   
   CLEAR_BIT(SYSCTL_RCC_R ,  DIV_BIT) ;   // DIVISOR DIABLE 
   } else {
    SET_BIT(SYSCTL_RCC_R ,  DIV_BIT) ;    // DIVISOR ENABLE  
    switch (PWM_Configuration[i].DIV) {
    case sys_clk_div2  :                 // 000
      CLEAR_BIT(SYSCTL_RCC_R  , DIV_LEAST ) ;   
      CLEAR_BIT(SYSCTL_RCC_R  , DIV_MED ) ;  
      CLEAR_BIT(SYSCTL_RCC_R  , DIV_HIGHEST ) ;  
    break;  
    case sys_clk_div4 :   //001
      SET_BIT(SYSCTL_RCC_R  , DIV_LEAST ) ;   
      CLEAR_BIT(SYSCTL_RCC_R  , DIV_MED ) ;  
      CLEAR_BIT(SYSCTL_RCC_R  , DIV_HIGHEST ) ;  
    break; 
    case sys_clk_div8:    //010
      CLEAR_BIT(SYSCTL_RCC_R  , DIV_LEAST ) ;   
      SET_BIT(SYSCTL_RCC_R  , DIV_MED ) ;  
      CLEAR_BIT(SYSCTL_RCC_R  , DIV_HIGHEST ) ;  
    break; 
   case sys_clk_div16 :  //100
      CLEAR_BIT(SYSCTL_RCC_R  , DIV_LEAST ) ;   
      CLEAR_BIT(SYSCTL_RCC_R  , DIV_MED ) ;  
      SET_BIT(SYSCTL_RCC_R  , DIV_HIGHEST ) ;  
    break;  
  case  sys_clk_div32 :  //101
      SET_BIT(SYSCTL_RCC_R  , DIV_LEAST ) ;   
      CLEAR_BIT(SYSCTL_RCC_R  , DIV_MED ) ;  
      SET_BIT(SYSCTL_RCC_R  , DIV_HIGHEST ) ;  
    break; 
  case sys_clk_div64 : //111
      SET_BIT(SYSCTL_RCC_R  , DIV_LEAST ) ;   
      SET_BIT(SYSCTL_RCC_R  , DIV_MED ) ;  
      SET_BIT(SYSCTL_RCC_R  , DIV_HIGHEST ) ;  
    break;     
    }
   }
  if(PWM_Configuration[i].Module == PWM_MODULE1 ) {
    SET_BIT(SYSCTL_RCGCPWM_R ,PWM_MODULE1) ; 
    while(!(SYSCTL_PRPWM_R & (1<<PWM_MODULE1)));  //Wait till being enabled   Module 1
} 
  else {
  SET_BIT(SYSCTL_RCGCPWM_R ,PWM_MODULE0) ;   
   while(!(SYSCTL_PRPWM_R & (1<<PWM_MODULE0)));  //Wait till being enabled   Module 0
  } 
  /*********************************** looping the channels *********************************************/ 
  switch(PWM_Configuration[i].channel) {   
    /********************************** FIRST CHANNEL *****************************************************/ 
  case PWM0_PB6  :   // FIRST CHANNEL  MODULE 0 GEN 0 
     //port = GPIO_PORTB_BASE_ADDRESS ; 
     pin  = pin6;
     port_Num = 1;
     SYSCTL_RCGCGPIO_R = SYSCTL_RCGC2_GPIOB ; //ENABLE GPIO CLOCK 
  while(!(SYSCTL_PRGPIO_R & (1<<port_Num))){} ; //WAIT THE CLCOK 
      Memory(GPIO_PORTB_BASE_ADDRESS , PORT_LOCK_REG_OFFSET) = GPIO_LOCK_KEY ;  // OPEN GATE 
     Memory(GPIO_PORTB_BASE_ADDRESS,PORT_COMMIT_REG_OFFSET)  |= (1<< pin) ;   // SET COMMIT REGISTER 
     Memory(GPIO_PORTB_BASE_ADDRESS, PORT_DIGITAL_ENABLE_REG_OFFSET) |= (1<<pin); //ENABLE DIGITAL OUTPUT 
     Memory(GPIO_PORTB_BASE_ADDRESS,PORT_DIR_REG_OFFSET) |= (1<<pin);  // ENALBE OUTPUT DIRECTION 
     Memory(GPIO_PORTB_BASE_ADDRESS,PORT_ANALOG_MODE_SEL_REG_OFFSET) &= ~(1<<pin); // DIABLE ANALOGE 
     Memory(GPIO_PORTB_BASE_ADDRESS,PORT_ALT_FUNC_REG_OFFSET ) |= (1<<pin) ; //ENABLE ALTERNATIVE FUNCTION 
     Memory(GPIO_PORTB_BASE_ADDRESS,PORT_CTL_REG_OFFSET)  |= (4<<(pin*4)); // ENABLE PWM MODE 
      PWM0_0_CTL_R  &= ~(1<<0); /* Disable Generator 0 counter */  
     if(PWM_Configuration[i].mode == DOWN_MODE)  {     // configure down mode 
     PWM0_0_CTL_R  &= ~(1<<1); /* select down count mode of counter 0*/ 
     PWM0_0_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
    PWM0_0_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK))  ;    /* set load value  */
    PWM0_0_CMPA_R = (PWM_Configuration[i].DutyCyle/100)*(PWM0_0_LOAD_R ) ;   /* set duty cyle to to minumum value*/
    PWM0_0_CTL_R = 1; /* Enable Generator 0 counter */
    PWM0_ENABLE_R = 0x01; /* Enable PWM0 channel 0 output */   
     } 
     else {
      PWM0_0_CTL_R  |=(1<<1); /* select down count mode of counter 3*/  
      PWM0_0_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
       PWM0_0_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK));    /* set load value  */
       PWM0_0_CMPA_R = ((100-PWM_Configuration[i].DutyCyle)/100)*(PWM0_0_LOAD_R ) ;   /* set duty cyle to to minumum value*/
       PWM0_0_CTL_R = 1; /* Enable Generator 0 counter */
       PWM0_ENABLE_R = 0x01; /* Enable PWM0 channel 0 output */   
     }  
    break;   
    
 /****************************** SECOND CHANNEL ************************************************/    
  case PWM0_PB7 :   // SECOND CHANNEL MODULE 0 GEN 0 
    //port = GPIO_PORTB_BASE_ADDRESS ;  
     pin  = pin7;
     port_Num = 1;   
 SYSCTL_RCGCGPIO_R = SYSCTL_RCGC2_GPIOB ; //ENABLE GPIO CLOCK 
  while(!(SYSCTL_PRGPIO_R & (1<<port_Num))){} ; //WAIT THE CLCOK 
      Memory(GPIO_PORTB_BASE_ADDRESS , PORT_LOCK_REG_OFFSET) = GPIO_LOCK_KEY ;  // OPEN GATE 
     Memory(GPIO_PORTB_BASE_ADDRESS,PORT_COMMIT_REG_OFFSET)  |= (1<< pin) ;   // SET COMMIT REGISTER 
     Memory(GPIO_PORTB_BASE_ADDRESS, PORT_DIGITAL_ENABLE_REG_OFFSET) |= (1<<pin); //ENABLE DIGITAL OUTPUT 
     Memory(GPIO_PORTB_BASE_ADDRESS,PORT_DIR_REG_OFFSET) |= (1<<pin);  // ENALBE OUTPUT DIRECTION 
     Memory(GPIO_PORTB_BASE_ADDRESS,PORT_ANALOG_MODE_SEL_REG_OFFSET) &= ~(1<<pin); // DIABLE ANALOGE 
     Memory(GPIO_PORTB_BASE_ADDRESS,PORT_ALT_FUNC_REG_OFFSET ) |= (1<<pin) ; //ENABLE ALTERNATIVE FUNCTION 
     Memory(GPIO_PORTB_BASE_ADDRESS,PORT_CTL_REG_OFFSET)  |= (4<<(pin*4)); // ENABLE PWM MODE 
     /*PWMCONFIG*/ 
     PWM0_0_CTL_R  &= ~(1<<0); /* Disable Generator 0 counter */  
     if(PWM_Configuration[i].mode == DOWN_MODE)  {     // configure down mode 
     PWM0_0_CTL_R  &= ~(1<<1); /* select down count mode of counter 0*/ 
     PWM0_0_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
    PWM0_0_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK))  ;    /* set load value  */
    PWM0_0_CMPA_R = (PWM_Configuration[i].DutyCyle/100)*(PWM0_0_LOAD_R ) ;   /* set duty cyle to to minumum value*/
    PWM0_0_CTL_R = 1; /* Enable Generator 0 counter */
    PWM0_ENABLE_R = 0x01; /* Enable PWM0 channel 0 output */   
     } 
     else {
      PWM0_0_CTL_R  |=(1<<1); /* select down count mode of counter 3*/  
      PWM0_0_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
       PWM0_0_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK));    /* set load value  */
       PWM0_0_CMPA_R = ((100-PWM_Configuration[i].DutyCyle)/100)*(PWM0_0_LOAD_R ) ;   /* set duty cyle to to minumum value*/
       PWM0_0_CTL_R = 1; /* Enable Generator 0 counter */
       PWM0_ENABLE_R = 0x01; /* Enable PWM0 channel 0 output */   
     
     }
    break;  
  /******************************************* THIRD CHANNEL ***********************************/   
  case PWM0_PB4:    // THIRD CHANNEL MODULE 0 GEN 1   
    // port = GPIO_PORTB_BASE_ADDRESS ; 
     pin  = pin4;
     port_Num = 1;
   SYSCTL_RCGCGPIO_R = SYSCTL_RCGC2_GPIOB ; //ENABLE GPIO CLOCK 
  while(!(SYSCTL_PRGPIO_R & (1<<port_Num))){} ; //WAIT THE CLCOK 
      Memory(GPIO_PORTB_BASE_ADDRESS , PORT_LOCK_REG_OFFSET) = GPIO_LOCK_KEY ;  // OPEN GATE 
     Memory(GPIO_PORTB_BASE_ADDRESS,PORT_COMMIT_REG_OFFSET)  |= (1<< pin) ;   // SET COMMIT REGISTER 
     Memory(GPIO_PORTB_BASE_ADDRESS, PORT_DIGITAL_ENABLE_REG_OFFSET) |= (1<<pin); //ENABLE DIGITAL OUTPUT 
     Memory(GPIO_PORTB_BASE_ADDRESS,PORT_DIR_REG_OFFSET) |= (1<<pin);  // ENALBE OUTPUT DIRECTION 
     Memory(GPIO_PORTB_BASE_ADDRESS,PORT_ANALOG_MODE_SEL_REG_OFFSET) &= ~(1<<pin); // DIABLE ANALOGE 
     Memory(GPIO_PORTB_BASE_ADDRESS,PORT_ALT_FUNC_REG_OFFSET ) |= (1<<pin) ; //ENABLE ALTERNATIVE FUNCTION 
     Memory(GPIO_PORTB_BASE_ADDRESS,PORT_CTL_REG_OFFSET)  |= (4<<(pin*4)); // ENABLE PWM MODE 
       /*PWMCONFIG*/ 
     PWM0_1_CTL_R  &= ~(1<<0); /* Disable Generator 1 counter */  
     if(PWM_Configuration[i].mode == DOWN_MODE)  {     // configure down mode 
     PWM0_1_CTL_R  &= ~(1<<1); /* select down count mode of counter 1*/ 
     PWM0_1_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
    PWM0_1_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK))  ;    /* set load value  */
    PWM0_1_CMPA_R = (PWM_Configuration[i].DutyCyle/100)*(PWM0_1_LOAD_R ) ;   /* set duty cyle to to minumum value*/
    PWM0_1_CTL_R = 1; /* Enable Generator 1 counter */
    PWM0_ENABLE_R = 0x01; /* Enable PWM0 channel 1 output */   
     } 
     else {
      PWM0_1_CTL_R  |=(1<<1); /* select down count mode of counter 3*/  
      PWM0_1_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
       PWM0_1_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK));    /* set load value  */
       PWM0_1_CMPA_R = ((100-PWM_Configuration[i].DutyCyle)/100)*(PWM0_1_LOAD_R ) ;   /* set duty cyle to to minumum value*/
       PWM0_1_CTL_R = 1; /* Enable Generator 1 counter */
       PWM0_ENABLE_R = 0x01; /* Enable PWM0 channel 0 output */   
     
     }
    break;  
 /***************************************** FOURTH CHANNEL *************************************************/    
  case PWM0_PB5 :   // FOURTH CHANNEL MODULE 0 GEN 1 
    // port = GPIO_PORTB_BASE_ADDRESS ; 
     pin  = pin5;
     port_Num = 1;
 SYSCTL_RCGCGPIO_R = SYSCTL_RCGC2_GPIOB ; //ENABLE GPIO CLOCK 
  while(!(SYSCTL_PRGPIO_R & (1<<port_Num))){} ; //WAIT THE CLCOK 
      Memory(GPIO_PORTB_BASE_ADDRESS , PORT_LOCK_REG_OFFSET) = GPIO_LOCK_KEY ;  // OPEN GATE 
     Memory(GPIO_PORTB_BASE_ADDRESS,PORT_COMMIT_REG_OFFSET)  |= (1<< pin) ;   // SET COMMIT REGISTER 
     Memory(GPIO_PORTB_BASE_ADDRESS, PORT_DIGITAL_ENABLE_REG_OFFSET) |= (1<<pin); //ENABLE DIGITAL OUTPUT 
     Memory(GPIO_PORTB_BASE_ADDRESS,PORT_DIR_REG_OFFSET) |= (1<<pin);  // ENALBE OUTPUT DIRECTION 
     Memory(GPIO_PORTB_BASE_ADDRESS,PORT_ANALOG_MODE_SEL_REG_OFFSET) &= ~(1<<pin); // DIABLE ANALOGE 
     Memory(GPIO_PORTB_BASE_ADDRESS,PORT_ALT_FUNC_REG_OFFSET ) |= (1<<pin) ; //ENABLE ALTERNATIVE FUNCTION 
     Memory(GPIO_PORTB_BASE_ADDRESS,PORT_CTL_REG_OFFSET)  |= (4<<(pin*4)); // ENABLE PWM MODE 
      /*PWMCONFIG*/ 
     PWM0_1_CTL_R  &= ~(1<<0); /* Disable Generator 1 counter */  
     if(PWM_Configuration[i].mode == DOWN_MODE)  {     // configure down mode 
     PWM0_1_CTL_R  &= ~(1<<1); /* select down count mode of counter 1*/ 
     PWM0_1_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
    PWM0_1_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK))  ;    /* set load value  */
    PWM0_1_CMPA_R = (PWM_Configuration[i].DutyCyle/100)*(PWM0_1_LOAD_R ) ;   /* set duty cyle to to minumum value*/
    PWM0_1_CTL_R = 1; /* Enable Generator 1 counter */
    PWM0_ENABLE_R = 0x01; /* Enable PWM0 channel 1 output */   
     } 
     else {
      PWM0_1_CTL_R  |=(1<<1); /* select down count mode of counter 3*/  
      PWM0_1_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
       PWM0_1_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK));    /* set load value  */
       PWM0_1_CMPA_R = ((100-PWM_Configuration[i].DutyCyle)/100)*(PWM0_1_LOAD_R ) ;   /* set duty cyle to to minumum value*/
       PWM0_1_CTL_R = 1; /* Enable Generator 1 counter */
       PWM0_ENABLE_R = 0x01; /* Enable PWM0 channel 0 output */   
     
     }
    break;    
    /************************************************* FIFTH CHANNEL *****************************************/ 
  case PWM0_PE4 :     // MODULE 0  GEN 2 
    // port = GPIO_PORTE_BASE_ADDRESS ; 
     pin  = pin4;
     port_Num = 4;
 SYSCTL_RCGCGPIO_R = SYSCTL_RCGC2_GPIOE;
while(!(SYSCTL_PRGPIO_R & (1<<port_Num))){} ;   
      Memory(GPIO_PORTE_BASE_ADDRESS , PORT_LOCK_REG_OFFSET) = GPIO_LOCK_KEY ; 
     Memory(GPIO_PORTE_BASE_ADDRESS,PORT_COMMIT_REG_OFFSET)  |= (1<< pin) ;
     Memory(GPIO_PORTE_BASE_ADDRESS,PORT_DIGITAL_ENABLE_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTE_BASE_ADDRESS,PORT_DIR_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTE_BASE_ADDRESS,PORT_ANALOG_MODE_SEL_REG_OFFSET) &= ~(1<<pin); 
     Memory(GPIO_PORTE_BASE_ADDRESS,PORT_ALT_FUNC_REG_OFFSET ) |= (1<<pin) ;
     Memory(GPIO_PORTE_BASE_ADDRESS,PORT_CTL_REG_OFFSET)  |= (4<<(pin*4));
     /*PWMCONFIG*/ 
     PWM0_2_CTL_R  &= ~(1<<0); /* Disable Generator 1 counter */  
     if(PWM_Configuration[i].mode == DOWN_MODE)  {     // configure down mode 
     PWM0_2_CTL_R  &= ~(1<<1); /* select down count mode of counter 1*/ 
     PWM0_2_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
    PWM0_2_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK))  ;    /* set load value  */
    PWM0_2_CMPA_R = (PWM_Configuration[i].DutyCyle/100)*(PWM0_2_LOAD_R ) ;   /* set duty cyle to to minumum value*/
    PWM0_2_CTL_R = 1; /* Enable Generator 1 counter */
    PWM0_ENABLE_R = 0x01; /* Enable PWM0 channel 1 output */   
     } 
     else {
      PWM0_2_CTL_R  |=(1<<1); /* select down count mode of counter 3*/  
      PWM0_2_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
       PWM0_2_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK));    /* set load value  */
       PWM0_2_CMPA_R = ((100-PWM_Configuration[i].DutyCyle)/100)*(PWM0_2_LOAD_R ) ;   /* set duty cyle to to minumum value*/
       PWM0_2_CTL_R = 1; /* Enable Generator 1 counter */
       PWM0_ENABLE_R = 0x01; /* Enable PWM0 channel 0 output */   
     
     }
    break;   
 /*********************************************************SIXTH CHANNEL *********************************************/    
  case PWM0_PE5 :     // MODULE 0 GEN 2 
  //   port = GPIO_PORTE_BASE_ADDRESS ; 
     pin  = pin5;
     port_Num = 4;
  SYSCTL_RCGCGPIO_R = SYSCTL_RCGC2_GPIOE;
while(!(SYSCTL_PRGPIO_R & (1<<port_Num))){} ; 
      Memory(GPIO_PORTE_BASE_ADDRESS , PORT_LOCK_REG_OFFSET) = GPIO_LOCK_KEY ; 
     Memory(GPIO_PORTE_BASE_ADDRESS,PORT_COMMIT_REG_OFFSET)  |= (1<< pin) ;
     Memory(GPIO_PORTE_BASE_ADDRESS,PORT_DIGITAL_ENABLE_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTE_BASE_ADDRESS,PORT_DIR_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTE_BASE_ADDRESS,PORT_ANALOG_MODE_SEL_REG_OFFSET) &= ~(1<<pin); 
     Memory(GPIO_PORTE_BASE_ADDRESS,PORT_ALT_FUNC_REG_OFFSET ) |= (1<<pin) ;
     Memory(GPIO_PORTE_BASE_ADDRESS,PORT_CTL_REG_OFFSET)  |= (4<<(pin*4));
     /*PWMCONFIG*/ 
     PWM0_2_CTL_R  &= ~(1<<0); /* Disable Generator 1 counter */  
     if(PWM_Configuration[i].mode == DOWN_MODE)  {     // configure down mode 
     PWM0_2_CTL_R  &= ~(1<<1); /* select down count mode of counter 1*/ 
     PWM0_2_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
    PWM0_2_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK))  ;    /* set load value  */
    PWM0_2_CMPA_R = (PWM_Configuration[i].DutyCyle/100)*(PWM0_2_LOAD_R ) ;   /* set duty cyle to to minumum value*/
    PWM0_2_CTL_R = 1; /* Enable Generator 1 counter */
    PWM0_ENABLE_R = 0x01; /* Enable PWM0 channel 1 output */   
     } 
     else {
      PWM0_2_CTL_R  |=(1<<1); /* select down count mode of counter 3*/  
      PWM0_2_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
       PWM0_2_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK));    /* set load value  */
       PWM0_2_CMPA_R = ((100-PWM_Configuration[i].DutyCyle)/100)*(PWM0_2_LOAD_R ) ;   /* set duty cyle to to minumum value*/
       PWM0_2_CTL_R = 1; /* Enable Generator 1 counter */
       PWM0_ENABLE_R = 0x01; /* Enable PWM0 channel 0 output */   
     
     }
    break;  
 /****************************************** SEVENTH CHANNEL ***********************************************/     
  case PWM0_PC4 :   // MODULE 0 GEN 3 
     //port = GPIO_PORTC_BASE_ADDRESS ; 
     pin  = pin4;
     port_Num = 2;
 SYSCTL_RCGCGPIO_R = SYSCTL_RCGC2_GPIOC;
while(!(SYSCTL_PRGPIO_R & (1<<port_Num))){} ; 
      Memory(GPIO_PORTC_BASE_ADDRESS , PORT_LOCK_REG_OFFSET) = GPIO_LOCK_KEY ; 
     Memory(GPIO_PORTC_BASE_ADDRESS,PORT_COMMIT_REG_OFFSET)  |= (1<< pin) ;
     Memory(GPIO_PORTC_BASE_ADDRESS,PORT_DIGITAL_ENABLE_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTC_BASE_ADDRESS,PORT_DIR_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTC_BASE_ADDRESS,PORT_ANALOG_MODE_SEL_REG_OFFSET) &= ~(1<<pin); 
     Memory(GPIO_PORTC_BASE_ADDRESS,PORT_ALT_FUNC_REG_OFFSET ) |= (1<<pin) ;
     Memory(GPIO_PORTC_BASE_ADDRESS,PORT_CTL_REG_OFFSET)  |= (4<<(pin*4));
      /*PWMCONFIG*/ 
     PWM0_3_CTL_R  &= ~(1<<0); /* Disable Generator 3 counter */  
     if(PWM_Configuration[i].mode == DOWN_MODE)  {     // configure down mode 
     PWM0_3_CTL_R  &= ~(1<<1); /* select down count mode of counter 3*/ 
     PWM0_3_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
    PWM0_3_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK))  ;    /* set load value  */
    PWM0_3_CMPA_R = (PWM_Configuration[i].DutyCyle/100)*(PWM0_3_LOAD_R ) ;   /* set duty cyle to to minumum value*/
    PWM0_3_CTL_R = 1; /* Enable Generator 1 counter */
    PWM0_ENABLE_R = 0x01; /* Enable PWM0 channel 3 output */   
     } 
     else {
      PWM0_3_CTL_R  |=(1<<1); /* select down count mode of counter 3*/  
      PWM0_3_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
       PWM0_3_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK));    /* set load value  */
       PWM0_3_CMPA_R = ((100-PWM_Configuration[i].DutyCyle)/100)*(PWM0_3_LOAD_R ) ;   /* set duty cyle to to minumum value*/
       PWM0_3_CTL_R = 1; /* Enable Generator 3 counter */
       PWM0_ENABLE_R = 0x01; /* Enable PWM0 channel 0 output */   
     
     }
    break;  
    
/************************************************ EIGHTH CHANNEL *************************************************/     
  case PWM0_PC5 :  // MODULE 0 GEN 3  
     //port = GPIO_PORTC_BASE_ADDRESS ; 
     pin  = pin5;  
     port_Num = 2;
 SYSCTL_RCGCGPIO_R = SYSCTL_RCGC2_GPIOC;    
while(!(SYSCTL_PRGPIO_R & (1<<port_Num))){} ; 
      Memory(GPIO_PORTC_BASE_ADDRESS , PORT_LOCK_REG_OFFSET) = GPIO_LOCK_KEY ; 
     Memory(GPIO_PORTC_BASE_ADDRESS,PORT_COMMIT_REG_OFFSET)  |= (1<< pin) ;
     Memory(GPIO_PORTC_BASE_ADDRESS,PORT_DIGITAL_ENABLE_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTC_BASE_ADDRESS,PORT_DIR_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTC_BASE_ADDRESS,PORT_ANALOG_MODE_SEL_REG_OFFSET) &= ~(1<<pin); 
     Memory(GPIO_PORTC_BASE_ADDRESS,PORT_ALT_FUNC_REG_OFFSET ) |= (1<<pin) ;
     Memory(GPIO_PORTC_BASE_ADDRESS,PORT_CTL_REG_OFFSET)  |= (4<<(pin*4));
     /*PWMCONFIG*/ 
     PWM0_3_CTL_R  &= ~(1<<0); /* Disable Generator 1 counter */  
     if(PWM_Configuration[i].mode == DOWN_MODE)  {     // configure down mode 
     PWM0_3_CTL_R  &= ~(1<<1); /* select down count mode of counter 1*/ 
     PWM0_3_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
    PWM0_3_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK))  ;    /* set load value  */
    PWM0_3_CMPA_R = (PWM_Configuration[i].DutyCyle/100)*(PWM0_3_LOAD_R ) ;   /* set duty cyle to to minumum value*/
    PWM0_3_CTL_R = 1; /* Enable Generator 1 counter */
    PWM0_ENABLE_R = 0x01; /* Enable PWM0 channel 1 output */   
     } 
     else {
      PWM0_3_CTL_R  |=(1<<1); /* select down count mode of counter 3*/  
      PWM0_3_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
       PWM0_3_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK));    /* set load value  */
       PWM0_3_CMPA_R = ((100-PWM_Configuration[i].DutyCyle)/100)*(PWM0_3_LOAD_R ) ;   /* set duty cyle to to minumum value*/
       PWM0_3_CTL_R = 1; /* Enable Generator 3 counter */
       PWM0_ENABLE_R = 0x01; /* Enable PWM0 channel 3 output */   
     
     }
    break; 
 /************************************************  FIRST CHANNEL Module 1 ************************************/     
  case PWM1_PD0 : 
     //port = GPIO_PORTD_BASE_ADDRESS ;    // MODULE 1 GEN 0  
     pin  = pin0;
     port_Num = 3;
 SYSCTL_RCGCGPIO_R =  SYSCTL_RCGC2_GPIOD;
while(!(SYSCTL_PRGPIO_R & (1<<port_Num))){} ; 
      Memory(GPIO_PORTD_BASE_ADDRESS , PORT_LOCK_REG_OFFSET) = GPIO_LOCK_KEY ; 
     Memory(GPIO_PORTD_BASE_ADDRESS,PORT_COMMIT_REG_OFFSET)  |= (1<< pin) ;
     Memory(GPIO_PORTD_BASE_ADDRESS,PORT_DIGITAL_ENABLE_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTD_BASE_ADDRESS,PORT_DIR_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTD_BASE_ADDRESS,PORT_ANALOG_MODE_SEL_REG_OFFSET) &= ~(1<<pin); 
     Memory(GPIO_PORTD_BASE_ADDRESS,PORT_ALT_FUNC_REG_OFFSET ) |= (1<<pin) ;
     Memory(GPIO_PORTD_BASE_ADDRESS,PORT_CTL_REG_OFFSET)  |= (4<<(pin*4));
    PWM1_0_CTL_R  &= ~(1<<0); /* Disable Generator 0 counter */  
     if(PWM_Configuration[i].mode == DOWN_MODE)  {     // configure down mode 
     PWM1_0_CTL_R  &= ~(1<<1); /* select down count mode of counter 0*/ 
     PWM1_0_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
    PWM1_0_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK))  ;    /* set load value  */
    PWM1_0_CMPA_R = (PWM_Configuration[i].DutyCyle/100)*(PWM1_0_LOAD_R ) ;   /* set duty cyle to to minumum value*/
    PWM1_0_CTL_R = 1; /* Enable Generator 0 counter */
    PWM1_ENABLE_R = 0x01; /* Enable PWM1 channel 0 output */   
     } 
     else {
      PWM1_0_CTL_R  |=(1<<1); /* select down count mode of counter 3*/  
      PWM1_0_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
       PWM1_0_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK));    /* set load value  */
       PWM1_0_CMPA_R = ((100-PWM_Configuration[i].DutyCyle)/100)*(PWM1_0_LOAD_R ) ;   /* set duty cyle to to minumum value*/
       PWM1_0_CTL_R = 1; /* Enable Generator 0 counter */
       PWM1_ENABLE_R = 0x01; /* Enable PWM1 channel 0 output */   
     }  
    break; 
  /************************************************  SECOND  CHANNEL Module 1 ************************************/     
  case PWM1_PD1 :   
     //port = GPIO_PORTD_BASE_ADDRESS ; 
     pin  = pin1;
     port_Num = 3;
     SYSCTL_RCGCGPIO_R =  SYSCTL_RCGC2_GPIOD;
while(!(SYSCTL_PRGPIO_R & (1<<port_Num))){} ; 
      Memory(GPIO_PORTD_BASE_ADDRESS , PORT_LOCK_REG_OFFSET) = GPIO_LOCK_KEY ; 
     Memory(GPIO_PORTD_BASE_ADDRESS,PORT_COMMIT_REG_OFFSET)  |= (1<< pin) ;
     Memory(GPIO_PORTD_BASE_ADDRESS,PORT_DIGITAL_ENABLE_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTD_BASE_ADDRESS,PORT_DIR_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTD_BASE_ADDRESS,PORT_ANALOG_MODE_SEL_REG_OFFSET) &= ~(1<<pin); 
     Memory(GPIO_PORTD_BASE_ADDRESS,PORT_ALT_FUNC_REG_OFFSET ) |= (1<<pin) ;
     Memory(GPIO_PORTD_BASE_ADDRESS,PORT_CTL_REG_OFFSET)  |= (4<<(pin*4));
      PWM1_0_CTL_R  &= ~(1<<0); /* Disable Generator 0 counter */  
     if(PWM_Configuration[i].mode == DOWN_MODE)  {     // configure down mode 
     PWM1_0_CTL_R  &= ~(1<<1); /* select down count mode of counter 0*/ 
     PWM1_0_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
    PWM1_0_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK))  ;    /* set load value  */
    PWM1_0_CMPA_R = (PWM_Configuration[i].DutyCyle/100)*(PWM1_0_LOAD_R ) ;   /* set duty cyle to to minumum value*/
    PWM1_0_CTL_R = 1; /* Enable Generator 0 counter */
    PWM1_ENABLE_R = 0x01; /* Enable PWM1 channel 0 output */   
     } 
     else {
      PWM1_0_CTL_R  |=(1<<1); /* select down count mode of counter 3*/  
      PWM1_0_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
       PWM1_0_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK));    /* set load value  */
       PWM1_0_CMPA_R = ((100-PWM_Configuration[i].DutyCyle)/100)*(PWM1_0_LOAD_R ) ;   /* set duty cyle to to minumum value*/
       PWM1_0_CTL_R = 1; /* Enable Generator 0 counter */
       PWM1_ENABLE_R = 0x01; /* Enable PWM1 channel 0 output */   
     }  
    break;   
 /*************************************** THIRD CHANNEL MODULE 1 *********************************************/    
  case PWM1_PA6 :      // MODULE 1  GEN 1 
     //port = GPIO_PORTA_BASE_ADDRESS ; 
     pin  = pin6;
     port_Num = 0;
      SYSCTL_RCGCGPIO_R =  SYSCTL_RCGC2_GPIOA;  
while(!(SYSCTL_PRGPIO_R & (1<<port_Num))){} ; 
      Memory(GPIO_PORTA_BASE_ADDRESS , PORT_LOCK_REG_OFFSET) = GPIO_LOCK_KEY ; 
     Memory(GPIO_PORTA_BASE_ADDRESS,PORT_COMMIT_REG_OFFSET)  |= (1<< pin) ;
     Memory(GPIO_PORTA_BASE_ADDRESS,PORT_DIGITAL_ENABLE_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTA_BASE_ADDRESS,PORT_DIR_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTA_BASE_ADDRESS,PORT_ANALOG_MODE_SEL_REG_OFFSET) &= ~(1<<pin); 
     Memory(GPIO_PORTA_BASE_ADDRESS,PORT_ALT_FUNC_REG_OFFSET ) |= (1<<pin) ;
     Memory(GPIO_PORTA_BASE_ADDRESS,PORT_CTL_REG_OFFSET)  |= (4<<(pin*4));
      PWM1_1_CTL_R  &= ~(1<<0); /* Disable Generator 1 counter */  
     if(PWM_Configuration[i].mode == DOWN_MODE)  {     // configure down mode 
     PWM1_1_CTL_R  &= ~(1<<1); /* select down count mode of counter 0*/ 
     PWM1_1_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
    PWM1_1_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK))  ;    /* set load value  */
    PWM1_1_CMPA_R = (PWM_Configuration[i].DutyCyle/100)*(PWM1_1_LOAD_R ) ;   /* set duty cyle to to minumum value*/
    PWM1_1_CTL_R = 1; /* Enable Generator 1 counter */
    PWM1_ENABLE_R = 0x01; /* Enable PWM1 channel 1 output */   
     } 
     else {
      PWM1_1_CTL_R  |=(1<<1); /* select down count mode of counter 1*/  
      PWM1_1_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
       PWM1_1_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK));    /* set load value  */
       PWM1_1_CMPA_R = ((100-PWM_Configuration[i].DutyCyle)/100)*(PWM1_1_LOAD_R ) ;   /* set duty cyle to to minumum value*/
       PWM1_1_CTL_R = 1; /* Enable Generator 1 counter */
       PWM1_ENABLE_R = 0x01; /* Enable PWM1 channel 1 output */   
     }   
    break;   
 /***************************************** FOURTH CHANNEL MODULE 1 *************************************/     
  case PWM1_PA7 :   //MODULE 1 GEN 1 
     //port = GPIO_PORTA_BASE_ADDRESS ; 
     pin  = pin7;
     port_Num = 0; 
    SYSCTL_RCGCGPIO_R =  SYSCTL_RCGC2_GPIOA; 
while(!(SYSCTL_PRGPIO_R & (1<<port_Num))){} ; 
      Memory(GPIO_PORTA_BASE_ADDRESS , PORT_LOCK_REG_OFFSET) = GPIO_LOCK_KEY ; 
     Memory(GPIO_PORTA_BASE_ADDRESS,PORT_COMMIT_REG_OFFSET)  |= (1<< pin) ;
     Memory(GPIO_PORTA_BASE_ADDRESS,PORT_DIGITAL_ENABLE_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTA_BASE_ADDRESS,PORT_DIR_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTA_BASE_ADDRESS,PORT_ANALOG_MODE_SEL_REG_OFFSET) &= ~(1<<pin); 
     Memory(GPIO_PORTA_BASE_ADDRESS,PORT_ALT_FUNC_REG_OFFSET ) |= (1<<pin) ;
     Memory(GPIO_PORTA_BASE_ADDRESS,PORT_CTL_REG_OFFSET)  |= (4<<(pin*4));
     PWM1_1_CTL_R  &= ~(1<<0); /* Disable Generator 1 counter */  
     if(PWM_Configuration[i].mode == DOWN_MODE)  {     // configure down mode 
     PWM1_1_CTL_R  &= ~(1<<1); /* select down count mode of counter 0*/ 
     PWM1_1_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
    PWM1_1_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK))  ;    /* set load value  */
    PWM1_1_CMPA_R = (PWM_Configuration[i].DutyCyle/100)*(PWM1_1_LOAD_R ) ;   /* set duty cyle to to minumum value*/
    PWM1_1_CTL_R = 1; /* Enable Generator 1 counter */
    PWM1_ENABLE_R = 0x01; /* Enable PWM1 channel 1 output */   
     } 
     else {
      PWM1_1_CTL_R  |=(1<<1); /* select down count mode of counter 1*/  
      PWM1_1_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
       PWM1_1_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK));    /* set load value  */
       PWM1_1_CMPA_R = ((100-PWM_Configuration[i].DutyCyle)/100)*(PWM1_1_LOAD_R ) ;   /* set duty cyle to to minumum value*/
       PWM1_1_CTL_R = 1; /* Enable Generator 1 counter */
       PWM1_ENABLE_R = 0x01; /* Enable PWM1 channel 1 output */   
     }   
    break;   
  /****************************************** FIFTH CHANNEL MODULE 1  ********************************************/    
  case PWM1_PF0 : 
     //port = GPIO_PORTF_BASE_ADDRESS ; 
     pin  = pin0;
     port_Num = 5; 
SYSCTL_RCGCGPIO_R =  SYSCTL_RCGC2_GPIOF; 
while(!(SYSCTL_PRGPIO_R & (1<<port_Num))){} ; 
      Memory(GPIO_PORTF_BASE_ADDRESS , PORT_LOCK_REG_OFFSET) = GPIO_LOCK_KEY ; 
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_COMMIT_REG_OFFSET)  |= (1<< pin) ;
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_DIGITAL_ENABLE_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_DIR_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_ANALOG_MODE_SEL_REG_OFFSET) &= ~(1<<pin); 
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_ALT_FUNC_REG_OFFSET ) |= (1<<pin) ;
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_CTL_REG_OFFSET)  |= (4<<(pin*4));
       PWM1_2_CTL_R  &= ~(1<<0); /* Disable Generator 2 counter */  
     if(PWM_Configuration[i].mode == DOWN_MODE)  {     // configure down mode 
     PWM1_2_CTL_R  &= ~(1<<1); /* select down count mode of counter 0*/ 
     PWM1_2_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
    PWM1_2_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK))  ;    /* set load value  */
    PWM1_2_CMPA_R = (PWM_Configuration[i].DutyCyle/100)*(PWM1_2_LOAD_R ) ;   /* set duty cyle to to minumum value*/
    PWM1_2_CTL_R = 1; /* Enable Generator 2 counter */
    PWM1_ENABLE_R = 0x01; /* Enable PWM1 channel 2 output */   
     } 
     else {
      PWM1_2_CTL_R  |=(1<<1); /* select down count mode of counter 1*/  
      PWM1_2_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
       PWM1_2_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK));    /* set load value  */
       PWM1_2_CMPA_R = ((100-PWM_Configuration[i].DutyCyle)/100)*(PWM1_2_LOAD_R ) ;   /* set duty cyle to to minumum value*/
       PWM1_2_CTL_R = 1; /* Enable Generator 2 counter */
       PWM1_ENABLE_R = 0x01; /* Enable PWM2 channel 2 output */   
     }   
    break;  
  /***************************************** SIXTH CHANNEL MODULE 1  *********************************/  
  case PWM1_PF1 : 
    // port = GPIO_PORTF_BASE_ADDRESS ; 
     pin  = pin1;
     port_Num = 5; 
       SYSCTL_RCGCGPIO_R  =  SYSCTL_RCGC2_GPIOF; 
while(!(SYSCTL_PRGPIO_R & (1<<port_Num))){} ; 
      Memory(GPIO_PORTF_BASE_ADDRESS , PORT_LOCK_REG_OFFSET) = GPIO_LOCK_KEY ; 
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_COMMIT_REG_OFFSET)  |= (1<< pin) ;
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_DIGITAL_ENABLE_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_DIR_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_ANALOG_MODE_SEL_REG_OFFSET) &= ~(1<<pin); 
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_ALT_FUNC_REG_OFFSET ) |= (1<<pin) ;
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_CTL_REG_OFFSET)  |= (4<<(pin*4));
     PWM1_2_CTL_R  &= ~(1<<0); /* Disable Generator 2 counter */  
     if(PWM_Configuration[i].mode == DOWN_MODE)  {     // configure down mode 
     PWM1_2_CTL_R  &= ~(1<<1); /* select down count mode of counter 0*/ 
     PWM1_2_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
    PWM1_2_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK))  ;    /* set load value  */
    PWM1_2_CMPA_R = (PWM_Configuration[i].DutyCyle/100)*(PWM1_2_LOAD_R ) ;   /* set duty cyle to to minumum value*/
    PWM1_2_CTL_R = 1; /* Enable Generator 2 counter */
    PWM1_ENABLE_R = 0x01; /* Enable PWM1 channel 2 output */   
     } 
     else {
      PWM1_2_CTL_R  |=(1<<1); /* select down count mode of counter 1*/  
      PWM1_2_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
       PWM1_2_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK));    /* set load value  */
       PWM1_2_CMPA_R = ((100-PWM_Configuration[i].DutyCyle)/100)*(PWM1_2_LOAD_R ) ;   /* set duty cyle to to minumum value*/
       PWM1_2_CTL_R = 1; /* Enable Generator 2 counter */
       PWM1_ENABLE_R = 0x01; /* Enable PWM2 channel 2 output */   
     }   
    break; 
 /***********************************************  SEVENTH CHANNEL MODULE 1 **********************************************/    
  case  PWM1_PF2 :
     //port = GPIO_PORTF_BASE_ADDRESS ; 
     pin  = pin2;
     port_Num = 5; 
     SYSCTL_RCGCGPIO_R  =  SYSCTL_RCGC2_GPIOF; 
     while(!(SYSCTL_PRGPIO_R & (1<<port_Num))){};
         Memory(GPIO_PORTF_BASE_ADDRESS , PORT_LOCK_REG_OFFSET) = GPIO_LOCK_KEY ; 
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_COMMIT_REG_OFFSET)  |= (1<< pin) ;
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_DIGITAL_ENABLE_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_DIR_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_ANALOG_MODE_SEL_REG_OFFSET) &= ~(1<<pin); 
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_ALT_FUNC_REG_OFFSET ) |= (1<<pin) ;
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_CTL_REG_OFFSET)  |= (4<<(pin*4));
   PWM1_3_CTL_R  &= ~(1<<0); /* Disable Generator 3 counter */  
     if(PWM_Configuration[i].mode == DOWN_MODE)  {     // configure down mode 
     PWM1_3_CTL_R  &= ~(1<<1); /* select down count mode of counter 0*/ 
     PWM1_3_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
    PWM1_3_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK))  ;    /* set load value  */
    PWM1_3_CMPA_R = (PWM_Configuration[i].DutyCyle/100)*(PWM1_3_LOAD_R ) ;   /* set duty cyle to to minumum value*/
    PWM1_3_CTL_R = 1; /* Enable Generator 2 counter */
    PWM1_ENABLE_R = 0x01; /* Enable PWM1 channel 2 output */   
     } 
     else {
      PWM1_3_CTL_R  |=(1<<1); /* select down count mode of counter 3*/  
      PWM1_3_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
       PWM1_3_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK));    /* set load value  */
       PWM1_3_CMPA_R = ((100-PWM_Configuration[i].DutyCyle)/100)*(PWM1_3_LOAD_R ) ;   /* set duty cyle to to minumum value*/
       PWM1_3_CTL_R = 1; /* Enable Generator 2 counter */
       PWM1_ENABLE_R = 0x01; /* Enable PWM1 channel 3 output */   
     } 
    break;  
 /********************************************** EIGHTH CHANNEL MODULE  1 **********************************************/    
 case PWM1_PF3 : 
    //port = GPIO_PORTF_BASE_ADDRESS ; 
     pin  = pin3;
     port_Num = 5; 
SYSCTL_RCGCGPIO_R  =  SYSCTL_RCGC2_GPIOF; 
while(!(SYSCTL_PRGPIO_R & (1<<port_Num))){} ; 
      Memory(GPIO_PORTF_BASE_ADDRESS , PORT_LOCK_REG_OFFSET) = GPIO_LOCK_KEY ; 
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_COMMIT_REG_OFFSET)  |= (1<< pin) ;
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_DIGITAL_ENABLE_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_DIR_REG_OFFSET) |= (1<<pin);
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_ANALOG_MODE_SEL_REG_OFFSET) &= ~(1<<pin); 
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_ALT_FUNC_REG_OFFSET ) |= (1<<pin) ;
     Memory(GPIO_PORTF_BASE_ADDRESS,PORT_CTL_REG_OFFSET)  |= (4<<(pin*4));
    PWM1_3_CTL_R  &= ~(1<<0); /* Disable Generator 3 counter */  
     if(PWM_Configuration[i].mode == DOWN_MODE)  {     // configure down mode 
     PWM1_3_CTL_R  &= ~(1<<1); /* select down count mode of counter 0*/ 
     PWM1_3_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
    PWM1_3_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK))  ;    /* set load value  */
    PWM1_3_CMPA_R = (PWM_Configuration[i].DutyCyle/100)*(PWM1_3_LOAD_R ) ;   /* set duty cyle to to minumum value*/
    PWM1_3_CTL_R = 1; /* Enable Generator 2 counter */
    PWM1_ENABLE_R = 0x01; /* Enable PWM1 channel 2 output */   
     } 
     else {     
      PWM1_3_CTL_R  |=(1<<1); /* select down count mode of counter 3*/  
      PWM1_3_GENA_R = 0x0000008C; /* Set PWM outpu1 when counter reloaded and clear when matches PWMCMPA */  
     /*
      EXAMPLE 
      PWM Module Clock Period = 1 / 16MHz = 62.5ns
     PWM Signal Period = 1 / 50Hz = 20ms
    Load register value = 20ms/62.5ns = 320, 000 */   
       PWM1_3_LOAD_R = ((1/PWM_Configuration[i].freq)/(1/SYSTEM_CLOCK));    /* set load value  */
       PWM1_3_CMPA_R = ((100-PWM_Configuration[i].DutyCyle)/100)*(PWM1_3_LOAD_R ) ;   /* set duty cyle to to minumum value*/
       PWM1_3_CTL_R = 1; /* Enable Generator 2 counter */
       PWM1_ENABLE_R = 0x01; /* Enable PWM1 channel 3 output */   
     } 
    break; 
  }
  } 
}
void Set_DutyCycle ( Pwm_ChannelType channel , uint16 DutyCyle )  {
  switch(channel) {
  case PWM0_PB6 :   
  PWM0_0_CMPA_R = (DutyCyle/100)*(PWM0_0_LOAD_R ) ;    
  break ;  
   case PWM0_PB7  :   
    PWM0_0_CMPA_R = (DutyCyle/100)*(PWM0_0_LOAD_R ) ;    
  break ;  
   case PWM0_PB4 :   
     PWM0_1_CMPA_R = (DutyCyle/100)*(PWM0_1_LOAD_R ) ;     
  break ;  
   case PWM0_PB5 :   
        PWM0_1_CMPA_R = (DutyCyle/100)*(PWM0_1_LOAD_R ) ;  
  break ;  
   case PWM0_PE4 :   
     PWM0_2_CMPA_R = (DutyCyle/100)*(PWM0_2_LOAD_R ) ;     
  break ;  
   case PWM0_PE5 :   
     PWM0_2_CMPA_R = (DutyCyle/100)*(PWM0_2_LOAD_R ) ;    
  break ;  
   case PWM0_PC4 :   
    PWM0_3_CMPA_R = (DutyCyle/100)*(PWM0_3_LOAD_R ) ;   
  break ;  
   case PWM0_PC5 :   
    PWM0_3_CMPA_R = (DutyCyle/100)*(PWM1_2_LOAD_R ) ;    
  break ;  
   case PWM1_PD0 :   
      PWM1_0_CMPA_R = (DutyCyle/100)*(PWM1_0_LOAD_R ) ; 
  break ;  
   case PWM1_PD1 :   
    PWM1_0_CMPA_R = (DutyCyle/100)*(PWM1_0_LOAD_R ) ; 
  break ;  
   case PWM1_PA6:   
    PWM1_1_CMPA_R = (DutyCyle/100)*(PWM1_1_LOAD_R ) ;     
  break ;  
   case PWM1_PA7:   
    PWM1_1_CMPA_R = (DutyCyle/100)*(PWM1_1_LOAD_R ) ;  
  break ;   
   case PWM1_PF0:   
    PWM1_2_CMPA_R = (DutyCyle/100)*(PWM1_2_LOAD_R ) ;    
  break ; 
   case PWM1_PF1:   
    PWM1_2_CMPA_R = (DutyCyle/100)*(PWM1_2_LOAD_R ) ;     
  break ;
   case PWM1_PF2:   
    PWM1_3_CMPA_R = (DutyCyle/100)*(PWM1_3_LOAD_R ) ;   
  break ;
   case PWM1_PF3:   
   PWM1_3_CMPA_R = (DutyCyle/100)*(PWM1_3_LOAD_R ) ;     
  break ;
  
  
  
  }



}