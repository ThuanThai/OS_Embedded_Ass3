#include "handleKernel.h"
#include "imagePixel.h"
#include "question3.h"
#include "gameContent.h"
#define SPACE_LEN 20
#define FRAMES_LEN 100
scrollPosition = 0;
isShowing = 0;
currentImage = 0;
unsigned int *images[] = {
    image1, // Pointer to your image data 1
    image2, // Pointer to your image data 2
    // Add more image pointers as needed
};
// unsigned int *frames[] = {
//     epd_bitmap_ezgif_frame_001,
//     epd_bitmap_ezgif_frame_002,
//     epd_bitmap_ezgif_frame_003,
//     epd_bitmap_ezgif_frame_004,
//     epd_bitmap_ezgif_frame_005,
//     epd_bitmap_ezgif_frame_006,
//     epd_bitmap_ezgif_frame_007,
//     epd_bitmap_ezgif_frame_008,
//     epd_bitmap_ezgif_frame_009,
//     epd_bitmap_ezgif_frame_010,
//     epd_bitmap_ezgif_frame_011,
//     epd_bitmap_ezgif_frame_012,
//     epd_bitmap_ezgif_frame_013,
//     epd_bitmap_ezgif_frame_014,
//     epd_bitmap_ezgif_frame_015,
//     epd_bitmap_ezgif_frame_016,
//     epd_bitmap_ezgif_frame_017,
//     epd_bitmap_ezgif_frame_018,
//     epd_bitmap_ezgif_frame_019,
//     epd_bitmap_ezgif_frame_020,
//     epd_bitmap_ezgif_frame_021,
//     epd_bitmap_ezgif_frame_022,
//     epd_bitmap_ezgif_frame_023,
//     epd_bitmap_ezgif_frame_024,
//     epd_bitmap_ezgif_frame_025,
//     epd_bitmap_ezgif_frame_026,
//     epd_bitmap_ezgif_frame_027,
//     epd_bitmap_ezgif_frame_028,
//     epd_bitmap_ezgif_frame_029,
//     epd_bitmap_ezgif_frame_030,
//     epd_bitmap_ezgif_frame_031,
//     epd_bitmap_ezgif_frame_032,
//     epd_bitmap_ezgif_frame_033,
//     epd_bitmap_ezgif_frame_034,
//     epd_bitmap_ezgif_frame_035,
//     epd_bitmap_ezgif_frame_036,
//     epd_bitmap_ezgif_frame_037,
//     epd_bitmap_ezgif_frame_038,
//     epd_bitmap_ezgif_frame_039,
//     epd_bitmap_ezgif_frame_040,
//     epd_bitmap_ezgif_frame_041,
//     epd_bitmap_ezgif_frame_042,
//     epd_bitmap_ezgif_frame_043,
//     epd_bitmap_ezgif_frame_044,
//     epd_bitmap_ezgif_frame_045,
//     epd_bitmap_ezgif_frame_046,
//     epd_bitmap_ezgif_frame_047,
//     epd_bitmap_ezgif_frame_048,
//     epd_bitmap_ezgif_frame_049,
//     epd_bitmap_ezgif_frame_050,
//     epd_bitmap_ezgif_frame_051,
//     epd_bitmap_ezgif_frame_052,
//     epd_bitmap_ezgif_frame_053,
//     epd_bitmap_ezgif_frame_054,
//     epd_bitmap_ezgif_frame_055,
//     epd_bitmap_ezgif_frame_056,
//     epd_bitmap_ezgif_frame_057,
//     epd_bitmap_ezgif_frame_058,
//     epd_bitmap_ezgif_frame_059,
//     epd_bitmap_ezgif_frame_060,
//     epd_bitmap_ezgif_frame_061,
//     epd_bitmap_ezgif_frame_062,
//     epd_bitmap_ezgif_frame_063,
//     epd_bitmap_ezgif_frame_064,
//     epd_bitmap_ezgif_frame_065,
//     epd_bitmap_ezgif_frame_066,
//     epd_bitmap_ezgif_frame_067,
//     epd_bitmap_ezgif_frame_068,
//     epd_bitmap_ezgif_frame_069,
//     epd_bitmap_ezgif_frame_070,
//     epd_bitmap_ezgif_frame_071,
//     epd_bitmap_ezgif_frame_072,
//     epd_bitmap_ezgif_frame_073,
//     epd_bitmap_ezgif_frame_074,
//     epd_bitmap_ezgif_frame_075,
//     epd_bitmap_ezgif_frame_076,
//     epd_bitmap_ezgif_frame_077,
//     epd_bitmap_ezgif_frame_078,
//     epd_bitmap_ezgif_frame_079,
//     epd_bitmap_ezgif_frame_080,
//     epd_bitmap_ezgif_frame_081,
//     epd_bitmap_ezgif_frame_082,
//     epd_bitmap_ezgif_frame_083,
//     epd_bitmap_ezgif_frame_084,
//     epd_bitmap_ezgif_frame_085,
//     epd_bitmap_ezgif_frame_086,
//     epd_bitmap_ezgif_frame_087,
//     epd_bitmap_ezgif_frame_088,
//     epd_bitmap_ezgif_frame_089,
//     epd_bitmap_ezgif_frame_090,
//     epd_bitmap_ezgif_frame_091,
//     epd_bitmap_ezgif_frame_092,
//     epd_bitmap_ezgif_frame_093,
//     epd_bitmap_ezgif_frame_094,
//     epd_bitmap_ezgif_frame_095,
//     epd_bitmap_ezgif_frame_096,
//     epd_bitmap_ezgif_frame_097,
//     epd_bitmap_ezgif_frame_098,
//     epd_bitmap_ezgif_frame_099,
//     epd_bitmap_ezgif_frame_100};

void clear()
{
    uart_puts("\033[2J");
    uart_puts("\033[H");
}
void displayCommand()
{
    uart_puts("\033[0GFor more information on specific command, type HELP command-name\n");
    uart_puts("\033[0GSHOWINFO            Display board revision\n");
    uart_puts("\033[0GCLEAR               Clear screen\n");
    uart_puts("\033[0GSETCOLOR            Set text color, and/or background color of the console\n");
}
void welcomeMessage()
{
    uart_puts("\n\033[36m ______   ______   ______   _______  \033[32m ___    _  _      ___     ___ \n");
    uart_puts("\033[36m|  ____| |  ____| |  ____| |__   __| \033[32m|__ \\  | || |    / _ \\   / _ \\ \n");
    uart_puts("\033[36m| |__    | |__    | |__       | |    \033[32m   ) | | || |_  | (_) | | | | |\n");
    uart_puts("\033[36m|  __|   |  __|   |  __|      | |    \033[32m  / /  |__   _|  \\__, | | | | |\n");
    uart_puts("\033[36m| |____  | |____  | |____     | |    \033[32m / /_     | |      / /  | |_| |\n");
    uart_puts("\033[36m|______| |______| |______|    |_|    \033[32m|____|    |_|     /_/    \\___/ \n");
    uart_puts("\n");
    uart_puts("\n");
    uart_puts("\033[36m ____               _____    ______     \033[32m ____     _____             \n");
    uart_puts("\033[36m|  _ \\      /\\     |  __ \\  |  ____| \033[32m   / __ \\   / ____|      \n");
    uart_puts("\033[36m| |_) |    /  \\    | |__) | | |__      \033[32m| |  | | | (___         \n");
    uart_puts("\033[36m|  _ <    / /\\ \\   |  _  /  |  __|    \033[32m | |  | |  \\___ \\        \n");
    uart_puts("\033[36m| |_) |  / ____ \\  | | \\ \\  | |____  \033[32m  | |__| |  ____) |    \n");
    uart_puts("\033[36m|____/  /_/    \\_\\ |_|  \\_\\ |______| \033[32m   \\____/  |_____/   \n");
    uart_puts("\n");
    uart_puts("\033[36mDeveloped by Thai Thuan\n\033[32m");
}
void getMacAddress()
{
    unsigned int *mac_data = NULL; // Pointer to response data
    // Set up mailbox buffer for MAC address query
    mbox_buffer_setup(ADDR(mBuf), 0x00010003, &mac_data, 6, 0, 0, 0, MBOX_TAG_LAST);

    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP))
    {

        uart_puts("MAC Address: ");

        printf("%x", mac_data[0] & 0xFF);

        uart_sendc(':');

        printf("%x", (mac_data[0] >> 8) & 0xFF);

        uart_sendc(':');

        printf("%x", (mac_data[1]) & 0xFF);

        uart_sendc(':');

        printf("%x", (mac_data[1] >> 8) & 0xFF);

        uart_puts("\n");
    }
    else
    {

        uart_puts("Unable to query!\n");
    }
}
void getBoardRevision()
{
    // unsigned int mBuf[36];
    unsigned int *board_data = NULL; // Pointer to response data
    // Set up mailbox buffer for Board Revision query
    mbox_buffer_setup(ADDR(mBuf), 0x00010002, &board_data, 4, 0, 0, 0, MBOX_TAG_LAST);

    if (mbox_call(ADDR(mBuf), MBOX_CH_PROP))
    {
        uart_puts("\033[0GBoard Revision: ");
        uart_hex(board_data[0]);
        uart_sendc(' ');
        // Compare board revision value with the table
        if (board_data[0] == 0x00900092)
        {
            uart_puts("rpi-Zero BCM2835 512MB Sony UK\n");
        }
        else if (board_data[0] == 0x00000010)
        {
            uart_puts("rpi-1B+ BCM2835\n");
        }
        else if (board_data[0] == 0x00a01041)
        {
            uart_puts("rpi-2B BCM2836 1GiB Sony UK\n");
        }
        else if (board_data[0] == 0x00a02082)
        {
            uart_puts("rpi-3B BCM2837 1GiB Sony UK\n");
        }
        else if (board_data[0] == 0x00b03111)
        {
            uart_puts("rpi-4B BCM2711 2GiB Sony UK\n");
        }
        else
        {
            uart_puts("Unknown board revision\n");
        }
    }
    else
    {

        uart_puts("Unable to query!\n");
    }
}
void showInfo()
{
    getBoardRevision();
    getMacAddress();
}
void setColor(char *s)
{
    char bColor[MAX_WORD_LEN] = "";
    char tColor[MAX_WORD_LEN] = "";
    int bState = 0;
    int tState = 0;

    while (*s != '\0')
    {
        if (*s == '-')
        {
            s++;
            if (*s == 't')
            {
                tState = 1;
                s += 2;
                int tIndex = 0;
                while (*s != ' ' && *s != '\0')
                {
                    tColor[tIndex++] = *s;
                    s++;
                }
            }
            else if (*s == 'b')
            {
                bState = 1;
                s += 2;
                int bIndex = 0;

                while (*s != ' ' && *s != '\0')
                {
                    bColor[bIndex++] = *s;
                    s++;
                }
            }
        }
        s++;
    }

    if (tState == 1)
    {

        if (isEqual(tColor, "black"))
        {
            uart_puts("\x1b[30m");
        }
        else if (isEqual(tColor, "red"))
        {
            uart_puts("\x1b[31m");
        }
        else if (isEqual(tColor, "green"))
        {
            uart_puts("\x1b[32m");
        }
        else if (isEqual(tColor, "yellow"))
        {
            uart_puts("\x1b[33m");
        }
        else if (isEqual(tColor, "blue"))
        {
            uart_puts("\x1b[34m");
        }
        else if (isEqual(tColor, "purple"))
        {
            uart_puts("\x1b[35m");
        }
        else if (isEqual(tColor, "cyan"))
        {
            uart_puts("\x1b[36m");
        }
        else if (isEqual(tColor, "white"))
        {
            uart_puts("\x1b[37m");
        }
    }
    if (bState == 1)
    {
        if (isEqual(bColor, "black"))
        {
            uart_puts("\x1b[40m");
        }
        else if (isEqual(bColor, "red"))
        {
            uart_puts("\x1b[41m");
        }
        else if (isEqual(bColor, "green"))
        {
            uart_puts("\x1b[42m");
        }
        else if (isEqual(bColor, "yellow"))
        {
            uart_puts("\x1b[43m");
        }
        else if (isEqual(bColor, "blue"))
        {
            uart_puts("\x1b[44m");
        }
        else if (isEqual(bColor, "purple"))
        {
            uart_puts("\x1b[45m");
        }
        else if (isEqual(bColor, "cyan"))
        {
            uart_puts("\x1b[46m");
        }
        else if (isEqual(bColor, "white"))
        {
            uart_puts("\x1b[47m");
        }
    }
    else if (bState == 0 && tState == 0)
    {
        uart_puts("\033[0GWrong command format\n");
    }
    uart_puts("\033[0G");
}
void clearScreen()
{
    for (int i = 0; i < screenWidth; i++)
    {
        for (int j = 0; j < screenHeight; j++)
        {
            drawPixelARGB32(i, j, 0x00000000);
        }
    }
}
void displayImage(int width, int height, unsigned int *src)
{
    // Calculate the visible area based on the scrollPosition
    int endY = height;
    int count = 0;
    for (int y = 0; y < endY; y++)
    {
        for (int x = 0; x < screenWidth; x++)
        {
            // Check if we're within the image bounds
            if (x < width && y + scrollPosition <= height)
            {
                // Get the pixel from the image

                unsigned int pixel = src[(y + scrollPosition) * width + x];
                // Draw the pixel at the current screen position
                drawPixelARGB32(x, y, pixel);
            }
            else
            {
                // If we're outside the image bounds, clear the pixel
                drawPixelARGB32(x, y, 0x00000000); // Set to transparent (black)
            }
        }
    }
}
// void displayVideo(int width, int height)
// {
//     clearScreen();
//     for (int i = 0; i < FRAMES_LEN; i++)
//     {
//         displayImage(width, height, frames[i]);
//         wait_msec(70000);
//     }
// }
void pressKey()
{
    drawChar('P', 200, 280, 0x000000FF);
    drawChar('R', 250, 280, 0x000000FF);
    drawChar('E', 300, 280, 0x000000FF);
    drawChar('S', 350, 280, 0x000000FF);
    drawChar('S', 400, 280, 0x000000FF);
    drawChar('A', 500, 280, 0x000000FF);
    drawChar('N', 550, 280, 0x000000FF);
    drawChar('Y', 600, 280, 0x000000FF);
    drawChar('K', 700, 280, 0x000000FF);
    drawChar('E', 750, 280, 0x000000FF);
    drawChar('Y', 800, 280, 0x000000FF);
    drawChar('T', 300, 350, 0x000000FF);
    drawChar('O', 350, 350, 0x000000FF);
    drawChar('S', 450, 350, 0x000000FF);
    drawChar('T', 500, 350, 0x000000FF);
    drawChar('A', 550, 350, 0x000000FF);
    drawChar('R', 600, 350, 0x000000FF);
    drawChar('T', 650, 350, 0x000000FF);
}
void handleReq(char *input)
{
    toLowerCase(input);
    char word[MAX_WORD_LEN];
    getFirstWord(input, word);

    if (isEqual(input, "help") == 1)
    {
        displayCommand();
    }
    else if (isEqual(input, "help showinfo") == 1)
    {
        uart_puts("\033[0GCommand: SHOWINFO\n");
        uart_puts("\033[0GUsage: Display board revision\n");
    }
    else if (isEqual(input, "help clear") == 1)
    {
        uart_puts("\033[0GCommand: Clear\n");
        uart_puts("\033[0GUsage: Clear CLI screen\n");
    }
    else if (isEqual(input, "help setcolor"))
    {
        uart_puts("\033[0GCommand: Setcolor -t <text color> -b <background color>\n");
        uart_puts("\033[0GUsage: Set text color (-t), and/or background color (-b) of the console to one of the following color:\nBLACK, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE\n");
    }
    else if (isEqual(input, "showinfo") == 1)
    {
        showInfo();
    }
    else if (isEqual(input, "clear") == 1)
    {
        clear();
    }
    else if (isEqual(input, "image") == 1)
    {
        isShowing++;
        displayImage(720, 1000, images[currentImage]);
    }
    else if (isEqual(input, "video") == 1)
    {
        // displayVideo(640, 360);
    }
    else if (isEqual(word, "setcolor") == 1)
    {
        setColor(input);
    }
    else if (isEqual(input, "game") == 1)
    {
        clearScreen();
        displayImage(800, 600, introGame);
        char ch;
        do
        {
            ch = uart_getc();
        } while (ch == 0);
        clearScreen();
        gamePlay();
    }
    uart_puts("\033[0GMyOS> ");
}