#include "question3.h"
#include "uart.h"
#include "mbox.h"
#include "printf.h"
#include "handleKernel.h"

#define MAX_SHOT 30
#define ENEMY_SPEED 50000
#define FIRE_W 80
#define FIRE_H 80
#define SPACESHIP_W 180
#define SPACESHIP_H 180
#define ENEMY_W 91
#define ENEMY_H 80
#define MAX_ENEMY 9
#define FIRE_W_FROM_ENEMY 80
#define SPACESHIP_HIT_RANGE_Y 58
#define X_CD 935
#define Y_CD 370
#define X_LineVertical 930
#define X_HEART 945
#define Y_HEART 50

// modify this to increase shooting speed of round 2 and 3
#define ENEMY_ROUND_2_SHOOT_SPEED 1000000 // (slow speed) (1 second random 1 shot)
#define ENEMY_ROUND_3_SHOOT_SPEED 200000  // (high speed) ( 0.2 second random 1 shot)

struct obj
{
    int x;
    int y;
};
struct obj hearts[4] = {{X_HEART, Y_HEART}, {X_HEART + 35, Y_HEART}, {X_HEART, Y_HEART + 35}, {X_HEART + 35, Y_HEART + 35}};

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

void displayRound(int num)
{
    char round = num + '0';
    drawChar('R', 300, 280, 0x00FF0000);
    drawChar('O', 350, 280, 0x00FF0000);
    drawChar('U', 400, 280, 0x00FF0000);
    drawChar('N', 450, 280, 0x00FF0000);
    drawChar('D', 500, 280, 0x00FF0000);
    drawInt(round, 550, 285, 0x00FF0000);
}

void displayWin()
{
    drawChar('Y', 300, 280, 0xA421C4);
    drawChar('O', 350, 280, 0xA421C4);
    drawChar('U', 400, 280, 0xA421C4);
    drawChar('W', 450, 280, 0xA421C4);
    drawChar('I', 500, 280, 0xA421C4);
    drawChar('N', 500, 280, 0xA421C4);
}

void displayLose()
{
    drawChar('Y', 300, 280, 0x217dc4);
    drawChar('O', 350, 280, 0x217dc4);
    drawChar('U', 400, 280, 0x217dc4);
    drawChar('L', 450, 280, 0x217dc4);
    drawChar('O', 500, 280, 0x217dc4);
    drawChar('S', 550, 280, 0x217dc4);
    drawChar('E', 600, 280, 0x217dc4);
}

int shootAvaibleEnemy(int is_enemy_arr[], int shootiny_x_enemy_arr[], int shooting_command_arr[])
{
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        if ((is_enemy_arr[i] == 1) && (shooting_command_arr[i] == 0))
        {
            shootiny_x_enemy_arr[i] = 500;
            return i;
        }
    }
    return 0;
}

void clearCountDown(int time)
{
    switch (time)
    {
    case 15:
        clearInt('1', 300, 400);
        clearInt('5', 350, 400);
        // wait_msec(1000000);
        break;
    case 14:
        clearInt('1', 300, 400);
        clearInt('4', 350, 400);
        // wait_msec(1000000);
        break;

    case 13:
        clearInt('1', 300, 400);
        clearInt('3', 350, 400);
        // wait_msec(1000000);
        break;

    case 12:
        clearInt('1', 300, 400);
        clearInt('2', 350, 400);
        // wait_msec(1000000);
        break;

    case 11:
        clearInt('1', 300, 400);
        clearInt('1', 350, 400);
        // wait_msec(1000000);
        break;

    case 10:
        clearInt('1', 300, 400);
        clearInt('0', 350, 400);
        // wait_msec(1000000);
        break;

    case 9:
        clearInt('9', 300, 400);
        // wait_msec(1000000);
        break;

    case 8:
        clearInt('8', 300, 400);
        // wait_msec(1000000);
        break;
    case 7:
        clearInt('7', 300, 400);
        // wait_msec(1000000);
        break;
    case 6:
        clearInt('6', 300, 400);
        // wait_msec(1000000);
        break;
    case 5:
        clearInt('5', 300, 400);
        // wait_msec(1000000);
        break;
    case 4:
        clearInt('4', 300, 400);
        // wait_msec(1000000);
        break;
    case 3:
        clearInt('3', 300, 400);
        // wait_msec(1000000);
        break;
    case 2:
        clearInt('2', 300, 400);
        // wait_msec(1000000);
        break;
    case 1:
        clearInt('1', 300, 400);
        // wait_msec(1000000);
        break;
    case 0:
        clearInt('0', 300, 400);
        // wait_msec(1000000);
        break;
    default:
        break;
    }
}

void displayCountDown(int time)
{
    switch (time)
    {
    case 15:
        drawInt('1', X_CD, Y_CD, 0x00F1C40F);
        drawInt('5', X_CD + 40, Y_CD, 0x00F1C40F);
        // wait_msec(1000000);
        break;
    case 14:
        drawInt('1', X_CD, Y_CD, 0x00F1C40F);
        drawInt('4', X_CD + 40, Y_CD, 0x00F1C40F);
        // wait_msec(1000000);
        break;

    case 13:
        drawInt('1', X_CD, Y_CD, 0x00F1C40F);
        drawInt('3', X_CD + 40, Y_CD, 0x00F1C40F);
        // wait_msec(1000000);
        break;

    case 12:
        drawInt('1', X_CD, Y_CD, 0x00F1C40F);
        drawInt('2', X_CD + 40, Y_CD, 0x00F1C40F);
        // wait_msec(1000000);
        break;

    case 11:
        drawInt('1', X_CD, Y_CD, 0x00F1C40F);
        drawInt('1', X_CD + 40, Y_CD, 0x00F1C40F);
        // wait_msec(1000000);
        break;

    case 10:
        drawInt('1', X_CD, Y_CD, 0x00F1C40F);
        drawInt('0', X_CD + 40, Y_CD, 0x00F1C40F);
        // wait_msec(1000000);
        break;

    case 9:
        drawInt('9', X_CD, Y_CD, 0x00F1C40F);
        // wait_msec(1000000);
        break;

    case 8:
        drawInt('8', X_CD, Y_CD, 0x00F1C40F);
        // wait_msec(1000000);
        break;
    case 7:
        drawInt('7', X_CD, Y_CD, 0x00F1C40F);
        // wait_msec(1000000);
        break;
    case 6:
        drawInt('6', X_CD, Y_CD, 0x00F1C40F);
        // wait_msec(1000000);
        break;
    case 5:
        drawInt('5', X_CD, Y_CD, 0x00F1C40F);
        // wait_msec(1000000);
        break;
    case 4:
        drawInt('4', X_CD, Y_CD, 0x00F1C40F);
        // wait_msec(1000000);
        break;
    case 3:
        drawInt('3', X_CD, Y_CD, 0x00F1C40F);
        // wait_msec(1000000);
        break;
    case 2:
        drawInt('2', X_CD, Y_CD, 0x00F1C40F);
        // wait_msec(1000000);
        break;
    case 1:
        drawInt('1', X_CD, 400, 0x00F1C40F);
        // wait_msec(1000000);
        break;
    case 0:
        drawInt('0', X_CD, 400, 0x00F1C40F);
        // wait_msec(1000000);
        break;
    default:
        break;
    }
}

void clearCurrentFire(int startX, int startY)
{
    int x = 1;
    int y = 1;
    while (y < FIRE_H)
    {

        drawPixelARGB32(x + startX, y + startY, 0x00000000);
        x++;
        if (x >= FIRE_W)
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
    while (y < FIRE_H)
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
        if (x >= FIRE_W)
        {
            x = 1;
            y++;
        }
    }
}

void displayFire_enemy(int startX, int startY)
{
    int x = FIRE_W - 1;
    int y = FIRE_H - 1;
    int x_print = 1;
    int y_print = 1;
    while (y >= 0)
    {

        if ((fire_180[y_print][x_print] != 0x00FFFFFF) && (fire_180[y_print][x_print] != 0x00FFFEFF) && (fire_180[y_print][x_print] != 0x00FFFFFD))
        {
            drawPixelARGB32(x_print + startX, y_print + startY, fire_180[y_print][x_print]);
        }
        else
        {
            // drawPixelARGB32(y, x, 0x00000000);
        }
        x--;
        if (x <= 0)
        {
            x = FIRE_W - 1;
            y--;
        }
        x_print++;
        if (x_print >= FIRE_W)
        {
            x_print = 1;
            y_print++;
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
            // drawPixelARGB32(y, x, 0x00FFFFFF);
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

void clearMultipleShot_enemy(int x_fire_arr[], int y_fire_arr[], int last_x_fire_arr[], int last_y_arr[], int shooting_command_arr[], int shooting_x_arr[], int shooting_y_arr[], int x[], int y[])
{
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        if (last_x_fire_arr[i] != shooting_x_arr[i])
        {
            clearCurrentFire(last_x_fire_arr[i], shooting_y_arr[i]);
            if (shooting_x_arr[i] - 10 < 15)
            {
                shooting_y_arr[i] = y[i];
            }
            last_x_fire_arr[i] = shooting_x_arr[i];
        }
    }
}

void clearCurrentEnemy(int startX, int startY)
{
    int x = 1;
    int y = 1;
    while (y < 80)
    {
        drawPixelARGB32(x + startX, y + startY, 0x00000000);
        x++;
        if (x >= 91)
        {
            x = 1;
            y++;
        }
    }
}

void checkHit(int shooting_x_arr[], int x_fire_arr[], int shooting_y_arr[], int y_fire_arr[], int i, int x_enemy_arr[], int y_enemy_arr[], int isEnemy_arr[], int shooting_command_arr[], int *isHit, int x_fire_from_ship)
{
    for (int k = 0; k < MAX_ENEMY; k++)
    {
        int fire_x_des = shooting_x_arr[i] + x_fire_arr[i] + FIRE_W;
        int fire_y_des = shooting_y_arr[i] + y_fire_arr[i] + FIRE_H;
        int enemy_x_des = x_enemy_arr[k] + ENEMY_W;
        int enemy_y_des = y_enemy_arr[k] + ENEMY_H;
        // check hit
        if (isEnemy_arr[k])
        {
            if (((fire_x_des >= x_enemy_arr[k]) && (fire_x_des <= enemy_x_des)) && ((fire_y_des >= y_enemy_arr[k]) && (fire_y_des <= enemy_y_des)))
            {
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

void displayMultipleShots(int shooting_command_arr[], int shooting_x_arr[], int shooting_y_arr[], int x_fire_arr[], int y_fire_arr[], int x_enemy_arr[], int y_enemy_arr[], int isEnemy_arr[], int x_fire_from_ship, int *hits)
{
    for (int i = 0; i < MAX_SHOT; i++)
    {
        if (shooting_command_arr[i] == 1)
        {
            int isHit = 0;
            checkHit(shooting_x_arr, x_fire_arr, shooting_y_arr, y_fire_arr, i, x_enemy_arr, y_enemy_arr, isEnemy_arr, shooting_command_arr, &isHit, x_fire_from_ship);
            if (isHit == 0)
            {

                displayFire(shooting_x_arr[i] + x_fire_arr[i], shooting_y_arr[i] + y_fire_arr[i]);
            }
            else
            {
                (*hits) += 1;
                clearCurrentFire(shooting_x_arr[i] + x_fire_arr[i], shooting_y_arr[i] + y_fire_arr[i]);
            }
        }
    }
}

void displayMultipleShots_enemy(int shooting_command_arr[], int shooting_x_arr[], int shooting_y_arr[], int x_fire_arr[], int y_fire_arr[], int x_enemy_arr[], int y_enemy_arr[], int isEnemy_arr[], int x_fire_from_ship[], int x_spaceship, int y_spaceship, int *is_endgame, int *lives)
{
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        if (shooting_command_arr[i] == 1)
        {
            int is_hit = 0;
            int spaceship_end_x = x_spaceship + SPACESHIP_W;
            int spaceship_end_y = y_spaceship + SPACESHIP_H;
            if (((shooting_x_arr[i] >= x_spaceship) && (shooting_x_arr[i] <= spaceship_end_x)))
            {
                if ((shooting_y_arr[i] >= y_spaceship) && (shooting_y_arr[i] <= spaceship_end_y))
                {
                    shooting_command_arr[i] = 0;
                    uart_puts("\n\n Hit");
                    displayNumber2(&i);
                    uart_sendc('\n');
                    (*lives) -= 1;
                    if ((*lives) == 0)
                    {
                        (*is_endgame) = 1;
                        uart_puts("\n\n End game \n\n");
                    }
                    clearCurrentFire(shooting_x_arr[i] + x_fire_arr[i], shooting_y_arr[i] + y_fire_arr[i]);
                    shooting_x_arr[i] = 900;
                    shooting_y_arr[i] = y_enemy_arr[i];
                }
                else
                {
                    displayFire_enemy(shooting_x_arr[i] + x_fire_arr[i], shooting_y_arr[i] + y_fire_arr[i]);
                }
            }
            else
            {
                displayFire_enemy(shooting_x_arr[i] + x_fire_arr[i], shooting_y_arr[i] + y_fire_arr[i]);
            }
        }
    }
}

void displayNumber2(int *i)
{

    switch (*i)
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

void displayNumber(int *i, int is_enemy_arr[], int shootiny_x_enemy_arr[], int shooting_command_arr[])
{

    switch (*i)
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
        (*i) = shootAvaibleEnemy(is_enemy_arr, shootiny_x_enemy_arr, shooting_command_arr);
        break;
    }
}

void enableShot_enemy(int idx, int shooting_command_arr[], int x[], int y[], int shooting_x_arr[], int shooting_y_arr[], int is_enemy[])
{
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        if (i == idx)
        {
            if ((shooting_command_arr[i] != 1) && (is_enemy[i] == 1))
            {
                shooting_command_arr[i] = 1;
                shooting_x_arr[i] = x[i];
                uart_puts("Enable idx: ");
                displayNumber2(&i);
                uart_sendc('-');
                if (shooting_x_arr[i] > 600)
                {
                    uart_puts(" at start");
                }
                uart_sendc('\n');
                shooting_y_arr[i] = y[i];
            }
            break;
        }
    }
}

void shoot(int shooting_command_arr[], int shooting_x_arr[], int shooting_y_arr[], int x_fire_arr[], int y_fire_arr[], int x_fire_from_ship)
{
    for (int i = 0; i < MAX_SHOT; i++)
    {
        if (shooting_command_arr[i] == 1)
        {

            x_fire_arr[i] += 5;
            if (x_fire_arr[i] + 5 > 950)
            {
                x_fire_arr[i] = x_fire_from_ship;
                shooting_command_arr[i] = 0;
            }
        }
    }
}

void shoot_enemy(int shooting_command_arr[], int shooting_x_arr[], int shooting_y_arr[], int x_fire_arr[], int y_fire_arr[], int x_fire_from_ship[])
{
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        if (shooting_command_arr[i] == 1)
        {
            // uart_puts("Shoot enemy IDX: ");
            // displayNumber2(&i);
            // uart_sendc('\n');
            shooting_x_arr[i] -= 3;
            if (shooting_x_arr[i] - 3 < 35)
            {
                shooting_x_arr[i] = x_fire_from_ship[i] - FIRE_W_FROM_ENEMY;
                uart_puts("Reset idx: ");
                displayNumber2(&i);
                uart_sendc('\n');
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

void displayEnemy(int startX, int startY)
{
    int x = 1;
    int y = 1;
    while (y < 80)
    {

        if ((enemy[y][x] != 0x00FFFFFF) && (enemy[y][x] != 0x00FFFEFF) && (enemy[y][x] != 0x00FFFFFD))
        {
            drawPixelARGB32(x + startX, y + startY, enemy[y][x]);
        }
        else
        {
            // drawPixelARGB32(y, x, 0x00000000);
        }
        x++;
        if (x >= 91)
        {
            x = 1;
            y++;
        }
    }
}

void displayMultipleEnemy(int isEnemy_arr[], int x_enemy_arr[], int y_enemy_arr[])
{
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        if (isEnemy_arr[i])
        {
            displayEnemy(x_enemy_arr[i], y_enemy_arr[i]);
        }
    }
}

void clearMultipleEnemy(int last_y_enemy_arr[], int y_enemy_arr[], int x_enemy_arr[], int isEnemy_arr[])
{
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        if (last_y_enemy_arr[i] != y_enemy_arr[i])
        {
            if (isEnemy_arr[i])
            {
                clearCurrentEnemy(x_enemy_arr[i], last_y_enemy_arr[i]);
                last_y_enemy_arr[i] = y_enemy_arr[i];
            }
        }
    }
}

void movingMultipleEnemy(int spaceShipToggle_arr[], int y_enemy_arr[])
{
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        // shot moving code
        if (spaceShipToggle_arr[i])
        {
            if (!(y_enemy_arr[i] + 20 > 700))
            {
                y_enemy_arr[i] += 20;
                // displayEnemy(x_enemy, y_enemy);
            }
            else
            {
                spaceShipToggle_arr[i] = 0;
            }
        }
        else
        {
            if (!(y_enemy_arr[i] - 20 <= 20))
            {
                y_enemy_arr[i] -= 20;
                // displayEnemy(x_enemy, y_enemy);
            }
            else
            {
                spaceShipToggle_arr[i] = 1;
            }
        }
    }
}

void gamePlay()
{
    //--------------------------------------------------------------- set up for enemy movement-----------------------------------------------------------------------
    mBuf[0] = 17 * 4;       // Message Buffer Size in bytes (8 elements * 4 bytes (32 bit) each)
    mBuf[1] = MBOX_REQUEST; // Message Request Code (this is a request message)

    mBuf[2] = 0x00040004; // TAG Identifier: Get clock rate
    mBuf[3] = 8;          // Value buffer size in bytes (max of request and response lengths)
    mBuf[4] = 0;          // REQUEST CODE = 0
    mBuf[5] = 0;          // clock id: ARM system clock
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

    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP))
    {
        uart_puts("Width: ");
        x_enemy = mBuf[5] - 100;
        uart_dec(mBuf[5]);
        uart_sendc('\n');
        uart_puts("Height: ");
        uart_dec(mBuf[6]);
    }
    // init max x_enemy
    int x_enemy_init = 200;
    unsigned int x_enemy_arr[MAX_ENEMY] = {};
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        x_enemy_arr[i] = mBuf[5] - x_enemy_init;
        x_enemy_init += 100;
        if (x_enemy_init >= 400)
        {
            x_enemy_init = 200;
        }
    }

    int y_enemy_init = 30;
    unsigned int y_enemy_arr[MAX_ENEMY] = {};
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        y_enemy_arr[i] = y_enemy_init;
        y_enemy_init += 30;
        if (y_enemy_init >= 500)
        {
            y_enemy_init = 30;
        }
    }
    int last_x_enemy_arr[MAX_ENEMY] = {x_enemy};
    int last_y_enemy_arr[MAX_ENEMY] = {last_y_enemy};
    int spaceShipToggle_arr[MAX_ENEMY] = {1};
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        spaceShipToggle_arr[i] = 1;
    }
    int isEnemy_arr[MAX_ENEMY] = {1};
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        isEnemy_arr[i] = 1;
    }

    // implement shots for enemy
    int x_fire_arr_enemy[MAX_ENEMY] = {0};
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        x_fire_arr_enemy[i] = 0;
    }
    int y_fire_arr_enemy[MAX_ENEMY] = {0};
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        y_fire_arr_enemy[i] = 0;
    }
    int last_x_fire_arr_enemy[MAX_ENEMY] = {x_fire_arr_enemy[0]};
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        last_x_fire_arr_enemy[i] = x_fire_arr_enemy[i];
    }

    int last_y_arr_enemy[MAX_ENEMY] = {0};
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        last_y_arr_enemy[i] = y_fire_arr_enemy[i];
    }
    int shooting_command_arr_enemy[MAX_ENEMY] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        shooting_command_arr_enemy[i] = 0;
    }
    int shooting_x_arr_enemy[MAX_ENEMY] = {0};
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        shooting_x_arr_enemy[i] = x_enemy_arr[i] - FIRE_W_FROM_ENEMY;
    }

    int shooting_y_arr_enemy[MAX_ENEMY] = {0};
    for (int i = 0; i < MAX_ENEMY; i++)
    {
        shooting_y_arr_enemy[i] = y_enemy_arr[i];
    }

    unsigned int n_enemy_shoot_duration = ENEMY_ROUND_2_SHOOT_SPEED;
    register unsigned long f_enemy_shoot_duration, t_enemy_shoot_duration, r_enemy_shoot_duration, expiredTime_enemy_shoot_duration;
    // Get the current counter frequency (Hz)
    asm volatile("mrs %0, cntfrq_el0"
                 : "=r"(f_enemy_shoot_duration));
    // Read the current counter value
    asm volatile("mrs %0, cntpct_el0"
                 : "=r"(t_enemy_shoot_duration));
    // Calculate expire value for counter
    expiredTime_enemy_shoot_duration = t_enemy_shoot_duration + ((f_enemy_shoot_duration / 1000) * n_enemy_shoot_duration) / 1000;

    // timer for enemy shoot
    unsigned int n_enemy_shoot = 30000;
    register unsigned long f_enemy_shoot, t_enemy_shoot, r_enemy_shoot, expiredTime_enemy_shoot;
    // Get the current counter frequency (Hz)
    asm volatile("mrs %0, cntfrq_el0"
                 : "=r"(f_enemy_shoot));
    // Read the current counter value
    asm volatile("mrs %0, cntpct_el0"
                 : "=r"(t_enemy_shoot));
    // Calculate expire value for counter
    expiredTime_enemy_shoot = t_enemy_shoot + ((f_enemy_shoot / 1000) * n_enemy_shoot) / 1000;

    unsigned int n_enemy = ENEMY_SPEED;
    register unsigned long f_enemy, t_enemy, r_enemy, expiredTime_enemy;
    // Get the current counter frequency (Hz)
    asm volatile("mrs %0, cntfrq_el0"
                 : "=r"(f_enemy));
    // Read the current counter value
    asm volatile("mrs %0, cntpct_el0"
                 : "=r"(t_enemy));
    // Calculate expire value for counter
    expiredTime_enemy = t_enemy + ((f_enemy / 1000) * n_enemy) / 1000;

    //----------------------------------------------------------------- MARK: setup for shots --------------------------------------------------------------------------
    // for timer purpose
    unsigned int n = 100;
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

    unsigned int n_count_down = 1000000;
    register unsigned long f_count_down, t_count_down, r_count_down, expiredTime_count_down;
    asm volatile("mrs %0, cntfrq_el0"
                 : "=r"(f_count_down));
    // Read the current counter value
    asm volatile("mrs %0, cntpct_el0"
                 : "=r"(t_count_down));
    // Calculate expire value for counter
    expiredTime_count_down = t_count_down + ((f_count_down / 1000) * n_count_down) / 1000;

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
    int increment = 0;
    int is_endgame = 0;
    int count_down_time = 15;
    int last_count_down_time = count_down_time;
    int lives = 4;
    int total_lives = 4;
    int hits = 0;

    // round changing
    int currentRound = 1;
    if (currentRound == 1)
    {
        displayRound(currentRound);
        wait_msec(1000000);
        clearScreen();
    }
    // WHILE BLOCK
    while (1)
    {
        drawLineVert(X_LineVertical, 0, 1024, 0xFFFFFF);

        for (int i = 0; i < total_lives; i++)
        {
            if (i < lives)
            {
                drawIcon(hearts[i].x, hearts[i].y, 30, gameContent[0]);
            }
            else
            {
                drawBlankIcon(hearts[i].x, hearts[i].y, 30);
            }
        }

        if ((is_endgame))
        {

            // show lose in here
            displayLose();
            break;
        }

        if (r >= expiredTime)
        {

            if (last_y != y)
            {
                clearCurrentSpaceShip(x, last_y);
                last_y = y;
            }

            if (last_count_down_time != count_down_time)
            {
                clearRegion(X_CD, Y_CD, X_CD + 100, Y_CD + 50);
            }

            // clear multiple enemy
            clearMultipleEnemy(last_y_enemy_arr, y_enemy_arr, x_enemy_arr, isEnemy_arr);
            // shot clearing code
            clearMultipleShot(x_fire_arr, y_fire_arr, last_x_fire_arr, last_y_arr, shooting_command_arr, shooting_x_arr, shooting_y_arr, x, y);
            clearMultipleShot_enemy(x_fire_arr_enemy, y_fire_arr_enemy, last_x_fire_arr_enemy, last_y_arr_enemy, shooting_command_arr_enemy, shooting_x_arr_enemy, shooting_y_arr_enemy, x_enemy_arr, y_enemy_arr);
            displaySpcaeShip(x, y);
            displayCountDown(count_down_time);

            displayMultipleEnemy(isEnemy_arr, x_enemy_arr, y_enemy_arr);
            // shot display code
            displayMultipleShots(shooting_command_arr, shooting_x_arr, shooting_y_arr, x_fire_arr, y_fire_arr, x_enemy_arr, y_enemy_arr, isEnemy_arr, x_fire_from_ship, &hits);
            displayMultipleShots_enemy(shooting_command_arr_enemy, shooting_x_arr_enemy, shooting_y_arr_enemy, x_fire_arr_enemy, y_fire_arr_enemy, x_fire_arr_enemy, y_enemy_arr, isEnemy_arr, x_enemy_arr, x, y, &is_endgame, &lives);
            asm volatile("mrs %0, cntfrq_el0"
                         : "=r"(f));
            // Read the current counter value
            asm volatile("mrs %0, cntpct_el0"
                         : "=r"(t));
            // Calculate expire value for counter
            expiredTime = t + ((f / 1000) * n) / 1000;
        }

        if ((hits) == MAX_ENEMY)
        {
            uart_puts("\n\nNext round\n\n");
            uart_puts("Press any key to next round\n");

            // reset enemy shoot array
            for (int i = 0; i < MAX_ENEMY; i++)
            {
                shooting_command_arr_enemy[i] = 0;
            }

            // for (int i = 0; i < MAX_ENEMY; i++)
            // {
            //     x_fire_arr_enemy[i] += 10;
            // }

            // this loop use for clear all spaceship shot
            for (int i = 0; i < MAX_SHOT; i++)
            {
                x_fire_arr[i] += 20;
            }

            clearMultipleShot(x_fire_arr, y_fire_arr, last_x_fire_arr, last_y_arr, shooting_command_arr, shooting_x_arr, shooting_y_arr, x, y);
            clearMultipleShot_enemy(x_fire_arr_enemy, y_fire_arr_enemy, last_x_fire_arr_enemy, last_y_arr_enemy, shooting_command_arr_enemy, shooting_x_arr_enemy, shooting_y_arr_enemy, x_enemy_arr, y_enemy_arr);

            //------------------------------- Clear screen and display round title before this loop ---------------------------------------

            //--------------------------------------------------------------------------
            // to next round if any key pressed
            displayRound(++currentRound);

            while ((UART0_FR & UART0_FR_RXFE))
            {
            }

            clearRegion(300, 280, 600, 330);
            // make all enemy alive
            for (int i = 0; i < MAX_ENEMY; i++)
            {
                isEnemy_arr[i] = 1;
            }

            // restart counter variable
            asm volatile("mrs %0, cntfrq_el0"
                         : "=r"(f_enemy_shoot_duration));
            // Read the current counter value
            asm volatile("mrs %0, cntpct_el0"
                         : "=r"(t_enemy_shoot_duration));
            // Calculate expire value for counter
            expiredTime_enemy_shoot_duration = t_enemy_shoot_duration + ((f_enemy_shoot_duration / 1000) * n_enemy_shoot_duration) / 1000;

            // reset hit count
            hits = 0;
            // if all rounds complete, you win
            if (currentRound > 3)
            {
                // show win in here
                displayWin();
                currentRound = 1;
                break;
            }

            // in round 3, the shooting speed of enemy wil be increased
            if (currentRound == 3)
            {
                n_enemy_shoot_duration = ENEMY_ROUND_3_SHOOT_SPEED;
            }
        }

        asm volatile("mrs %0, cntpct_el0"
                     : "=r"(r));

        asm volatile("mrs %0, cntpct_el0"
                     : "=r"(r_enemy_shoot_duration));

        if (r_fire >= expiredTime_fire)
        {

            // shot moving code
            shoot(shooting_command_arr, shooting_x_arr, shooting_y_arr, x_fire_arr, y_fire_arr, x_fire_from_ship);
            shoot_enemy(shooting_command_arr_enemy, shooting_x_arr_enemy, shooting_y_arr_enemy, x_fire_arr_enemy, y_fire_arr_enemy, x_enemy_arr);
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

        if (r_enemy >= expiredTime_enemy)
        {
            // enemy moving in here
            movingMultipleEnemy(spaceShipToggle_arr, y_enemy_arr);

            asm volatile("mrs %0, cntfrq_el0"
                         : "=r"(f_enemy));
            // Read the current counter value
            asm volatile("mrs %0, cntpct_el0"
                         : "=r"(t_enemy));
            // Calculate expire value for counter
            expiredTime_enemy = t_enemy + ((f_enemy / 1000) * n_enemy) / 1000;
        }
        asm volatile("mrs %0, cntpct_el0"
                     : "=r"(r_enemy));

        if (r_count_down >= expiredTime_count_down)
        {
            count_down_time -= 1;
            if (count_down_time < 0)
            {
                // Show lose in here if hit is not reach (optional)
                is_endgame = 1;
                clearScreen();
                displayLose();
                count_down_time = 0;
                break;
            }
            asm volatile("mrs %0, cntfrq_el0"
                         : "=r"(f_count_down));
            // Read the current counter value
            asm volatile("mrs %0, cntpct_el0"
                         : "=r"(t_count_down));
            // Calculate expire value for counter
            expiredTime_count_down = t_count_down + ((f_count_down / 1000) * n_count_down) / 1000;
        }
        asm volatile("mrs %0, cntpct_el0"
                     : "=r"(r_count_down));

        if (currentRound != 1)
        {
            if (r_enemy_shoot_duration >= expiredTime_enemy_shoot_duration)
            {
                asm volatile("mrs %0, cntpct_el0"
                             : "=r"(increment));

                int idx = (increment / 10) % MAX_ENEMY;
                if (increment == 0)
                {
                    uart_puts("Is 0 \n");
                }
                // uart_puts("Random: ");
                displayNumber(&idx, isEnemy_arr, shooting_x_arr_enemy, shooting_command_arr_enemy);
                uart_sendc('\n');
                enableShot_enemy(idx, shooting_command_arr_enemy, x_enemy_arr, y_enemy_arr, shooting_x_arr_enemy, shooting_y_arr_enemy, isEnemy_arr);

                asm volatile("mrs %0, cntfrq_el0"
                             : "=r"(f_enemy_shoot_duration));
                // Read the current counter value
                asm volatile("mrs %0, cntpct_el0"
                             : "=r"(t_enemy_shoot_duration));
                // Calculate expire value for counter
                expiredTime_enemy_shoot_duration = t_enemy_shoot_duration + ((f_enemy_shoot_duration / 1000) * n_enemy_shoot_duration) / 1000;
            }
        }
        else
        {
            asm volatile("mrs %0, cntfrq_el0"
                         : "=r"(f_enemy_shoot_duration));
            // Read the current counter value
            asm volatile("mrs %0, cntpct_el0"
                         : "=r"(t_enemy_shoot_duration));
            // Calculate expire value for counter
            expiredTime_enemy_shoot_duration = t_enemy_shoot_duration + ((f_enemy_shoot_duration / 1000) * n_enemy_shoot_duration) / 1000;
        }
        asm volatile("mrs %0, cntpct_el0"
                     : "=r"(r_enemy_shoot_duration));

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