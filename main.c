#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "FreeRTOS.h"
#include "task.h"

// Base Morse timing unit. 1 unit = length of a dot.
#define UNIT_MS         200
#define DOT_MS          (1 * UNIT_MS)
#define DASH_MS         (3 * UNIT_MS)
#define SYMBOL_GAP_MS   (1 * UNIT_MS)   // gap between symbols within the same letter
#define LETTER_GAP_MS   (3 * UNIT_MS)   // gap between letters (1 symbol gap already
                                         // elapsed, so this adds 2 more)
#define WORD_GAP_MS     (7 * UNIT_MS)   // gap between words (1 symbol gap already
                                         // elapsed, so this adds 6 more)

static inline void led_set(bool on)
{
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
}

static void symbol(uint32_t on_ms)
{
    led_set(true);
    vTaskDelay(pdMS_TO_TICKS(on_ms));
    led_set(false);
    vTaskDelay(pdMS_TO_TICKS(SYMBOL_GAP_MS));
}

static inline void dot(void)  { symbol(DOT_MS); }
static inline void dash(void) { symbol(DASH_MS); }

// letter/word gap delays need to subtract the 1 unit already delayed inside symbol()
static inline void letter_gap(void) { vTaskDelay(pdMS_TO_TICKS(LETTER_GAP_MS - SYMBOL_GAP_MS)); }
static inline void word_gap(void)   { vTaskDelay(pdMS_TO_TICKS(WORD_GAP_MS - SYMBOL_GAP_MS)); }

// configCHECK_FOR_STACK_OVERFLOW=2 requires this hook to be provided, or the link will fail
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void) xTask;
    panic("stack overflow in task: %s", pcTaskName);
}

// configUSE_MALLOC_FAILED_HOOK=1 requires this hook to be provided
void vApplicationMallocFailedHook(void)
{
    panic("malloc failed");
}

static void sos_task(__unused void *params)
{
    printf("[sos_task] started\n");

    int rc = cyw43_arch_init();
    printf("[sos_task] cyw43_arch_init() returned %d\n", rc);

    if (rc) {
        printf("[sos_task] cyw43 init FAILED, deleting task\n");
        vTaskDelete(NULL);
    }

    printf("[sos_task] entering SOS loop\n");

    for (;;) {
        printf("[sos_task] S\n");
        dot(); dot(); dot();       // S
        letter_gap();
        printf("[sos_task] O\n");
        dash(); dash(); dash();    // O
        letter_gap();
        printf("[sos_task] S\n");
        dot(); dot(); dot();       // S
        word_gap();
    }
}

int main(void)
{
    stdio_init_all();
    sleep_ms(3000);   // give you time to open a serial monitor; otherwise the boot
                       // messages will be missed
    printf("\n[main] boot, creating SOS task\n");

    xTaskCreate(
        sos_task,
        "SOS",
        configMINIMAL_STACK_SIZE + 256,
        NULL,
        tskIDLE_PRIORITY + 1,
        NULL
    );

    printf("[main] starting scheduler\n");
    vTaskStartScheduler();

    // execution should never return here once the scheduler starts; if it does,
    // heap is exhausted or something is misconfigured
    printf("[main] !!! scheduler returned, this should never happen !!!\n");
    for (;;) { }
}
