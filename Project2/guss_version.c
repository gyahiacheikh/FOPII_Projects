#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "small.h" // Me da la adjecent matrix y los registros

#define INF 99999999 //Infinite cost for i==j

void PrintCityName(int city_id){ //para imprimir el nombre de la ciudad por si aca
    printf("City: %s", citiesInfo[city_id].city_name);
}

// 2. RoadMap type shi
// RoadMap es una llista enllaçada on cada nodo es una ciutat, guarda el cost total fins la ciutat aquella i apunta al seguent ciutat (node)

void printRoadMap(struct RoadMap*head){ //pa ver la rout
    struct RoadMap*temp=head;
    while (temp!=NULL){
        PrintCityName(temp->city_id);
        printf(" %d \n", temp->total_cost);
        temp=temp->next;
    }
}


void addToRoadMap(struct RoadMap**head, int city_id, int total_cost){
    if (*head != NULL){
        struct RoadMap*last = *head;
        while (last->next != NULL) last = last->next;
        if (last->city_id == city_id) return; // Evita duplicados consecutivos
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
void deleteAllRoadMap(struct RoadMap **head){//LLAMAR AL FINAL DEL PROGRAMA SOLO EH
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

        // Encuentra el nodo no visitado con menor distancia
        for(int i=0; i<NUMBER_CITIES; i++){
            if(!visited[i] && dist[i]<min){
                min=dist[i];
                u=i;
            }
        }

        if(u==-1) break; // No hay más alcanzables

        visited[u]=1;

        // Actualiza distancias a los vecinos
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

    // Reconstruye el camino desde destination hasta source
    int path[NUMBER_CITIES];
    int path_length=0;
    int current=destination;
    while(current!=-1){
        path[path_length++] = current;
        current = prev[current];
    }

    // Inserta en roadmap en orden correcto (source -> ... -> destination)
    for(int i=path_length-1; i>=0; i--){
        addToRoadMap(roadmap, path[i], dist[path[i]]);
    }

    return dist[destination];
}
//DFS

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

void DFSroute(struct FamilyTreeNode*node, struct RoadMap**roadmap){
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


//BFS
/*Pseudocode:
create a queue Q 
mark v as visited and put v into Q 
while Q is non-empty 
    remove the head u of Q 
    mark and enqueue all (unvisited) neighbours of u


// Function to perform BFS codigo plantilla
void bfs(struct Node* root) {
    if (root == NULL) return;

    // Create a queue for BFS
    struct Node* queue;
    int front = 0, rear = 0;

    // Enqueue root and initialize rear
    queue[rear++] = root;

    while (front != rear) {
        // Print front of queue and remove it from queue
        struct Node* node = queue[front++];
        printf("%d ", node->data);

        // Enqueue left child
        if (node->left != NULL) {
            queue[rear++] = node->left;
        }

        // Enqueue right child
        if (node->right != NULL) {
            queue[rear++] = node->right;
        }
    }
}
*/
void printFamilyTree(struct FamilyTreeNode* node, int level){
    if (node==NULL) return;
    for (int i = 0; i < level; i++) printf("-> ");
    printf("%s and %s (", node->motherName, node->fatherName);
    PrintCityName(node->city_id);
    printf(")\n");

    printFamilyTree(node->mother_parents, level + 1);
    printFamilyTree(node->father_parents, level + 1);
}


int main(){
    // DFS BLOCK
    printf("Ancestors’ tree:\n");
    printf("DFS -> Names:\n");

    struct FamilyTreeNode* treeDFS = createFamilyTreeDFS(0);
    printFamilyTree(treeDFS, 0);

    struct RoadMap* roadmapDFS = NULL;
    DFSroute(treeDFS, &roadmapDFS);

    printf("\nPartial road map:\n");
    printRoadMap(roadmapDFS);

    printf("Complete Road Map:\n");
    struct RoadMap* temp = roadmapDFS;
    while (temp != NULL) {
        printf("%s", citiesInfo[temp->city_id].city_name);
        if (temp->next != NULL) printf("-");
        temp = temp->next;
    }

    int total=0;
    if (roadmapDFS != NULL) {
        struct RoadMap* first = roadmapDFS;
        struct RoadMap* last = roadmapDFS;
        while (last->next != NULL) last = last->next;
        total = last->total_cost - first->total_cost;
    }
    printf("\nTotal cost: %d\n", total);

    deleteAllRoadMap(&roadmapDFS);

    printf("----------------------------------\n");

    // BFS BLOCK
    printf("BFS -> Names:\n");


    printf("\nPartial road map:\n");
    // igual que DFS pero siguiendo orden BFS

    printf("Complete Road Map:\n");
    // igual que DFS

    return 0;
}