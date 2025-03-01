#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"

//Initializes a GPIO pin as an output
void gpioInitOut(int GPIO_PIN);

//Initializes a GPIO pin as an input
void gpioInitIn(int GPIO_PIN);

//Desc: Plays sound on buzzer for given time at given frequency
//Input: Frequency in Hz, duration in seconds, and pin buzzer is connected to
void buzzer(int freq, double time, int GPIO_PIN);

//Desc: Takes in 4 button inputs and returns the direction in degree that they indicate
//Input: 4 GPIO Pins which correspond to buttons
//Return: Integer value in degrees
int getDirection(int UP, int DOWN, int LEFT, int RIGHT);

//Desc: Play 5 seconds of a random song on a list and have user guess from multiple options
//Inputs: Genre (A == Anime, P == Pop, R == Rap)
void fiveSecondSongQuiz(char genre, char difficulty);


void songs(char genre, int choice);


int main()
{
    stdio_init_all();
    gpioInitOut(16);
    gpioInitOut(20);
    gpioInitIn(10);
    gpioInitIn(11);
    gpioInitIn(12);
    gpioInitIn(13);

    int yellow = 1;
    printf("Press the yellow button to start\n");

    while (1) {
        /*
        int direction = getDirection(10, 11, 12, 13);
        // rand()%(MAX-MIN+1) + MIN == Random number between MAX & MIN
        int random = rand()%6+2;
        if(random != -1) {
            printf("Direction: %d\n", random);
        }
        */
        yellow = gpio_get(10);
        if (yellow == 0) {
            fiveSecondSongQuiz('A', 'E');
            printf("Press the yellow button to play again\n");
        }
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

void buzzer(int freq, double time, int GPIO_PIN) {
    int duration = freq*time;
    double period = 1000000/freq;
    int halfWave = period/2;
    for(int i = 0; i < duration; i++) {
        gpio_put(GPIO_PIN, 1);
        sleep_us(halfWave);
        gpio_put(GPIO_PIN, 0);
        sleep_us(halfWave);
    }
    sleep_ms(50);
}


int getDirection(int UP, int DOWN, int LEFT, int RIGHT) {
    int degreeFromX = -1;
    if(gpio_get(10) == 0 || gpio_get(11) == 0 || gpio_get(12) == 0 || gpio_get(13) == 0) {
        sleep_ms(250);
    }
    int buttonYState = gpio_get(10);
    int buttonWState = gpio_get(11);
    int buttonBState = gpio_get(12);
    int buttonRState = gpio_get(13);

    if(buttonYState == 0 && buttonWState == 1 && buttonBState == 1 && buttonRState == 1 || buttonYState == 0 && buttonWState == 1 && buttonBState == 0 && buttonRState == 0) {
        degreeFromX = 180;
        return degreeFromX;
    } else if(buttonYState == 1 && buttonWState == 0 && buttonBState == 1 && buttonRState == 1 || buttonYState == 1 && buttonWState == 0 && buttonBState == 0 && buttonRState == 0) {
        degreeFromX = 0;
        return degreeFromX;
    } else if(buttonYState == 1 && buttonWState == 1 && buttonBState == 0 && buttonRState == 1 || buttonYState == 0 && buttonWState == 0 && buttonBState == 0 && buttonRState == 1) {
        degreeFromX = 90;
        return degreeFromX;
    }  else if(buttonYState == 1 && buttonWState == 1 && buttonBState == 1 && buttonRState == 0 || buttonYState == 0 && buttonWState == 0 && buttonBState == 1 && buttonRState == 0) {
        degreeFromX = 270;
        return degreeFromX;
    } else if(buttonYState == 0 && buttonWState == 1 && buttonBState == 0 && buttonRState == 1) {
        degreeFromX = 135;
        return degreeFromX;
    } else if(buttonYState == 1 && buttonWState == 0 && buttonBState == 0 && buttonRState == 1) {
        degreeFromX = 45;
        return degreeFromX;
    } else if(buttonYState == 0 && buttonWState == 1 && buttonBState == 1 && buttonRState == 0) {
        degreeFromX = 225;
        return degreeFromX;
    }  else if(buttonYState == 1 && buttonWState == 0 && buttonBState == 1 && buttonRState == 0) {
        degreeFromX = 315;
        return degreeFromX;
    } else {
        return degreeFromX;
    }
}

void fiveSecondSongQuiz(char genre, char difficulty) {
    if(genre == 'A' && difficulty == 'E') {
        int answer = rand()%3+1;
        songs(genre, answer);
        printf("Was that:\n1. Unravel - Tokyo Ghoul\n2. Red Swan - Attack On Titan\n3. Specialz - Jujutsu Kaisen\n4. No Clue\n");
        int direction = getDirection(10,11,12,13);
        while (direction != 0 && direction != 90 && direction != 180 && direction != 270) {
            direction = getDirection(10,11,12,13);
        }
        int buttonSelect;
        if(direction == 0) {
            buttonSelect = 2;
        } else if (direction == 90) {
            buttonSelect = 3;
        } else if (direction == 180) {
            buttonSelect = 1;
        } else if (direction == 270) {
            buttonSelect = 4;
        }
        int userAns = buttonSelect;
        printf("User Answer: %d\n", userAns);
        if (userAns == answer) {
            buzzer(2960, 0.1, 20);
            sleep_ms(100);
            buzzer(2960, 0.1, 20);
            sleep_ms(100);
            buzzer(2960, 0.1, 20);
            printf("Correct! It was %d.\n", answer);
        } else {
            buzzer(147, 1, 20);
            printf("Sorry that is incorrect. The correct answer was %d.\n", answer);
        }
    }
}

void songs(char genre, int choice) {
    //Unravel - Tokyo Ghoul - BPM 132 = BPS 2.2
    if(genre == 'A' && choice == 1) {
        buzzer(932, 0.5/2.2, 20);
        buzzer(1046, 1/2.2, 20);
        buzzer(932, 0.5/2.2, 20);
        buzzer(880, 0.5/2.2, 20);
        buzzer(784, 1/2.2, 20);
        buzzer(1046, 1/2.2, 20);
        buzzer(932, 1/2.2, 20);
        buzzer(880, 1/2.2, 20);
        buzzer(784, 1/2.2, 20);
        buzzer(784, 0.5/2.2, 20);
        buzzer(698, 1.5/2.2, 20);
        buzzer(622, 0.5/2.2, 20);
        buzzer(622, 0.75/2.2, 20);
        buzzer(698, 0.75/2.2, 20);
        buzzer(587, 1.75/2.2, 20);
    }
    // Red Swan - Attack on Titan - BPM 132? = BPS 2.2?
    else if(genre == 'A' && choice == 2) {
        buzzer(698, 2/2.2, 20);
        buzzer(659, 2/2.2, 20);
        buzzer(784, 1.5/2.2, 20);
        buzzer(698, 2.5/2.2, 20);
        buzzer(698, 2/2.2, 20);
        buzzer(659, 2/2.2, 20);
    }

    // Specialz - Jujustu Kaisen - BPM 117 = BPS 1.95
    else if(genre == 'A' && choice == 3) {
        buzzer(880, 0.5/1.95, 20);
        buzzer(740, 0.5/1.95, 20);
        buzzer(740, 0.5/1.95, 20);
        buzzer(880, 0.5/1.95, 20);
        sleep_ms((0.5/1.95)*1000);
        buzzer(740, 1/1.95, 20);
        buzzer(880, 0.5/1.95, 20);
        buzzer(740, 0.5/1.95, 20);
        buzzer(1109, 0.5/1.95, 20);
        buzzer(1109, 0.5/1.95, 20);
        buzzer(988, 0.25/1.95, 20);
        buzzer(880, 0.25/1.95, 20);
        buzzer(740, 0.25/1.95, 20);
        sleep_ms((1.75/1.95)*1000);
        buzzer(740, 0.5/1.95, 20);
        buzzer(1109, 0.5/1.95, 20);
        buzzer(1109, 0.5/1.95, 20);
        buzzer(988, 0.25/1.95, 20);
        buzzer(880, 0.25/1.95, 20);
        buzzer(740, 0.25/1.95, 20);
    }
}