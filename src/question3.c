#include "question3.h"
#include "uart.h"
#include "mbox.h"
#include "printf.h"

#define MAX_SHOT 30
#define ENEMY_SPEED 50000
#define FIRE_W 80
#define FIRE_H 80
#define SPACESHIP_W 180
#define SPACESHIP_H 180
#define ENEMY_W 91
#define ENEMY_H 80
#define MAX_ENEMY 5
#define FIRE_W_FROM_ENEMY 80

void clearCurrentFire(int startX, int startY) {
    int x = 1;
    int y = 1;
    while (y < FIRE_H) { 
        
        drawPixelARGB32(x + startX, y + startY, 0x00000000);
        x++;
        if (x >= FIRE_W) { 
            x = 1;
            y++;
        }
        
    }
}

void displayFire(int startX, int startY) {
    int x = 1;
    int y = 1;
    while (y < FIRE_H) { 
        
        if ((fire[y][x] != 0x00FFFFFF) && (fire[y][x] != 0x00FFFEFF) && (fire[y][x] != 0x00FFFFFD) ) {
            drawPixelARGB32(x + startX, y + startY, fire[y][x]);
        }else {
            // drawPixelARGB32(y, x, 0x00000000);
        }
        x++;
        if (x >= FIRE_W) { 
            x = 1;
            y++;
        }
        
    }
}

void displayFire_enemy(int startX, int startY) {
    int x = FIRE_W - 1;
    int y = FIRE_H - 1;
    int x_print = 1;
    int y_print = 1;
    while (y >= 0) { 
        
        if ((fire_180[y_print][x_print] != 0x00FFFFFF) && (fire_180[y_print][x_print] != 0x00FFFEFF) && (fire_180[y_print][x_print] != 0x00FFFFFD) ) {
            drawPixelARGB32(x_print + startX, y_print + startY, fire_180[y_print][x_print]);
        }else{
            // drawPixelARGB32(y, x, 0x00000000);
        }
        x--;
        if (x <= 0) { 
            x = FIRE_W - 1;
            y--;
        }
        x_print++;
        if (x_print >= FIRE_W) {
            x_print = 1;
            y_print++;
        }
        
    }
}

void clearCurrentSpaceShip(int startX, int startY) {
    int x = 1;
    int y = 1;
    while (y < 180) { 
        drawPixelARGB32(x + startX, y + startY, 0x00000000);
        x++;
        if (x >= 180) { 
            x = 20;
            y++;
        }
        
    }
}

void displaySpcaeShip(int startX, int startY) {
    int x = 1;
    int y = 1;
    while (y < 180) { 
        if ((spaceShip[y][x] != 0x00FFFFFF) && (spaceShip[y][x] != 0x00FFFEFF) && (spaceShip[y][x] != 0x00FFFFFD) ) {
            drawPixelARGB32(x + startX, y + startY, spaceShip[y][x]);
        }else {
            // drawPixelARGB32(y, x, 0x00000000);
        }
        x++;
        if (x >= 180) { 
            x = 20;
            y++;
        }
        
    }
}

void clearMultipleShot(int x_fire_arr[], int y_fire_arr[], int last_x_fire_arr[], int last_y_arr[], int shooting_command_arr[], int shooting_x_arr[], int shooting_y_arr[], int x, int y) {
    
    for (int i = 0; i < MAX_SHOT; i++) {
        if (last_x_fire_arr[i] != x_fire_arr[i]) {
            clearCurrentFire(shooting_x_arr[i] + last_x_fire_arr[i], shooting_y_arr[i] + y_fire_arr[i]);
            if (x_fire_arr[i] + 10 > 850) {
                shooting_y_arr[i] = y;
            }
            last_x_fire_arr[i] = x_fire_arr[i];
        }
    }
}

void clearMultipleShot_enemy(int x_fire_arr[], int y_fire_arr[], int last_x_fire_arr[], int last_y_arr[], int shooting_command_arr[], int shooting_x_arr[], int shooting_y_arr[], int x[], int y[]) {
    for(int i = 0; i < MAX_ENEMY; i++) {
        if (last_x_fire_arr[i] != shooting_x_arr[i]) {
            clearCurrentFire(last_x_fire_arr[i], shooting_y_arr[i]);
            if (shooting_x_arr[i] - 10 < 15) {
                shooting_y_arr[i] = y[i];
            }
            last_x_fire_arr[i] = shooting_x_arr[i];
        }
    }
}

void clearCurrentEnemy(int startX, int startY) {
    int x = 1;
    int y = 1;
    while (y < 80) { 
        drawPixelARGB32(x + startX, y + startY, 0x00000000);
        x++;
        if (x >= 91) { 
            x = 1;
            y++;
        }
        
    }
}

void checkHit(int shooting_x_arr[], int x_fire_arr[], int shooting_y_arr[], int y_fire_arr[], int i, int x_enemy_arr[], int y_enemy_arr[], int isEnemy_arr[], int shooting_command_arr[], int *isHit, int x_fire_from_ship) {
    for (int k = 0; k < MAX_ENEMY; k++) {
        int fire_x_des = shooting_x_arr[i] + x_fire_arr[i] + FIRE_W;
        int fire_y_des = shooting_y_arr[i] + y_fire_arr[i] + FIRE_H;
        int enemy_x_des = x_enemy_arr[k] + ENEMY_W;
        int enemy_y_des = y_enemy_arr[k] + ENEMY_H;
        // check hit
        if (isEnemy_arr[k]) {
            if (((fire_x_des >= x_enemy_arr[k]) && (fire_x_des <= enemy_x_des)) && ((fire_y_des >= y_enemy_arr[k]) && (fire_y_des <= enemy_y_des))) {
                shooting_command_arr[i] = 0;
                isEnemy_arr[k] = 0;
                x_fire_arr[i] = x_fire_from_ship;
                clearCurrentEnemy(x_enemy_arr[k], y_enemy_arr[k]);
                (*isHit) = 1;
                uart_puts("HITS\n");
                uart_puts("------\n");
            }
        }
    }
}

void displayMultipleShots(int shooting_command_arr[], int shooting_x_arr[], int shooting_y_arr[], int x_fire_arr[], int y_fire_arr[], int x_enemy_arr[], int y_enemy_arr[], int isEnemy_arr[], int x_fire_from_ship) {
    for (int i = 0; i < MAX_SHOT; i++) {
        if (shooting_command_arr[i] == 1) {
            int isHit = 0;
            checkHit(shooting_x_arr, x_fire_arr, shooting_y_arr, y_fire_arr, i, x_enemy_arr, y_enemy_arr, isEnemy_arr, shooting_command_arr, &isHit, x_fire_from_ship);
            if (isHit == 0) {
                displayFire(shooting_x_arr[i] + x_fire_arr[i], shooting_y_arr[i] + y_fire_arr[i]);
            }else {
                clearCurrentFire(shooting_x_arr[i] + x_fire_arr[i], shooting_y_arr[i] + y_fire_arr[i]);
            }
            
        }
    }
}

void displayMultipleShots_enemy(int shooting_command_arr[], int shooting_x_arr[], int shooting_y_arr[], int x_fire_arr[], int y_fire_arr[], int x_enemy_arr[], int y_enemy_arr[], int isEnemy_arr[], int x_fire_from_ship[]) {
    for (int i = 0; i < MAX_ENEMY; i++) {
        if (shooting_command_arr[i] == 1) {
            // int isHit = 0;
            // checkHit(shooting_x_arr, x_fire_arr, shooting_y_arr, y_fire_arr, i, x_enemy_arr, y_enemy_arr, isEnemy_arr, shooting_command_arr, &isHit, x_fire_from_ship);
            // if (isHit == 0) {
                
            // }else {
            //     clearCurrentFire(shooting_x_arr[i] + x_fire_arr[i], shooting_y_arr[i] + y_fire_arr[i]);
            // }
            displayFire_enemy(shooting_x_arr[i] + x_fire_arr[i], shooting_y_arr[i] + y_fire_arr[i]);
        }
    }
}

void displayNumber(int i) {
    switch (i)
    {
    case 0:
        uart_sendc('0');
        break;
    case 1:
        uart_sendc('1');
        break;
    case 2:
        uart_sendc('2');
        break;
    case 3:
        uart_sendc('3');
        break;
    case 4:
        uart_sendc('4');
        break;
    case 5:
        uart_sendc('5');
        break;
    case 6:
        uart_sendc('6');
        break;
    case 7:
        uart_sendc('7');
        break;
    case 8:
        uart_sendc('8');
        break;
    case 9:
        uart_sendc('9');
        break;
    default:
        break;
    }
}

void shoot(int shooting_command_arr[], int shooting_x_arr[], int shooting_y_arr[], int x_fire_arr[], int y_fire_arr[], int x_fire_from_ship) {
    for (int i = 0; i < MAX_SHOT; i++) {
        if (shooting_command_arr[i] == 1) {
            
            x_fire_arr[i] += 5;
            if (x_fire_arr[i] + 5 > 950) {
                x_fire_arr[i] = x_fire_from_ship;
                shooting_command_arr[i] = 0;
            }
        }
        
    }
}

void shoot_enemy(int shooting_command_arr[], int shooting_x_arr[], int shooting_y_arr[], int x_fire_arr[], int y_fire_arr[], int x_fire_from_ship[]) {
    for (int i = 0; i < MAX_ENEMY; i++) {
        if (shooting_command_arr[i] == 1) {
            shooting_x_arr[i] -= 5;
            if (shooting_x_arr[i] - 5 < 15) {
                shooting_x_arr[i] = x_fire_from_ship[i] - FIRE_W_FROM_ENEMY;
                shooting_command_arr[i] = 0;
            }
        }
        
    }
}


void enableShot(int *shotCount, int shooting_command_arr[], int x, int y, int shooting_x_arr[], int shooting_y_arr[]) {
    if ((*shotCount) + 1 < MAX_SHOT) {
        (*shotCount) += 1;
        for (int i = 0; i < (*shotCount); i++) {
            if (shooting_command_arr[i] != 1) {
                shooting_command_arr[i] = 1;
                shooting_x_arr[i] = x;
                shooting_y_arr[i] = y;
                break;
            }
        }
        if ((*shotCount) == MAX_SHOT - 1) {
            (*shotCount) = 0;
        }
    }
}



void displayEnemy(int startX, int startY) {
    int x = 1;
    int y = 1;
    while (y < 80) { 
        
        if ((enemy[y][x] != 0x00FFFFFF) && (enemy[y][x] != 0x00FFFEFF) && (enemy[y][x] != 0x00FFFFFD) ) {
            drawPixelARGB32(x + startX, y + startY, enemy[y][x]);
        }else {
            // drawPixelARGB32(y, x, 0x00000000);
        }
        x++;
        if (x >= 91) { 
            x = 1;
            y++;
        }
        
    }
}

void displayMultipleEnemy(int isEnemy_arr[], int x_enemy_arr[], int y_enemy_arr[]) {
    for (int i = 0; i < MAX_ENEMY; i++) {
        if (isEnemy_arr[i]) {
            displayEnemy(x_enemy_arr[i], y_enemy_arr[i]);
        }
    }
}

void clearMultipleEnemy(int last_y_enemy_arr[], int y_enemy_arr[], int x_enemy_arr[], int isEnemy_arr[]) {
    for (int i = 0; i < MAX_ENEMY; i++) {
        if (last_y_enemy_arr[i] != y_enemy_arr[i]) {
            if (isEnemy_arr[i]) {
                clearCurrentEnemy(x_enemy_arr[i], last_y_enemy_arr[i]);
                last_y_enemy_arr[i] = y_enemy_arr[i];
            }
            
        }
    }
}

void movingMultipleEnemy(int spaceShipToggle_arr[], int y_enemy_arr[]) {
    for (int i = 0; i < MAX_ENEMY; i++) {
        // shot moving code
        if (spaceShipToggle_arr[i]) {
            if (!(y_enemy_arr[i] + 20 > 700)) {
                y_enemy_arr[i] += 20;
                // displayEnemy(x_enemy, y_enemy);
            }else {
                spaceShipToggle_arr[i] = 0;
            }
        }else {
            if (!(y_enemy_arr[i] - 20 <= 20)) {
                y_enemy_arr[i] -= 20;
                // displayEnemy(x_enemy, y_enemy);
            }else {
                spaceShipToggle_arr[i] = 1;
            }
        }
    }
}

void gamePlay() {

    //--------------------------------------------------------------- set up for enemy movement-----------------------------------------------------------------------
    mBuf[0] = 17*4; // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
    mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message)
    
    mBuf[2] = 0x00040004; // TAG Identifier: Get clock rate
    mBuf[3] = 8; // Value buffer size in bytes (max of request and response lengths)
    mBuf[4] = 0; // REQUEST CODE = 0
    mBuf[5] = 0; // clock id: ARM system clock
    mBuf[6] = 0;
    mBuf[7] = MBOX_TAG_LAST; // clear output buffer (response data are mBuf[5] & mBuf[6])

    unsigned int x_enemy = 300;
    unsigned int y_enemy = 30;
    int last_x_enemy = x_enemy;
    int last_y_enemy = last_y_enemy;
    int spaceShipToggle = 1;
    int isEnemy = 1;

    // unsigned int x_enemy[MAX_ENEMY] = 0;
    // unsigned int y_enemy[MAX_ENEMY] = 30;
    // int last_x_enemy[MAX_ENEMY] = x_enemy;
    // int last_y_enemy[MAX_ENEMY] = last_y_enemy;
    // int spaceShipToggle[MAX_ENEMY] = 1;
    // int isEnemy[MAX_ENEMY] = 1;

    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP)) {
        uart_puts("Width: ");
        x_enemy = mBuf[5] - 100;
        uart_dec(mBuf[5]);
        uart_sendc('\n');
        uart_puts("Height: ");
        uart_dec(mBuf[6]);
    }
    // init max x_enemy
    int x_enemy_init = 100;
    unsigned int x_enemy_arr[MAX_ENEMY] = {};
    for (int i = 0; i < MAX_ENEMY; i++) {
        x_enemy_arr[i] = mBuf[5] - x_enemy_init;
        x_enemy_init += 100;
        if (x_enemy_init >= 400) {
            x_enemy_init = 100;
        }
    }
    
    int y_enemy_init = 30;
    unsigned int y_enemy_arr[MAX_ENEMY]= {};
    for (int i = 0; i < MAX_ENEMY; i++) {
        y_enemy_arr[i] = y_enemy_init;
        y_enemy_init += 30;
        if (y_enemy_init >= 500) {
            y_enemy_init = 30;
        }
    }
    int last_x_enemy_arr[MAX_ENEMY] = {x_enemy};
    int last_y_enemy_arr[MAX_ENEMY] = {last_y_enemy};
    int spaceShipToggle_arr[MAX_ENEMY] = {1};
    for (int i = 0; i < MAX_ENEMY; i++) {
        spaceShipToggle_arr[i] = 1;
    }
    int isEnemy_arr[MAX_ENEMY] = {1};
    for (int i = 0; i < MAX_ENEMY; i++) {
        isEnemy_arr[i] = 1;
    }

    // implement shots for enemy
    int x_fire_arr_enemy[MAX_ENEMY] = {0};
    for (int i = 0; i < MAX_ENEMY; i++) {
        x_fire_arr_enemy[i] = 0;
    }
    int y_fire_arr_enemy[MAX_ENEMY] = {0};
    for (int i = 0; i < MAX_ENEMY; i++) {
        y_fire_arr_enemy[i] = 0;
    }
    int last_x_fire_arr_enemy[MAX_ENEMY] = {x_fire_arr_enemy[0]};
    for (int i = 0; i < MAX_ENEMY; i++) {
        last_x_fire_arr_enemy[i] = x_fire_arr_enemy[i];
    }


    int last_y_arr_enemy[MAX_ENEMY] = {0};
    for (int i = 0; i < MAX_ENEMY; i++) {
        last_y_arr_enemy[i] = y_fire_arr_enemy[i];
    }
    int shooting_command_arr_enemy[MAX_ENEMY] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < MAX_ENEMY; i++) {
        shooting_command_arr_enemy[i] = 1;
    }
    int shooting_x_arr_enemy[MAX_ENEMY] = {0};
    for (int i = 0; i < MAX_ENEMY; i++) {
        shooting_x_arr_enemy[i] = x_enemy_arr[i] - FIRE_W_FROM_ENEMY;
    }

    int shooting_y_arr_enemy[MAX_ENEMY] = {0};
    for (int i = 0; i < MAX_ENEMY; i++) {
        shooting_y_arr_enemy[i] = y_enemy_arr[i];
    }

    // timer for enemy shoot
    unsigned int n_enemy_shoot = 30000;
    register unsigned long f_enemy_shoot, t_enemy_shoot, r_enemy_shoot, expiredTime_enemy_shoot;
    // Get the current counter frequency (Hz)
    asm volatile ("mrs %0, cntfrq_el0" : "=r"(f_enemy_shoot));
    // Read the current counter value
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t_enemy_shoot));
    // Calculate expire value for counter
    expiredTime_enemy_shoot = t_enemy_shoot + ( (f_enemy_shoot/1000)*n_enemy_shoot )/1000;
    
    
    unsigned int n_enemy = ENEMY_SPEED;
    register unsigned long f_enemy, t_enemy, r_enemy, expiredTime_enemy;
    // Get the current counter frequency (Hz)
    asm volatile ("mrs %0, cntfrq_el0" : "=r"(f_enemy));
    // Read the current counter value
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t_enemy));
    // Calculate expire value for counter
    expiredTime_enemy = t_enemy + ( (f_enemy/1000)*n_enemy )/1000;
    
    

    
    //----------------------------------------------------------------- MARK: setup for shots --------------------------------------------------------------------------
    // for timer purpose
    unsigned int n = 60;
    register unsigned long f, t, r, expiredTime;
    // Get the current counter frequency (Hz)
    asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
    // Read the current counter value
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
    // Calculate expire value for counter
    expiredTime = t + ( (f/1000)*n )/1000;

    // fire timer
    unsigned int n_fire = 6800;
    register unsigned long f_fire, t_fire, r_fire, expiredTime_fire;
    asm volatile ("mrs %0, cntfrq_el0" : "=r"(f_fire));
    // Read the current counter value
    asm volatile ("mrs %0, cntpct_el0" : "=r"(t_fire));
    // Calculate expire value for counter
    expiredTime_fire = t_fire + ( (f_fire/1000)*n_fire )/1000;

    asm volatile ("mrs %0, cntpct_el0" : "=r"(r));
    asm volatile ("mrs %0, cntpct_el0" : "=r"(r_fire));
    int x = 0;
    int y = 0;
    int x_fire_from_ship = 200;
    int y_fire_from_ship = 50;

    // implementing multiple shot
    int x_fire_arr[MAX_SHOT] = {x_fire_from_ship};
    int y_fire_arr[MAX_SHOT] = {y_fire_from_ship};
    int last_x_fire_arr[MAX_SHOT] = {x_fire_arr[0]};
    int last_y_arr[MAX_SHOT] = {y};
    int shooting_command_arr[MAX_SHOT] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < MAX_SHOT; i++) {
        shooting_command_arr[i] = 0;
    }
    int shooting_x_arr[MAX_SHOT] = {x};
    int shooting_y_arr[MAX_SHOT] = {y};
    //-------------------------------------

    int x_fire = x_fire_from_ship; // array
    int y_fire = y_fire_from_ship; // array
    int last_x_fire = x_fire; // array
    int last_y = y; // array
    int shooting = 1; // command array

    // shooting from spaceship (array)
    int shooting_x = x; 
    int shooting_y = y;

    //shot tracking
    int shotCount = 0;
    while (1)
    {
        if (r >= expiredTime) {
            if (last_y != y) {
                clearCurrentSpaceShip(x, last_y);
                last_y = y;
            }

            // clear multiple enemy
            clearMultipleEnemy(last_y_enemy_arr, y_enemy_arr, x_enemy_arr, isEnemy_arr);
            // shot clearing code
            clearMultipleShot(x_fire_arr, y_fire_arr, last_x_fire_arr, last_y_arr, shooting_command_arr, shooting_x_arr, shooting_y_arr, x, y);
            clearMultipleShot_enemy(x_fire_arr_enemy, y_fire_arr_enemy, last_x_fire_arr_enemy, last_y_arr_enemy, shooting_command_arr_enemy, shooting_x_arr_enemy, shooting_y_arr_enemy, x_enemy_arr, y_enemy_arr);
            displaySpcaeShip(x, y);

            
            
            displayMultipleEnemy(isEnemy_arr, x_enemy_arr, y_enemy_arr);
            // shot display code
            displayMultipleShots(shooting_command_arr, shooting_x_arr, shooting_y_arr, x_fire_arr, y_fire_arr, x_enemy_arr, y_enemy_arr, isEnemy_arr, x_fire_from_ship);
            displayMultipleShots_enemy(shooting_command_arr_enemy, shooting_x_arr_enemy, shooting_y_arr_enemy, x_fire_arr_enemy, y_fire_arr_enemy, x_fire_arr_enemy, y_enemy_arr, isEnemy_arr, x_enemy_arr);
            asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
            // Read the current counter value
            asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
            // Calculate expire value for counter
            expiredTime = t + ( (f/1000)*n )/1000;
        }
        asm volatile ("mrs %0, cntpct_el0" : "=r"(r));

        if (r_fire >= expiredTime_fire) {
            
            // shot moving code
            shoot(shooting_command_arr, shooting_x_arr, shooting_y_arr, x_fire_arr, y_fire_arr, x_fire_from_ship);
            shoot_enemy(shooting_command_arr_enemy, shooting_x_arr_enemy, shooting_y_arr_enemy, x_fire_arr_enemy, y_fire_arr_enemy, x_enemy_arr);
            asm volatile ("mrs %0, cntfrq_el0" : "=r"(f_fire));
            // Read the current counter value
            asm volatile ("mrs %0, cntpct_el0" : "=r"(t_fire));
            // Calculate expire value for counter
            expiredTime_fire = t_fire + ( (f_fire/1000)*n_fire )/1000;
        }
        asm volatile ("mrs %0, cntpct_el0" : "=r"(r_fire));


        if (r_enemy >= expiredTime_enemy) {
            // enemy moving in here
            movingMultipleEnemy(spaceShipToggle_arr, y_enemy_arr);
            
            asm volatile ("mrs %0, cntfrq_el0" : "=r"(f_enemy));
            // Read the current counter value
            asm volatile ("mrs %0, cntpct_el0" : "=r"(t_enemy));
            // Calculate expire value for counter
            expiredTime_enemy = t_enemy + ( (f_enemy/1000)*n_enemy )/1000;
        }
        asm volatile ("mrs %0, cntpct_el0" : "=r"(r_enemy));

        if (!(UART0_FR & UART0_FR_RXFE)) {
            char c = (unsigned char) (UART0_DR);
            c = (c == '\r' ? '\n' : c);
            if (c == 's') {
                
                if (!(y + 20 > 600)) {
                    y += 20;
                    displaySpcaeShip(x, y);
                }
            }

            if (c == 'w') {
                if (!(y - 20 <= 0)) {
                    y -= 20;
                    displaySpcaeShip(x, y);
                }
            }

            if (c == 'k') {
                enableShot(&shotCount, shooting_command_arr, x, y, shooting_x_arr, shooting_y_arr);
            }

            if (c == 127) {
                break;
            }
        }
    }

}