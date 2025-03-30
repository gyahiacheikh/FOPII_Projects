/*
Degree: Artificial Intelligence
Subject: Fundamentals of Programming 2
Practical project: 1

Simulator - main program
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "project1.h" // header with all data structures

int booksRemoved = 0;
int platesRemoved = 0;
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

	int EventNumbers = atoi(argv[1]); // Convert the argument to an integer
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
	strcpy (RobotPackage->supplier, suppliers[RobotPackageNum]);
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
		top[i] = -1; // Initialize as empty
	}
}

// function to print all stacks with all Packages
void PrintPackages()
{
	for (int i = 0; i < 3; i++) {
		printf("Stack %d:\n", i);
		for (int j = 0; j <= top[i]; j++) {
			printf("Package Type: %d, Color: %d\n", stacks[i][j].type, stacks[i][j].color);
		}
		printf("\n");
	}
}

// function to remove all packages from a given stack when its MAX_CAPACITY is reached
void RemoveStack(int stackIndex, struct Package **stacks, int *top) {
	if (stackIndex == 0) { // Books are in stack 0
        booksRemoved += (top[stackIndex] + 1);
    } else if (stackIndex == 1) { // Plates are in stack 1
        platesRemoved += (top[stackIndex] + 1);
    } else if (stackIndex == 2) { // Robots are in stack 2
		robotsRemoved += (top[stackIndex] + 1);
	}
	
    top[stackIndex] = -1; // Reset stack
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
}

// function to clean all stacks before the end of the program
void CleanPackageStacks()
{

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
	printf("STATISTICS WHEN CLEANING THE SIMULATION:\n");
    printf("Removing books...\n");
    printf("%d books has been removed.\n", booksRemoved);

    printf("Cleaning all stacks of plates...\n");
    printf("%d plates has been removed.\n", platesRemoved);

    printf("Cleaning shopping queue...\n");
    printf("%d robots has been removed.\n", robotsRemoved);
}

// function to add a robot to a shopping queue
void AddToQueue(struct Shopping * shopping)
{

}

// function to remove a robot from the queue and serve it
// it may return the number of things to buy to simulate the time
int Dequeue ()
{

}

// function to simulate the time the robot is in the queue
void UpdateShoppingQueue (/*...*/)
{

}

// function to simulate a robot going for shopping - add to the queue
void SimulateGoForShopping(struct Shopping * shopping)
{

}

// function to clean shopping queue before the end of the program
void CleanShoppingQueue()
{

}

//----------------------------------------------------------main
// This is the main loop of the program. 
// It generates and consumes events.
void SimulationLoop(int EventNumbers)
{
	srand(time(NULL)); // to ensure randomization with each run
	int eventType;
	InitStacks();
	
	for (int i=0; i<EventNumbers; i++){
		// generate event type
		eventType = GenerateEventType();
		
		// depending on the generated event type:
		// Event type 0: 
		if (eventType == 0) {
			// generate RobotPackage 
			struct RobotPackage* newPackage = GenerateRobotPackage();
			// Simulate managing RobotPackages (sorting)
			SimulateManagingRobotPackages(newPackage);
		}

		// event type 1: 
		else if (eventType == 1) {
			//generate Package
			struct Package* newPackage = GeneratePackage();
			// Simulate classifying Packages (pputting to a corresponding stack)
			SimulateClassifyPackage(newPackage);
		}

		// event type 2:
		else if (eventType == 2) {
			// generate shopping
			struct Shopping* newShopping = GenerateShopping();
			// Simulate go for shopping
			SimulateGoForShopping(newShopping);
		}
		
		// UpdateShopping
		UpdateShoppingQueue();
	}
	
	// CLEANING THE SIMULATION
	RemoveAllRobotPackages();
	CleanPackageStacks();
	CleanShoppingQueue();
}
		

int main (int argc, char ** argv)
{
	int EventNumbers = atoi(argv[1]);
	printf ("Starting... \n");
	CheckArguments(argc, argv);

	SimulationLoop(EventNumbers);
	PrintShopping();
	return 0;
}
