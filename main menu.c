#define TB_IMPL
#include "termbox2.h"

#include "create_project.h" // To call the create project function.
#include "load_project_menu.h" // To call the load project menu function.

#include <ctype.h> // For converting to string.

#include "globals.h" // For defining global variables that can be used across all compilation files (header file functions included). 

#include <stdbool.h> // For using bool

void InterruptConf(struct tb_event interrupt, int center_x, int quarter3_d_y, bool* P_active, bool* P_cancel, bool exit);

int main() 
{
	tb_init(); // Initilalize termbox2

	// Screen height and width
	int width = tb_width(); 
	int height = tb_height();

	// Window center coordinates.
	int center_x = width / 2; 
	int center_y = height / 2;

	// 1/4 away from the top of the window.
	int quarter_d_y = center_y / 2;

	// 3/4 away from the top of the window.
	int quarter3_d_y = (center_y / 2) * 3;

	// Event to listen out for input.
	struct tb_event ev;

	// Event to listen out for Ctrl+C interrupt confirmation.
	struct tb_event interrupt;

	bool active = true; // Menu active? 
	bool* P_active = &active; // Pointer to active.

	bool cancel; // Cancel input?
	bool* P_cancel = &cancel; // Pointer to cancel.	
	
	// Check for input:
	while(active)
	{	
		// Main menu graphics:
		tb_print(center_x-14, quarter_d_y-1, TB_DEFAULT, TB_DEFAULT, "|||||||||||||");
		tb_print(center_x-14, quarter_d_y, TB_DEFAULT, TB_DEFAULT, "| MAIN MENU |");	
		tb_print(center_x-14, quarter_d_y+1, TB_DEFAULT, TB_DEFAULT, "|||||||||||||");
	
		tb_print(center_x-20, center_y-2, TB_DEFAULT, TB_DEFAULT, "1. Make project");
	 	
		tb_print(center_x-20, center_y, TB_DEFAULT, TB_DEFAULT, "2. Load project menu");

		tb_print(center_x-20, center_y+2, TB_DEFAULT, TB_DEFAULT, "3. Delete project menu");

		tb_print(center_x-20, center_y+4, TB_DEFAULT, TB_DEFAULT, "4 (or Ctrl+C). Exit");
	
		tb_present(); // Render the screen.
		*P_cancel = false; // Cancel input set to false (don't cancel the input when writing the project name or etc.).
		tb_poll_event(&ev); // Check for an event (input event).
		
		if(ev.ch == '1') // Create project:
		{

			while (!*P_cancel) // While the input isn't cancelled:
			{
				char *name = (char*) malloc(30*sizeof(char)); // Name of the project.	
				memset(name, 0, 30);  // Initialize the buffer to prevent garbage characters.
	
				int maxlen = 30; // Stores the max length of project name.
				char remaining[4]; // Stores the remaining characters of project name.		

				// Empty out the previous output.
				tb_print(center_x-20, quarter3_d_y, TB_DEFAULT, TB_DEFAULT, "                                                              ");						
				tb_print(center_x-20, quarter3_d_y, TB_DEFAULT, TB_DEFAULT, "Project name (30 chars max), '/' to stop, 'Ctrl+C' to cancel: ");					
				tb_present(); // Render the screen.

				for (int i = 0; i < 30;) // Check input:
				{
					tb_poll_event(&ev); // Event to check for keypress.

					if (ev.key == TB_KEY_CTRL_C) // If Ctrl+C is pressed:
					{
						InterruptConf(interrupt, center_x, quarter3_d_y, P_active, P_cancel, false); // If interupt is confirmed = change *P_cancel to true.
						if (*P_cancel) // If *P_cancel is true:
						{ 
							break; // Break out of the for loop.
						}	
					}
					if (ev.ch == '/' || i == 30) // Stop input using '\0' if '/' char is inputed or if the number of characters entered is 30 (to prevent overflow).
					{
						name[i] = '\0';
						break;	
					}
					if ((ev.key == TB_KEY_BACKSPACE || ev.key == TB_KEY_BACKSPACE2) && i != 0) // If there was a backspace and the input buffer isn't empty:	
					{	
						--i; // Decrement i by 1.
						name[i] = ' '; 	// Make the previous i equal to an empty space.
						++maxlen; // Increment maxlen by 1.
					}
					else if (isalpha(ev.ch) || isdigit(ev.ch)) // If the character inputed is alphabetical:
					{
						name[i] = ev.ch; // Make that character equal to the current char in the name buffer.
						++i; // Increment i by 1.
						--maxlen; // Decrement maxlen by 1.
					}
					else {} // If none of these conditions are true, dont do anything.
			
					tb_print(center_x-20, quarter3_d_y+2, TB_DEFAULT, TB_DEFAULT, name); // Output each char of the project.
			
					snprintf(remaining, sizeof(maxlen), "%d", maxlen);  // Convert maxlen to string.

					// Replace the second byte of the remaining char variable with a space to make the output 1 digit long, like "4" instead of something like "40".
					if (maxlen < 10) 
					{
						remaining[1] = ' ';
					}
				
					tb_print(center_x-20, quarter3_d_y+4, TB_DEFAULT, TB_DEFAULT, "Chars remaining:"); // Output how much characters the user has left.
					tb_print(center_x-2, quarter3_d_y+4, TB_DEFAULT, TB_DEFAULT, remaining); // Output how much characters the user has left.		
				
					tb_present(); // Render the screen.
			
				}

				int size_of_name = strlen(name); // Get the size of the name string variable.
				
				// If *P_cancel is true:
				if (*P_cancel)
				{
					// Clear the input prompt to input the name.
					tb_print(center_x-20, quarter3_d_y, TB_DEFAULT, TB_DEFAULT, "                                                              ");					
					// Clear out "Chars remaining" and output after that.				
					tb_print(center_x-20, quarter3_d_y+4, TB_DEFAULT, TB_DEFAULT, "                    ");

					// Clear out the currently displayed name buffer. 
					for(int i = 0; i <= size_of_name; ++i)
					{	
						tb_print(center_x-20+i, quarter3_d_y+2, TB_GREEN, TB_DEFAULT, " ");
					}	
 
					// Clear out "Creating project...".
					tb_print(center_x-20, quarter3_d_y+10, TB_YELLOW, TB_DEFAULT, "                   ");
 
					// Break out of the while loop.
					break;
				}
				else // If it's false continue:
				{

					// Output the confirmed project name in green, plus a '.c' extension at the end.
					tb_print(center_x-20, quarter3_d_y+2, TB_GREEN, TB_DEFAULT, name); 			
					tb_print(center_x-20+size_of_name, quarter3_d_y+2, TB_GREEN, TB_DEFAULT, ".c"); 
			
					create_project(center_x, center_y, name); // Create project function, defined in "create project.h" and create project.c" files.	
	
				}
	
				free(name); // Free the buffer.
				*P_cancel = true; // Cancel input for the next iteration (if the input hasn't been cancelled by the user).
			}
			
		}	
		else if(ev.ch == '2') // Load project menu:
		{
			// Clear the screen entirely.
			tb_clear();
			
			// Load up the load project menu.
			load_project_menu(center_x, center_y, quarter_d_y);	
		}
		else if(ev.ch == '3') // Delete project menu:
		{		
			// Empty out the previous output.
			tb_print(center_x-20, quarter3_d_y, TB_DEFAULT, TB_DEFAULT, "                                                              ");					
			
			tb_print(center_x-20, quarter3_d_y, TB_DEFAULT, TB_DEFAULT, "Going the the delete projects menu...");	
		}
		else if((ev.ch == '4') || (ev.key == TB_KEY_CTRL_C)) // Exit:
		{
			InterruptConf(interrupt, center_x, quarter3_d_y, P_active, P_cancel, true);
		}
		else // If input doesn't match 1,2,3 or 4:
		{	
			// Empty out the previous output.
			tb_print(center_x-20, quarter3_d_y, TB_DEFAULT, TB_DEFAULT, "                                                              ");				
			
			tb_print(center_x-20, quarter3_d_y, TB_WHITE, TB_RED, "Please enter a valid input");
			tb_present();
		}
	}
	
	tb_present(); //render screen

	tb_poll_event(&ev); //stop to check for an event so the program can stop for the user to see the output 
	
	tb_shutdown(); // stop termbox2

	return 0;
}


void InterruptConf(struct tb_event interrupt, int center_x, int quarter3_d_y, bool* P_active, bool* P_cancel, bool exit)
{
	tb_print(center_x-20, quarter3_d_y+10, TB_WHITE, TB_RED, "Are you sure?"); // Ask for a confirmation.
	tb_present(); // Refresh the screen.
	tb_poll_event(&interrupt); // Check for confirmation using a new input event.

	if ((interrupt.ch == '4') || (interrupt.key == TB_KEY_CTRL_C)) // If Ctrl+C is pressed again:
	{
		if (exit) 
		{
			// Shutdown termbox2 and make the menu while loop false.
			tb_shutdown();
			*P_active = false;
		}
		else
		{
			// Cancel the current operation.
			*P_cancel = true;		
		}
	}
	else // If not:
	{
		tb_print(center_x-20, quarter3_d_y+10, TB_DEFAULT, TB_DEFAULT, "             "); // Clear the the confirmation question and dont interrupt.
		tb_present();
	}
}
