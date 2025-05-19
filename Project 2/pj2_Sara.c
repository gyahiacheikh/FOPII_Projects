//Sara's version
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "small.h"

// The first part of the project must be done with graph structs. 13/05 Sara
// The header has 3 structs defined: struct CivilRegistry, struct FamilyTreeNode and struct RoadMap. 13/05 Sara
//Veo que has cogido partes mias me parece top reinona eso si me dijiste que me dejaste un comentario noloveo uuu guss 19/05



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

int RouteSearch(struct CivilRegistry * citiesInfo, int id_sourcecity, int id_destcity, struct RoadMap **roadmap) {
    int i = citiesInfo[id_sourcecity].city_id;
    int j = citiesInfo[id_destcity].city_id;

    if (adjacency_matrix[i][j] != 0) {
        addToRoadMap(roadmap, j, adjacency_matrix[i][j]);
        return adjacency_matrix[i][j];
    } else {
        int distance[NUMBER_CITIES];
        int previous[NUMBER_CITIES];
        dijkstra(adjacency_matrix, NUMBER_CITIES, i, distance, previous);

        if (distance[j] == INF) {
            printf("No route exists from %s to %s.\n", citiesInfo[i].city_name, citiesInfo[j].city_name);
            return INF;
        }

        // Reconstruct path (reversed order)
        int path[NUMBER_CITIES];
        int count = 0, temp = j;
        while (temp != -1) {
            path[count++] = temp;
            temp = previous[temp];
        }

        // Add path to roadmap in correct order
        for (int k = count - 1; k >= 0; k--) {
            int cost = (k == 0) ? 0 : adjacency_matrix[path[k - 1]][path[k]];
            addToRoadMap(roadmap, path[k], cost);
        }

        return distance[j];
    }
}


void addToRoadMap(struct RoadMap**head, int city_id, int total_cost){ //afegeix un nou node al final de la llista 13/05 Guss
    struct RoadMap *new_node= malloc(sizeof(struct RoadMap));
    new_node->city_id=city_id; 
    new_node->total_cost =total_cost;
    new_node-> next= NULL; 

    if (*head == NULL){
        *head= new_node; //If list empty creates node 13/05 Guss
    } else { // Sino pues al final ou yeah
        struct RoadMap*temp= *head;
        while(temp->next!= NULL) temp=temp->next;
        temp->next= new_node;
    }

}


void PrintCityName(int city_id){ //para imprimir el nombre de la ciudad por si aca
    printf("City: %s", citiesInfo[city_id].city_name);
}

void printRoadMap(struct RoadMap*head){ //pa ver la rout 13/05 Guss
    struct RoadMap*temp=head;
    while (temp!=NULL){
        PrintCityName(temp->city_id);
        printf(" %d \n", temp->total_cost);
        temp=temp->next;
    }
}

void deleteAllRoadMap(struct RoadMap **head){//LLAMAR AL FINAL DEL PROGRAMA SOLO EH 13/05 Guss
    struct RoadMap*temp;
    while(*head!=NULL){
        temp=*head;
        *head=(*head)->next;
        free(temp);
    }
}


struct FamilyTreeNode*createFamilyTreeDFS(int city_id){
    struct FamilyTreeNode*node=malloc(sizeof(struct FamilyTreeNode));
    if (node==NULL){
        printf("ERROR: No se pudo reservar memoria para el nodo.\n");
        return NULL;
    }
    strcpy(node->motherName, citiesInfo[city_id].mother_name);
    strcpy(node->fatherName, citiesInfo[city_id].father_name);
    node->city_id=city_id;
    node->mother_parents=NULL;
    node->father_parents=NULL;

    if(citiesInfo[city_id].mother_parents_city_id != -1){
        node->mother_parents =createFamilyTreeDFS(citiesInfo[city_id].mother_parents_city_id);
    }
    if(citiesInfo[city_id].father_parents_city_id != -1){
        node->father_parents =createFamilyTreeDFS(citiesInfo[city_id].father_parents_city_id);
    }

    return node;
}


void DFSroute(struct FamilyTreeNode*node, struct RoadMap**roadmap){ //Guss's code
    if (node==NULL) return;
    if (node->mother_parents != NULL){
        RouteSearch(node->city_id, node->mother_parents->city_id, roadmap);
        DFSroute(node->mother_parents, roadmap); //pedazo recursion aqui chaval
    }
    //misma shit con el papa
    if (node->father_parents != NULL){
        RouteSearch(node->city_id, node->father_parents->city_id, roadmap);
        DFSroute(node->father_parents, roadmap); //pedazo recursion aqui chaval
    }
}


struct FamilyTreeNode*createFamilyTreeBFS(int city_id){
    struct FamilyTreeNode*node=malloc(sizeof(struct FamilyTreeNode));
    if (node==NULL){
        printf("ERROR: No se pudo reservar memoria para el nodo.\n");
        return NULL;
    }
    strcpy(node->motherName, citiesInfo[city_id].mother_name);
    strcpy(node->fatherName, citiesInfo[city_id].father_name);
    node->city_id=city_id;
    node->mother_parents=NULL;
    node->father_parents=NULL;

    if(citiesInfo[city_id].mother_parents_city_id != -1){
        node->mother_parents = createFamilyTreeBFS(citiesInfo[city_id].mother_parents_city_id);
    }
    if(citiesInfo[city_id].father_parents_city_id != -1){
        node->father_parents = createFamilyTreeBFS(citiesInfo[city_id].father_parents_city_id);
    }

    return node;
}

void BFSroute()

// He pegado tu main tal cual guss (perfe -guss 19/5)
int main(){
    struct RoadMap*roadmap=NULL;

    //PRUEBAAAAAS
    addToRoadMap(&roadmap, 0, 0);
    addToRoadMap(&roadmap, 1, 50);   
    addToRoadMap(&roadmap, 2, 180);

    printf("Actual route: (DEMO)\n");
    printRoadMap(roadmap);
    deleteAllRoadMap(&roadmap);

    return 0;
}