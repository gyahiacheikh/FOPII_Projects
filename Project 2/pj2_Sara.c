#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "small.h"

// The first part of the project must be done with graph structs. 13/05 Sara
// The header has 3 structs defined: struct CivilRegistry, struct FamilyTreeNode and struct RoadMap. 13/05 Sara



// El dijkstra lo he copiado tal cual del chatgpt, deberiamos hacerlo nosotras pero asi podia seguir la funcion q estaba haciendo
#define INF 1000000
void dijkstra(int graph[NUMBER_CITIES][NUMBER_CITIES], int n, int start, int dist[], int prev[]) {
    int visited[NUMBER_CITIES] = {0};

    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[start] = 0;

    for (int count = 0; count < n - 1; count++) {
        int min = INF, u = -1;

        // Find the unvisited node with the smallest distance
        for (int v = 0; v < n; v++) {
            if (!visited[v] && dist[v] <= min) {
                min = dist[v];
                u = v;
            }
        }

        if (u == -1) break; // no reachable node

        visited[u] = 1;

        for (int v = 0; v < n; v++) {
            if (!visited[v] && graph[u][v] && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                prev[v] = u;
            }
        }
    }
}


void RouteSearch(struct CivilRegistry * citiesInfo, int id_sourcecity, int id_destcity){
    int cost;
    int i = citiesInfo[id_sourcecity].city_id;
    int j = citiesInfo[id_destcity].city_id;
    adjacency_matrix[i][j];

    if (adjacency_matrix[i][j] =! 0) {
        cost = adjacency_matrix[i][j];
    }
    else {
        // No direct route between the two cities exist; an heuristic (dijkstra) must be implemented 13/05 Sara
        int distance[NUMBER_CITIES];
        int previous[NUMBER_CITIES];

        dijkstra(adjacency_matrix, NUMBER_CITIES, i, distance, previous);

        if (distance[j] != INT_MAX) {
            cost = distance[j];
        }
        else {
            printf("No route exists from %s to %s.\n", citiesInfo[id_sourcecity].city_name, citiesInfo[id_destcity].city_name);
        }
    }
}

void addToRoadMap(){

}

void printRoadMap(){

}

void deleteAllRoadMap(){

}

