#include "extentions.h"
#include "framebf.h"

int scrollPosition;
int isShowing;
int currentImage;
extern unsigned int *images[];
extern unsigned int *frames[];
extern unsigned int *gameContent[];
void welcomeMessage();
void displayCommand();
void showInfo();
void clear();
void setColor(char *s);
void displayImage(int width, int height, unsigned int *src);
void handleReq(char *input);
void clearScreen();
void displayOffsetImage(int width, int height, int offsetX, int offsetY, unsigned int *src);