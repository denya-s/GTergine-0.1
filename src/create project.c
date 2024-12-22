#include "termbox2.h"
#include "globals.h"
#include <stdio.h>

int create_project(int center_x, int center_y, char* name)
{
	// Generate a string for specifiying the project location, name and file extension.
	char finalname[256];
	
	snprintf(finalname, sizeof(finalname), "../projects/%s.cpp", name);

	// Create the project file.
  	FILE *ProjectFile = fopen(finalname, "w"); 

	//Write all the necessary setup stuff into it:
	const char *needed_stuff[] = 
	{
	
	"#define TB_IMPL;", 
	"#include <termbox2.h>;", 
	"", 
	"int main()",  
	"{",
	"", 
	"return 0;", 
	"",
	"}"
	
	};

	// Get the total number of elements in the needed_stuff array.
	int sizeof_needed_stuff = sizeof(needed_stuff)/sizeof(needed_stuff[0]);

	// Loop through the needed_stuff array and write everything thats needed string by string into the project file
	for (int i = 0; i < sizeof_needed_stuff; ++i)
	{
		fputs(needed_stuff[i], ProjectFile); 
        	fputs("\n", ProjectFile);  // Add a newline after each line		
	}
 
	// Close the file.
	fclose(ProjectFile);

	// Create buffer to store the output.
	char output[512] = {0};

	// Output the project file name and its extension and where it is located.
	snprintf(output, sizeof(output), "Created new project file in ../projects/: %s.cpp", name);
	tb_print(center_x-(strlen(output)/2), center_y, TB_WHITE, TB_GREEN, output);

    // Refresh the screen to show the output of the function. 
    tb_present();	

	// Event to listen out for any input to resume.
	struct tb_event pause;
    tb_poll_event(&pause);

    return 0;
}
