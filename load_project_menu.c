#include "termbox2.h"
#include "globals.h" // For using global variables.

#include <stdlib.h>
#include <stdio.h> // For working with files.

#ifdef _WIN32
#include <windows.h> // For working with directories in Windows.
#else
#include <dirent.h> // For working with directories in Linux.
#endif

#include <dirent.h> 
#include <string.h>
#include <stdbool.h> // For bool.


// Helper function to check if a file has a .c extension:
// Takes a constant char array pointer variable, representing the file name.
int has_c_extension(const char *filename) {
  
    // Creates a const char pointer that stores the result of a search for a '.' character in the given filename name. Performes the search from right to left using strchr().
    const char *dot = strrchr(filename, '.'); 

    // If there is a dot and it pointes to a ".c" string at the end of the filename - return 0, if not return 1.    
    return (dot && strcmp(dot, ".c") == 0);
}

// Function to add a filename to a dynamic array of strings:
void add_filename(char ***filenames, int *count, const char *filename, int *filenames_size) {

    // Calculate the new size needed, accounting for an extra pointer for the new filename.
    *filenames_size = (*filenames_size) + (sizeof(char *) + strlen(filename) + 1);    
    
    printf("Filename: %s | Filename length: %d", filename, strlen(filename)); 

    // Increase the array size of "filenames" to the new capacity of characters.
    *filenames = realloc(*filenames, *filenames_size);
    
    // If *filenames is NULL then do: 
    if (*filenames == NULL) 
    {
        // Print an error message and exit the program. 
        perror("* Failed to resize filename array *");
        exit(1);
    }

    // Allocate memory for the new filename and add it to the array:
   
    // Create space for the next filename in the array.   
    (*filenames)[*count] = (char *) malloc(strlen(filename) + 1);
    
    // If the current element of the "filenames" array is NULL, do: 
    if ((*filenames)[*count] == NULL) 
    {
        // Print out an error message and exit the program. 
        perror("* Failed to allocate memory for filename *");
        exit(1);
    }
    
    // Copy the filename name into to the current space for it in the "filenames" array. 
    strcpy((*filenames)[*count], filename); 
   
    // Increment the file number counter. 
    (*count)++;
}

// Function to free the dynamic array of strings
void free_filenames(char ***filenames, int count) 
{ 
    for (int i = 0; i < count; i++) 
    {
        free((*filenames)[i]);
    }
    free(*filenames);
}

// Function to list files in the project/ directory:
// Takes a constat char array pointer variable, representing the path to the directory. 
void list_c_files(const char *path, int *file_count, char ***filenames, int *filenames_size) {

    // If Windows was detected - execute the next piece of code: 
    #ifdef _WIN32

    // Creates a Windows specific structure for storing information about files and directories found in a search. 
    // Holds details like the filename and file attributes.    
    WIN32_FIND_DATA findFileData; 
    
    // Will store the result of the "findFileData" variable that initialises the search for file and directories. 
    HANDLE hFind;

    // Declares a character array "search_path" to store the directory path with a wildcard character (*). 
    // This wildcard is used to match all files and directories in the specified path.
    char search_path[2048];

    // "snprintf" safely writes the formatted string into "search_path", ensuring it does not exceed sizeof(search_path).
    // "%s\\*" appends the wildcard * to the specified path, enabling FindFirstFile to find all files in the directory. 
    snprintf(search_path, sizeof(search_path), "%s\\*", path); 

    // "FindFirstFile" takes "search_path" and stores details of the first file or directory found in "findFileData".
    // If successful, "hFind" stores a handle to the search for use in subsequent calls to "FindNextFile".
    hFind = FindFirstFile(search_path, &findFileData);

    // If "FindFirstFile" returns INVALID_HANDLE_VALUE, the directory could not be opened.
    // An error message is printed, and the function returns early.
    if (hFind == INVALID_HANDLE_VALUE) 
    {
        // Print an error message to the terminal screen and provide the directory that could not be opened and return early from the function.
        printf("Could not open directory: %s\n", path);
        return;
    }

    // If everything is okay search for each file in the given directory path, execute this do-while loop:
    do {

        // Assigns the file or directory name found in findFileData.cFileName to name for easier reference. 
        const char *name = findFileData.cFileName;
       
        // If the current file has a ".c" extension, do this: 
        if (has_c_extension(name) && (strlen(name) <= 30)) 
        {
            // Add the filename to the filenames string array on each iteration.
            add_filename(filenames, file_count, name, filenames_size)
        }

    } while (FindNextFile(hFind, &findFileData) != 0); // Loop over the directory until there are no more files left.

    // Closes the search handle "hFind" with "FindClose", releasing system resources associated with the directory search.
    FindClose(hFind);

    #else // If Windows was not detected, execute this piece of code (for Linux).

    // This structure represents directory entries and provides fields for the file name (d_name) and the file type (d_type).   
    struct dirent *entry;
  
    // Opens the specified directory path with "opendir", returning a pointer of type DIR to represent the open directory stream. 
    DIR *dir = opendir(path);

    // "dir" returns NULL if the directory cannot be opened.
    if (dir == NULL) {
        // Print an error message to the terminal screen and return early from the function.
        perror("Could not open directory");
        return;
    }

    // Iterate through each file in the directory until they end, in that case readdir() returns a NULL.
    while ((entry = readdir(dir)) != NULL)
    {

        // Skip current directory (".") and parent directory ("..")
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // If the current file entry name has a ".c" extension at the end, execute this code:       
        if (has_c_extension(entry->d_name) && (strlen(entry->d_name) <= 30))
        {
            // Add the filename to the filenames string array on each iteration.
            add_filename(filenames, file_count, entry->d_name, filenames_size);
        }
    }
    
    // Closes the directory stream, to free resources associated with the directory listing.
    closedir(dir);

    // End the preprocessor directive if statement.
    #endif
}


int load_project_menu(int center_x, int center_y, int quarter_d_y) 
{

/*

	1. Allow the user to execute code in the project within the local environment.
    
    2. (separate file) Allow the user to change whats going on in the local environment, thus changing the code of the original project file.

	3. Close the file returning to the load project menu.

	4. Close the load project menu returning to the main menu. 

*/
	
    // Specify the search path.
    const char *path = "../projects/";
        
    // For storing the amount of project files (.c) files in the given path.
    int file_count = 0;

    // For storing the total size of the filenames array (in bytes).
    int filenames_size = 0;

    // Allocate memory for the filenames array of strings: 
    char **filenames = malloc(31 * sizeof(char *));
    if (filenames == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1; // Exit if memory allocation fails.
    } 
    
    memset(filenames, 0, 30);  // Initialize the filenames buffer to prevent garbage characters.


    // Search for filenames, count their number and store each of them in a filenames string array.
    list_c_files(path, &file_count, &filenames, &filenames_size);
    
    bool active = true; // Menu active? 
	bool* P_active = &active; // Pointer to active.

	// For taking user_input.
	struct tb_event input; 	

    char *menu_deco_1 = malloc(41 * sizeof(char *)); // Create the menu decoration.
    
    // Fill array with '|' character: 
    for (int i = 0; i < 41; i++)
    {
        menu_deco_1[i] = '|';
    }

    // Null terminate the string.    
    menu_deco_1[41] = '\0';
 
    // Spaces out the menu decorations. 
    int spacer = 2; 
    
	while(active)
    {
		
        // Main menu graphics:
		tb_print(center_x-14, quarter_d_y+1, TB_DEFAULT, TB_DEFAULT, "|||||||||||||||||||||");
		tb_print(center_x-14, quarter_d_y, TB_DEFAULT, TB_DEFAULT,   "| LOAD PROJECT MENU |");	
		tb_print(center_x-14, quarter_d_y-1, TB_DEFAULT, TB_DEFAULT, "|||||||||||||||||||||");

        tb_print(center_x-24, center_y+1, TB_DEFAULT, TB_DEFAULT, menu_deco_1);

        for (int i = 0; i < file_count; ++i)
        {
            
            tb_print(center_x-22, center_y+1+spacer-1, TB_RED, TB_DEFAULT, filenames[i]);
            
            tb_print(center_x-24, center_y+1+spacer-1, TB_DEFAULT, TB_DEFAULT, "|"); 
            tb_print(center_x+16, center_y+1+spacer-1, TB_DEFAULT, TB_DEFAULT, "|");
            
            tb_print(center_x-24, center_y+1+spacer, TB_DEFAULT, TB_DEFAULT, menu_deco_1);
       
            // Icrement the spacer so it puts the menu decoration into the correct spot.
            spacer+=2;
        }	
    
        tb_present();
       
        tb_poll_event(&input);
        active = false;	 
	}	

    // Free alocated memory for storing filenames.
    free_filenames(&filenames, file_count);

    // Free allocated memory for menu decorations.
    free(menu_deco_1);

    // Clear the screen entirely.
	tb_clear();

	return 0;
}



