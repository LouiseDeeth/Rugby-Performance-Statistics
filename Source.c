/*Rugby Performance Metric Ltd.currently produce rugby performance statistics in Ireland and
has requested you to develop a new player performance statistic application for them.
This program will create a database which will store the details of all the players’ performance
statistics.In addition to storing all the player data the application should allow performance
statistics to be generated.*/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include <float.h>
#include<conio.h>

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
void loadDatabase(playerT** head, const char* filename);
void addPlayer(playerT** head);
void displayPlayerDetails(playerT* head);
void displayAllPlayers(playerT* head);
void updatePlayerDetails(playerT** head);
void deletePlayer(playerT** head);
void generatePlayerStatistics(playerT* head);
void saveDatabase(playerT* head);
void swapPlayerData(playerT* a, playerT* b);
void listByHeight(playerT* head);

int main()
{
	playerT* myDatabase = NULL;
	int choice;
	bool loggedIn = login("login.txt");
	loadDatabase(&myDatabase, "Rugby.txt");

		if (loggedIn) {
			printf("\n***Login successful***\n\n");

			printf("Please enter 1 to add a player\n");
			printf("Please enter 2 to display all players to screen\n");
			printf("Please enter 3 to display player details\n");
			printf("Please enter 4 to update players statastics\n");
			printf("Please enter 5 to delete a player\n");
			printf("Please enter 6 to generate statistics\n");
			printf("Please enter 7 to save to file\n");
			printf("Please enter 8 to list players by height\n");
			printf("Please enter -1 to terminate\n");
			scanf("%d", &choice);

			while (choice != -1) {
				//Add player: This will add a new player in linked list.The IRFU number must be unique
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
					updatePlayerDetails(&myDatabase);
				}								
				//Delete Player: Allows the user to delete a player from the list by IRFU number.
				else if (choice == 5) {
					deletePlayer(&myDatabase);
				}
				// Generate statistics (a – h) based on a range of player weights
				else if (choice == 6) {
					generatePlayerStatistics(&myDatabase);
				}				
				// Print all player details into a report file.
				else if (choice == 7) {
					saveDatabase(myDatabase);
				}			
				// List all the players of the following categories in order of height
				else if (choice == 8) {
					listByHeight(myDatabase);
				}

				printf("Please enter 1 to add a player\n");
				printf("Please enter 2 to display all players to screen\n");
				printf("Please enter 3 to display player details\n");
				printf("Please enter 4 to update players statastics\n");
				printf("Please enter 5 to delete a player\n");
				printf("Please enter 6 to generate statistics\n");
				printf("Please enter 7 to save to file\n");
				printf("Please enter 8 to list players by height\n");
				printf("Please enter -1 to terminate\n");
				scanf("%d", &choice);
			}
		}
		else {
			printf("\nLogin failed\n");
		}
		if (myDatabase != NULL) {
			printf("Saving player data to Rugby.txt\n");
			saveDatabase(myDatabase);
		}
		printf("Exiting!\n");
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
	bool loginSuccess = false;
	int attempts = 0;

	FILE* fp = fopen(loginFilename, "r");

	if (fp == NULL) {
		printf("Could not open login file!\n\n");
		return false;
	}

	//read the number of login records
	fscanf(fp, "%d\n", &recordCount);

	while (attempts < 3 && !loginSuccess) {//allow 3 attempts
		//Prompt user for username and password
		printf("Attempt %d - Please enter username: ", attempts + 1);
		scanf("%s", usernameEntered);
		clearInputBuffer();

		printf("Please enter password: ");
		getPassword(passwordEntered, sizeof(passwordEntered));
		//printf("\nDEBUG: Password on file: %s and Password entered: %s\n", sysPassword, passwordEntered);

		rewind(fp); //go to beginning of the file
		fscanf(fp, "%d\n", &recordCount);

		for (int i = 0; i < recordCount; i++) {
			fscanf(fp, "%s %s", sysUsername, sysPassword);
				if (strcmp(usernameEntered, sysUsername) == 0 && strcmp(passwordEntered, sysPassword) == 0) {
					loginSuccess = true;// Successful login
					break;
				}
		}
		if (!loginSuccess) {
			printf("\nLogin attempt %d failed.\n", attempts + 1);
			attempts++;
		}
	}
	fclose(fp);
	return loginSuccess;// return true if successful, false if not
}

//clear the buffer
void clearInputBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {}
}

void loadDatabase(playerT** head, const char* filename) {
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Could not open the file %s for reading.\n", filename);
		return;
	}

	// Prepare a temporary structure to read into
	playerT tempPlayer;
	while (fscanf(file, "%d %s %s %d %f %f %s %s %s %s %s\n",
		&tempPlayer.IRFU, tempPlayer.firstName, tempPlayer.surname, &tempPlayer.age,
		&tempPlayer.height, &tempPlayer.weight, tempPlayer.club, tempPlayer.email,
		tempPlayer.position, tempPlayer.missedTackles, tempPlayer.metresMade) == 11) {
		// Allocate a new node and copy the data from tempPlayer
		playerT* newPlayer = (playerT*)malloc(sizeof(playerT));
		if (newPlayer == NULL) {
			printf("Memory allocation failed.\n");
			continue; // Skip this player if we cannot allocate memory
		}
		*newPlayer = tempPlayer; // Copy data
		newPlayer->next = NULL;

		// Insert the new player into the list
		if (*head == NULL) {
			*head = newPlayer; // The list was empty, newPlayer is now the head
		}
		else {
			playerT* current = *head;
			while (current->next != NULL) {
				current = current->next;
			}
			current->next = newPlayer; // Append newPlayer at the end of the list
		}
	}
	fclose(file);
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

	printf("Enter player details:\n");
	// Prompt for IRFU number and check for uniqueness
	do {
		printf("Enter IRFU Number: ");
		scanf("%d", &newPlayer->IRFU);
		clearInputBuffer();
		if (!isIRFUUnique(*head, newPlayer->IRFU)) {
			printf("A player with IRFU number %d already exists. Please enter a different IRFU number: \n", newPlayer->IRFU);
		}
		else {
			break; //exit loop if IRFU is unique
		}
	} while (1);

	int choice;

	// Input player details
	//First name
	printf("First Name: ");
	scanf("%29s", newPlayer->firstName);

	//Surname
	printf("Surname: ");
	scanf("%29s", newPlayer->surname);

	//Age must be positive
	do {
		printf("Age: ");
		scanf("%d", &newPlayer->age);
		if (newPlayer->age <= 0) {
			printf("Invalid age. Please enter a positive number.\n");
		}
	} while (newPlayer->age <= 0 || newPlayer->age >= 90);

	//Height
	do {
		printf("Height (in meters): ");
		scanf("%f", &newPlayer->height);
		if (newPlayer->height <= 0 || newPlayer->height > 3) {
			printf("Invalid height. Please enter a positive number less than or equal to 3.\n");
		}
	} while (newPlayer->height <= 0 || newPlayer->height > 3);

	//Weight
	do {
		printf("Weight (in kg): ");
		scanf("%f", &newPlayer->weight);
		if (newPlayer->weight <= 0 || newPlayer->weight >= 300) {
			printf("Invalid weight. Please enter a positive number less than or equal to 300.\n");
		}
	} while (newPlayer->weight <= 0 || newPlayer->weight >= 300);

	//Club
	printf("Club: ");
	scanf("%29s", newPlayer->club);

	//Email address - (must contain an @, a full stop and a .com)
	printf("Email Address: ");
	scanf("%s", newPlayer->email);
	while (!strstr(newPlayer->email, "@") || !strstr(newPlayer->email, ".com")) {
		printf("Invalid email. Please include an '@' and a '.com': ");
		scanf("%s", newPlayer->email);
	}

	// Player Position
	do {
		printf("Choose Player Position:\n");
		printf("   1. Prop\n   2. Hooker\n   3. Second Row\n   4. Back Row\n   5. Half Back\n   6. Centres\n   7. Wingers/Full Back\n");
		scanf("%d", &choice);
		switch (choice) {
			case 1: strcpy(newPlayer->position, "Prop"); break;
			case 2: strcpy(newPlayer->position, "Hooker"); break;
			case 3: strcpy(newPlayer->position, "Second Row"); break;
			case 4: strcpy(newPlayer->position, "Back Row"); break;
			case 5: strcpy(newPlayer->position, "Half Back"); break;
			case 6: strcpy(newPlayer->position, "Centres"); break;
			case 7: strcpy(newPlayer->position, "Wingers/Full Back"); break;
			default: strcpy(newPlayer->position, "Unknown"); break;
		}
	} while (choice < 1 || choice > 7);

	// Missed Tackles
	do {
		printf("How many tackles does the player miss per match?\n");
		printf("   1. Never\n   2. Less than three times\n   3. Less than five times\n   4. More than five times\n");
		scanf("%d", &choice);
		switch (choice) {
			case 1: strcpy(newPlayer->missedTackles, "Never"); break;
			case 2: strcpy(newPlayer->missedTackles, "Less than three times"); break;
			case 3: strcpy(newPlayer->missedTackles, "Less than five times"); break;
			case 4: strcpy(newPlayer->missedTackles, "More than five times"); break;
			default: strcpy(newPlayer->missedTackles, "Unknown"); break;
		}
	} while (choice < 1 || choice > 4);

	// Metres Made
	do {
		printf("How many metres does the player make in a game?\n");
		printf("   1. None\n   2. Less than 10 metres\n   3. Less than 20 metres\n   4. More than 20 metres\n");
		scanf("%d", &choice);
		switch (choice) {
			case 1: strcpy(newPlayer->metresMade, "None"); break;
			case 2: strcpy(newPlayer->metresMade, "Less than 10 metres"); break;
			case 3: strcpy(newPlayer->metresMade, "Less than 20 metres"); break;
			case 4: strcpy(newPlayer->metresMade, "More than 20 metres"); break;
			default: strcpy(newPlayer->metresMade, "Unknown"); break;
		}
	} while (choice < 1 || choice > 4);
	clearInputBuffer();	// Clear buffer after all inputs

	// Add new player at the end of the list
	if (*head == NULL) {
		*head = newPlayer; // The list was empty, newPlayer is now the head
	}
	else {
		playerT* current = *head;
		while (current->next != NULL) {
			current = current->next; 
		}
		current->next = newPlayer; // Insert newPlayer at the end
	}
	printf("Player added successfully.\n\n");

		//DEBUG
		/*printf("IRFU No: %d\n", newPlayer->IRFU);
		printf("First Name: %s\n", newPlayer->firstName);
		printf("Surname: %s\n", newPlayer->surname);
		printf("Age: %d\n", newPlayer->age);
		printf("Height: %.2f m\n", newPlayer->height);
		printf("Weight: %.2f kg\n", newPlayer->weight);
		printf("Club: %s\n", newPlayer->club);
		printf("Email: %s\n", newPlayer->email);
		printf("Position: %s\n", newPlayer->position);
		printf("Missed Tackles: %s\n", newPlayer->missedTackles);
		printf("Metres Made: %s\n", newPlayer->metresMade);
		printf("\n");*/
}

// Display all players to screen: Display all player details to screen.
void displayAllPlayers(playerT* head) {
	if (head == NULL) {
		printf("No players in the database.\n");
		return;
	}
	playerT* current = head;
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
		current = current->next;
	}
}

//display player details using IFRU or name
void displayPlayerDetails(playerT * head) {
		int choice;
		int searchIRFUNum;
		char searchName[30];
		bool found = false;

		printf("Display Player Details\nSearch by:\n1. IRFU Number\n2. Name\nEnter choice: ");
		scanf("%d", &choice);
		clearInputBuffer(); // Clears the input buffer

		playerT* current = head;

		if (choice == 1) {
			printf("Enter the IRFU Number: ");
			scanf("%d", &searchIRFUNum);
			while (current != NULL) {
				if (current->IRFU == searchIRFUNum) {
					found = true;
					break;
				}
				current = current->next;
			}
		}
		else if (choice == 2) {
			printf("Enter the player's name: ");
			scanf("%29s", searchName);
			while (current != NULL) {
				if (strcmp(current->firstName, searchName) == 0 || strcmp(current->surname, searchName) == 0) {
					found = true;
					break;
				}
				current = current->next;
			}
		}
		if (found && current != NULL) {
			// Display the found player's details
			printf("Player Details:\n");
			printf("IRFU Number: %d\n", current->IRFU);
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
			printf("Player not found.\n");
		}
	}

//Update Player: Allows the user to update player statistics based on either a name or IRFU number being entered.
void updatePlayerDetails(playerT * *head) {
		if (*head == NULL) {
			printf("The list is empty.\n");
			return;
		}

		int choice, IRFUNumber;
		char searchName[30];
		playerT* current = *head;
		printf("Update by:\n1. IRFU Number\n2. Name\nEnter choice: ");
		scanf("%d", &choice);
		clearInputBuffer(); // To handle any leftover characters in the input buffer

		if (choice == 1) {
			printf("Enter IRFU Number: ");
			scanf("%d", &IRFUNumber);
			while (current != NULL) {
				if (current->IRFU == IRFUNumber) {
					break;
				}
				current = current->next;
			}
		}
		else if (choice == 2) {
			printf("Enter Name: ");
			fgets(searchName, 30, stdin);
			searchName[strcspn(searchName, "\n")] = 0; // Remove newline character
			while (current != NULL) {
				if (strcmp(current->firstName, searchName) == 0 || strcmp(current->surname, searchName) == 0) {
					break;
				}
				current = current->next;
			}
		}
		else {
			printf("Invalid choice.\n");
			return;
		}
		if (current == NULL) {
			printf("Player not found.\n");
			return;
		}

		// Update Player Position
		printf("Select new Player Position:\n");
		printf("1. Prop\n2. Hooker\n3. Second Row\n4. Back Row\n5. Half Back\n6. Centres\n7. Wingers/Full Back\n");
		scanf("%d", &choice);
		switch (choice) {
			case 1: strcpy(current->position, "Prop"); break;
			case 2: strcpy(current->position, "Hooker"); break;
			case 3: strcpy(current->position, "Second Row"); break;
			case 4: strcpy(current->position, "Back Row"); break;
			case 5: strcpy(current->position, "Half Back"); break;
			case 6: strcpy(current->position, "Centres"); break;
			case 7: strcpy(current->position, "Wingers/Full Back"); break;
			default: printf("Invalid choice.\n"); return;
		}

		// Update Tackles Missed
		printf("Select how many tackles does the player miss per match:\n");
		printf("1. Never\n2. Less than three times per match\n3. Less than five times per match\n4. More than five times per match\n");
		scanf("%d", &choice);
		switch (choice) {
			case 1: strcpy(current->missedTackles, "Never"); break;
			case 2: strcpy(current->missedTackles, "Less than three times"); break;
			case 3: strcpy(current->missedTackles, "Less than five times"); break;
			case 4: strcpy(current->missedTackles, "More than five times"); break;
			default: printf("Invalid choice.\n"); return;
		}

		// Update Metres Made
		printf("Select how many metres does the player make in a game:\n");
		printf("1. None\n2. Less than 10 metres\n3. Less than 20 metres\n4. More than 20 metres\n");
		scanf("%d", &choice);
		switch (choice) {
			case 1: strcpy(current->metresMade, "None"); break;
			case 2: strcpy(current->metresMade, "Less than 10 metres"); break;
			case 3: strcpy(current->metresMade, "Less than 20 metres"); break;
			case 4: strcpy(current->metresMade, "More than 20 metres"); break;
			default: printf("Invalid choice.\n"); return;
		}
		printf("Player details updated successfully.\n");
	}

//Delete Player: Allows the user to delete a player from the list by IRFU number.
void deletePlayer(playerT** head) {
	int IRFUNumber;
	printf("Enter IRFU Number of the player to delete: ");
	scanf("%d", &IRFUNumber);

	if (*head == NULL) {
		printf("The list is empty. No player to delete.\n");
		return;
	}

	playerT* temp = *head, * prev = NULL;

	// Deleting the head node if it matches the IRFU Number
	if (temp != NULL && temp->IRFU == IRFUNumber) {
		*head = temp->next; // Change head
		free(temp); // Free old head
		printf("\nPlayer with IRFU Number %d deleted successfully.\n\n", IRFUNumber);
		return;
	}

	// Search for the player to be deleted
	while (temp != NULL && temp->IRFU != IRFUNumber) {
		prev = temp;
		temp = temp->next;
	}

	// If the player with the specified IRFU Number is not found
	if (temp == NULL) {
		printf("Player with IRFU Number %d not found.\n", IRFUNumber);
		return;
	}

	// Unlink the node from the list and free memory
	prev->next = temp->next;
	free(temp);
	printf("Player with IRFU Number %d deleted successfully.\n", IRFUNumber);
}

// Generate statistics (a – h) based on a range of player weights
void generatePlayerStatistics(playerT* head) {
	if (head == NULL) {
		printf("No players in the database.\n");
		return;
	}

	int totalPlayers = 0;
	int noTacklesMissed = 0, lessThanThreeTacklesMissed = 0, lessThanFiveTacklesMissed = 0, moreThanFiveTacklesMissed = 0;
	int noMetresMade = 0, lessThanTenMetresMade = 0, lessThanTwentyMetresMade = 0, moreThanTwentyMetresMade = 0;

	playerT* current = head;
	while (current != NULL) {
		totalPlayers++;

		// Tackles
		if (strcmp(current->missedTackles, "Never") == 0) noTacklesMissed++;
		else if (strcmp(current->missedTackles, "Less than three times") == 0) lessThanThreeTacklesMissed++;
		else if (strcmp(current->missedTackles, "Less than five times") == 0) lessThanFiveTacklesMissed++;
		else if (strcmp(current->missedTackles, "More than five times") == 0) moreThanFiveTacklesMissed++;

		// Metres
		if (strcmp(current->metresMade, "None") == 0) noMetresMade++;
		else if (strcmp(current->metresMade, "Less than 10 metres") == 0) lessThanTenMetresMade++;
		else if (strcmp(current->metresMade, "Less than 20 metres") == 0) lessThanTwentyMetresMade++;
		else if (strcmp(current->metresMade, "More than 20 metres") == 0) moreThanTwentyMetresMade++;

		current = current->next;
	}

	printf("Player Statistics:\n");
	printf("A. %% of players who miss no tackles: %.2f%%\n", 100.0 * noTacklesMissed / totalPlayers);
	printf("B. %% of players who miss less than 3 tackles per match: %.2f%%\n", 100.0 * lessThanThreeTacklesMissed / totalPlayers);
	printf("C. %% of players who miss less than 5 tackles per match: %.2f%%\n", 100.0 * lessThanFiveTacklesMissed / totalPlayers);
	printf("D. %% of players who miss more than 5 tackles per match: %.2f%%\n", 100.0 * moreThanFiveTacklesMissed / totalPlayers);
	printf("E. %% of players who make no metres in a game: %.2f%%\n", 100.0 * noMetresMade / totalPlayers);
	printf("F. %% of players who make less than 10 metres in a game: %.2f%%\n", 100.0 * lessThanTenMetresMade / totalPlayers);
	printf("G. %% of players who make less than 20 metres in a game: %.2f%%\n", 100.0 * lessThanTwentyMetresMade / totalPlayers);
	printf("H. %% of players who make more than 20 metres in a game: %.2f%%\n", 100.0 * moreThanTwentyMetresMade / totalPlayers);
}

// Print all player details into a report file - new file with labels
void saveDatabase(playerT* head) {
	FILE* op;
	playerT* current = head;

	op = fopen("Rugby.txt", "w");

	if (op == NULL)
		printf("Sorry the database could not be backed up\n");
	else
	{
		fprintf(op, "%d %s %s %d %f %f %s %s %s %s %s\n", "IRFU", "First Name", "Surname", "Age", "Height", "Weight", "Club", "Email", "Position", "Missed Tackles", "Metres Made");

		while (current != NULL) {
			fprintf(op, "%d %s %s %d %.2f %.2f %s %s %s %s %s\n",
				current->IRFU, current->firstName, current->surname, current->age,
				current->height, current->weight, current->club, current->email,
				current->position, current->missedTackles, current->metresMade);
			current = current->next;
		}
		fclose(op);
	}
}

void swapPlayerData(playerT* a, playerT* b) {
	playerT temp = *a;
	a->IRFU = b->IRFU;
	strcpy(a->firstName, b->firstName);
	strcpy(a->surname, b->surname);
	a->age = b->age;
	a->height = b->height;
	a->weight = b->weight;
	strcpy(a->club, b->club);
	strcpy(a->email, b->email);
	strcpy(a->position, b->position);
	strcpy(a->missedTackles, b->missedTackles);
	strcpy(a->metresMade, b->metresMade);

	b->IRFU = temp.IRFU;
	strcpy(b->firstName, temp.firstName);
	strcpy(b->surname, temp.surname);
	b->age = temp.age;
	b->height = temp.height;
	b->weight = temp.weight;
	strcpy(b->club, temp.club);
	strcpy(b->email, temp.email);
	strcpy(b->position, temp.position);
	strcpy(b->missedTackles, temp.missedTackles);
	strcpy(b->metresMade, temp.metresMade);
}

// List all the players of the following categories in order of height
void listByHeight(playerT* head) {
	if (head == NULL) {
		printf("The player list is empty.\n");
		return;
	}

	int swapped;
	playerT* ptr1;
	playerT* lptr = NULL;

	//Checking for empty list
	if (head == NULL) {
		return;
	}

	do {
		swapped = 0;
		ptr1 = head;

		while (ptr1->next != lptr) {
			if (ptr1->height > ptr1->next->height) {
				swapPlayerData(ptr1, ptr1->next);
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		lptr = ptr1;
	} while (swapped);

	// Now the list is sorted, display all players
	displayAllPlayers(head);
}
