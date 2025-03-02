#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "time.h"

//Project Title: The Dles

#define I2C_PORT i2c1
#define I2C_SDA_PIN 26
#define I2C_SCL_PIN 27
#define I2C_FREQ 100000 
#define LCD_ADDR 0x27    

// LCD Commands
#define LCD_CLEAR 0x01
#define LCD_RETURN_HOME 0x02
#define LCD_ENTRY_MODE_SET 0x06
#define LCD_DISPLAY_ON 0x0C
#define LCD_FUNCTION_SET 0x28
#define LCD_INIT 0x33

//Initializes a GPIO pin as an output
void gpioInitOut(int GPIO_PIN);

//Initializes a GPIO pin as an input
void gpioInitIn(int GPIO_PIN);

//Desc: Plays sound on buzzer for given time at given frequency
//Input: Frequency in Hz, duration in seconds, and pin buzzer is connected to
void buzzer(int freq, double time, int GPIO_PIN);

// Desc: Initializes I2C
void i2c_init_lcd();

// Desc: Sends a command to the LCD
void lcd_send_cmd(char cmd);

// Desc: Sendd data (characters) to the LCD
void lcd_send_data(char data);

// Desc: Initialize the LCD
void lcd_init();

// Desc: Sets cursor at given coordinate
void lcd_set_cursor(int row, int col);

// Desc: Uses character pointer to display string
void lcd_send_string(const char *str);

// Desc: Clears LCD Display
void lcd_clear();

// Desc: Takes in 4 button inputs and returns the direction in degree that they indicate
// Input: 4 GPIO Pins which correspond to buttons
// Return: Integer value in degrees
int getDirection(int UP, int DOWN, int LEFT, int RIGHT);

// Desc: Play 5 seconds of a random song on a list and have user guess from multiple options
// Inputs: Genre (A == Anime, P == Pop, R == Rap)
void fiveSecondSongQuiz(char genre, char difficulty);

// Desc: Play song given a genre and number
void songs(char genre, int choice);

//Desc: Wordle game
void wordle();

// Main function
int main()
{
    stdio_init_all();
    i2c_init_lcd();
    lcd_init();
    for(int i = 0; i < 19; i++) {
        if(i < 10 || i > 13){
            gpioInitOut(i);            
        }
    }
    gpioInitOut(16);
    gpioInitOut(20);
    gpioInitIn(10);
    gpioInitIn(11);
    gpioInitIn(12);
    gpioInitIn(13);

    // Makes Sure LCD is configrued correctly
    lcd_set_cursor(0, 0);
    lcd_send_string("The Dles");
    lcd_set_cursor(1, 0);
    lcd_send_string("By: Eli Smith");
    sleep_ms(2000);
    lcd_send_cmd(LCD_CLEAR);

    srand(time(NULL));
    int yellow = 1, white = 1, red = 1, blue = 1;
    lcd_send_string("Y:Musedle");
    lcd_set_cursor(1,0);
    lcd_send_string("W:Wordle");

    while (1) {
            yellow = gpio_get(10);
            white = gpio_get(11);
            blue = gpio_get(12);
            red = gpio_get(13);
            if (yellow == 0) {
                lcd_clear();
                fiveSecondSongQuiz('A', 'E');
                lcd_clear();
                lcd_set_cursor(0,0);
                lcd_send_string("Y:Musedle");
                lcd_set_cursor(1,0);
                lcd_send_string("W:Wordle");
            } else if (white == 0) {
                wordle();
                lcd_clear();
                lcd_set_cursor(0,0);
                lcd_send_string("Y:Musedle");
                lcd_set_cursor(1,0);
                lcd_send_string("W:Wordle");
            } else if (blue == 0) {

            } else if (red == 0) {

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

void i2c_init_lcd() {
    i2c_init(I2C_PORT, I2C_FREQ);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
}

void lcd_send_cmd(char cmd) {
    char data_u = (cmd & 0xF0);
    char data_l = ((cmd << 4) & 0xF0);
    uint8_t data_t[2] = {
        data_u | 0x0C,  // en=1, rs=0
        data_u | 0x08  // en=0, rs=0
    };
    i2c_write_blocking(I2C_PORT, LCD_ADDR, data_t, 2, false);
    sleep_ms(5);
    data_t[0] = data_l | 0x0C;  // en=1, rs=0
    data_t[1] = data_l | 0x08;   // en=0, rs=0
    i2c_write_blocking(I2C_PORT, LCD_ADDR, data_t, 2, false);
    sleep_ms(5);
}

void lcd_send_data(char data) {
    char data_u = (data & 0xF0);
    char data_l = ((data << 4) & 0xF0);
    uint8_t data_t[2] = {
        data_u | 0x0D,  // en=1, rs=1
        data_u | 0x09  // en=0, rs=1
       
    }; 
    i2c_write_blocking(I2C_PORT, LCD_ADDR, data_t, 2, false);
    sleep_ms(3);
    
    data_t[0] = data_l | 0x0D;  // en=1, rs=1
    data_t[1] = data_l | 0x09;  // en=0, rs=1

    i2c_write_blocking(I2C_PORT, LCD_ADDR, data_t, 2, false);
    sleep_ms(3);
}

void lcd_init() {
    sleep_ms(50);
    lcd_send_cmd(LCD_INIT);
    lcd_send_cmd(0x32);
    lcd_send_cmd(LCD_FUNCTION_SET);
    lcd_send_cmd(LCD_DISPLAY_ON); 
    lcd_send_cmd(LCD_ENTRY_MODE_SET);
    lcd_send_cmd(LCD_CLEAR);
    sleep_ms(5);
}

void lcd_set_cursor(int row, int col) {
    int addr = (row == 0) ? (0x80 + col) : (0xC0 + col);
    lcd_send_cmd(addr);
}

void lcd_send_string(const char *str) {
    while (*str) {
        lcd_send_data(*str++);
    }
}

void lcd_clear() {
    lcd_send_cmd(LCD_CLEAR);
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
    srand(time(NULL));
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_send_string("Musedle Loading");
    sleep_ms(2000);
    lcd_clear();
    if(genre == 'A' && difficulty == 'E') {
        int answer = rand()%3+1;
        songs(genre, answer);
        lcd_set_cursor(0, 0);
        lcd_send_string("What opening ");
        lcd_set_cursor(1, 0);
        lcd_send_string("was that?");
        sleep_ms(1000);
        lcd_send_cmd(LCD_CLEAR);
        lcd_send_string("Y: Tokyo Ghoul");
        lcd_set_cursor(1, 0);
        lcd_send_string("W: AOT");
        sleep_ms(500);
        lcd_send_cmd(LCD_CLEAR);
        lcd_send_string("B: JJK");
        lcd_set_cursor(1, 0);
        lcd_send_string("R: No Clue");
        sleep_ms(500);
        int direction = getDirection(10,11,12,13);
        while (direction != 0 && direction != 90 && direction != 180 && direction != 270) {
            lcd_send_cmd(LCD_CLEAR);
            lcd_send_string("Y: Tokyo Ghoul");
            lcd_set_cursor(1, 0);
            lcd_send_string("W: AOT");
            sleep_ms(1000);
            lcd_send_cmd(LCD_CLEAR);
            lcd_send_string("B: JJK");
            lcd_set_cursor(1, 0);
            lcd_send_string("R: No Clue");
            sleep_ms(1000);
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
        if (userAns == answer) {
            buzzer(2960, 0.1, 20);
            sleep_ms(100);
            buzzer(2960, 0.1, 20);
            sleep_ms(100);
            buzzer(2960, 0.1, 20);
            lcd_send_cmd(LCD_CLEAR);
            lcd_send_string("That's Correct!");
            sleep_ms(1500);
        } else {
            buzzer(147, 1, 20);
            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_send_string("Incorrect :(");
            sleep_ms(1500);
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

void wordle() {
    srand(time(NULL));
    lcd_clear();
    lcd_set_cursor(0, 0);
    lcd_send_string("Wordle Loading");
    sleep_ms(2000);
    int attempt = 1;
    bool win = false;
    char wordBank[5][5], test[5];
    char alphabet[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    wordBank[0][0] = 'H';
    wordBank[0][1] = 'A';
    wordBank[0][2] = 'C';
    wordBank[0][3] = 'K';
    wordBank[0][4] = 'S';
    wordBank[1][0] = 'P';
    wordBank[1][1] = 'U';
    wordBank[1][2] = 'P';
    wordBank[1][3] = 'P';
    wordBank[1][4] = 'Y';
    wordBank[2][0] = 'H';
    wordBank[2][1] = 'I';
    wordBank[2][2] = 'L';
    wordBank[2][3] = 'L';
    wordBank[2][4] = 'Y';
    wordBank[3][0] = 'B';
    wordBank[3][1] = 'E';
    wordBank[3][2] = 'A';
    wordBank[3][3] = 'C';
    wordBank[3][4] = 'H';
    wordBank[4][0] = 'C';
    wordBank[4][1] = 'O';
    wordBank[4][2] = 'D';
    wordBank[4][3] = 'E';
    wordBank[4][4] = 'S';
    char guess[5][26];
    for(int j = 0; j < 5; ++j) {
        for(int i = 0; i < 26; ++i) {
            guess[j][i] = alphabet[i];
        }
    }
    int choice = rand()%5;
    char answer[5];
    for(int i = 0; i < 5; ++i) {
        answer[i] = wordBank[choice][i];
    }
    lcd_clear();
    while(attempt < 6 && win != true) {
        int length = 1, letter1 = 0, letter2 = 0, letter3 = 0, letter4 = 0, letter5 = 0, letter, total = 0;
        lcd_set_cursor(0, 0);
        lcd_send_string("Attempt ");
        lcd_send_data(attempt + '0');
        lcd_send_data(':');
        lcd_set_cursor(0, 11);
        for(int i = 0; i < 5; ++i) {
            lcd_send_data(guess[i][0]);
            sleep_ms(100);
        }
        lcd_set_cursor(0, 11);
        while(length != 6) {
            if(gpio_get(10) == 0) {
                if (length == 1) {
                    if(letter1 == 25) {                        
                        letter1 = 0;
                        } else {
                            letter1++;
                        }
                        letter = letter1;
                    } else if (length == 2) {
                        if(letter2 == 25) {
                            letter2 = 0;
                        } else {
                            letter2++;
                        }
                        letter = letter2;
                    } else if (length == 3) {
                        if(letter3 == 25) {
                            letter3 = 0;
                        } else {
                            letter3++;
                        }
                        letter = letter3;
                    }  else if (length == 4) {
                        if(letter4 == 25) {
                            letter4 = 0;
                        } else {
                            letter4++;
                        }
                        letter = letter4;
                    } else if (length == 5) {
                        if(letter5 == 25) {
                            letter5 = 0;
                        } else {
                            letter5++;
                        }
                        letter = letter5;
                    }
                    int position = length + 10;
                    lcd_set_cursor(0, position);
                    lcd_send_data(guess[length-1][letter]);
                    sleep_ms(250); 
                } else if(gpio_get(11) == 0) {
                    if (length == 1) {
                        if(letter1 == 0) {                        
                            letter1 = 25;
                            } else {
                                letter1--;
                            }
                            letter = letter1;
                        } else if (length == 2) {
                            if(letter2 == 0) {
                                letter2 = 25;
                            } else {
                                letter2--;
                            }
                            letter = letter2;
                        } else if (length == 3) {
                            if(letter3 == 0) {
                                letter3 = 25;
                            } else {
                                letter3--;
                            }
                            letter = letter3;
                        }  else if (length == 4) {
                            if(letter4 == 0) {
                                letter4 = 25;
                            } else {
                                letter4--;
                            }
                            letter = letter4;
                        } else if (length == 5) {
                            if(letter5 == 0) {
                                letter5 = 25;
                            } else {
                                letter5--;
                            }
                            letter = letter5;
                        }
                        int position = length + 10;
                        lcd_set_cursor(0, position);
                        lcd_send_data(guess[length - 1][letter]);
                        sleep_ms(250); 
                } else if (gpio_get(12) == 0) {
                    if(length > 1) {
                        length--;
                    }
                    sleep_ms(250);
                }  else if (gpio_get(13) == 0) {
                    if(length < 5) {
                        length++;
                    } else {
                        test[0] = guess[0][letter1];
                        test[1] = guess[1][letter2];
                        test[2] = guess[2][letter3];
                        test[3] = guess[3][letter4];
                        test[4] = guess[4][letter5];
                        lcd_set_cursor(1, 0);
                        lcd_send_string("Last Guess:");
                        lcd_set_cursor(1, 11);
                        for(int i = 0; i < 5; ++i) {
                            lcd_send_data(test[i]);
                        }
                        if(test[0] == answer[0]) {
                            gpio_put(0, 1);
                            gpio_put(5, 0);
                            gpio_put(14, 0);
                            total++;
                        } else if (test[0] == answer[1] || test[0] == answer[2] || test[0] == answer[3] || test[0] == answer[4]) {
                            gpio_put(0, 0);
                            gpio_put(5, 1);
                            gpio_put(14, 0);
                        } else {
                            gpio_put(0, 0);
                            gpio_put(5, 0);
                            gpio_put(14, 1);
                        }
                        if(test[1] == answer[1]) {
                            gpio_put(1, 1);
                            gpio_put(6, 0);
                            gpio_put(15, 0);
                            total++;
                        } else if (test[1] == answer[0] || test[1] == answer[2] || test[1] == answer[3] || test[1] == answer[4]) {
                            gpio_put(1, 0);
                            gpio_put(6, 1);
                            gpio_put(15, 0);
                        } else {
                            gpio_put(1, 0);
                            gpio_put(6, 0);
                            gpio_put(15, 1);
                        }
                        if(test[2] == answer[2]) {
                            gpio_put(2, 1);
                            gpio_put(7, 0);
                            gpio_put(16, 0);
                            total++;
                        } else if (test[2] == answer[0] || test[2] == answer[1] || test[2] == answer[3] || test[2] == answer[4]) {
                            gpio_put(2, 0);
                            gpio_put(7, 1);
                            gpio_put(16, 0);
                        } else {
                            gpio_put(2, 0);
                            gpio_put(7, 0);
                            gpio_put(16, 1);
                        }
                        if(test[3] == answer[3]) {
                            gpio_put(3, 1);
                            gpio_put(8, 0);
                            gpio_put(17, 0);
                            total++;
                        } else if (test[3] == answer[0] || test[3] == answer[1] || test[3] == answer[2] || test[0] == answer[4]) {
                            gpio_put(3, 0);
                            gpio_put(8, 1);
                            gpio_put(17, 0);
                        } else {
                            gpio_put(3, 0);
                            gpio_put(8, 0);
                            gpio_put(17, 1);
                        }
                        if(test[4] == answer[4]) {
                            gpio_put(4, 1);
                            gpio_put(9, 0);
                            gpio_put(18, 0);
                            total++;
                        } else if(test[4] == answer[0] || test[4] == answer[1] || test[4] == answer[2] || test[4] == answer[3]) {
                            gpio_put(4, 0);
                            gpio_put(9, 1);
                            gpio_put(18, 0);
                        } else {
                            gpio_put(4, 0);
                            gpio_put(9, 0);
                            gpio_put(18, 1);
                        }
                        if(total == 5) {
                            win = true;
                        }
                        length++;
                        attempt++;
                    }
                    sleep_ms(250);
                }
            }
        }
        lcd_set_cursor(0,0);
        if(win == true) {
            lcd_clear();
            lcd_send_string("You Got It!");
            lcd_set_cursor(1, 0);
            lcd_send_string("Answer: ");
            for(int i = 0; i < 5; ++i) {
                lcd_send_data(answer[i]);
            }
            sleep_ms(2000);
        } else {
            lcd_clear();
            lcd_send_string("You are out");
            lcd_set_cursor(1, 0);
            lcd_send_string("of tries");
            sleep_ms(2000);
            lcd_clear();
            lcd_set_cursor(0, 0);
            lcd_send_string("The word was:");
            lcd_set_cursor(1, 0);
            for(int i = 0; i < 5; ++i) {
                lcd_send_data(answer[i]);
            }
            sleep_ms(2000);
        }
}