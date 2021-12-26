#include <inc/os.h>

/**
 *  NOTE
 *
 *  The majority of this code is taken (with and without modification)
 *  from Quantum Leaps' QK ARM Cortex-M Port found here:
 *
 *  https://github.com/QuantumLeaps/qpn/tree/master/ports/arm-cm/qk/arm
 *
 *  The QK ARM Cortex-M Port is licensed under the GNU General Public
 *  License version 3.
 *
 *  https://github.com/QuantumLeaps/qpn
 *
 */

// clang-format off

// helper macros to "stringify" values
#define VAL(x) #x
#define STRINGIFY(x) VAL(x)

/**
 * @brief Tail chains AO activator to exceptions
 *        need to fabricate exception stack frame since OS is single stack
 * 
 */
__attribute__((naked, optimize("-fno-stack-protector"))) void PendSV_Handler()
{
    __asm volatile(
    " LDR r3,=0xE000ED04\n"
    " MOV r1,#1   \n"
    " LSL r1,r1,#27 \n"
#if (__ARM_FP != 0)
    " PUSH {r0,lr} \n"
#endif
    " MOV r0,#" STRINGIFY(OS_BASEPRI) "\n"
    " CPSID i \n"
    " MSR BASEPRI,r0 \n"
    " CPSIE i \n"
    " STR r1,[r3] \n"
    " LSR r3,r1,#3 \n"
    " LDR r2,=SchedulerActivateAO \n"
    " SUB r2,r2,#1 \n"
    " LDR r1,=AO_return \n"
    " SUB sp,sp,#8*4 \n"
    " ADD r0,sp,#5*4 \n"
    " STM r0!,{r1-r3} \n"
    " MOV r0,#6 \n"
    " MVN r0,r0 \n"
    " DSB \n"
    " BX r0 \n");
}

__attribute__((naked, optimize("-fno-stack-protector"))) void AO_return(void)
{
    __asm volatile(
#if (__ARM_FP != 0)
    "  MRS r0,CONTROL \n"
    "  BICS r0,r0,#4 \n"
    "  MSR CONTROL,r0 \n"
    "  ISB \n"
#endif    
    " LDR r0,=0xE000ED04 \n"
    " MOV r1,#1 \n"
    " LSL r1,r1,#31 \n"
    " STR r1,[r0] \n"
    " B . \n");
}

__attribute__((naked, optimize("-fno-stack-protector"))) void NMI_Handler(void)
{
    __asm volatile(
    " ADD sp,sp,#(8*4) \n"
    " MOV r0,#0 \n"
    " MSR BASEPRI,r0 \n"
#if (__ARM_FP != 0)
    " POP {r0,lr} \n"
    " DSB \n"
#endif  
    " BX lr \n");
}

// clang-format on
