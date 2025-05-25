/* =====================================================================
 * MAZE SOLVER – tugas besar Struktur Data & Algoritma
 * ----------------------------------------------------
 * Fitur:
 *   - Builder: user membuat map labirin (tree dinamis)
 *   - Game: jelajah maze, undo langkah, hint (BFS), inventori (linked list)
 *   - ADT yang dipakai:
 *       • Tree       : struktur MazeNode (pointer‑based)
 *       • Queue      : generic queue (linked list) – dipakai BFS & antrian builder
 *       • Stack      : generic stack (linked list) – dipakai DFS/undo
 *       • LinkedList : inventori & daftar seluruh node (NodeList)
 * ---------------------------------------------------------------------
 * Kompilasi:
 *   gcc main.c maze.c stack.c queue.c list.c -o maze_solver
 * =====================================================================*/


/* ============================= maze.c ============================== */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "maze.h"

/* global list of nodes */
static List allNodes;
MazeNode *startNode = NULL;
MazeNode *goalNode  = NULL;

/* ---- internal helpers ---- */
static MazeNode *allocNode(int id) {
    MazeNode *n = malloc(sizeof(MazeNode));
    if (!n) return NULL;
    n->id = id;
    n->isGoal = false;
    n->north = n->south = n->east = n->west = NULL;
    return n;
}

MazeNode *createNode(int id) {
    if (!allNodes.head) listInit(&allNodes, sizeof(NodeRef));
    /* prevent duplicate */
    if (findNode(id)) {
        printf("[Builder] Node %d sudah ada.\n", id);
        return NULL;
    }
    MazeNode *n = allocNode(id);
    NodeRef ref = { .ptr = n };
    listInsertFront(&allNodes, &ref);
    printf("[Builder] Node %d dibuat.\n", id);
    return n;
}

MazeNode *findNode(int id) {
    if (!allNodes.head) return NULL;
    for (LNode *p = allNodes.head; p; p = p->next) {
        NodeRef *ref = p->data;
        if (ref->ptr->id == id) return ref->ptr;
    }
    return NULL;
}

void connectNodes(int fromID, int toID, char dir) {
    MazeNode *from = findNode(fromID);
    MazeNode *to   = findNode(toID);
    if (!from || !to) {
        printf("[Builder] Node tidak ditemukan!\n");
        return;
    }
    switch (dir) {
        case 'N': from->north = to; break;
        case 'S': from->south = to; break;
        case 'E': from->east  = to; break;
        case 'W': from->west  = to; break;
        default:  printf("[Builder] Arah tidak valid! Gunakan N/S/E/W.\n"); return;
    }
    printf("[Builder] %d --%c--> %d\n", fromID, dir, toID);
}

/* ------ Debug print maze ------ */
static void printOne(void *data) {
    NodeRef *ref = data;
    MazeNode *n = ref->ptr;
    printf("Node %d : N=%d S=%d E=%d W=%d %s\n",
        n->id,
        n->north ? n->north->id : -1,
        n->south ? n->south->id : -1,
        n->east  ? n->east ->id : -1,
        n->west  ? n->west ->id : -1,
        n->isGoal ? "[GOAL]" : "");
}

void printMaze() {
    printf("===== MAZE STRUCTURE =====\n");
    listTraverse(&allNodes, printOne);
}

/* ------ Destroy Maze ------ */
static void freeOne(void *data) {
    NodeRef *ref = data;
    free(ref->ptr);
}

void destroyMaze() {
    /* free MazeNode objects */
    listTraverse(&allNodes, freeOne);
    listDestroy(&allNodes);
    startNode = goalNode = NULL;
}

/* ===== Gameplay Implementation ===== */

static List inventory; /* linked list of Item */

static void showInventory() {
    printf("--- Inventori ---\n");
    if (!inventory.head) {
        printf("(kosong)\n");
        return;
    }
    void printItem(void *d) { Item *it = d; printf("- %s\n", it->name); }
    listTraverse(&inventory, printItem);
}

/* BFS Hint: find shortest path distance only */
static int bfsHint(MazeNode *start) {
    Queue q; queueInit(&q, sizeof(MazeNode *));
    enqueue(&q, &start);
    /* simple visited via list of ids */
    int visited[1000] = {0};
    visited[start->id] = 1;
    int steps = 0;
    while (!queueIsEmpty(&q)) {
        int levelSize = 0;
        /* count current level */
        QNode *tmp = q.front;
        while (tmp) { levelSize++; tmp = tmp->next; }
        while (levelSize--) {
            MazeNode *cur; dequeue(&q, &cur);
            if (cur == goalNode) {
                queueDestroy(&q);
                return steps;
            }
            MazeNode *nbrs[4] = {cur->north, cur->south, cur->east, cur->west};
            for (int i=0;i<4;i++) if (nbrs[i] && !visited[nbrs[i]->id]) {
                visited[nbrs[i]->id] = 1;
                enqueue(&q, &nbrs[i]);
            }
        }
        steps++;
    }
    queueDestroy(&q);
    return -1; /* unreachable */
}

void playGame() {
    if (!startNode || !goalNode) {
        printf("Maze belum siap!\n");
        return;
    }
    listInit(&inventory, sizeof(Item));
    Stack path; stackInit(&path, sizeof(MazeNode *));
    MazeNode *curr = startNode;
    stackPush(&path, &curr);

    char cmd[16];
    while (1) {
        printf("\n[Posisi: %d]%s > ", curr->id, curr==goalNode?" [GOAL]":"");
        scanf("%15s", cmd);
        if (strcmp(cmd,"N")==0 || strcmp(cmd,"S")==0 || strcmp(cmd,"E")==0 || strcmp(cmd,"W")==0) {
            MazeNode *next = NULL;
            if (cmd[0]=='N') next = curr->north;
            else if (cmd[0]=='S') next = curr->south;
            else if (cmd[0]=='E') next = curr->east;
            else if (cmd[0]=='W') next = curr->west;
            if (!next) {
                printf("Tidak ada jalur %c!\n", cmd[0]);
            } else {
                curr = next;
                stackPush(&path, &curr);
                if (curr==goalNode) {
                    printf("\n🎉 Selamat! Anda mencapai tujuan. Langkah: %d\n", (int)(stackIsEmpty(&path)?0:1));
                    break;
                }
            }
        } else if (strcmp(cmd,"undo")==0) {
            MazeNode *dummy;
            /* pop posisi sekarang */
            stackPop(&path, &dummy);
            if (stackPop(&path,&curr)) {
                stackPush(&path,&curr);
            } else {
                curr = startNode; stackPush(&path,&curr);
            }
        } else if (strcmp(cmd,"hint")==0) {
            int dist = bfsHint(curr);
            if (dist>=0) printf("Hint: jarak terpendek %d langkah.\n", dist);
            else printf("Goal tidak dapat dicapai dari sini!\n");
        } else if (strcmp(cmd,"inv")==0) {
            showInventory();
        } else if (strcmp(cmd,"take")==0) {
            Item it; snprintf(it.name, NAME_LEN, "Batu#%d", curr->id);
            listInsertFront(&inventory, &it);
            printf("Anda mengambil %s\n", it.name);
        } else if (strcmp(cmd,"quit")==0) {
            break;
        } else {
            printf("Perintah tidak dikenal! [N|S|E|W|undo|hint|inv|take|quit]\n");
        }
    }
    /* cleanup */
    stackDestroy(&path);
    listDestroy(&inventory);
}


