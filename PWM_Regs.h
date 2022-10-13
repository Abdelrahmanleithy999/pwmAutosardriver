/**********************************************************************************************************************
 *
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  PWM_Regs.h
 *       Module:  Pulse width modulation  Driver 
 *
 *  Description: Pulse width modulation Driver Registers     
 *  
 *********************************************************************************************************************/
#ifndef PWM_REGS.H
#define PWM_REGS.H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "Std_Types.h"

/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define PWM0_CTL_R              (*((volatile unsigned long *)0x40028000))
#define PWM0_SYNC_R             (*((volatile unsigned long *)0x40028004))
#define PWM0_ENABLE_R           (*((volatile unsigned long *)0x40028008))
#define PWM0_INVERT_R           (*((volatile unsigned long *)0x4002800C))
#define PWM0_FAULT_R            (*((volatile unsigned long *)0x40028010))
#define PWM0_INTEN_R            (*((volatile unsigned long *)0x40028014))
#define PWM0_RIS_R              (*((volatile unsigned long *)0x40028018))
#define PWM0_ISC_R              (*((volatile unsigned long *)0x4002801C))
#define PWM0_STATUS_R           (*((volatile unsigned long *)0x40028020))
#define PWM0_FAULTVAL_R         (*((volatile unsigned long *)0x40028024))
#define PWM0_ENUPD_R            (*((volatile unsigned long *)0x40028028))
#define PWM0_0_CTL_R            (*((volatile unsigned long *)0x40028040))
#define PWM0_0_INTEN_R          (*((volatile unsigned long *)0x40028044))
#define PWM0_0_RIS_R            (*((volatile unsigned long *)0x40028048))
#define PWM0_0_ISC_R            (*((volatile unsigned long *)0x4002804C))
#define PWM0_0_LOAD_R           (*((volatile unsigned long *)0x40028050))
#define PWM0_0_COUNT_R          (*((volatile unsigned long *)0x40028054))
#define PWM0_0_CMPA_R           (*((volatile unsigned long *)0x40028058))
#define PWM0_0_CMPB_R           (*((volatile unsigned long *)0x4002805C))
#define PWM0_0_GENA_R           (*((volatile unsigned long *)0x40028060))
#define PWM0_0_GENB_R           (*((volatile unsigned long *)0x40028064))
#define PWM0_0_DBCTL_R          (*((volatile unsigned long *)0x40028068))
#define PWM0_0_DBRISE_R         (*((volatile unsigned long *)0x4002806C))
#define PWM0_0_DBFALL_R         (*((volatile unsigned long *)0x40028070))
#define PWM0_0_FLTSRC0_R        (*((volatile unsigned long *)0x40028074))
#define PWM0_0_FLTSRC1_R        (*((volatile unsigned long *)0x40028078))
#define PWM0_0_MINFLTPER_R      (*((volatile unsigned long *)0x4002807C))
#define PWM0_1_CTL_R            (*((volatile unsigned long *)0x40028080))
#define PWM0_1_INTEN_R          (*((volatile unsigned long *)0x40028084))
#define PWM0_1_RIS_R            (*((volatile unsigned long *)0x40028088))
#define PWM0_1_ISC_R            (*((volatile unsigned long *)0x4002808C))
#define PWM0_1_LOAD_R           (*((volatile unsigned long *)0x40028090))
#define PWM0_1_COUNT_R          (*((volatile unsigned long *)0x40028094))
#define PWM0_1_CMPA_R           (*((volatile unsigned long *)0x40028098))
#define PWM0_1_CMPB_R           (*((volatile unsigned long *)0x4002809C))
#define PWM0_1_GENA_R           (*((volatile unsigned long *)0x400280A0))
#define PWM0_1_GENB_R           (*((volatile unsigned long *)0x400280A4))
#define PWM0_1_DBCTL_R          (*((volatile unsigned long *)0x400280A8))
#define PWM0_1_DBRISE_R         (*((volatile unsigned long *)0x400280AC))
#define PWM0_1_DBFALL_R         (*((volatile unsigned long *)0x400280B0))
#define PWM0_1_FLTSRC0_R        (*((volatile unsigned long *)0x400280B4))
#define PWM0_1_FLTSRC1_R        (*((volatile unsigned long *)0x400280B8))
#define PWM0_1_MINFLTPER_R      (*((volatile unsigned long *)0x400280BC))
#define PWM0_2_CTL_R            (*((volatile unsigned long *)0x400280C0))
#define PWM0_2_INTEN_R          (*((volatile unsigned long *)0x400280C4))
#define PWM0_2_RIS_R            (*((volatile unsigned long *)0x400280C8))
#define PWM0_2_ISC_R            (*((volatile unsigned long *)0x400280CC))
#define PWM0_2_LOAD_R           (*((volatile unsigned long *)0x400280D0))
#define PWM0_2_COUNT_R          (*((volatile unsigned long *)0x400280D4))
#define PWM0_2_CMPA_R           (*((volatile unsigned long *)0x400280D8))
#define PWM0_2_CMPB_R           (*((volatile unsigned long *)0x400280DC))
#define PWM0_2_GENA_R           (*((volatile unsigned long *)0x400280E0))
#define PWM0_2_GENB_R           (*((volatile unsigned long *)0x400280E4))
#define PWM0_2_DBCTL_R          (*((volatile unsigned long *)0x400280E8))
#define PWM0_2_DBRISE_R         (*((volatile unsigned long *)0x400280EC))
#define PWM0_2_DBFALL_R         (*((volatile unsigned long *)0x400280F0))
#define PWM0_2_FLTSRC0_R        (*((volatile unsigned long *)0x400280F4))
#define PWM0_2_FLTSRC1_R        (*((volatile unsigned long *)0x400280F8))
#define PWM0_2_MINFLTPER_R      (*((volatile unsigned long *)0x400280FC))
#define PWM0_3_CTL_R            (*((volatile unsigned long *)0x40028100))
#define PWM0_3_INTEN_R          (*((volatile unsigned long *)0x40028104))
#define PWM0_3_RIS_R            (*((volatile unsigned long *)0x40028108))
#define PWM0_3_ISC_R            (*((volatile unsigned long *)0x4002810C))
#define PWM0_3_LOAD_R           (*((volatile unsigned long *)0x40028110))
#define PWM0_3_COUNT_R          (*((volatile unsigned long *)0x40028114))
#define PWM0_3_CMPA_R           (*((volatile unsigned long *)0x40028118))
#define PWM0_3_CMPB_R           (*((volatile unsigned long *)0x4002811C))
#define PWM0_3_GENA_R           (*((volatile unsigned long *)0x40028120))
#define PWM0_3_GENB_R           (*((volatile unsigned long *)0x40028124))
#define PWM0_3_DBCTL_R          (*((volatile unsigned long *)0x40028128))
#define PWM0_3_DBRISE_R         (*((volatile unsigned long *)0x4002812C))
#define PWM0_3_DBFALL_R         (*((volatile unsigned long *)0x40028130))
#define PWM0_3_FLTSRC0_R        (*((volatile unsigned long *)0x40028134))
#define PWM0_3_FLTSRC1_R        (*((volatile unsigned long *)0x40028138))
#define PWM0_3_MINFLTPER_R      (*((volatile unsigned long *)0x4002813C))
#define PWM0_0_FLTSEN_R         (*((volatile unsigned long *)0x40028800))
#define PWM0_0_FLTSTAT0_R       (*((volatile unsigned long *)0x40028804))
#define PWM0_0_FLTSTAT1_R       (*((volatile unsigned long *)0x40028808))
#define PWM0_1_FLTSEN_R         (*((volatile unsigned long *)0x40028880))
#define PWM0_1_FLTSTAT0_R       (*((volatile unsigned long *)0x40028884))
#define PWM0_1_FLTSTAT1_R       (*((volatile unsigned long *)0x40028888))
#define PWM0_2_FLTSTAT0_R       (*((volatile unsigned long *)0x40028904))
#define PWM0_2_FLTSTAT1_R       (*((volatile unsigned long *)0x40028908))
#define PWM0_3_FLTSTAT0_R       (*((volatile unsigned long *)0x40028984))
#define PWM0_3_FLTSTAT1_R       (*((volatile unsigned long *)0x40028988))
#define PWM0_PP_R               (*((volatile unsigned long *)0x40028FC0))

//*****************************************************************************
//
// PWM registers (PWM1)
//
//*****************************************************************************
#define PWM1_CTL_R              (*((volatile unsigned long *)0x40029000))
#define PWM1_SYNC_R             (*((volatile unsigned long *)0x40029004))
#define PWM1_ENABLE_R           (*((volatile unsigned long *)0x40029008))
#define PWM1_INVERT_R           (*((volatile unsigned long *)0x4002900C))
#define PWM1_FAULT_R            (*((volatile unsigned long *)0x40029010))
#define PWM1_INTEN_R            (*((volatile unsigned long *)0x40029014))
#define PWM1_RIS_R              (*((volatile unsigned long *)0x40029018))
#define PWM1_ISC_R              (*((volatile unsigned long *)0x4002901C))
#define PWM1_STATUS_R           (*((volatile unsigned long *)0x40029020))
#define PWM1_FAULTVAL_R         (*((volatile unsigned long *)0x40029024))
#define PWM1_ENUPD_R            (*((volatile unsigned long *)0x40029028))
#define PWM1_0_CTL_R            (*((volatile unsigned long *)0x40029040))
#define PWM1_0_INTEN_R          (*((volatile unsigned long *)0x40029044))
#define PWM1_0_RIS_R            (*((volatile unsigned long *)0x40029048))
#define PWM1_0_ISC_R            (*((volatile unsigned long *)0x4002904C))
#define PWM1_0_LOAD_R           (*((volatile unsigned long *)0x40029050))
#define PWM1_0_COUNT_R          (*((volatile unsigned long *)0x40029054))
#define PWM1_0_CMPA_R           (*((volatile unsigned long *)0x40029058))
#define PWM1_0_CMPB_R           (*((volatile unsigned long *)0x4002905C))
#define PWM1_0_GENA_R           (*((volatile unsigned long *)0x40029060))
#define PWM1_0_GENB_R           (*((volatile unsigned long *)0x40029064))
#define PWM1_0_DBCTL_R          (*((volatile unsigned long *)0x40029068))
#define PWM1_0_DBRISE_R         (*((volatile unsigned long *)0x4002906C))
#define PWM1_0_DBFALL_R         (*((volatile unsigned long *)0x40029070))
#define PWM1_0_FLTSRC0_R        (*((volatile unsigned long *)0x40029074))
#define PWM1_0_FLTSRC1_R        (*((volatile unsigned long *)0x40029078))
#define PWM1_0_MINFLTPER_R      (*((volatile unsigned long *)0x4002907C))
#define PWM1_1_CTL_R            (*((volatile unsigned long *)0x40029080))
#define PWM1_1_INTEN_R          (*((volatile unsigned long *)0x40029084))
#define PWM1_1_RIS_R            (*((volatile unsigned long *)0x40029088))
#define PWM1_1_ISC_R            (*((volatile unsigned long *)0x4002908C))
#define PWM1_1_LOAD_R           (*((volatile unsigned long *)0x40029090))
#define PWM1_1_COUNT_R          (*((volatile unsigned long *)0x40029094))
#define PWM1_1_CMPA_R           (*((volatile unsigned long *)0x40029098))
#define PWM1_1_CMPB_R           (*((volatile unsigned long *)0x4002909C))
#define PWM1_1_GENA_R           (*((volatile unsigned long *)0x400290A0))
#define PWM1_1_GENB_R           (*((volatile unsigned long *)0x400290A4))
#define PWM1_1_DBCTL_R          (*((volatile unsigned long *)0x400290A8))
#define PWM1_1_DBRISE_R         (*((volatile unsigned long *)0x400290AC))
#define PWM1_1_DBFALL_R         (*((volatile unsigned long *)0x400290B0))
#define PWM1_1_FLTSRC0_R        (*((volatile unsigned long *)0x400290B4))
#define PWM1_1_FLTSRC1_R        (*((volatile unsigned long *)0x400290B8))
#define PWM1_1_MINFLTPER_R      (*((volatile unsigned long *)0x400290BC))
#define PWM1_2_CTL_R            (*((volatile unsigned long *)0x400290C0))
#define PWM1_2_INTEN_R          (*((volatile unsigned long *)0x400290C4))
#define PWM1_2_RIS_R            (*((volatile unsigned long *)0x400290C8))
#define PWM1_2_ISC_R            (*((volatile unsigned long *)0x400290CC))
#define PWM1_2_LOAD_R           (*((volatile unsigned long *)0x400290D0))
#define PWM1_2_COUNT_R          (*((volatile unsigned long *)0x400290D4))
#define PWM1_2_CMPA_R           (*((volatile unsigned long *)0x400290D8))
#define PWM1_2_CMPB_R           (*((volatile unsigned long *)0x400290DC))
#define PWM1_2_GENA_R           (*((volatile unsigned long *)0x400290E0))
#define PWM1_2_GENB_R           (*((volatile unsigned long *)0x400290E4))
#define PWM1_2_DBCTL_R          (*((volatile unsigned long *)0x400290E8))
#define PWM1_2_DBRISE_R         (*((volatile unsigned long *)0x400290EC))
#define PWM1_2_DBFALL_R         (*((volatile unsigned long *)0x400290F0))
#define PWM1_2_FLTSRC0_R        (*((volatile unsigned long *)0x400290F4))
#define PWM1_2_FLTSRC1_R        (*((volatile unsigned long *)0x400290F8))
#define PWM1_2_MINFLTPER_R      (*((volatile unsigned long *)0x400290FC))
#define PWM1_3_CTL_R            (*((volatile unsigned long *)0x40029100))
#define PWM1_3_INTEN_R          (*((volatile unsigned long *)0x40029104))
#define PWM1_3_RIS_R            (*((volatile unsigned long *)0x40029108))
#define PWM1_3_ISC_R            (*((volatile unsigned long *)0x4002910C))
#define PWM1_3_LOAD_R           (*((volatile unsigned long *)0x40029110))
#define PWM1_3_COUNT_R          (*((volatile unsigned long *)0x40029114))
#define PWM1_3_CMPA_R           (*((volatile unsigned long *)0x40029118))
#define PWM1_3_CMPB_R           (*((volatile unsigned long *)0x4002911C))
#define PWM1_3_GENA_R           (*((volatile unsigned long *)0x40029120))
#define PWM1_3_GENB_R           (*((volatile unsigned long *)0x40029124))
#define PWM1_3_DBCTL_R          (*((volatile unsigned long *)0x40029128))
#define PWM1_3_DBRISE_R         (*((volatile unsigned long *)0x4002912C))
#define PWM1_3_DBFALL_R         (*((volatile unsigned long *)0x40029130))
#define PWM1_3_FLTSRC0_R        (*((volatile unsigned long *)0x40029134))
#define PWM1_3_FLTSRC1_R        (*((volatile unsigned long *)0x40029138))
#define PWM1_3_MINFLTPER_R      (*((volatile unsigned long *)0x4002913C))
#define PWM1_0_FLTSEN_R         (*((volatile unsigned long *)0x40029800))
#define PWM1_0_FLTSTAT0_R       (*((volatile unsigned long *)0x40029804))
#define PWM1_0_FLTSTAT1_R       (*((volatile unsigned long *)0x40029808))
#define PWM1_1_FLTSEN_R         (*((volatile unsigned long *)0x40029880))
#define PWM1_1_FLTSTAT0_R       (*((volatile unsigned long *)0x40029884))
#define PWM1_1_FLTSTAT1_R       (*((volatile unsigned long *)0x40029888))
#define PWM1_2_FLTSTAT0_R       (*((volatile unsigned long *)0x40029904))
#define PWM1_2_FLTSTAT1_R       (*((volatile unsigned long *)0x40029908))
#define PWM1_3_FLTSTAT0_R       (*((volatile unsigned long *)0x40029984))
#define PWM1_3_FLTSTAT1_R       (*((volatile unsigned long *)0x40029988))
#define PWM1_PP_R               (*((volatile unsigned long *)0x40029FC0)) 
/* GPIO Registers base addresses */
#define GPIO_PORTA_BASE_ADDRESS           0x40004000
#define GPIO_PORTB_BASE_ADDRESS           0x40005000
#define GPIO_PORTC_BASE_ADDRESS           0x40006000
#define GPIO_PORTD_BASE_ADDRESS           0x40007000
#define GPIO_PORTE_BASE_ADDRESS           0x40024000
#define GPIO_PORTF_BASE_ADDRESS           0x40025000
/******************************************/
/* GPIO Registers offset addresses */
#define PORT_DATA_REG_OFFSET              0x3FC
#define PORT_DIR_REG_OFFSET               0x400
#define PORT_ALT_FUNC_REG_OFFSET          0x420
#define PORT_DR2R_REG_OFFSET              0X500
#define PORT_DR4R_REG_OFFSET              0X504
#define PORT_DR8R_REG_OFFSET              0X508
#define PORT_ODR_REG_OFFSET               0X50C
#define PORT_PULL_UP_REG_OFFSET           0x510
#define PORT_PULL_DOWN_REG_OFFSET         0x514
#define PORT_DIGITAL_ENABLE_REG_OFFSET    0x51C
#define PORT_LOCK_REG_OFFSET              0x520
#define PORT_COMMIT_REG_OFFSET            0x524
#define PORT_ANALOG_MODE_SEL_REG_OFFSET   0x528
#define PORT_CTL_REG_OFFSET               0x52C 
#define SYSCTL_RIS_REG            (*((volatile uint32 *)0x400FE050))
#define SYSCTL_RCC_REG            (*((volatile uint32 *)0x400FE060))
#define SYSCTL_RCC2_REG           (*((volatile uint32 *)0x400FE070))
/* Clock enable register For Port Driver */
#define SYSCTL_REGCGC2_REG        (*((volatile uint32 *)0x400FE108))

#endif /*PWM_REGS.H*/ 