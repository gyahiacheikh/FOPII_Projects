#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "small.h"

#define INF 99999999

// Forward declarations for family tree creation functions
struct FamilyTreeNode* createFamilyTreeDFS(int start_id);
struct FamilyTreeNode* createFamilyTreeBFS(int start_id);

// Stub implementations to fix linker errors
struct FamilyTreeNode* createFamilyTreeDFS(int start_id) {
    // TODO: Replace with actual implementation
    return NULL;
}
struct FamilyTreeNode* createFamilyTreeBFS(int start_id) {
    // TODO: Replace with actual implementation
    return NULL;
}

void PrintCityName(int city_id){
    printf("%s", citiesInfo[city_id].city_name);
}

// Imprime el árbol genealógico en el formato pedido
void printFamilyTree(struct FamilyTreeNode* node, int level){
    if (node==NULL) return;
    for (int i = 0; i < level; i++) printf("-> ");
    printf("%s and %s (", node->motherName, node->fatherName);
    PrintCityName(node->city_id);
    printf(")\n");

    printFamilyTree(node->mother_parents, level + 1);
    printFamilyTree(node->father_parents, level + 1);
}

void deleteFamilyTree(struct FamilyTreeNode*node){
    if (node==NULL) return;
    deleteFamilyTree(node->mother_parents);
    deleteFamilyTree(node->father_parents);
    free(node);
}

void addToRoadMap(struct RoadMap**head, int city_id, int total_cost){
    if (*head != NULL){
        struct RoadMap*last = *head;
        while (last->next != NULL) last = last->next;
        if (last->city_id == city_id) return;
    }
    struct RoadMap *new_node = malloc(sizeof(struct RoadMap));
    new_node->city_id = city_id; 
    new_node->total_cost = total_cost;
    new_node->next = NULL;
    if (*head == NULL){
        *head = new_node;
    } else {
        struct RoadMap*temp = *head;
        while(temp->next != NULL) temp = temp->next;
        temp->next = new_node;
    }
}

void deleteAllRoadMap(struct RoadMap **head){
    struct RoadMap*temp;
    while(*head!=NULL){
        temp=*head;
        *head=(*head)->next;
        free(temp);
    }
}

int RouteSearch(int source, int destination, struct RoadMap** roadmap){
    int dist[NUMBER_CITIES];
    int visited[NUMBER_CITIES] = {0};
    int prev[NUMBER_CITIES];
    for(int i=0; i<NUMBER_CITIES; i++){
        dist[i]=INF;
        prev[i]=-1;
    }
    dist[source]=0;
    for(int count=0; count<NUMBER_CITIES-1; count++){
        int u=-1;
        int min=INF;
        for(int i=0; i<NUMBER_CITIES; i++){
            if(!visited[i] && dist[i]<min){
                min=dist[i];
                u=i;
            }
        }
        if(u==-1) break;
        visited[u]=1;
        for(int v=0; v<NUMBER_CITIES; v++){
            if(adjacency_matrix[u][v]>0 && !visited[v]){
                int new_dist = dist[u] + adjacency_matrix[u][v];
                if(new_dist < dist[v]){
                    dist[v] = new_dist;
                    prev[v] = u;
                }
            }
        }
    }
    if(dist[destination]==INF){
        printf("We couldn't find a route from %s to %s.\n", 
            citiesInfo[source].city_name, citiesInfo[destination].city_name);
        return -1;
    }
    int path[NUMBER_CITIES];
    int path_length=0;
    int current=destination;
    while(current!=-1){
        path[path_length++] = current;
        current = prev[current];
    }
    for(int i=path_length-1; i>=0; i--){
        addToRoadMap(roadmap, path[i], dist[path[i]]);
    }
    return dist[destination];
}

// Estructura para guardar trayectos parciales
struct PartialRoute {
    int from;
    int to;
    int cost;
    struct RoadMap* snapshot;
};

// DFS con almacenamiento de trayectos parciales
void DFSroute(struct FamilyTreeNode*node, struct RoadMap**roadmap, struct PartialRoute* partials, int* partial_count){
    if (node==NULL) return;
    if (node->mother_parents != NULL){
        struct RoadMap* before = *roadmap;
        int cost = RouteSearch(node->city_id, node->mother_parents->city_id, roadmap);
        partials[*partial_count].from = node->city_id;
        partials[*partial_count].to = node->mother_parents->city_id;
        partials[*partial_count].cost = cost;
        partials[*partial_count].snapshot = before ? before->next : *roadmap;
        (*partial_count)++;
        DFSroute(node->mother_parents, roadmap, partials, partial_count);
    }
    if (node->father_parents != NULL){
        struct RoadMap* before = *roadmap;
        int cost = RouteSearch(node->city_id, node->father_parents->city_id, roadmap);
        partials[*partial_count].from = node->city_id;
        partials[*partial_count].to = node->father_parents->city_id;
        partials[*partial_count].cost = cost;
        partials[*partial_count].snapshot = before ? before->next : *roadmap;
        (*partial_count)++;
        DFSroute(node->father_parents, roadmap, partials, partial_count);
    }
}

// BFS con almacenamiento de trayectos parciales
void BFSroute(struct FamilyTreeNode* root, struct RoadMap** roadmap, struct PartialRoute* partials, int* partial_count) {
    if (root == NULL) return;
    struct FamilyTreeNode* queue[NUMBER_CITIES];
    int front = 0, rear = 0;
    queue[rear++] = root;
    while (front < rear) {
        struct FamilyTreeNode* current = queue[front++];
        if (current->mother_parents != NULL) {
            struct RoadMap* before = *roadmap;
            int cost = RouteSearch(current->city_id, current->mother_parents->city_id, roadmap);
            partials[*partial_count].from = current->city_id;
            partials[*partial_count].to = current->mother_parents->city_id;
            partials[*partial_count].cost = cost;
            partials[*partial_count].snapshot = before ? before->next : *roadmap;
            (*partial_count)++;
            queue[rear++] = current->mother_parents;
        }
        if (current->father_parents != NULL) {
            struct RoadMap* before = *roadmap;
            int cost = RouteSearch(current->city_id, current->father_parents->city_id, roadmap);
            partials[*partial_count].from = current->city_id;
            partials[*partial_count].to = current->father_parents->city_id;
            partials[*partial_count].cost = cost;
            partials[*partial_count].snapshot = before ? before->next : *roadmap;
            (*partial_count)++;
            queue[rear++] = current->father_parents;
        }
    }
}

// Imprime un trayecto parcial
void printPartialRoute(struct RoadMap* roadmap, int from, int to) {
    struct RoadMap* curr = roadmap;
    int started = 0;
    while (curr != NULL) {
        if (curr->city_id == from) started = 1;
        if (started) {
            PrintCityName(curr->city_id);
            if (curr->city_id != to) printf("-");
        }
        if (curr->city_id == to && started) break;
        curr = curr->next;
    }
}

// Imprime el roadmap completo
void printCompleteRoadMap(struct RoadMap* roadmap) {
    struct RoadMap* curr = roadmap;
    int prev = -1;
    while (curr != NULL) {
        if (curr->city_id != prev) {
            PrintCityName(curr->city_id);
            if (curr->next != NULL) printf("-");
        }
        prev = curr->city_id;
        curr = curr->next;
    }
    printf("\n");
}

int computeTotalCost(struct RoadMap* roadmap){
    int total=0;
    struct RoadMap* current=roadmap;
    while (current!=NULL && current->next!=NULL){
        int from=current->city_id;
        int to=current->next->city_id;
        int step=adjacency_matrix[from][to];
        if (step<0) step=0;
        total+=step;
        current=current->next;
    }
    return total;
}

int main(){
    // DFS BLOCK
    printf("Ancestors’ tree:\n");
    printf("DFS -> Names:\n");

    struct FamilyTreeNode* treeDFS = createFamilyTreeDFS(0);
    printFamilyTree(treeDFS, 0);

    struct RoadMap* roadmapDFS = NULL;
    struct PartialRoute partialsDFS[32];
    int partial_count_DFS = 0;
    DFSroute(treeDFS, &roadmapDFS, partialsDFS, &partial_count_DFS);

    printf("\nPartial road map:\n");
    for (int i = 0; i < partial_count_DFS; i++) {
        printPartialRoute(roadmapDFS, partialsDFS[i].from, partialsDFS[i].to);
        printf(" %d\n", partialsDFS[i].cost);
    }

    printf("Complete Road Map:\n");
    printCompleteRoadMap(roadmapDFS);

    int total=computeTotalCost(roadmapDFS);
    printf("Total cost: %d\n", total);

    deleteAllRoadMap(&roadmapDFS);
    deleteFamilyTree(treeDFS);

    printf("----------------------------------\n");

    // BFS BLOCK
    printf("BFS -> Names:\n");

    struct FamilyTreeNode* treeBFS = createFamilyTreeBFS(0);
    printFamilyTree(treeBFS, 0);

    struct RoadMap* roadmapBFS = NULL;
    struct PartialRoute partialsBFS[32];
    int partial_count_BFS = 0;
    BFSroute(treeBFS, &roadmapBFS, partialsBFS, &partial_count_BFS);

    printf("\nPartial road map:\n");
    for (int i = 0; i < partial_count_BFS; i++) {
        printPartialRoute(roadmapBFS, partialsBFS[i].from, partialsBFS[i].to);
        printf(" %d\n", partialsBFS[i].cost);
    }

    printf("Complete Road Map:\n");
    printCompleteRoadMap(roadmapBFS);

    int totalB=computeTotalCost(roadmapBFS);
    printf("Total cost: %d\n", totalB);

    deleteAllRoadMap(&roadmapBFS);
    deleteFamilyTree(treeBFS);
}