/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  PWM.h
 *       Module:  Pulse Width Modulation (PWM) 
 *
 *  Description:  <Write File DESCRIPTION here>     
 *  
 *********************************************************************************************************************/
#ifndef PWM.H 
#define PWM.H  
/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Common_Macros.h"
#include "PWM_Cfg.h"
#include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define PWM_INITIALIZED                (1U)
#define PWM_NOT_INITIALIZED            (0U)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/
#define Memory(X,Y)			 	(*((volatile uint32*)(((uint32)X)+((uint32)Y))))

/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/  
typedef uint8 Pwm_ChannelType ;  
typedef uint16 Pwm_PeriodType ;   
typedef uint8  Pwm_DutyCyle ;  
typedef uint8  Idle_State ;   
typedef uint8 Pwm_Polarity ;  
typedef uint8  Pwm_Module ; 
/**************************** ENUMS ************************************************************************/  
typedef enum PWM_Module
{
	PWM_0 =0x40028000,
	PWM_1 =0x40029000
}PWM_Module_BaseAddress; 
typedef enum GENERATOR{
	GENERATOR1,
	GENERATOR2,
	GENERATOR3,
	GENERATOR4,
}GENERATOR;
typedef enum PWM_Regs{
	PWM_ENABLE_R=0x008,
	SRPWM=0x540,
	PWM_G0_CTL=0x040,
	PWM_G1_CTL=0x080,
	PWM_G2_CTL=0x0C0, 	
	PWM_G3_CTL=0x100,
	PWM_G0_LOAD_R=0x050,
	PWM_G1_LOAD_R=0x090,
	PWM_G2_LOAD_R=0x0D0,
	PWM_G3_LOAD_R=0x110,
	PWM_G0_COUNTER_R=0x054,
	PWM_G1_COUNTER_R=0x094,
	PWM_G2_COUNTER_R=0x0D4,
	PWM_G3_COUNTER_R=0x114,
	PWM_G0_CMPA_R=0x058,
	PWM_G1_CMPA_R=0x098,
	PWM_G2_CMPA_R=0x0D8,
	PWM_G3_CMPA_R=0x118,
	PWM_G0_CMPB_R=0x05C,
	PWM_G1_CMPB_R=0x08C,
	PWM_G2_CMPB_R=0x0DC,
	PWM_G3_CMPB_R=0x11C,
	PWM_G0_GENPINA_R=0x060,
	PWM_G1_GENPINA_R=0x0A0,
	PWM_G2_GENPINA_R=0x0E0,
	PWM_G3_GENPINA_R=0x120,
	PWM_G0_GENPINB_R=0x064,
	PWM_G1_GENPINB_R=0x0A4,
	PWM_G2_GENPINB_R=0x0E4,
	PWM_G3_GENPINB_R=0x124,
}PWM_Regs;
typedef enum { 
  PWM_LOW , PWM_HIGH  

}Pwm_OutputStateType ;   
typedef enum {
PWM_FALLING_EDGE , PWM_RAISING_EDGE , BOTH_EDGES 
}Pwm_EdgeNotificationType ;    
typedef enum {
PWM_VARIABLE_PERIOD , PWM_FIXED_PERIOD , PWM_FIXED_PERIOD_SHIFTED
}Pwm_ChannelClassType ;   
typedef enum {
PWM_SERVICE_ACCEPTED , PWM_NOT_INIT , PWM_SEQUENCE_ERROR , PWM_HW_FAILURE , PWM_POWER_STATE_NOT_SUPP , PWM_TRANS_NOT_POSSIBLE 
}Pwm_PowerStateRequestResultType;   
typedef enum {
NO_POWER , FULL_POWER=255 
}Pwm_PowerStateType ;   
typedef enum {
  sys_clk_div2,
  sys_clk_div4,
  sys_clk_div8,
  sys_clk_div16,
  sys_clk_div32,
  sys_clk_div64 ,
  sys_clk_no_div, 
}PWM_DIVISOR; 
typedef enum {   
Freq_1Khz = 1000  , Freq_7Khz = 7000 ,  Freq_5Khz = 5000 , Freq_2Khz = 2000 , Freq_10Khz = 10000 
 
}Output_Frequncy  ; 
typedef enum {
DOWN_MODE , UP_MODE  
}PWM_MODE ;    
typedef enum {
  pin0 ,pin1  ,pin2  ,pin3 ,pin4 ,pin5 ,pin6 ,pin7   
}pin_config ;   
typedef enum {
  port0 ,port1  ,port2  ,port3 ,port4 ,port5 ,port6 ,port7   
}port_config ; 
typedef enum {
 //PWM MODULE 0
  PWM0_PB6,//G0/pin0
  PWM0_PB7,//G0/pin1
  PWM0_PB4,//G1/pin0
  PWM0_PB5,//G1/pin1
  PWM0_PE4,//G2/pin0
  PWM0_PE5,//G2/pin1
  PWM0_PC4,//G3/pin0
  PWM0_PC5,//G3/pin1
    //PWM MODULE 1
  PWM1_PD0,//G0/pin0
  PWM1_PD1,//G0/pin01
  PWM1_PA6,//G1/pin0
  PWM1_PA7,//G1/pin1
  PWM1_PF0,//G2/pin0
  PWM1_PF1,//G2/pin1
  PWM1_PF2,//G3/pin0
  PWM1_PF3 //G3/pin1
}pwm_channel ; 

typedef struct { 
Pwm_Module Module ;  
Pwm_ChannelType  channel ;   
Pwm_DutyCyle  DutyCyle ;      
Pwm_PeriodType  freq ;  
PWM_MODE mode ;  
Pwm_OutputStateType  Output ;        
PWM_DIVISOR DIV ; 
#if (EXTRA_FEATURES == STD_ON) 
Pwm_EdgeNotificationType Edge  ;   
Pwm_ChannelClassType  channel_class ;  
Idle_State idlestate ;  
#endif 
}Pwm_ConfigChannel ;  
typedef struct {

Pwm_ConfigChannel PWM_Channels [PWM_CONFIGURED_CHANNLES]  ;

}Pwm_ConfigType ;    

/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/  
void Pwm_Init(Pwm_ConfigType* PWM_ptr );  
void Set_DutyCycle ( Pwm_ChannelType channel , uint16 DutyCyle ) ; 
//void setPortPin(uint8  Channel ,  uint8  Module ) ;  
#endif 
