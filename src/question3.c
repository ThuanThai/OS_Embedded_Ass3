#include "question3.h"
#include "uart.h"
#include "framebf.h"

#define MAX_SHOT 30

void clearRegion(int startX, int startY, int endX, int endY)
{
    for (int i = startX; i < endX; i++)
    {
        for (int j = startY; j < endY; j++)
        {
            drawPixelARGB32(i, j, 0x00000000);
        }
    }
}

void displayCountDown(int time)
{
    switch (time)
    {
    case 15:
        drawInt('1', 300, 400, 0x00F1C40F);
        drawInt('5', 350, 400, 0x00F1C40F);
        wait_msec(1000000);
        break;
    case 14:
        drawInt('1', 300, 400, 0x00F1C40F);
        drawInt('4', 350, 400, 0x00F1C40F);
        wait_msec(1000000);
        break;

    case 13:
        drawInt('1', 300, 400, 0x00F1C40F);
        drawInt('3', 350, 400, 0x00F1C40F);
        wait_msec(1000000);
        break;

    case 12:
        drawInt('1', 300, 400, 0x00F1C40F);
        drawInt('2', 350, 400, 0x00F1C40F);
        wait_msec(1000000);
        break;

    case 11:
        drawInt('1', 300, 400, 0x00F1C40F);
        drawInt('1', 350, 400, 0x00F1C40F);
        wait_msec(1000000);
        break;

    case 10:
        drawInt('1', 300, 400, 0x00F1C40F);
        drawInt('0', 350, 400, 0x00F1C40F);
        wait_msec(1000000);
        break;

    case 9:
        drawInt('9', 300, 400, 0x00F1C40F);
        wait_msec(1000000);
        break;

    case 8:
        drawInt('8', 300, 400, 0x00F1C40F);
        wait_msec(1000000);
        break;
    case 7:
        drawInt('7', 300, 400, 0x00F1C40F);
        wait_msec(1000000);
        break;
    case 6:
        drawInt('6', 300, 400, 0x00F1C40F);
        wait_msec(1000000);
        break;
    case 5:
        drawInt('5', 300, 400, 0x00F1C40F);
        wait_msec(1000000);
        break;
    case 4:
        drawInt('4', 300, 400, 0x00F1C40F);
        wait_msec(1000000);
        break;
    case 3:
        drawInt('3', 300, 400, 0x00F1C40F);
        wait_msec(1000000);
        break;
    case 2:
        drawInt('2', 300, 400, 0x00F1C40F);
        wait_msec(1000000);
        break;
    case 1:
        drawInt('1', 300, 400, 0x00F1C40F);
        wait_msec(1000000);
        break;
    case 0:
        drawInt('0', 300, 400, 0x00F1C40F);
        wait_msec(1000000);
        break;
    default:
        break;
    }
}

void clearCurrentFire(int startX, int startY)
{
    int x = 1;
    int y = 1;
    while (y < 80)
    {

        drawPixelARGB32(x + startX, y + startY, 0x00000000);
        x++;
        if (x >= 80)
        {
            x = 1;
            y++;
        }
    }
}

void displayFire(int startX, int startY)
{
    int x = 1;
    int y = 1;
    while (y < 80)
    {

        if ((fire[y][x] != 0x00FFFFFF) && (fire[y][x] != 0x00FFFEFF) && (fire[y][x] != 0x00FFFFFD))
        {
            drawPixelARGB32(x + startX, y + startY, fire[y][x]);
        }
        else
        {
            // drawPixelARGB32(y, x, 0x00000000);
        }
        x++;
        if (x >= 80)
        {
            x = 1;
            y++;
        }
    }
}

void clearCurrentSpaceShip(int startX, int startY)
{
    int x = 1;
    int y = 1;
    while (y < 180)
    {
        drawPixelARGB32(x + startX, y + startY, 0x00000000);
        x++;
        if (x >= 180)
        {
            x = 20;
            y++;
        }
    }
}

void displaySpcaeShip(int startX, int startY)
{
    int x = 1;
    int y = 1;
    while (y < 180)
    {
        if ((spaceShip[y][x] != 0x00FFFFFF) && (spaceShip[y][x] != 0x00FFFEFF) && (spaceShip[y][x] != 0x00FFFFFD))
        {
            drawPixelARGB32(x + startX, y + startY, spaceShip[y][x]);
        }
        else
        {
            // drawPixelARGB32(y, x, 0x00000000);
        }
        x++;
        if (x >= 180)
        {
            x = 20;
            y++;
        }
    }
}

void clearMultipleShot(int x_fire_arr[], int y_fire_arr[], int last_x_fire_arr[], int last_y_arr[], int shooting_command_arr[], int shooting_x_arr[], int shooting_y_arr[], int x, int y)
{

    for (int i = 0; i < MAX_SHOT; i++)
    {
        if (last_x_fire_arr[i] != x_fire_arr[i])
        {
            clearCurrentFire(shooting_x_arr[i] + last_x_fire_arr[i], shooting_y_arr[i] + y_fire_arr[i]);
            if (x_fire_arr[i] + 10 > 850)
            {
                shooting_y_arr[i] = y;
            }
            last_x_fire_arr[i] = x_fire_arr[i];
        }
    }
}

void displayMultipleShots(int shooting_command_arr[], int shooting_x_arr[], int shooting_y_arr[], int x_fire_arr[], int y_fire_arr[])
{
    for (int i = 0; i < MAX_SHOT; i++)
    {
        if (shooting_command_arr[i] == 1)
        {
            displayFire(shooting_x_arr[i] + x_fire_arr[i], shooting_y_arr[i] + y_fire_arr[i]);
        }
    }
}

void displayNumber(int i)
{
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

void shoot(int shooting_command_arr[], int shooting_x_arr[], int shooting_y_arr[], int x_fire_arr[], int y_fire_arr[], int x_fire_from_ship)
{
    for (int i = 0; i < MAX_SHOT; i++)
    {
        if (shooting_command_arr[i] == 1)
        {

            x_fire_arr[i] += 5;
            if (x_fire_arr[i] + 5 > 850)
            {
                x_fire_arr[i] = x_fire_from_ship;
                shooting_command_arr[i] = 0;
            }
        }
    }
}

void enableShot(int *shotCount, int shooting_command_arr[], int x, int y, int shooting_x_arr[], int shooting_y_arr[])
{
    if ((*shotCount) + 1 < MAX_SHOT)
    {
        (*shotCount) += 1;
        for (int i = 0; i < (*shotCount); i++)
        {
            if (shooting_command_arr[i] != 1)
            {
                shooting_command_arr[i] = 1;
                shooting_x_arr[i] = x;
                shooting_y_arr[i] = y;
                break;
            }
        }
        if ((*shotCount) == MAX_SHOT - 1)
        {
            (*shotCount) = 0;
        }
    }
}

void gamePlay()
{

    // for timer purpose
    unsigned int n = 10;
    register unsigned long f, t, r, expiredTime;
    // Get the current counter frequency (Hz)
    asm volatile("mrs %0, cntfrq_el0"
                 : "=r"(f));
    // Read the current counter value
    asm volatile("mrs %0, cntpct_el0"
                 : "=r"(t));
    // Calculate expire value for counter
    expiredTime = t + ((f / 1000) * n) / 1000;

    // fire timer
    unsigned int n_fire = 6800;
    register unsigned long f_fire, t_fire, r_fire, expiredTime_fire;
    asm volatile("mrs %0, cntfrq_el0"
                 : "=r"(f_fire));
    // Read the current counter value
    asm volatile("mrs %0, cntpct_el0"
                 : "=r"(t_fire));
    // Calculate expire value for counter
    expiredTime_fire = t_fire + ((f_fire / 1000) * n_fire) / 1000;

    // countDown clock
    unsigned int n_cd = 100000;
    register unsigned long f_cd, t_cd, r_cd, expiredTime_cd;
    asm volatile("mrs %0, cntfrq_el0"
                 : "=r"(f_cd));
    // Read the current counter value
    asm volatile("mrs %0, cntpct_el0"
                 : "=r"(t_cd));
    // Calculate expire value for counter
    expiredTime_cd = t_cd + ((f_cd / 1000) * n_cd) / 1000;

    asm volatile("mrs %0, cntpct_el0"
                 : "=r"(r));
    asm volatile("mrs %0, cntpct_el0"
                 : "=r"(r_fire));
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
    for (int i = 0; i < MAX_SHOT; i++)
    {
        shooting_command_arr[i] = 0;
    }
    int shooting_x_arr[MAX_SHOT] = {x};
    int shooting_y_arr[MAX_SHOT] = {y};
    //-------------------------------------

    int x_fire = x_fire_from_ship; // array
    int y_fire = y_fire_from_ship; // array
    int last_x_fire = x_fire;      // array
    int last_y = y;                // array
    int shooting = 1;              // command array

    // shooting from spaceship (array)
    int shooting_x = x;
    int shooting_y = y;

    // shot tracking
    int shotCount = 0;
    int countDown = 15;

    while (1)
    {
        if (r_cd >= expiredTime_cd)
        {
            clearRegion(300, 400, 450, 450);
            displayCountDown(countDown);
            asm volatile("mrs %0, cntfrq_el0"
                         : "=r"(f_cd));
            // Read the current counter value
            asm volatile("mrs %0, cntpct_el0"
                         : "=r"(t_cd));
            // Calculate expire value for counter
            expiredTime_cd = t_cd + ((f_cd / 1000) * n_cd) / 1000;
            countDown--;
        }
        asm volatile("mrs %0, cntpct_el0"
                     : "=r"(r_cd));
        if (r >= expiredTime)
        {
            if (last_y != y)
            {
                clearCurrentSpaceShip(x, last_y);
                last_y = y;
            }

            // shot clearing code
            clearMultipleShot(x_fire_arr, y_fire_arr, last_x_fire_arr, last_y_arr, shooting_command_arr, shooting_x_arr, shooting_y_arr, x, y);
            displaySpcaeShip(x, y);

            // shot display code
            displayMultipleShots(shooting_command_arr, shooting_x_arr, shooting_y_arr, x_fire_arr, y_fire_arr);
            asm volatile("mrs %0, cntfrq_el0"
                         : "=r"(f));
            // Read the current counter value
            asm volatile("mrs %0, cntpct_el0"
                         : "=r"(t));
            // Calculate expire value for counter
            expiredTime = t + ((f / 1000) * n) / 1000;
        }
        asm volatile("mrs %0, cntpct_el0"
                     : "=r"(r));

        if (r_fire >= expiredTime_fire)
        {

            // shot moving code
            shoot(shooting_command_arr, shooting_x_arr, shooting_y_arr, x_fire_arr, y_fire_arr, x_fire_from_ship);
            asm volatile("mrs %0, cntfrq_el0"
                         : "=r"(f_fire));
            // Read the current counter value
            asm volatile("mrs %0, cntpct_el0"
                         : "=r"(t_fire));
            // Calculate expire value for counter
            expiredTime_fire = t_fire + ((f_fire / 1000) * n_fire) / 1000;
        }
        asm volatile("mrs %0, cntpct_el0"
                     : "=r"(r_fire));

        if (!(UART0_FR & UART0_FR_RXFE))
        {
            char c = (unsigned char)(UART0_DR);
            c = (c == '\r' ? '\n' : c);
            if (c == 's')
            {

                if (!(y + 20 > 600))
                {
                    y += 20;
                    displaySpcaeShip(x, y);
                }
            }

            if (c == 'w')
            {
                if (!(y - 20 <= 0))
                {
                    y -= 20;
                    displaySpcaeShip(x, y);
                }
            }

            if (c == 'k')
            {
                enableShot(&shotCount, shooting_command_arr, x, y, shooting_x_arr, shooting_y_arr);
            }

            if (c == 127)
            {
                break;
            }
        }
    }
}