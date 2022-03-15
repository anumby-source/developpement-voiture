#include "soc/rtc_cntl_reg.h"
#include "soc/rtc_io_reg.h"
#include "soc/soc_ulp.h"

// bit/pin 0/36 1/37 2/38 3/39 4/34 5/35 6/25 7/26 8/33 9/32

.set PIN_1_RTC_NUM, 8 //GPIO33
.set PIN_2_RTC_NUM, 6 //GPIO25
//.set PIN_2_RTC_NUM, 7 //GPIO26

.bss

.global debug_pin2_flag_isr
debug_pin2_flag_isr: .long 0 // flag positionné pour selection origine isr dans appli soc

.global pin1_status
pin1_status: .long 0

.global pin2_status
pin2_status: .long 0

.global pin1_last
pin1_last: .long 0 

.global pin2_last
pin2_last: .long 0 

.global pin2_cnt_ref
pin2_cnt_ref: .long 0 // nombre de cycles successifs avec pin 2 à 0 pour provoquer wake

pin2_cnt_cur: .long 0 // nombre courant de cycles successifs à 0

flag_isr: .long 0

.global pin1_flag_isr
pin1_flag_isr: .long 0 // flag positionné pour selection origine isr dans appli soc


/*  for timestamp */
  .global timestamp_low_array
timestamp_low_array: .long 0
  .global timestamp_mid_array
timestamp_mid_array: .long 0
  .global timestamp_hi_array
timestamp_hi_array: .long 0

.text

exit:
  wake  
  halt

try_wakeup:
  // try to wakeup SoC, if not => halt
  STAGE_INC  1
  JUMPS exit, 10, GT

  READ_RTC_FIELD(RTC_CNTL_LOW_POWER_ST_REG, RTC_CNTL_RDY_FOR_WAKEUP)
  AND r0, r0, 1
  JUMP try_wakeup, eq
  wake
  halt 

  .global entry
entry:
    move r0, 0
    move r2, flag_isr
    st r0, r2, 0    /* ==> clear flag_isr à 0 false    */
    move r2, pin1_flag_isr
    st r0, r2, 0    /* ==> clear pin1_flag_isr à 0 false    */

/* -------------------------------------------------------- essai ajout timestamp  */
  /* Trigger update of rtc time register */
    WRITE_RTC_REG(RTC_CNTL_TIME_UPDATE_REG, RTC_CNTL_TIME_UPDATE_S, 1, 1)
check_time_valid:
  /* Check if RTC_CNTL_TIME_VALID bit is 1, otherwise repeat */
    READ_RTC_REG(RTC_CNTL_TIME_UPDATE_REG, RTC_CNTL_TIME_VALID_S, 1)
    and r0, r0, 1
    jump check_time_valid, eq
      /* store three 16bit parts of rtc timestamp*/
    move r2, timestamp_low_array
    add r2, r2, r1
    READ_RTC_REG(RTC_CNTL_TIME0_REG, 0, 16)
    st r0, r2, 0
    move r2, timestamp_mid_array
    add r2, r2, r1
    READ_RTC_REG(RTC_CNTL_TIME0_REG, 16, 16)
    st r0, r2, 0
    move r2, timestamp_hi_array
    add r2, r2, r1
    READ_RTC_REG(RTC_CNTL_TIME1_REG, 0, 16)
    st r0, r2, 0
    
      
pin1_check:
  /* remplacer pin1_last par pin1_status */
    move r0, pin1_status
    ld r0, r0, 0
    move r2, pin1_last
    st r0, r2, 0
  /*read input registor of RTC_GPIO 0-15 into r0*/
    READ_RTC_REG(RTC_GPIO_IN_REG, RTC_GPIO_IN_NEXT_S, 16)
    move r2, PIN_1_RTC_NUM
    rsh r0, r0, r2
    and r0, r0, 1 /* le bit significatifs est le poids le plus faible */
    move r2, pin1_status
    st r0, r2, 0   /* stocké dans status */
    move r2, pin1_last
    ld r2, r2, 0  /* pin_last dans r2 */
    add r0, r0, r2
    and r0, r0, 1 /* = 0 si status et last identiques en binaire 0+0=0 1+1=10 poids faible 0 */
    jump pin2_check, eq
  /* traiter cas diff donc front */
    move r0, 1
    move r2, flag_isr
    st r0, r2, 0     /* ==> set flag_isr à 1 true    */ 
    move r2, pin1_flag_isr
    st r0, r2, 0     /* ==> set pin1_flag_isr à 1 true    */ 

pin2_check:
  /* remplacer pin2_last par pin2_status */
    move r0, pin2_status
    ld r0, r0, 0
    move r2, pin2_last
    st r0, r2, 0
  /*read input registor of RTC_GPIO 0-15 into r0*/
    READ_RTC_REG(RTC_GPIO_IN_REG, RTC_GPIO_IN_NEXT_S, 16)
    move r2, PIN_2_RTC_NUM
    rsh r0, r0, r2
    and r0, r0, 1 /* le bit significatifs est le poids le plus faible */
    move r2, pin2_status
    st r0, r2, 0   /* stocké dans status */ 
            // tel que prévu si pin2_cnt_ref == 0 : jamais d'isr
    and r0, r0, 1           // test de pin2_status
    jump pin2_0, eq         //pin2_status == 0 ? 
    move r0, 0              // sinon pin2_cnt_cur = 0 puis jump global_check
    move r2, pin2_cnt_cur
    st r0, r2, 0
    jump global_check
pin2_0:    
    move r2, pin2_cnt_cur            // si oui pin2_cnt_cur +1 ; 
    ld r0, r2, 0              
    add r0, r0, 1
    st r0, r2, 0
    move r2, pin2_cnt_ref
    ld r1, r2, 0                         
    sub r0,r0,r1        //  pin2_cnt_cur - pin2_cnt_ref
    jump pin2_0_good,eq     // si pin2_cnt_cur == pin2_cnt_ref 
    jump global_check     // seuil pas atteint


pin2_0_good:          // alors pin2_cnt_cur = 0, pin2_flag_isr = 1, flag_isr =1 puis global_check 
    move r0, 0
    move r2, pin2_cnt_cur
    st r0, r2, 0              // pin2_cnt_cur = 0
    
    move r3,1            
    move r2, debug_pin2_flag_isr  
    st r3, r2, 0          /* ==> set pin2_flag_isr à 1 true    */
    
    move r0, 1
    move r2, flag_isr
    st r0, r2, 0              /* ==> set flag_isr à 1 true    */
       
global_check:
  /* si flag_isr wake sinon halt */
    move r0, flag_isr
    ld r0, r0, 0
    and r0, r0, r0
    jump fin_check, eq /* pas d'isr */
    jump try_wakeup

fin_check:
halt
