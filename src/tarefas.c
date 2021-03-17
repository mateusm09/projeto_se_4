#include "system_tm4c1294.h"
#include "driverleds.h"
#include "cmsis_os2.h"

typedef struct
{
    uint8_t led;
    uint32_t period;
} taskArgs;

void blinkLed(void *args)
{
    taskArgs *arg = (taskArgs *)args;
    uint8_t toggle = 0;

    while (1)
    {
        toggle ^= arg->led;
        LEDWrite(arg->led, toggle);
        osDelay(arg->period);
    }
}

void main()
{
    LEDInit(LED1 | LED2 | LED3 | LED4);

    osKernelInitialize();

    taskArgs ledsArgs[4] = {
        {.led = LED1, .period = 200},
        {.led = LED2, .period = 300},
        {.led = LED3, .period = 500},
        {.led = LED4, .period = 700}};

    uint8_t i;
    for (i = 0; i < 4; i++)
    {
        osThreadNew(blinkLed, &ledsArgs[i], NULL);
    }

    if (osKernelGetState() == osKernelReady)
        osKernelStart();

    while (1)
        ;
}