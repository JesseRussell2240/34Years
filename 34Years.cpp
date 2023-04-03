/*
38 Years Old
Prison escape text based game
Jesse Rusell
Jan 2023

------------------------------------------------TODO --------------------------------------------------------

    -maybe have hints or phrasing request if only one word is correct?


-add some delays or typeing effects to messege genoration to add natural feel to reading each line and have it come up at the pase of an average reader
-ensure all prototypes are listed in correct order
-reorder functions A-Z?
-rensure userinput and sentance filter are not hardcoded max limits of arrays and strings?

--add reporting method so when you talk to guards you can report stuff
--add interaction to other inmates into room logic

-replace placeholder maps with unique locations maps

-add clear screan after each prompt

add help function that can be called from main screen or inide game when help is entered
write a help function that can be called from anywhere and explains in detial the game and the promps and game functions etc

improved guard and inamte interactions
maybe use a CSV to hold possible interaction messeges
formated the same way a room is formated?
possibly have a subroom function that can be called such as 
talkingToGuard(int fromRoom);
readCSV(talkingToGuard)
player.currentRoom=fromRoom.
fix reporting not working when talking to guard


show inventory function? request in userInput and display function?
add inventory diplay function for structure

add buffer overflow handling
-userinput
-read CSV

add if statments to scanF because these return values and an error messege should occure if this returns 0? maybe supress these warnings?

need to re write location methods to use new dictionary method. logic here will be vastily diffrent. sorta dreading this.
clean up mashup of snake and camel case. aswell as various mashups of formating.

add input validation for names
menu screen
etc

fix userinput bug where global tasks dont always work as expected. I should have it crash the game if you type inv. INv displays but it dose not know where to return too.

add settings screen?
-font?
-color?
-typing speed?

why is booter calling cell twice initally but working fine after???
add a check in locations to see if location = null because user ented map or something etc

---------------------------------------CHANGE LOG--------------------------------------------------
added welcome screen for future save and load function
added read write methods for CSV files for future dictionary expantion and code simplicity
fixed bug where the game would display not sure what you meant by XXX when entering universal functions such as help or inv etc
removed if statment logic in each room and replaced with look for dictionary file
re wrote read csv final to only return one less then # of rows becuase of header
moved row++ in readCSV to only add one if the line is complete by putting it inside the colum 5 completion check.
changed sentance filter bassed on learning activity 10 to pointers, and moddified to work with array pointers for verb[i] and noun[i]


//read about strstr function and used it to simplify my sentence searching function
//this function was giving me issues when I converted by rooms to use CSV for the input options.

//modified read CSV to add one to row if the result coloum is filled not the last because the last two rows are not used on every implimentation on on special interactions

//possible add a helper function to load and save game that does something like checkIf name in file... these both have identical while loops...
//removed write person to file
changed game save style to use.dat not .txt
save games are not all stored in one file and are a collection of structures that are binary append to them
changed booter start game set up for a more refined mainmenu function set up

added main menu and save from any game stateFu
added auto save game between all rooms

*/

//PACKGAGES DECLAIRED
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <stdlib.h>

//remove errors for non _s input
//should eventually remove to Ruliability and robustnes
#pragma warning(disable: 4996)



//boolean basics
#define FALSE 0
#define TRUE 1
#define NULL 0

//assorted  defs for CSV import and data manipulation
//#define MAXWORDCOUNT 10 // this needs to be set implemented and inserted into all the sentance and input code because it is currently hardcoded at 100 or 20 etc
#define MAX_FIELD_LEN 250  //limits size of each "cell" in CSV read in and outresult has entire messeges to be printed hen specific actions are preformed so this is higher then a word, more liek a sentance
#define MAX_ROWS 75     //LIMITS  THE NUMBER OF VERB NOUN agreements in each CSV
#define MAX_COLS 5      //how many cols are in each csv 
#define CHARLIM 25 //size of file directory
#define dictSize 1000

//room location index
#define CELL 0
#define CAFF 1
#define UTILITY 2
#define YARD 3
#define OFFICE 4
#define INFERMARY 5
#define LOBBY 6
#define FREEDOM 7
#define KITCHEN 8

//STRUCTURES
struct person {

    
    int progressionGating[10] = { {TRUE}, {FALSE},{FALSE},{FALSE},{FALSE},{FALSE},{FALSE},{FALSE},{FALSE},{FALSE} };//progression gate conains 10 bools that give premission to enter the next room
    int inventory[10] = {0}; // stores the items a person has. It has been defined to hold 10 items.Inventory bulk initilized to zero
    int morality_score = 0; //An integer that represents a person's morality score should be between -5 and 5
    char name[50] = "JohnDoe";  //stores the name of the persoin
    char occupation[50] = "prisoner"; //occupartion nurse, guard, theif etc
    int current_location = 0; //defined current location
    int health_factor = 10;      //score of how healthy they are (0-10)
    int gameTime = 0;
};


//gloabal variables
//globaly defined person 
struct person player;
//these global vars should be removed at some point
char filePath[10][CHARLIM];
const char* saveData = "saveData.dat";
//these are temporary arrays to hold CSV data. Is modified when entering a diffrent room etc
char verb[MAX_ROWS][MAX_FIELD_LEN];
char noun[MAX_ROWS][MAX_FIELD_LEN];
char result[MAX_ROWS][MAX_FIELD_LEN];
char hint[MAX_ROWS][MAX_FIELD_LEN];
char object[MAX_ROWS][MAX_FIELD_LEN];


//prototypes
int main();
void mainMenu();
void booter();

int startNewGame();
int saveGame();
int checkPlayerExists(char* name, person* savedPlayer);
int loadSavedGame();

int read_csv_file(const char* filename, char verb[MAX_ROWS][MAX_FIELD_LEN], char noun[MAX_ROWS][MAX_FIELD_LEN], char result[MAX_ROWS][MAX_FIELD_LEN], char hint[MAX_ROWS][MAX_FIELD_LEN], char object[MAX_ROWS][MAX_FIELD_LEN]);
void createUserLog();
void appendUserLog(const char* text);

void helpFunction();
void displayUserInventory();
void map();
int talkingToGuard();
void talkingToInmate();

int sentenceFilter(char* sentence, char* word1, char* word2);
char* userInput(char message[]);

int locationHelper(int location);
int cell();
int caff();
int utility();
int yard();
int office();
int infirmary();
int lobby();
int freedom();
int kitchen();




//---------------------Start of Functions-------------------------

int main() {
    int loop = TRUE;
    while (loop) {
        mainMenu();
    }
    
  

    return 0;
}


//----------------------------Main Menu and game booter functions ---------------------------

//prompts user for mainmenu options and calls appropriate helper functions to achive desired results
void mainMenu() {
   
    printf("===== Main Menu =====\n");
    printf("1. Start new game\n");
    printf("2. Load saved game\n");
    printf("3. Settings\n");
    printf("4. Help\n");
    printf("=====================\n");

    //prompt user for menu choice
    int choice;
    printf("Enter a Number to chose: ");
    
    scanf("%d", &choice);
   

    switch (choice) {
    case 1:
        if (startNewGame() == TRUE) {
           
            //add clear buffer
            booter();
           
        }  
        break;
    case 2:
        if (loadSavedGame() == TRUE) {
            ;
            //clear buffer
            booter();
           
        }
        break;
    case 3:
        printf("Settings(not yet implemented)\n");
        //  possible settings menu here
        
        break;
    case 4:
        printf("Help(not yet implemented)\n");
           //calls to helpscreen function need added here
        break;
    default:
        printf("Invalid choice. Please try again.\n");
        break;
    }
}

//booter does all the setup work and functions that should only be called once
//also controls menu screen
void booter() {

    //sets file path for dictionary access into globally declared array filePath
    strcpy_s(filePath[CELL], "Dictionary-Cell.csv");
    strcpy_s(filePath[CAFF], "Dictionary-Caff.csv");
    strcpy_s(filePath[UTILITY], "Dictionary-Utility.csv");
    strcpy_s(filePath[YARD], "Dictionary-Yard.csv");
    strcpy_s(filePath[OFFICE], "Dictionary-Office.csv");
    strcpy_s(filePath[INFERMARY], "Dictionary-Infermary.csv");
    strcpy_s(filePath[LOBBY], "Dictionary-Lobby.csv");
    strcpy_s(filePath[FREEDOM], "Dictionary-Freedom.csv");
    strcpy_s(filePath[KITCHEN], "Dictionary-Kitchen.csv");

    createUserLog();

    printf("You wakeup in a cold room");
    printf("\nThe prison cell is a cramped, dimly lit space\nThe walls are made of gray concrete and are covered in graffiti.");
    printf("\n A single light bulb hanging from the ceiling provides the only artificial light in the cell.");
    printf("\n A small metal sink and toilet are built into one corner of the cell");
    printf("\nThe cell is also equipped with a small metal table and a chair that are bolted to the floor.");
    printf("\nremember at any point you can type hint, help or map\n");

 
    
    //loops through all the rooms indefenently, only chanigng when currentRoom in player structure changes. Auto saves everytime you switch rooms
    while (player.current_location != 8) {

        switch (player.current_location)
        {
        case CELL:
            //currentLocation = 1;
          
                player.current_location = cell();
                break;
            
            
            saveGame();
            
        case CAFF:
            player.current_location = caff();
            saveGame();
            break;
        case UTILITY:
            player.current_location = utility();
            saveGame();
            break;
        case YARD:
            player.current_location = yard();
            saveGame();
            break;
        case OFFICE:
            player.current_location = office();
            saveGame();
            break;
        case INFERMARY:
            player.current_location = infirmary();
            saveGame();
            break;
        case LOBBY:
            player.current_location = lobby();
            saveGame();
            break;
        case FREEDOM:
            player.current_location = freedom();
            saveGame();
            break;
        case KITCHEN:
            player.current_location = kitchen();
            saveGame();
            break;
        default:
            break;
        }

    }

}

//----------------file handing functions and asociated helpers -------------------------

//start new game returns true if successfull
// checks if a save file exsists and creates one where necissary then checks if the name exsists. it promps a overwrite confirmation and then returns accordingly.
//https://stackoverflow.com/questions/8910087/understanding-usage-of-fseek I used this link to help with the implimentation of fseek. I did not want to read through the file twice inorder to rewrite an exsisting save state, so this seemed like an appropriate approch
int startNewGame() {


    printf("\nEnter a name for a new game: ");
    char name[50];
    scanf("%s", name);
    player.name[strcspn(player.name, "\n")] = '\0'; // remove newline char from name
    strcpy(player.name, name);
    
  


    //this if statment should create a new gameSave.dat on a new mechine when the program is run so there is not need to move this file around...
    FILE* saveFile = fopen("gameSave.dat", "rb");
    if (saveFile == NULL) {
        printf("Creating new  file... gameSave.dat...\n");
        saveFile = fopen("gameSave.dat", "wb");
    }

     struct person pastSave;
     int found = checkPlayerExists(name, &pastSave);


    // if the name was not found, add the player to the end of the savegame file
    if (!found) {
        fwrite(&player, sizeof(player), 1, saveFile);
        printf("New Game loading...!\n");
        return TRUE;
    } else {
        printf("This name already exsists. Are you sure you want to overwrite progress?(Y/N)");



        char input;
    
        scanf(" %c", &input);
       
        if (input == 'y' || input == 'Y') {
            // seek back to the beginning of the matching record
            fseek(saveFile, -(long int)sizeof(player), SEEK_CUR); //this line using casting seems sloppy. Not sure this is great, but is better then a double loop...
            // write the new player data
            fwrite(&player, sizeof(player), 1, saveFile);
            printf("Game saved successfully!\n");
            return TRUE;
        }
        else {
            printf("New game was not created. Returning to Main Menu.\n");
            return FALSE;
        }
    }

    //clear buffer

    fclose(saveFile);

    return FALSE;
}

// Helper function to save the current player state to the save file
// Returns 1 if save was successful, 0 otherwise
int saveGame() {
    FILE* saveFile = fopen("gameSave.dat", "ab");
    if (saveFile == NULL) {
        printf("Error: Could not open save file for writing.\n");
        return 0; // Failed to open file
    }

    fwrite(&player, sizeof(player), 1, saveFile);
    fclose(saveFile);

    printf("Game saved successfully!\n");
    return 1; // Save successful
}

// Helper function to check if a name already exists in the save file
// Returns 1 if name already exists, 0 otherwise
int checkPlayerExists(char* name, person *savedPlayer) {
    FILE* saveFile = fopen("gameSave.dat", "rb");
    if (saveFile == NULL) {
        return 0; // If file does not exist, name does not exist
    }

    struct person pastSave;
    while (fread(&pastSave, sizeof(pastSave), 1, saveFile) == 1) {
        if (strcmp(pastSave.name, name) == 0) {
            fclose(saveFile);
            player = *savedPlayer;
            return 1; // Name already exists
        }
    }


    fclose(saveFile);
    return 0; // Name does not exist
}

//returns true uppon sucessful operation.
//asks user for a name and checks saveGame.dat if one exsists and opens it if it does.
int loadSavedGame() {

    //get the name of the savegame state from the user
    char name[50];
    printf("\nEnter the name of the saved game: ");
   
    scanf("%s", name);
 
    //open the .dat
    FILE* saveFile = fopen("gameSave.dat", "rb");
    if (saveFile == NULL) {
        printf("No save game file found.\n");
        return FALSE;
    }

    //greate a tempStruct to import if the saved name is found
    struct person savedPlayer;
    int found = checkPlayerExists(name, &savedPlayer);
   

    fclose(saveFile);


    //if the search results is found then load else no savedgame found
    if (found == TRUE) {
        printf("Loading saved game...\n");
        player = savedPlayer;
        
    }
    else {
        printf("No saved game found with that name.\n");
        return FALSE;
       // mainMenu();
    }

    return TRUE;
}


/*
this is a method used to read infor from a CSV.

the information handiling in the CSV is defined as the following
Each room has an indivdual CSV
each row containes one action/reaction
it is stored in the following order
Verb,Noun,Result,hint,object
the start of every file contains the header aswell
if the user says a phrase containing the Verb Noun agreement
the result method should be printed
the hints are for future expantion, so if the user correctly uses one word maybe a hint messege can be displayed to push them to rewrite a sentace.
multiple verb-noun agrements can be present for each result
multiple verb-noun agrements can be present for each hint
multipkle verb-noun agrements can be oresent for each object
in these situations the hint is only stored once
the results and object is stored for each
we dont need multiple versions of the same hint
we only need one proper verb-noun agreements and not every one displayed for each possible path and result
*/

//reading specifically formated CSV for dictionarys of each room
int read_csv_file(const char* filename, char verb[MAX_ROWS][MAX_FIELD_LEN], char noun[MAX_ROWS][MAX_FIELD_LEN], char result[MAX_ROWS][MAX_FIELD_LEN], char hint[MAX_ROWS][MAX_FIELD_LEN], char object[MAX_ROWS][MAX_FIELD_LEN])
{
    FILE* file;
    if (fopen_s(&file, filename, "r") != 0) {
        printf("Error opening file %s\n", filename);
        return 0;
    }

    char line[MAX_FIELD_LEN * MAX_COLS];
    int row = 0;


    while (fgets(line, sizeof(line), file) != NULL) {
        line[strcspn(line, "\n")] = '\0'; // Remove trailing newline character

        char* ptr = strtok(line, ",");
        if (ptr != NULL)
            strcpy(verb[row], ptr);

        //WE ADD ONE TO THE ROW BECAUSE THE FIRST 3 ROWS ARE THE ONLY REQUIRED ROWS. THE HINT AND OBJ ARE ONLY FOR SPECIALS. SO ONLY IF A RESULT EXSISTS ON THIS DO WE ADD ONE
        ptr = strtok(NULL, ",");
        if (ptr != NULL) {
            strcpy(noun[row], ptr);
            row++;
        }


        ptr = strtok(NULL, ",");
        if (ptr != NULL)
            strcpy(result[row], ptr);

        ptr = strtok(NULL, ",");
        if (ptr != NULL)
            strcpy(hint[row], ptr);

        //row is only added to if the entier row functionally works and the item in column  reads in correctly
        ptr = strtok(NULL, ",");
        if (ptr != NULL)
            strcpy(object[row], ptr);





    }

    fclose(file);

    return row;//THIS IS THE TOTAL number of rows, minus one for the header. We should add checks to ensure that the user dosent enter NOUN VERB and get RESULT back

}

//creates userLog returns nothing and takes nothing as input
//erases exsisting log when called from booter.
void createUserLog() {
    FILE* file;
    errno_t err = fopen_s(&file, "userLog.dat", "w");
    if (err != 0 || file == NULL) {
        printf("Error creating file.\n");
        return;
    }

    fprintf(file, "Start of userlog\n");
    fclose(file);
}

//method is called in every user input to append the user log and add the last user input to the file.
void appendUserLog(const char* text) {
    FILE* file;
    errno_t err = fopen_s(&file, "userLog.dat", "a");
    if (err != 0 || file == NULL) {
        printf("Error writing to user log file.\n");
        return;
    }
    fprintf(file, "%s\n", text);

    fclose(file);
}

//---------------user interface functions-----------------------------

//helper function to display basic commands and game info
//can be called from main menu or from game at any point
void helpFunction() {

}

//method called when user wants to display current inventory
//access struc and shows quantity and what is in inventory
//needs to be cleaned up for better handling of multiple objects
void displayUserInventory() {

    if (player.inventory[0] == FALSE) {
        printf("you dont have anything in your inventory. Try picking something up");
    }

    else {
        printf("%s's Inventory:\n", player.name);
        for (int i = 0; i < 1; i++) {
            if (player.inventory[i] == 1) {
                printf("%d ", player.inventory[i]);
                printf(" fork in inventory");
            }
            //add inventory index list here to complete inventory



        }
        printf("\n");
    }
}

//access struc for inv and displays map specific to the current location
void map() {
    if (player.current_location == CELL) {
        printf("Cell\n");
        printf("+---------------+\n");
        printf("|    cell       |\n");
        printf("|               |\n");
        printf("|               |\n");
        printf("+---------------+\n");
    }
    else if (player.current_location == CAFF) {
        printf("Caff\n");
        printf("+---------------+\n");
        printf("|   Caff        |\n");
        printf("|               |\n");
        printf("|               |\n");
        printf("+---------------+\n");
    }
    else if (player.current_location == UTILITY) {
        printf("Utility Room\n");
        printf("+---------------+\n");
        printf("|   U room      |\n");
        printf("|               |\n");
        printf("|               |\n");
        printf("+---------------+\n");
    }
    else if (player.current_location == YARD) {
        printf("Yard\n");
        printf("+---------------+\n");
        printf("|   Yard        |\n");
        printf("|               |\n");
        printf("|               |\n");
        printf("+---------------+\n");
    }
    else if (player.current_location == OFFICE) {
        printf("Warden's Office\n");
        printf("+---------------+\n");
        printf("|   W           |\n");
        printf("|               |\n");
        printf("|               |\n");
        printf("+---------------+\n");
    }
    else if (player.current_location == INFERMARY) {
        printf("Hospital\n");
        printf("+---------------+\n");
        printf("|   Hospita     |\n");
        printf("|               |\n");
        printf("|               |\n");
        printf("+---------------+\n");
    }
    else if (player.current_location == LOBBY) {
        printf("Lobby\n");
        printf("+---------------+\n");
        printf("|   Placeholder |\n");
        printf("|               |\n");
        printf("|               |\n");
        printf("+---------------+\n");
    }
    else if (player.current_location == FREEDOM) {
        printf("Freedom\n");
        printf("+---------------+\n");
        printf("|   Placeholder |\n");
        printf("|               |\n");
        printf("|               |\n");
        printf("+---------------+\n");
    }
    else if (player.current_location == KITCHEN) {
        printf("Kitchen\n");
        printf("+---------------+\n");
        printf("|   Placeholder |\n");
        printf("|               |\n");
        printf("|               |\n");
        printf("+---------------+\n");
    }
    else {
        printf("Invalid location cannot print map\n");
    }
    /*
    38  YEARS  OLD

         Prison Cell Map
         ++--+-+-++-----++
         ||B |0| ||    B||
         ||E     ||    E||
         ||D     ||    D||
         ||      ||     ||
         ||      ||     ||
         ||      ||     ||
         ++xxxxxx++xxxxx++
         |               |
    YOU R++xxxxxx++xxxxx++
     HERE||      ||     ||
      |  ||      ||     ||
      +--+>      ||     ||
         ||B     ||    B||
         ||E     ||    E||
         ||D |0| ||    D||
         ++--+-+-++-----++

++--------++
||  CELL  || 38 YEARS OLD
||        ||
||        ||
|| HOLE   ||
||   |    ||
||   v    ||
|+--+-+---++--------------------------+
|+--+ +------------------------------+|
||      UTILLITY ROOM                ||
||                             +-----+|
||                             +-^---+|
|+---+                           |   ||
|++^++---------------------------+---+|
+--+-----------------------------+----+
   |                             |
TOOL CABNIT                  SEWER PIPE
 */




}

/*


//----------------------NPC interaction methods----------------------------------
//genaric talking to guard function to be called from several rooms and provide user with a similar experiance.
int talkingToGuard() {
    int talking = false;
    char userSentence[100];

    char key90[] = "report";
    char key91[] = "contraband";
    char key92[] = "ask";
    char key93[] = "help";
    char key94[] = "leave";
    char key95[] = "goodbye";

    while (!talking) {
        char prompt[] = "Guard: What can I help you with?";
        char options[][20] = { "Report contraband", "Ask for help", "End conversation" };
        int numberOfOptions = 3;

      //  strcpy_s(userSentence, userInput(prompt, options, numberOfOptions));

        if (sentenceFilter(userSentence, key90, key91) == TRUE) {
            char detailsPrompt[] = "Guard: Can you tell me more about the contraband?";
            char detailsOptions[][20] = { "Yes", "No" };
            int numberOfDetailsOptions = 2;

            char details[100];
          //  strcpy_s(details, userInput(detailsPrompt, detailsOptions, numberOfDetailsOptions));

            char key90[] = "yes";
            char key91[] = "";

            if (sentenceFilter(details, key90, key91) == TRUE) {
                //printf("", details);

                char key97[] = "Knife";
                char key98[] = "kitchen";
                char key99[] = "drugs";
                char key100[] = "cell";

            //    strcpy_s(userSentence, userInput(prompt, options, numberOfOptions));
                if (sentenceFilter(userSentence, key97, key98) == TRUE) {
                    player.morality_score++;
                }
                else if (sentenceFilter(userSentence, key99, key100) == TRUE) {
                    player.morality_score++;
                }
                else {
                    printf("I dont I dont understand. Im going to search you...\n");
                    if (player.inventory[0] == TRUE) {
                        printf("Why have you stolen this!\n");
                    }
                    else {

                    }
                }



                player.morality_score++;

                printf("Guard: Thank you for reporting the contraband. We will take care of it.\n");
            }
        }
        else if (sentenceFilter(userSentence, key92, key93) == TRUE) {
            printf("Guard: If you need anything, don't hesitate to ask. We are here to help you.\n");
        }
        else if (sentenceFilter(userSentence, key95, key94) == TRUE) {
            printf("Guard: Have a good day.\n");
            talking = true;
        }
    }
    return 0;
}

//genaric inmate chatbot to be called anytime intereaction is done with inmate.
void talkingToInmate() {
    char prompt[] = "What do you want to say to the inmate?";
    char options[][20] = { "Start a gang", "Report inmate", "Say hello", "Leave" };
    char key80[] = "start";
    char key81[] = "gang";
    char key82[] = "report";
    char key83[] = "inmate";
    char key84[] = "say";
    char key85[] = "hello";
    char key86[] = "leave";
    char key87[] = "";
    int talkingToInmate = TRUE;

    while (talkingToInmate == TRUE) {
     //   char* userChoice = userInput(prompt, options, 4);

        if (sentenceFilter(userChoice, key80, key81) == TRUE && player.morality_score <= -5) {
            player.morality_score -= 2;
            printf("You have started a gang. Your morality is now %d\n", player.morality_score);
        }
        else if (sentenceFilter(userChoice, key82, key83) == TRUE) {
            player.morality_score += 1;
            printf("You reported the inmate. Your morality is now %d\n", player.morality_score);
        }
        else if (sentenceFilter(userChoice, key84, key85) == TRUE) {
            printf("You said hello to the inmate.\n");
        }
        else if (sentenceFilter(userChoice, key86, key87) == TRUE) {
            talkingToInmate = FALSE;
        }
        else {
            printf("Invalid choice.\n");
        }
    }


}
*/
//--------------------messege filtering and userinput functions----------------
//looks through a user provided sentance for keywords
//also helps have comands implemented anywhere in game such as help,map,inv,save,exit,mainmenu
int sentenceFilter(char* sentence, char* word1, char* word2) {
    //convert sentence to lower case
    for (int i = 0; sentence[i]; i++) {
        sentence[i] = tolower(sentence[i]);
    }

    //search for word1 and word2
    char* word1Ptr = strstr(sentence, word1);
    char* word2Ptr = strstr(sentence, word2);

    return word1Ptr && word2Ptr;
}

//userinput validation and global messege handing inorder to ensure users type in valid or acceptable sentances
char* userInput(char message[]) {
    char userSentance[100];

    
/*

 // clear the input buffer and discards chars. This was a issue. fflush would not work on this line...
    int c;
    while ((c = getchar()) != '\n' && c != EOF);

*/
   
    // Print the message
    printf("%s", message);

    // Read the user input
    printf("Enter your choice: ");
    fgets(userSentance, 100, stdin);
    ;
    // Replace the newline character with a null terminator
    int len = strlen(userSentance);
    if (userSentance[len - 1] == '\n') {
        userSentance[len - 1] = '\0';
    }

    scanf("%*c");//potential to remove input buffer dogsbrekfast
   
        // Check if the user input is "help", "hint", or "map"
        if (strcmp(userSentance, "help") == FALSE) {
            // Handle "help"

            //
        }
        else if (strcmp(userSentance, "hint") == 0) {
            // Handle "hint"
            //this should enable the 
             // Print the options
            for (int i = 0; i < MAX_ROWS; i++) {
                if (strcmp(hint[i], ";") != 0);                   //this should not be a semicolin but this is what was in the CSV--------------------FIX ME
                 printf("%d. %s\n", i + 1, hint[i]);
            }
            return userSentance;
        }
        else if (strcmp(userSentance, "map") == 0) {
            // Handle "map"
            map();
            return userSentance;
        }
        else if (strcmp(userSentance, "inv") == 0) {
            displayUserInventory();
            return userSentance;
        }
        else if (strcmp(userSentance, "save") == 0) {
            saveGame();
            return userSentance;
        }
        else if (strcmp(userSentance, "main menu") == 0) {
            saveGame();
            mainMenu();
            return userSentance;
           
        }
        else if (strcmp(userSentance, "exit") == 0) {
            saveGame();
            exit(3);
            return userSentance;
        }
        else {
            // Handle other input 
            helpFunction();
         //   return NULL;

        }

    
    appendUserLog(userSentance);

    player.gameTime++;
  
    return userSentance;

}

//-----------------------------------location heper functions-------------------------

//needs to be modified to work for all locations.
int locationHelper(int location) {


    //calls reasCSV inorder to set the arrays.
    int cellDictRow = read_csv_file(filePath[location], verb, noun, result, hint, object);

    // Call userInput
    char message[] = "\nWhat would you like to do?\n";
   
    //calls userInput and assigned the returned value to userSenace via string copy
    char userSentence[100] = "\n";
    strcpy(userSentence, userInput(message));

    //keep the current location in our switch case when users enter a help, inv or other global accessible menus
    if (strcmp(userSentence, "\n") == 0) {
        player.current_location = location;
        return player.current_location;
    } 

    int matchingWordIndex = 0;

    for (int i = 1; i < cellDictRow + 1; i++) {
        if (sentenceFilter(userSentence, verb[i], noun[i]) == 1) {
            matchingWordIndex = i + 1;//needs to be plus one becuase the 
            //  printf("the user entered keywords were found in the file and a result should be printed");
            if (object[i] != NULL) {
                //player.inventory[(int)object[i]] = 1;
            }
              
              
              
            break;      //I am leaving this break here rather then using a flag or setting my I to a value because I belive this is an appropriate usage and is readable
        }

    }

   // system("cls"); this ll clear map and inventory needs to moved to a new location

    if (matchingWordIndex == 0) {
        printf("I am not sure what you meant... Try doing something else");
        printf(hint[matchingWordIndex]);
    }
    else {
        printf("%s\n", result[matchingWordIndex]);
    }


    if (object[matchingWordIndex] == "CAFF") {
        return 1;
    }

    return location;
}


//-------------------------------------location fucntions-----------------------------

//cell uses .0001gb of stack. This should be ok, continuing with causion.
//cell is function for the cell room and is indexed at 0
int cell() {
    printf("cell function called");
    if (locationHelper(CELL) == CELL) {

     }
    

    if (player.gameTime == 5) {
        printf("The lunch buzzer sounds. A guard ushers all the inmates into the Cafateria.");
        return CAFF;
    }
    return CELL;
}

//caff is indexed at 1
int caff() {

    return CAFF;


}

//utility is indexed at 2
int utility() {
    return UTILITY;
}

//yard is indexed to 3
int yard() {
   
    return YARD;
}

//office is indexed to 4
int office() {

    return OFFICE;
}

//infermary is indexed to 5
int infirmary() {
    return 0;
}

//lobby is indexed to 6
int lobby() {
    return 0;

}

//freedom is indexed to 7
int freedom() {
  
    return FREEDOM;
}

//kitchen is indexed to 8
int kitchen() {
    return KITCHEN;
}
