#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "small.h"

// The first part of the project must be done with graph structs. 13/05 Sara
// The header has 3 structs defined: struct CivilRegistry, struct FamilyTreeNode and struct RoadMap. 13/05 Sara

void RouteSearch(struct CivilRegistry * citiesInfo, int id_sourcecity, int id_destcity){
    int cost;
    int i = citiesInfo[id_sourcecity].city_id;
    int j = citiesInfo[id_destcity].city_id;
    adjacency_matrix[i][j];

    if (adjacency_matrix[i][j] =! 0) {
        //printf("There exist a direct route from %s to %s, with cost: %d.\n", citiesInfo[id_sourcecity].city_name, citiesInfo[id_destcity].city_name, adjacency_matrix[i][j]);
        cost = adjacency_matrix[i][j];
    }
    else {
        // No direct route between the two cities exist; an heuristic must be implemented 13/05 Sara


    }
}

void addToRoadMap(){

}

void printRoadMap(){

}

void deleteAllRoadMap(){

}

