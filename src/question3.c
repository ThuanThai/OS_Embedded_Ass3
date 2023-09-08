#include "question3.h"
#include "uart.h"

void clearCurrentFire(int startX, int startY) {
    int x = 1;
    int y = 1;
    while (y < 80) { 
        
        drawPixelARGB32(x + startX, y + startY, 0x00000000);
        x++;
        if (x >= 80) { 
            x = 1;
            y++;
        }
        
    }
}

void displayFire(int startX, int startY) {
    int x = 1;
    int y = 1;
    while (y < 80) { 
        
        if ((fire[y][x] != 0x00FFFFFF) && (fire[y][x] != 0x00FFFEFF) && (fire[y][x] != 0x00FFFFFD) ) {
            drawPixelARGB32(x + startX, y + startY, fire[y][x]);
        }else {
            // drawPixelARGB32(y, x, 0x00000000);
        }
        x++;
        if (x >= 80) { 
            x = 1;
            y++;
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

void clearMultipleShot(int x_fire_arr[], int y_fire_arr[], int last_x_fire_arr[], int last_y_arr[], int shooting_command_arr[], int shooting_x_arr[], int shooting_y_arr[]) {

}

void gamePlay() {
    

    // for timer purpose
    unsigned int n = 10;
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
    int x_fire_arr[1] = {x_fire_from_ship};
    int y_fire_arr[1] = {y_fire_from_ship};
    int last_x_fire_arr[1] = {x_fire_arr[0]};
    int last_y_arr[1] = {y};
    int shooting_command_arr[1] = {1};
    int shooting_x_arr[1] = {x};
    int shooting_y_arr[1] = {y};
    //-------------------------------------

    int x_fire = x_fire_from_ship; // array
    int y_fire = y_fire_from_ship; // array
    int last_x_fire = x_fire; // array
    int last_y = y; // array
    int shooting = 1; // command array

    // shooting from spaceship (array)
    int shooting_x = x; 
    int shooting_y = y;

    while (1)
    {
        if (r >= expiredTime) {
            if (last_y != y) {
                clearCurrentSpaceShip(x, last_y);
                last_y = y;
            }

            if (last_x_fire != x_fire) {
                clearCurrentFire(shooting_x + last_x_fire, shooting_y + y_fire);
                if (x_fire + 10 > 850) {
                    shooting_y = y;
                }
                last_x_fire = x_fire;
            }
            displaySpcaeShip(x, y);
            
            if (shooting == 1) {
                displayFire(shooting_x + x_fire, shooting_y + y_fire);
            }
            asm volatile ("mrs %0, cntfrq_el0" : "=r"(f));
            // Read the current counter value
            asm volatile ("mrs %0, cntpct_el0" : "=r"(t));
            // Calculate expire value for counter
            expiredTime = t + ( (f/1000)*n )/1000;
        }
        asm volatile ("mrs %0, cntpct_el0" : "=r"(r));


        if (r_fire >= expiredTime_fire) {
            
            if (shooting == 1) {
                x_fire += 5;
                if (x_fire + 5 > 850) {
                    
                    x_fire = x_fire_from_ship;
                    shooting = 0;
                }
            }
            asm volatile ("mrs %0, cntfrq_el0" : "=r"(f_fire));
            // Read the current counter value
            asm volatile ("mrs %0, cntpct_el0" : "=r"(t_fire));
            // Calculate expire value for counter
            expiredTime_fire = t_fire + ( (f_fire/1000)*n_fire )/1000;
        }
        asm volatile ("mrs %0, cntpct_el0" : "=r"(r_fire));

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
                shooting = 1;
            }

            if (c == 127) {
                break;
            }
        }
    }

}