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
/*
void addToRoadMap(struct RoadMap**head, int city_id, int total_cost){ //afegeix un nou node al final de la llista
    struct RoadMap *new_node= malloc(sizeof(struct RoadMap));
    new_node->city_id=city_id; 
    new_node->total_cost =total_cost;
    new_node-> next= NULL; 

    if (*head == NULL){
        *head= new_node; //If list empty creates node
    } else{ // Sino pues al final ou yeah
        struct RoadMap*temp= *head;
        while(temp->next!= NULL) temp=temp->next;
        temp->next= new_node;
    }

}
*/
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

/*
FALTA:
    - muderfucking fuck heuristics (el routesearch)
    - El DFS (deberia follar serpientes) y BFS (bolleras, fumetas, sinceras)
    - el printeo de los trees
    - y bueno otras cosas que seguro que me dejo
sharaut sara del 13 de mayo si estas viendo esto dimelou

*/
/*
int RouteSearch(int source, int destination, struct RoadMap**roadmap){
    int visited[NUMBER_CITIES]={0};
    int current=source;
    int total_cost=0;
    while (current!=destination){
        visited[current]=1;
        int next_city=-1;
        int min_cost=INF; //garanteix que qualsevol ciutat sigui mes barata 

        //provem si es pot directament
        if (adjacency_matrix[current][destination]>0 && adjacency_matrix[current][destination]<min_cost){
            next_city=destination;
            min_cost=adjacency_matrix[current][destination];
        }
        //aqui busque, la ciutat mes barata connectada a current 
        for(int i=0;i<NUMBER_CITIES;i++){
            if(!visited[i]&&adjacency_matrix[current][i]>0 && adjacency_matrix[current][i]<min_cost){
                next_city=i;
                min_cost=adjacency_matrix[current][i];
            }
        }
        // si no hi han mes ciutats disponibles doncs que salti un missatge
        if (next_city==-1){
            printf("We couldn't find a rout from %d to %d. \n", source, destination);
            return -1;
        }

        //Actualizamos
        total_cost+=min_cost;
        addToRoadMap(roadmap,next_city,total_cost);
        current=next_city;
    }
    return total_cost;
}
*/

int RouteSearch(int source, int destination, struct RoadMap** roadmap, int cost_offset) {
    int dist[NUMBER_CITIES], visited[NUMBER_CITIES] = {0}, prev[NUMBER_CITIES];
    for (int i = 0; i < NUMBER_CITIES; i++) {
        dist[i] = INF;
        prev[i] = -1;
    }
    dist[source] = 0;

    for (int count = 0; count < NUMBER_CITIES - 1; count++) {
        int u = -1, min = INF;
        for (int i = 0; i < NUMBER_CITIES; i++) {
            if (!visited[i] && dist[i] < min) {
                min = dist[i];
                u = i;
            }
        }

        if (u == -1) break;
        visited[u] = 1;

        for (int v = 0; v < NUMBER_CITIES; v++) {
            if (adjacency_matrix[u][v] > 0 && !visited[v]) {
                int new_dist = dist[u] + adjacency_matrix[u][v];
                if (new_dist < dist[v]) {
                    dist[v] = new_dist;
                    prev[v] = u;
                }
            }
        }
    }

    if (dist[destination] == INF) {
        printf("No route from %s to %s.\n", citiesInfo[source].city_name, citiesInfo[destination].city_name);
        return -1;
    }

    // Rebuild the path in correct order
    int path[NUMBER_CITIES];
    int length = 0, current = destination;
    while (current != -1) {
        path[length++] = current;
        current = prev[current];
    }

    // Find the tail of the roadmap
    struct RoadMap* last = *roadmap;
    int last_id = -1;
    while (last && last->next) last = last->next;
    if (last) last_id = last->city_id;

    // Add only *new* cities from the path
    for (int i = length - 1; i >= 0; i--) {
        int city = path[i];
        if (city == last_id) continue; // skip already added
        addToRoadMap(roadmap, city, dist[city] + cost_offset);
    }

    return dist[destination];
}


int getCurrentTotalCost(struct RoadMap* roadmap) {
    if (!roadmap) return 0;
    while (roadmap->next != NULL) roadmap = roadmap->next;
    return roadmap->total_cost;
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
        int offset = getCurrentTotalCost(*roadmap);
        RouteSearch(node->city_id, node->mother_parents->city_id, roadmap, offset);
        DFSroute(node->mother_parents, roadmap); //pedazo recursion aqui chaval
    }
    //misma shit con el papa
    if (node->father_parents != NULL){
        int offset = getCurrentTotalCost(*roadmap);
        RouteSearch(node->city_id, node->father_parents->city_id, roadmap, offset);
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


 Function to perform BFS codigo plantilla
*/


struct FamilyTreeNode* createFamilyTreeBFS(int city_id)
{
    struct FamilyTreeNode* root = malloc(sizeof(struct FamilyTreeNode));
    if (!root) return NULL;

    strcpy(root->motherName, citiesInfo[city_id].mother_name);
    strcpy(root->fatherName, citiesInfo[city_id].father_name);
    root->city_id = city_id;
    root->mother_parents = root->father_parents = NULL;

    struct FamilyTreeNode* q[NUMBER_CITIES];
    int f = 0, r = 0;
    q[r++] = root;

    while (f < r) {
        struct FamilyTreeNode* cur = q[f++];
        int mid = citiesInfo[cur->city_id].mother_parents_city_id;
        int fid = citiesInfo[cur->city_id].father_parents_city_id;

        if (mid != -1) {
            struct FamilyTreeNode* m = malloc(sizeof(struct FamilyTreeNode));
            strcpy(m->motherName, citiesInfo[mid].mother_name);
            strcpy(m->fatherName, citiesInfo[mid].father_name);
            m->city_id = mid;
            m->mother_parents = m->father_parents = NULL;
            cur->mother_parents = m;
            q[r++] = m;
        }
        if (fid != -1) {
            struct FamilyTreeNode* p = malloc(sizeof(struct FamilyTreeNode));
            strcpy(p->motherName, citiesInfo[fid].mother_name);
            strcpy(p->fatherName, citiesInfo[fid].father_name);
            p->city_id = fid;
            p->mother_parents = p->father_parents = NULL;
            cur->father_parents = p;
            q[r++] = p;
        }
    }
    return root;
}


void BFSroute(struct FamilyTreeNode* root, struct RoadMap** roadmap) {
    if (root == NULL) return;

    // Create a queue for BFS
    struct FamilyTreeNode* queue[NUMBER_CITIES];  // Cola de nodos juajauaja
    int front = 0, rear = 0;

    // Enqueue root and initialize rear
    queue[rear++] = root;

    while (front < rear) {
        // Print front of queue and remove it from queue
        struct FamilyTreeNode* current = queue[front++];

        if (current->mother_parents != NULL) {
            int offset = getCurrentTotalCost(*roadmap);
            RouteSearch(current->city_id, current->mother_parents->city_id, roadmap, offset);
            queue[rear++] = current->mother_parents;
        }

        // same here with the papa
        if (current->father_parents != NULL) {
            int offset = getCurrentTotalCost(*roadmap);
            RouteSearch(current->city_id, current->father_parents->city_id, roadmap, offset);
            queue[rear++] = current->father_parents;
        }
    }
}

void printFamilyTree(struct FamilyTreeNode* node, int level){
    if (node==NULL) return;
    for (int i = 0; i < level; i++) printf("-> ");
    printf("%s and %s (", node->motherName, node->fatherName);
    PrintCityName(node->city_id);
    printf(")\n");

    printFamilyTree(node->mother_parents, level + 1);
    printFamilyTree(node->father_parents, level + 1);
}
/*
int main(){
    struct RoadMap*roadmap=NULL;
    // mas pruebas
    int source=0;
    int destination=3;

    addToRoadMap(&roadmap, source,0);

    int total_cost= RouteSearch(source, destination, &roadmap);

    printf("\nFinal route (DEMO): \n");
    printRoadMap(roadmap);
    printf("\nTotal cost: %d\n", total_cost);

    deleteAllRoadMap(&roadmap);

    printf("Test: root = %s and %s\n", citiesInfo[0].mother_name, citiesInfo[0].father_name);

    printf("\nDFS Ancestor Tree:\n");
    struct FamilyTreeNode *tree = createFamilyTreeDFS(0);  // Empiezas por ciudad 0 (Barcelona)
    printFamilyTree(tree, 0);

    return 0;
}
*/

void printFormattedRoadMap(struct RoadMap* head) {
    if (!head || !head->next) return;

    struct RoadMap* current = head;
    struct RoadMap* next = current->next;

    int segment_cost = 0;

    printf("%s", citiesInfo[current->city_id].city_name);

    while (next != NULL) {
        int from = current->city_id;
        int to = next->city_id;
        int cost = adjacency_matrix[from][to];

        // Si hay una conexión válida, es parte del mismo segmento
        if (cost > 0) {
            printf("-%s", citiesInfo[to].city_name);
            segment_cost += cost;
        } else {
            // Si no hay conexión directa, nuevo segmento
            printf(" %d\n", segment_cost);
            segment_cost = 0;
            printf("%s", citiesInfo[to].city_name);
        }

        current = next;
        next = next->next;
    }

    // Último tramo
    printf(" %d\n", segment_cost);
}



/*
HOLIIIIIS tenemos un par d errores en el DFS. Esto es lo que printea:abortDFS -> Names:
Maria and Jordi (City: Barcelona)
-> Louise and Paul (City: Paris)
-> -> Anna and Kazimierz (City: Varsovia)
-> -> Agnese and Leonardo (City: Rome)
-> Eva and Albert (City: Zurich)
-> -> Madalena and Louren├ºo (City: Lisbon)
-> -> Amber and Finn (City: Amsterdam)

DEBERIA printear esto:
-> Louise and Pol (Paris)
-> Eva and Albert (Zurich)
->-> Anna and Kazimierz (Varsovia)
->-> Agnese and Leonardo (Rome)
->-> Madalena and Lourenc¸o (Lisboa)
->-> Amber and Fin (Amsterdam)

Es decir, los que tienen solo una flecha -> deberian ir primero. Asi la suma de los costos seria menor (deberia serlo)

*/



int main(){
    // DFS BLOCK
    printf("Ancestors’ tree:\n");
    printf("DFS -> Names:\n");

    struct FamilyTreeNode* treeDFS = createFamilyTreeDFS(0);
    printFamilyTree(treeDFS, 0);

    struct RoadMap* roadmapDFS = NULL;
    DFSroute(treeDFS, &roadmapDFS);

    printf("\nPartial road map:\n");
    printFormattedRoadMap(roadmapDFS);

    printf("Complete Road Map:\n");
    struct RoadMap* temp = roadmapDFS;
    while (temp != NULL) {
        printf("%s", citiesInfo[temp->city_id].city_name);
        if (temp->next != NULL) printf("-");
        temp = temp->next;
    }

    int total = getCurrentTotalCost(roadmapDFS);

    printf("\nTotal cost: %d\n", total);

    deleteAllRoadMap(&roadmapDFS);

    printf("----------------------------------\n");

    //BFS BLOCK
    printf("BFS -> Names:\n");

    struct FamilyTreeNode* treeBFS = createFamilyTreeBFS(0);
    printFamilyTree(treeBFS, 0);

    struct RoadMap* roadmapBFS = NULL;
    BFSroute(treeBFS, &roadmapBFS);

    printf("\nPartial road map:\n");
    // igual que DFS pero siguiendo orden BFS
    printFormattedRoadMap(roadmapBFS);

    printf("Complete Road Map:\n");
    // igual que DFS
    struct RoadMap* temp2 = roadmapBFS;
    while (temp2 != NULL) {
        printf("%s", citiesInfo[temp2->city_id].city_name);
        if (temp2->next != NULL) printf("-");
        temp2 = temp2->next;
    }

    int total2 = getCurrentTotalCost(roadmapBFS);
    printf("\nTotal cost: %d\n\n\n", total2);

    deleteAllRoadMap(&roadmapBFS);

    return 0;
}