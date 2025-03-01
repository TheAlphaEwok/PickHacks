#include <stdio.h>
#include "pico/stdlib.h"

//Initializes a GPIO pin as an output
void gpioInitOut(int GPIO_PIN);

//Initializes a GPIO pin as an input
void gpioInitIn(int GPIO_PIN);

int main()
{
    stdio_init_all();
    gpioInitOut(14);
    gpioInitOut(15);
    gpioInitIn(16);

    while (true) {
        int buttonState = gpio_get(16);
        if (buttonState == 0) {
            gpio_put(15, 0);
            gpio_put(14, 1);
        }
        else {
            gpio_put(15, 1);
            gpio_put(14,0);
        }
        sleep_ms(100);
    }
}


void gpioInitOut(int GPIO_PIN) {
    gpio_init(GPIO_PIN);
    gpio_set_dir(GPIO_PIN, GPIO_OUT);
}

void gpioInitIn(int GPIO_PIN) {
    gpio_init(GPIO_PIN);
    gpio_set_dir(GPIO_PIN, GPIO_IN);
    gpio_pull_up(GPIO_PIN);
}