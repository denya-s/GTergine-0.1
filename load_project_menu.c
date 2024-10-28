#include "termbox2.h"
#include "globals.h" // For using global variables.

#include <stdlib.h>
#include <stdio.h> // For working with files.

#include <dirent.h> // For working with directories.
#include <string.h>


int initialise_files(int* P_num_of_pfiles, char **filenames, int option)
{
	struct dirent *entry; // Creates a structure that will store filenames on each iteration.
	const char *directory_path = "../projects"; // Specifies the directory path.

	// Read each entry in the directory:
   	while ((entry = readdir(directory)) != NULL) 
	{
        
		// Check if entry is a regular file and ends with ".c".
        	if (entry->d_type == DT_REG) // DT_REG means a regular file.
		{
			// Make the filename point to the name of the file on each iteration.
            		const char *filename = entry->d_name;
			
			// Get the length of filename in character amount.
            		size_t len = strlen(filename);

            		// Check if file extension is ".c":
            		if (len > 2 && strcmp(filename + len - 2, ".c") == 0) 
			{
				if(option == 1) 
				{
					P_num_of_pfiles++; // If it is and the option is set to 1: increase the project file count.
				}
				else if
				{

				}
				else 
				{
					fprintf(stderr, "Option not correctly specified\n");
        				return 1;

				}
            		}
        	}
    	}


	return 0;
}

int load_project_menu(int center_x, int center_y, int quarter_d_y) 
{

/*

	1. Allow the user to execute code in the project within the local environment.

	2.  (separate file) Allow the user to change whats going on in the local environment, thus changing the code of the original project file.

	3. Close the file returning to the load project menu.

	4. Close the load project menu returning to the main menu. 
*/
	 
	bool active = true; // Menu active? 
	bool* P_active = &active; // Pointer to active.

	// For taking user_input.
	struct tb_event input; 

	// To store the number of files in the projects directory.
	int num_of_pfiles = 0;
	int* P_num_of_pfiles = &num_of_pfiles; // pointer to num_of_pfiles to pass it to a function by reference.

		// Open the directory
    	DIR *directory = opendir(directory_path);
    	if (directory == NULL) 
	{
        	fprintf(stderr, "Unable to open directory\n");
        	return 1;
    	}

	
    	// Count the amount of project files in the project file directory.
	int initialise_files(P_num_of_pfiles, filenames, 1)


	// Array buffer for storing filenames.
	char **filenames = (char **)malloc(num_of_pfiles * sizeof(char)); 
	
	if (filenames == NULL) 
	{
        	fprintf(stderr, "Memory allocation for filenames array failed\n");
        	return 1; // Error exit.
    	}

	// Allocate memory for each filename:
	for (int i = 0; i < *P_num_of_pfiles; i++) {

		// Maximum length of a project file name (30 characters) + ".c" (2 characters) + null terminator (1).
        	filenames[i] = (char *)malloc(33 * sizeof(char));
        	
		// Check if there are no project filenames and print an error message:
		if (filenames[i] == NULL) {
            		fprintf(stderr, "Memory allocation for filename %d failed\n", i);
            
			// Free previously allocated memory before exiting:
            		for (int j = 0; j < i; j++) 
			{
                		free(filenames[j]);
            		}
            		
			free(filenames);
            	
			return 1; // Error exit
        	}
    	}
	

    	// Put the files names into each element of the filenames array.
	int initialise_files(P_num_of_pfiles, filenames, 2)

	// Convert num_of_pfiles into a string format to print it out:
	char Project_File_Count[25];
	snprintf(Project_File_Count, sizeof(Project_File_Count), "Project file count: %d", num_of_pfiles);

	char Menu_Decoration_1[41]; // 40 '|' characters + 1 for the null terminator.

    	// Fill the array with '|' characters:
    	for (int i = 0; i < 40; i++) {
        	Menu_Decoration_1[i] = '|'; // Assign '|' to each position.
    	}
    
    	Menu_Decoration_1[41] = '\0'; // Null terminate the string.
	
	
	while(active)
	{

		// Main menu graphics:
		tb_print(center_x-14, quarter_d_y-1, TB_DEFAULT, TB_DEFAULT, "|||||||||||||||||||||");
		tb_print(center_x-14, quarter_d_y, TB_DEFAULT, TB_DEFAULT,   "| LOAD PROJECT MENU |");	
		tb_print(center_x-14, quarter_d_y+1, TB_DEFAULT, TB_DEFAULT, "|||||||||||||||||||||");

		tb_print(center_x-20, center_y-4, TB_DEFAULT, TB_DEFAULT, Project_File_Count);
		
		tb_print(center_x-18, center_y-2, TB_DEFAULT, TB_DEFAULT, Menu_Decoration_1);

		for (int i = 0; i <= num_of_pfiles; ++i)
		{

			tb_print(center_x-20, center_y+i+1, TB_DEFAULT, TB_DEFAULT, );


		}

		tb_print(center_x-18, center_y-, TB_DEFAULT, TB_DEFAULT, Menu_Decoration_1);



		tb_present();
		tb_poll_event(&input);

		active = false;
	 
		// Close the directory
    		closedir(directory);
	}	
	

	return 0;
}



