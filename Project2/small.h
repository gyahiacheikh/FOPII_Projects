/*
Degree: Artificial Intelligence
Subject: Fundamentals of Programming 2
Practical project: 2

small.h
Header of adjacency matrix - data structures required by the project 2.
This is the small case, only 10 cities

We've already declared and initialized both data structures:
- adjacency matrix with the cost between cities
- cities info that contains information about civil registry for each city
*/

#define NUMBER_CITIES 10
#define MAX_NAME 10
#define MAX_CITY_NAME 20

int adjacency_matrix[NUMBER_CITIES][NUMBER_CITIES] = 
{
    {0, 100, 50, 0, 0, 0, 0, 0, 0, 0},
    {100, 0, 0, 120, 0, 0, 0, 0, 0, 0},
    {50, 0, 0, 130, 110, 0, 0, 0, 0, 0},
    {0, 120, 130, 0, 0, 0, 0, 0, 80, 0},
    {0, 0, 110, 0, 0, 70, 80, 0, 0, 0},
    {0, 0, 0, 0, 70, 0, 0, 40, 0, 0},
    {0, 0, 0, 0, 80, 0, 0, 50, 0, 0},
    {0, 0, 0, 0, 0, 40, 50, 0, 30, 100},
    {0, 0, 0, 80, 0, 0, 0, 30, 0, 90},
    {0, 0, 0, 0, 0, 0, 0, 100, 90, 0}
};

struct CivilRegistry
{
    int city_id;
    char city_name[MAX_CITY_NAME];
    char mother_name[MAX_NAME];
    char father_name[MAX_NAME];
    int mother_parents_city_id;
    int father_parents_city_id;
};

struct CivilRegistry citiesInfo[NUMBER_CITIES] =
{
    {0, "Barcelona", "Maria", "Jordi", 3, 5},
    {1, "Lisbon", "Madalena", "Lourenço", -1, -1},
    {2, "Madrid", "", "", -1, -1},
    {3, "Paris", "Louise", "Paul", 9, 4},
    {4, "Rome", "Agnese", "Leonardo", -1, -1},
    {5, "Zurich", "Eva", "Albert", 1, 6},
    {6, "Amsterdam", "Amber", "Finn", -1, -1},
    {7, "Viena", "", "", -1, -1},
    {8, "Berlin", "", "", -1, -1},
    {9, "Varsovia", "Anna", "Kazimierz", -1, -1}
};

struct FamilyTreeNode
{
    char motherName[MAX_NAME];
    char fatherName[MAX_NAME];
    int city_id;
    struct FamilyTreeNode * mother_parents;
    struct FamilyTreeNode * father_parents;
};

struct RoadMap
{
    int city_id;
    int total_cost;
    struct RoadMap * next;
};
struct PartialRoute
{
    int from;
    int to;
    int cost; // opcional, si quieres guardar el coste parcial aquí
};