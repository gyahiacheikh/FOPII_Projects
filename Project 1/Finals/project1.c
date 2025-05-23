/*
Degree: Artificial Intelligence
Subject: Fundamentals of Programming 2
Practical project: 1
Sara Díez and Gússem Yahia-Cheikh

Simulator - main program
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "project1.h" // header with all data structures

int packages1Removed = 0;
int packages2Removed = 0;
int robotsRemoved = 0;


//----------------------------------------------------------General
// WARNING: do not change this function
enum EventType GenerateEventType()
{
	return rand()%3;
}

void CheckArguments (int argc, char ** argv)
{
	if (argc != 2) {
        printf("The number of arguments must be two");
		exit(1);
    }

	int EventNumbers = atoi(argv[1]); // This is the numebr of events passed. Convert the argument to an integer
    if (EventNumbers <= 0) {
        printf("The number of events must be a positive integer.\n");
        exit(1); 
    };
}

//----------------------------------------------------------RobotPackages -> Sorted list
// WARNING: do not change this function
struct RobotPackage * GenerateRobotPackage()
{
	// reserve memory for a RobotPackage
	struct RobotPackage * RobotPackage=malloc(sizeof(struct RobotPackage));
	int RobotPackageNum=rand()%8;
	// initialize the RobotPackage's fields
    // from the project.h header; predefined suppliers of RobotPackages: there are 8 RobotPackages
	strcpy (RobotPackage->supplier, suppliers[RobotPackageNum]);
    // from the project.h header; predefined ids of RobotPackages; there are 8 RobotPackages
	strcpy (RobotPackage->id, ids[RobotPackageNum]);
	int year=rand()%40+1980;
	RobotPackage->year=year;
	return RobotPackage;
}

struct RobotPackage* robotPackageList = NULL;
// function to print a list of RobotPackages
void PrintRobotPackages(struct RobotPackage* head) 
{
    struct RobotPackage* temp = head;

    if (temp == NULL) {
        printf("No Robot Packages available.\n");
        return;
    }

    printf("Robot Packages (Sorted List):\n");
    while (temp != NULL) {
        printf("Supplier: %s, ID: %s, Year: %d\n", temp->supplier, temp->id, temp->year);
        temp = temp->next;
    }
    printf("\n");
}

// function to search for a RobotPackage
struct RobotPackage* SearchRobotPackage(struct RobotPackage* head, char* supplier) {
    struct RobotPackage* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->supplier, supplier) == 0) {
            return temp; // Found the package
        }
        temp = temp->next;
    }
    return NULL; // Not found
}

// function to simulate an insertion of RobotPackages in a ordered way (sorted by supplier)
void SimulateManagingRobotPackages(struct RobotPackage * RobotPackage)
{
    if (RobotPackage == NULL) {
        printf("Error: Attempted to insert NULL RobotPackage!\n");
        return;
    }

	if (robotPackageList == NULL || strcmp(RobotPackage->supplier, robotPackageList->supplier) < 0) {
        // Insert at the beginning
        RobotPackage->next = robotPackageList;
        robotPackageList = RobotPackage;
        return;
    }

    // Traverse the list to find the correct position
    struct RobotPackage *current = robotPackageList;
    while (current->next != NULL && strcmp(current->next->supplier, RobotPackage->supplier) < 0) {
        current = current->next;
    }

    // Insert the package in the correct position
    RobotPackage->next = current->next;
    current->next = RobotPackage;
}

// function to remove all the RobotPackages from the list at the end of the program
void RemoveAllRobotPackages()
{
	struct RobotPackage* current = robotPackageList;
    struct RobotPackage* nextNode;

    while (current != NULL) {
        nextNode = current->next;
        free(current);
        current = nextNode;
    }
    robotPackageList = NULL;
}

//----------------------------------------------------------Packages -> different Stacks
// WARNING: do not change this function
struct Package * GeneratePackage()
{
	// reserve memory for a Package
	struct Package * Package=malloc(sizeof(struct Package));
	// initialize the Package's fields
	enum PackageType type=rand()%3;
	enum Colors color=rand()%4;
	Package->type = type;
	Package->color = color;
	return Package;
}

// function to initialize all stacks of Packages 
struct Package** stacks;
int* top;

void InitStacks() 
{
	stacks = (struct Package**)malloc(3 * sizeof(struct Package*)); // Allocate space for 3 stacks
	top = (int*)malloc(3 * sizeof(int)); // Allocate space for top indices

	if (!stacks || !top) {
		printf("Memory allocation failed for stacks.\n");
		exit(1);
	}

	for (int i = 0; i < 3; i++) {
		stacks[i] = (struct Package*)malloc(MAX_CAPACITY * sizeof(struct Package));
		if (!stacks[i]) {
			printf("Memory allocation failed for stack %d.\n", i);
			exit(1);
		}
		top[i] = EMPTY_STACK; // Initialize as empty
	}
}

// function to print all stacks with all Packages
void PrintPackages() {
    for (int i = 0; i < 3; i++) {
        printf("Stack %d:\n", i);
        for (int j = 0; j <= top[i]; j++) {
            printf("Package Type: %d, Color: %s\n", stacks[i][j].type, colorNames[stacks[i][j].color]);
        }
        printf("\n");
    }
}

// function to remove all packages from a given stack when its MAX_CAPACITY is reached
void RemoveStack(int stackIndex, struct Package **stacks, int *top) {
	if (stackIndex == 0) { // Books are in stack 0
        packages1Removed += (top[stackIndex] + 1);
    } else if (stackIndex == 1) { // Plates are in stack 1
        packages2Removed += (top[stackIndex] + 1);
    } else if (stackIndex == 2) { // Robots are in stack 2
		robotsRemoved += (top[stackIndex] + 1);
	}
	
    top[stackIndex] = EMPTY_STACK; // Reset stack
}

// function to simulate putting a generated Package to a corresponding stack depending on the type (size)
void SimulateClassifyPackage(struct Package * Package)
{
	int stackIndex = Package->type;

	if (top[stackIndex] >= MAX_CAPACITY - 1) {
		RemoveStack(stackIndex, stacks, top);
	}

	top[stackIndex]++;
	stacks[stackIndex][top[stackIndex]] = *Package;
    free(Package);
}

// function to clean all stacks before the end of the program
void CleanPackageStacks()
{
	for (int i = 0; i < 3; i++) {  // Iterate over all three stacks
        free(stacks[i]); // Free the allocated memory for each stack
        stacks[i] = NULL; // Set pointer to NULL to prevent dangling pointer issues
        top[i] = EMPTY_STACK; // Reset the stack top index
    }
    
    free(stacks); // Free the array of stack pointers
    stacks = NULL;
    
    free(top); // Free the array holding top indices
    top = NULL;
}

//----------------------------------------------------------Shopping -> Queue
// WARNING: do not change this function
struct Shopping * GenerateShopping()
{
	// reserve memory for a Shopping
	struct Shopping * shopping=malloc(sizeof(struct Shopping));
	// initialize the shopping's fields
	int n=rand()%5+1;
	shopping->numberThingsToBuy = n;
	nextRobotID++;
	shopping->robot_id=nextRobotID;
	return shopping;
}

// function to print a list of robots in a shopping queue
void PrintShopping() 
{
	printf("\nSTATISTICS WHEN CLEANING THE SIMULATION:\n");
    printf("Removing packages...\n");
    printf("%d packages have been removed.\n", packages1Removed);

    printf("Cleaning all stacks of packages...\n");
    printf("%d packages have been removed.\n", packages2Removed);

    printf("Cleaning shopping queue...\n");
    printf("%d robots have been removed.\n", robotsRemoved);

	//Printing all the robotd in the queue
	struct Shopping* current=queueFirst;
	if (current == NULL){
		printf("No robots remain in the shopping queue.\n");
	} else {
		printf("Robots remaining in the shopping queue:\n");
		int count=0;
        while (current != NULL) {
	        count++;
	        printf("Robot ID: %d, Items to buy: %d\n", current->robot_id, current->numberThingsToBuy);
	        current=current->next;
        }
        printf("Total remaining robots in queue: %d\n", count);
	}
}


// function to add a robot to a shopping queue
void AddToQueue(struct Shopping * shopping)
{
	shopping->next = NULL; // Ensure the new robot does not point to anything

    if (queueFirst == NULL) { 
        // If the queue is empty, set the new robot as both first and last
        queueFirst = shopping;
        queueLast = shopping;
    } else { 
        // Otherwise, add to the end of the queue
        queueLast->next = shopping;
        queueLast = shopping;
    }
}

// function to remove a robot from the queue and serve it
// it may return the number of things to buy to simulate the time
int Dequeue ()
{
	if (queueFirst == NULL) {
        printf("No robots in the queue.\n");
        return 0; // No robot to serve
    }

    struct Shopping* temp = queueFirst; // Store the first robot
    int shoppingTime = temp->numberThingsToBuy; // Number of items to buy (simulation time)

    queueFirst = queueFirst->next; // Move queueFirst to the next robot

    if (queueFirst == NULL) { 
        // If the queue becomes empty, reset queueLast as well
        queueLast = NULL;
    }

    robotsRemoved++;
    free(temp); // Free the memory allocated for the removed robot

    return shoppingTime; // Return the shopping time to simulate time passing
}

// function to simulate the time the robot is in the queue
void UpdateShoppingQueue ()
{
    // Only the first robot in the queue is actively shopping
    if (queueFirst != NULL) {
        // Decrease one item (simulate one unit of time)
        queueFirst->numberThingsToBuy--;

        // If the robot has finished shopping, remove it from the queue
        if (queueFirst->numberThingsToBuy <= 0) {
            Dequeue(); // This will also free the robot and update the queue
        }
    }
}

// function to simulate a robot going for shopping - add to the queue
void SimulateGoForShopping(struct Shopping * shopping)
{
	// Check if the queue is empty
    if (queueFirst == NULL) {
        // If the queue is empty, both the first and last pointers point to the new robot
        queueFirst = shopping;
        queueLast = shopping;
    } else {
        // If the queue is not empty, add the new robot at the end of the queue
        queueLast->next = shopping;
        queueLast = shopping;  // Update the queueLast pointer to the new robot
    }

    // Make sure the new robot's next pointer is NULL as it's the last in the queue
    shopping->next = NULL;
}

// function to clean shopping queue before the end of the program
void CleanShoppingQueue()
{
	struct Shopping *current = queueFirst;
    struct Shopping *nextRobot;

    if (current==NULL){
        return;
    }
    // Iterate through the queue and free each robot
    while (current != NULL) {
        nextRobot = current->next;  // Store the next robot before freeing the current one
        free(current);              // Free the current robot
        current = nextRobot;        // Move to the next robot
    }

    // After freeing all robots, reset the queue pointers to NULL
    queueFirst = NULL;
    queueLast = NULL;
}

//----------------------------------------------------------main
// This is the main loop of the program. 
// It generates and consumes events.
void SimulationLoop(int EventNumbers)
{
    srand(time(NULL)); // to ensure randomization with each run
    int eventType;
    int stackIndex;
    InitStacks();
    
    for (int i = 0; i < EventNumbers; i++) {
        // generate event type
        eventType = GenerateEventType();
        
        // depending on the generated event type:
        if (eventType == 0) {
            struct RobotPackage* newPackage = GenerateRobotPackage();
            SimulateManagingRobotPackages(newPackage);
        } else if (eventType == 1) {
            struct Package* newPackage = GeneratePackage();
            SimulateClassifyPackage(newPackage);
        } else if (eventType == 2) {
            struct Shopping* newShopping = GenerateShopping();
            SimulateGoForShopping(newShopping);
        }
        
        UpdateShoppingQueue();
    }
    
    PrintRobotPackages(robotPackageList); 
    PrintPackages();
    
    // CLEANING THE SIMULATION
    RemoveAllRobotPackages();
    CleanPackageStacks();
    PrintShopping();         // Print robots in the queue
    CleanShoppingQueue();    // Clean the queue without printing
}
		

int main (int argc, char ** argv)
{
	printf ("Starting... \n");
	CheckArguments(argc, argv);
    int EventNumbers = atoi(argv[1]);
	SimulationLoop(EventNumbers);
	return 0;
}
