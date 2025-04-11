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
        printf("Usage: %s <number_of_events>\n", argv[0]);
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

    //printf("Inserted RobotPackage: %s, %s, %d\n", 
        //RobotPackage->supplier, RobotPackage->id, RobotPackage->year);
}

// function to remove all the RobotPackages from the list at the end of the program
void RemoveAllRobotPackages()
{
	struct RobotPackage* current = robotPackageList;
    struct RobotPackage* nextNode;

    int count=0;
        while (current != NULL) {
            count++;
            printf("Robot ID: %d, Items to buy: %d
", current->robot_id, current->numberThingsToBuy);
            current=current->next;
        }
        printf("Total remaining robots in queue: %d
", count);
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

    //printf("Robot ID=%d added to the shopping queue with %d items to buy.\n", shopping->robot_id, shopping->numberThingsToBuy);
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

    //FUNCIONA printf("Robot ID=%d is now shopping, buying %d items.\n", temp->robot_id, shoppingTime);
    robotsRemoved++;
    free(temp); // Free the memory allocated for the removed robot

    return shoppingTime; // Return the shopping time to simulate time passing
}

// function to simulate the time the robot is in the queue
void UpdateShoppingQueue ()
{
	// If the queue is not empty
    if (queueFirst != NULL) {
        // Process the first robot in the queue
        struct Shopping *current = queueFirst;
        
        // If the robot's shopping is not complete, simulate time by buying 1 item per event
        while (current != NULL) {
            if (current->numberThingsToBuy > 0) {
                // Decrease the number of things the robot needs to buy (simulate 1 event)
                current->numberThingsToBuy--;
                
                //printf("Robot ID=%d is shopping, %d items left.\n", current->robot_id, current->numberThingsToBuy);
            }

            // If the robot has finished shopping, dequeue it from the queue
            if (current->numberThingsToBuy == 0) {
                // Robot finished shopping, remove it from the queue
                Dequeue();
                break; // Move to the next robot (if there are any left)
            }
            
            // Move to the next robot in the queue if the current one is still shopping
            current = current->next;
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

    //printf("Robot ID=%d with %d items to buy has joined the shopping queue.\n", shopping->robot_id, shopping->numberThingsToBuy);
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
