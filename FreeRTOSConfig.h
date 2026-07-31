#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

// Required by the RP2350 ARM_NTZ port (portmacrocommon.h will #error out if these
// aren't defined)
#define configENABLE_FPU                       1   // Cortex-M33 has a hardware FPU, so enable it
#define configENABLE_MPU                       0   // not needed for this demo (no MPU-based task isolation)
#define configENABLE_TRUSTZONE                 0   // NTZ = No TrustZone; this port doesn't use TZ, always 0
#define configRUN_FREERTOS_SECURE_ONLY         1   // not guarded by an #error, so missing it won't block
                                                    // the build — but the very first context switch will
                                                    // take the wrong SVC/exception vector and hard fault

// Required by the RP2350 SMP port
#define configNUMBER_OF_CORES                  1   // core0 only; this SOS demo doesn't need dual-core
#define configTICK_RATE_HZ                     1000
#define configCPU_CLOCK_HZ                     133000000
#define configUSE_PREEMPTION                   1
#define configUSE_TIME_SLICING                 1
#define configMAX_PRIORITIES                   8
#define configMINIMAL_STACK_SIZE               256
#define configTOTAL_HEAP_SIZE                  (64 * 1024)
#define configMAX_TASK_NAME_LEN                16
#define configUSE_16_BIT_TICKS                 0
#define configIDLE_SHOULD_YIELD                1
#define configUSE_MUTEXES                      1
#define configUSE_RECURSIVE_MUTEXES            1
#define configUSE_COUNTING_SEMAPHORES          1
#define configQUEUE_REGISTRY_SIZE              8
#define configUSE_TASK_NOTIFICATIONS           1
#define configUSE_TIMERS                       1   // the RP2350 port relies on the timer task for
                                                    // deferred ISR calls internally — this must be on
#define configTIMER_TASK_PRIORITY              (configMAX_PRIORITIES - 1)
#define configTIMER_QUEUE_LENGTH               10
#define configTIMER_TASK_STACK_DEPTH           configMINIMAL_STACK_SIZE
#define configSUPPORT_STATIC_ALLOCATION         0
#define configSUPPORT_DYNAMIC_ALLOCATION        1

// Without configASSERT defined, vPortRecursiveLock() in portmacro.h produces an
// implicit-declaration warning (and a failed assert won't actually halt for debugging)
#include <assert.h>
#define configASSERT(x) assert(x)

// hooks / debug
#define configUSE_IDLE_HOOK                    0
#define configUSE_TICK_HOOK                    0
#define configCHECK_FOR_STACK_OVERFLOW         2
#define configUSE_MALLOC_FAILED_HOOK           1

// runtime stats aren't used here, disabled to cut overhead
#define configGENERATE_RUN_TIME_STATS          0
#define configUSE_TRACE_FACILITY               0
#define configUSE_STATS_FORMATTING_FUNCTIONS   0

// API switches
#define INCLUDE_xTimerPendFunctionCall         1   // xTimerPendFunctionCallFromISR needs this in
                                                    // addition to configUSE_TIMERS
#define INCLUDE_vTaskPrioritySet               1
#define INCLUDE_uxTaskPriorityGet              1
#define INCLUDE_vTaskDelete                    1
#define INCLUDE_vTaskDelay                     1
#define INCLUDE_vTaskDelayUntil                1
#define INCLUDE_xTaskGetSchedulerState         1
#define INCLUDE_uxTaskGetStackHighWaterMark    1

// Cortex-M33 interrupt priority settings (kept consistent with pico-examples/freertos)
#define configPRIO_BITS                        4
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY        15
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY   3
#define configKERNEL_INTERRUPT_PRIORITY \
    (configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))
#define configMAX_SYSCALL_INTERRUPT_PRIORITY \
    (configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS))

#endif // FREERTOS_CONFIG_H
