/*Rugby Performance Metric Ltd.currently produce rugby performance statistics in Ireland and
has requested you to develop a new player performance statistic application for them.
This program will create a database which will store the details of all the players’ performance
statistics.In addition to storing all the player data the application should allow performance
statistics to be generated.*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<conio.h>

#define MAX_USERS 10

typedef struct player {
	int IRFU;
	char firstName[30];
	char surname[25];
	int age;
	float height;
	float weight;
	char club[30];
	char email[50];
	char position[20];
	char missedTackles[30];
	char metresMade[30];
	struct player* next;
}playerT;

typedef struct {
	char username[30];
	char passwordEntered[7];//6 characters and null terminator
}LoginT;

//Functions
void getPassword(char* password, int maxLength);
bool login(const char* loginFilename);
void clearInputBuffer();
void addPlayer(playerT** head);
void inputPlayerDetails(playerT* player);
void displayPlayerDetails(playerT** head);
void displayAllPlayers(playerT* head);
void updatePlayerDetails(playerT* head);
void deletePlayerDetails(playerT* head);
void generatePlayerStatistics(playerT* head);
void saveDatabase(playerT* head);


// List all the players of the following categories in order of height


void main()
{
	playerT* myDatabase = NULL;
	int choice;
	int numPlayers;
	bool loggedIn = login("login.txt");
	char password[8];
	char passwordEntered[30];
	char sysUsername[30];
	char sysPassword[30];

		if (loggedIn) {
			printf("\n***Login successful***\n\n");

			printf("Please enter 1 to add a player\n");
			printf("Please enter 2 to display all players to screen\n");
			printf("Please enter 3 to display player details\n");
			printf("Please enter 4 to update players statastics\n");
			printf("Please enter 5 to delete a player\n");
			printf("Please enter 6 to generate statistics\n");
			printf("Please enter 7 to restore players details\n");
			printf("Please enter 8 to retrieve player details\n");
			printf("Please enter -1 to terminate\n");
			scanf("%d", &choice);

			while (choice != -1) {
				//Add player: This will add a new player at the correct position in your sorted linked list based on their IRFU Number.The IRFU number must be unique
				if (choice == 1) {
					addPlayer(&myDatabase);
				}	

				//Display all players to screen: Display all player details to screen.
				else if (choice == 2) {
					displayAllPlayers(myDatabase);
				}

				//Display Player Details: Allow the user to input either a IRFU ID or a name of the player and display the details for that player.
				else if (choice == 3) {
					displayPlayerDetails(myDatabase);
				}

				//Update Player: Allows the user to update player statistics based on either a name or IRFU number being entered.
				else if (choice == 4) {

				}					
				
				//Delete Player: Allows the user to delete a player from the list by IRFU number.
				else if (choice == 5) {

				}

				// Generate statistics (a – h) based on a range of player weights
				else if (choice == 6) {

				}
					
				// Print all player details into a report file.
				else if (choice == 7) {

				}
					
				// List all the players of the following categories in order of height
				else if (choice == 8) {

				}

				printf("Please enter 1 to add a player\n");
				printf("Please enter 2 to display all players to screen\n");
				printf("Please enter 3 to display player details\n");
				printf("Please enter 4 to update players statastics\n");
				printf("Please enter 5 to delete a player\n");
				printf("Please enter 6 to generate statistics\n");
				printf("Please enter 7 to restore players details\n");
				printf("Please enter 8 to retrieve player details\n");
				printf("Please enter -1 to terminate\n");
				scanf("%d", &choice);
			}
		}
		else {
			printf("Login failed\n");
		}
		return 0;
}
	
//Functions
//Get password with asterisk
void getPassword(char* password, int maxLength) {
	int i = 0;
	char ch;
	while (i < maxLength - 1) {
		ch = _getch();//reads a character without echoing it
		if (ch == 13) {//enter key is pressed
			break;
		}
		else if (ch == 8) {//handles backspace
			if (i > 0){
				i--;
				printf("\b \b");
			}
		}
		else {
			password[i++] = ch;
			_putch('*');//puts * instead of the actual character
		}
	}
	password[i] = '\0'; //null terminating the string
}

//login function
bool login(const char* loginFilename) {
	char usernameEntered[30];
	char passwordEntered[30];
	char sysUsername[30];
	char sysPassword[30];
	int recordCount = 0;
	FILE* fp = fopen(loginFilename, "r");

	if (fp == NULL) {
		printf("Could not open login file!\n\n");
		return false;
	}

	//read the number of login records
	fscanf(fp, "%d", &recordCount);

	//Prompt user for username and password
	printf("Please enter username: ");
	scanf("%s", usernameEntered);
	clearInputBuffer();

	printf("Please enter password: ");
	getPassword(passwordEntered, sizeof(passwordEntered));
	//printf("\nDEBUG: Password on file: %s and Password entered: %s\n", sysPassword, passwordEntered);

	for (int i = 0; i < recordCount; i++) {
		if (fscanf(fp, "%s %s", sysUsername, sysPassword) == 2) {
			if (strcmp(usernameEntered, sysUsername) == 0 && strcmp(passwordEntered, sysPassword) == 0) {
				fclose(fp);
				return true; // Successful login
			}
		}
	}
	fclose(fp);
	return false;//login fails if there is no matching record
}

//clear the buffer
void clearInputBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {}
}

// Function to check if an IRFU number is unique
bool isIRFUUnique(playerT* head, int irfu) {
	playerT* current = head;
	while (current != NULL) {
		if (current->IRFU == irfu) {
			return false; // Found a player with the same IRFU number
		}
		current = current->next;
	}
	return true; // No player with the same IRFU number was found
}

//add Player to database
void addPlayer(playerT** head) {
	playerT* newPlayer = (playerT*)malloc(sizeof(playerT));
	if (newPlayer == NULL) {
		printf("Memory allocation error : Not enough memory.\n");
		return;
	}

	newPlayer->next = NULL;

	// Prompt for IRFU number and check for uniqueness
	do {
		printf("Enter IRFU Number: ");
		scanf("%d", &newPlayer->IRFU);
		if (!isIRFUUnique(*head, newPlayer->IRFU)) {
			printf("A player with IRFU number %d already exists. Please enter a different IRFU number.\n", newPlayer->IRFU);
		}
		else {
			break; //exit loop if IRFU is unique
		}
	} while (true);

	// Input player details
	inputPlayerDetails(newPlayer);

	// Find correct position and insert
	playerT** tracer = head;//tracer points to the address of the 'head' pointer
	while (*tracer != NULL && (*tracer)->IRFU < newPlayer->IRFU) {
		tracer = &(*tracer)->next;
	}
	newPlayer->next = *tracer;
	*tracer = newPlayer;

	printf("Player added successfully.\n\n");
}

//inputing player details
void inputPlayerDetails(playerT* player) {
	int choice;

	printf("Enter player details:\n");
	//First name
	printf("First Name: ");
	scanf("%s", player->firstName);
	
	//Surname
	printf("Surname: ");
	scanf("%s", player->surname);
	
	//Age must be positive
	do {
		printf("Age: ");
		scanf("%d", &player->age);
		if (player->age <= 0) {
			printf("Invalid age. Please enter a positive number.\n");
		}
	} while (player->age <= 0);
	
	//Height
	do {
		printf("Height (in meters): ");
		scanf("%f", &player->height);
		if (player->height <= 0 || player->height > 3) {
			printf("Invalid height. Please enter a positive number less than or equal to 3.\n");
		}
	} while (player->height <= 0 || player->height > 3);
	
	//Weight
	do {
		printf("Weight (in kg): ");
		scanf("%f", &player->weight);
		if (player->weight <= 0) {
			printf("Invalid weight. Please enter a positive number.\n");
		}
	} while (player->weight <= 0);
	
	//Club
	printf("Club: ");
	scanf("%s", player->club);
	
	//Email address - (must contain an @, a full stop and a .com)
	printf("Email Address: ");
	scanf("%s", player->email);
	while (!strstr(player->email, "@") || !strstr(player->email, ".com")) {
		printf("Invalid email. Please include an '@' and a '.com': ");
		scanf("%s", player->email);
	}
	
	// Player Position
	do {
		printf("Choose Player Position:\n");
		printf("   1. Prop\n   2. Hooker\n   3. Second Row\n   4. Back Row\n   5. Half Back\n   6. Centres\n   7. Wingers/Full Back\n");
		scanf("%d", &choice);
		switch (choice) {
			case 1: strcpy(player->position, "Prop"); break;
			case 2: strcpy(player->position, "Hooker"); break;
			case 3: strcpy(player->position, "Second Row"); break;
			case 4: strcpy(player->position, "Back Row"); break;
			case 5: strcpy(player->position, "Half Back"); break;
			case 6: strcpy(player->position, "Centres"); break;
			case 7: strcpy(player->position, "Wingers/Full Back"); break;
			default: strcpy(player->position, "Unknown"); break;
		}
	} while (choice < 1 || choice > 7);

	// Missed Tackles
		do {
		printf("How many tackles does the player miss per match?\n");
		printf("   1. Never\n   2. Less than three times\n   3. Less than five times\n   4. More than five times\n");
		scanf("%d", &choice);
		switch (choice) {
			case 1: strcpy(player->missedTackles, "Never"); break;
			case 2: strcpy(player->missedTackles, "Less than three times"); break;
			case 3: strcpy(player->missedTackles, "Less than five times"); break;
			case 4: strcpy(player->missedTackles, "More than five times"); break;
			default: strcpy(player->missedTackles, "Unknown"); break;
		}
	} while (choice < 1 || choice > 4);

	// Metres Made
	do {
		printf("How many metres does the player make in a game?\n");
		printf("   1. None\n   2. Less than 10 metres\n   3. Less than 20 metres\n   4. More than 20 metres\n");
		scanf("%d", &choice);
		switch (choice) {
			case 1: strcpy(player->metresMade, "None"); break;
			case 2: strcpy(player->metresMade, "Less than 10 metres"); break;
			case 3: strcpy(player->metresMade, "Less than 20 metres"); break;
			case 4: strcpy(player->metresMade, "More than 20 metres"); break;
			default: strcpy(player->metresMade, "Unknown"); break;
		}
	} while (choice < 1 || choice > 4);

	// Flush stdin buffer 
	clearInputBuffer();
}

// Display all players to screen: Display all player details to screen.
void displayAllPlayers(playerT* head) {
	playerT* current = head;
	if (current == NULL) {
		printf("No players in the database.\n");
		return;
	}

	while (current != NULL) {
		printf("IRFU No: %d\n", current->IRFU);
		printf("First Name: %s\n", current->firstName);
		printf("Surname: %s\n", current->surname);
		printf("Age: %d\n", current->age);
		printf("Height: %.2f m\n", current->height);
		printf("Weight: %.2f kg\n", current->weight);
		printf("Club: %s\n", current->club);
		printf("Email: %s\n", current->email);
		printf("Position: %s\n", current->position);
		printf("Missed Tackles: %s\n", current->missedTackles);
		printf("Metres Made: %s\n", current->metresMade);
		printf("\n");

		// Move to the next player
		current = current->next;
	}
}

//display player details using IFRU or name
void displayPlayerDetails(playerT** head) {
	long searchIRFUNum;
	char searchName[30];
	int found = 0;// Flag to indicate if the player was found
	int choice;

	printf("Display Player Details\nSearch by:\n1.IRFU Number:\n2. Name\nEnter choice: ");
	scanf("%d", &choice);
	clearInputBuffer(); // Clear the input buffer so fgets works

	playerT* current = head;

	if (choice == 1) {
		printf("Please enter the IRFU Number of the player you wish to find: ");
		scanf("%ld", &searchIRFUNum);
		clearInputBuffer(); // Clear the buffer after scanf
	}

	playerT* current = head; // Use current pointer to traverse the list

	while (current != NULL && !found) {
		if ((choice == 1 && current->IRFU == searchIRFUNum) || // Check for IRFU match
			(choice == 2 && (strcmp(current->firstName, searchName) == 0 || strcmp(current->surname, searchName) == 0))) { // Or name match
			found = 1; // Mark as found
			break; // Exit loop
		}
		current = current->next; // Move to next node
	}
	if (found) {
		printf("IRFU No: %d\n", current->IRFU);
		printf("Name: %s %s\n", current->firstName, current->surname);
		printf("Age: %d\n", current->age);
		printf("Height: %.2f m\n", current->height);
		printf("Weight: %.2f kg\n", current->weight);
		printf("Club: %s\n", current->club);
		printf("Email: %s\n", current->email);
		printf("Position: %s\n", current->position);
		printf("Missed Tackles: %s\n", current->missedTackles);
		printf("Metres Made: %s\n", current->metresMade);
		printf("\n");
	}
	else { 
		printf("Player not found)");
	}
}


//Update Player: Allows the user to update player statistics based on either a name or IRFU number being entered.
void updatePlayerDetails(playerT* head) {

}

//Delete Player: Allows the user to delete a player from the list by IRFU number.
void deletePlayerDetails(playerT* head) {

}

// Generate statistics (a – h) based on a range of player weights
void generatePlayerStatistics(playerT* head) {

}

// Print all player details into a report file.
void saveDatabase(playerT* head) {

}

// List all the players of the following categories in order of height


