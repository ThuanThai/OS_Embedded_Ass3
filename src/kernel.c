#include "handleKernel.h"
#include "printf.h"
// #include "imagePixel.h"

#define MAX_CMD_SIZE 100
#define MAX_HISTORY_SIZE 10
#define TAB_LEN 100
#define SCROLL_AMOUNT 10
#define IMAGES_ARRAY_LEN 2

char command_history[MAX_HISTORY_SIZE][MAX_CMD_SIZE];
int history_index = 0; // Index of the current command in history
int history_count = 0; // Total number of commands in history

const char *available_cmds[] = {
	"help",
	"setcolor",
	"showinfo",
	"clear",
};
void displayName()
{
	drawChar('N', 150, 125, 0x00FF5733); // Coral
	drawChar('G', 200, 125, 0x00E74C3C); // Alizarin
	drawChar('U', 250, 125, 0x00F39C12); // Sunflower
	drawChar('Y', 300, 125, 0x00F1C40F); // Emerald
	drawChar('E', 350, 125, 0x0039B552); // Nephritis
	drawChar('N', 400, 125, 0x002ECC71); // Green Sea
	drawChar('H', 500, 125, 0x00E08283); // Indian Red
	drawChar('U', 550, 125, 0x00CD6155); // Dark Coral
	drawChar('N', 600, 125, 0x00D98880); // Antique Ruby
	drawChar('G', 650, 125, 0x00D98880); // Antique Ruby
	drawChar('A', 750, 125, 0x00FF5733); // Coral
	drawChar('N', 800, 125, 0x00E74C3C); // Alizarin
	drawChar('H', 850, 125, 0x00CD6155); // Dark Coral

	drawChar('H', 150, 200, 0x00F1C40F); // Emerald
	drawChar('O', 200, 200, 0x0039B552); // Nephritis
	drawChar('A', 250, 200, 0x00F39C12); // Sunflower
	drawChar('N', 300, 200, 0x00E74C3C); // Alizarin
	drawChar('G', 350, 200, 0x00E74C3C); // Alizarin
	drawChar('P', 450, 200, 0x00D98880); // Antique Ruby
	drawChar('H', 500, 200, 0x00CD6155); // Dark Coral
	drawChar('U', 550, 200, 0x00CD6155); // Dark Coral
	drawChar('C', 600, 200, 0x00CD6155); // Dark Coral

	drawChar('T', 150, 275, 0x00F1C40F); // Emerald
	drawChar('A', 200, 275, 0x0039B552); // Nephritis
	drawChar('N', 250, 275, 0x00F39C12); // Sunflower
	drawChar('P', 350, 275, 0x00E74C3C); // Alizarin
	drawChar('H', 400, 275, 0x00E74C3C); // Alizarin
	drawChar('O', 450, 275, 0x00E74C3C); // Alizarin
	drawChar('N', 500, 275, 0x00E74C3C); // Alizarin
	drawChar('G', 550, 275, 0x00E74C3C); // Alizarin

	drawChar('T', 150, 350, 0x00FF5733); // Coral
	drawChar('H', 200, 350, 0x00FF5733); // Coral
	drawChar('A', 250, 350, 0x00F1C40F); // Emerald
	drawChar('I', 300, 350, 0x00D98880); // Antique Ruby
	drawChar('T', 400, 350, 0x0039B552); // Nephritis
	drawChar('H', 450, 350, 0x00FF5733); // Coral
	drawChar('U', 500, 350, 0x00F39C12); // Sunflower
	drawChar('A', 550, 350, 0x00F1C40F); // Emerald
	drawChar('N', 600, 350, 0x00D98880); // Antique Ruby
}
void cli()
{
	static char cli_buffer[MAX_CMD_SIZE];
	static int index = 0;
	int available_cmds_len = array_length(available_cmds);
	int tab_count = 0;
	// read and send back each char
	char c = uart_getc();
	uart_sendc(c);
	// put into a buffer until got new line character
	if (c != '\n')
	{
		if (c == '\b' || c == 127) // Handle both backspace and Delete key (ASCII 127)
		{
			isShowing--;
			scrollPosition = 0;
			if (index > 0)
			{
				index--;
				uart_sendc('\b'); // Move cursor back
				uart_sendc(' ');  // Erase character with a space
				uart_sendc('\b'); // Move cursor back again
			}
		}
		else if (c == 9)
		{
			int num_matches = 0;
			char matching_cmds[10][MAX_CMD_SIZE];
			char partial_cmd[MAX_CMD_SIZE];
			// copy clibuffer to partial_cmd
			strncpy(partial_cmd, cli_buffer, index);
			partial_cmd[index] = '\0';

			for (int i = 0; i < available_cmds_len; i++)
			{

				if (strncmp(available_cmds[i], partial_cmd, strlen(partial_cmd)) == 0)
				{
					strncpy(matching_cmds[num_matches], available_cmds[i], MAX_CMD_SIZE - 1);
					matching_cmds[num_matches][MAX_CMD_SIZE - 1] = '\0'; // Ensure null-termination
					num_matches++;
				}
			}
			// remove all included "MyOS> "
			index += TAB_LEN;
			while (index > 0)
			{
				index--;
				uart_sendc('\b'); // Move cursor back
				uart_sendc(' ');  // Erase character with a space
				uart_sendc('\b'); // Move cursor back again
			}
			if (num_matches == 1)
			{
				strncpy(cli_buffer, matching_cmds[0], MAX_CMD_SIZE);
				index = strlen(cli_buffer);
				printf("MyOS> %s", matching_cmds[0]);
			}
			else
			{
				// Display the list of possible completions to the user
				uart_puts("\nPossible completions:");
				for (int i = 0; i < num_matches; i++)
				{
					uart_puts("\n- ");
					uart_puts(matching_cmds[i]);
				}
				uart_puts("\nMyOS> ");
				index = 0;
			}
			// Clear the matching_cmds array before each TAB key press
			for (int i = 0; i < 10; i++)
			{
				for (int j = 0; j < MAX_CMD_SIZE; j++)
				{
					matching_cmds[i][j] = '\0';
				}
			}
			// remove tab
		}
		else if (c == '_')
		{
			index++;
			if (history_count > 0 && history_index > 0)
			{
				history_index--;
				// Clear the current command display

				while (index > 0)
				{
					index--;
					uart_sendc('\b'); // Move cursor back
					uart_sendc(' ');  // Erase character with a space
					uart_sendc('\b'); // Move cursor back again
				}
				// Display the previous command from history
				strncpy(cli_buffer, command_history[history_index], MAX_CMD_SIZE);
				index = strlen(cli_buffer);
				printf("%s", cli_buffer);
			}
			else
			{
				while (index > 0)
				{
					index--;
					uart_sendc('\b'); // Move cursor back
					uart_sendc(' ');  // Erase character with a space
					uart_sendc('\b'); // Move cursor back again
				}
			}
		}
		else if (c == '+')
		{
			index++;
			if (history_count > 0 && history_index < history_count - 1)
			{
				history_index++;
				// Clear the current command display
				while (index > 0)
				{
					index--;
					uart_sendc('\b'); // Move cursor back
					uart_sendc(' ');  // Erase character with a space
					uart_sendc('\b'); // Move cursor back again
				}
				// Display the next command from history
				strncpy(cli_buffer, command_history[history_index], MAX_CMD_SIZE);
				index = strlen(cli_buffer);
				printf("%s", cli_buffer);
			}
			else if (c == "w")
			{
			}
			else
			{
				while (index > 0)
				{
					index--;
					uart_sendc('\b'); // Move cursor back
					uart_sendc(' ');  // Erase character with a space
					uart_sendc('\b'); // Move cursor back again
				}
			}
		}
		else if (c == 'w' || c == 's')
		{
			cli_buffer[index++] = c; // Store into the buffer
			if (isShowing == 1)
			{
				while (index > 0)
				{
					index--;
					uart_sendc('\b'); // Move cursor back
					uart_sendc(' ');  // Erase character with a space
					uart_sendc('\b'); // Move cursor back again
				}

				if (c == 'w' && scrollPosition > 0)
				{
					scrollPosition -= SCROLL_AMOUNT;
				}
				else
				{
					if (c == 's' && scrollPosition < 1024 / 3)
					{
						scrollPosition += SCROLL_AMOUNT;
					}
				}

				displayImage(720, 1000, images[currentImage]);
			}
		}
		else if (c == 'a' || c == 'd')
		{
			cli_buffer[index++] = c; // Store into the buffer
			if (isShowing == 1)
			{
				while (index > 0)
				{
					index--;
					uart_sendc('\b'); // Move cursor back
					uart_sendc(' ');  // Erase character with a space
					uart_sendc('\b'); // Move cursor back again
				}
				if (c == 'a' && currentImage > 0)
				{
					currentImage--;
					clearScreen();
				}
				else if (c == 'd' && currentImage < IMAGES_ARRAY_LEN - 1)
				{
					currentImage++;
					clearScreen();
				}

				displayImage(720, 1000, images[currentImage]);
			}
		}
		else
		{
			cli_buffer[index] = c; // Store into the buffer
			index++;
		}
	}
	else // Handle newline
	{
		cli_buffer[index] = '\0';
		uart_puts("MyOS> ");
		handleReq(cli_buffer);
		index = 0; // Reset index for the next command
		// Update command history
		strncpy(command_history[history_count % MAX_HISTORY_SIZE], cli_buffer, MAX_CMD_SIZE);
		history_count++;
		history_index++;
	}
}
void main()
{
	uart_init();
	framebf_init();
	displayName();
	welcomeMessage();

	// printf("Characters: %c %c \n", 'a', 65);
	// printf("Decimals: %d %d\n", 1977, 650000L);
	// printf("Preceding with blanks: %10d \n", 1977);
	// printf("Preceding with zeros: %010d \n", 1977);
	// printf("Some different radices: %d %x %o  \n", 100, 100, 100);
	// printf("floats: %4.2f\n", 3.1416);
	// printf("%s \n", "A string");

	// unsigned int *physize = 0; // pointer to response data
	// mbox_buffer_setup(ADDR(mBuf), MBOX_TAG_SETPHYWH, &physize, 8, 0, 1024, 720, MBOX_TAG_LAST);
	// mbox_call(ADDR(mBuf), MBOX_CH_PROP);
	// uart_puts("\nGot Actual Physical Width: ");
	// uart_dec(physize[0]);
	// uart_puts("\nGot Actual Physical Height: ");
	// uart_dec(physize[1]);
	// uart_sendc('\n');

	uart_puts("\033[0GMyOS> ");
	// run CLI
	while (1)
	{
		cli();
	}
}
