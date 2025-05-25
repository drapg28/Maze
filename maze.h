/* ============================= maze.h ============================== */
#ifndef MAZE_H
#define MAZE_H
#include <stdbool.h>
#include "list.h"
#include "stack.h"
#include "queue.h"

#define NAME_LEN 32

/* ---- Maze Node ---- */
typedef struct MazeNode {
    int id;
    bool isGoal;
    struct MazeNode *north, *south, *east, *west;
} MazeNode;

/* List node that stores all maze nodes (for builder) */
typedef struct {
    MazeNode *ptr;
} NodeRef;

/* ---- Inventory Item ---- */
typedef struct {
    char name[NAME_LEN];
} Item;

/* ===== Builder API ===== */
MazeNode *createNode(int id);
MazeNode *findNode(int id);
void connectNodes(int fromID, int toID, char dir);
void printMaze(); /* simple textual list of connections */
void destroyMaze();

/* ===== Game API ===== */
extern MazeNode *startNode; /* ditetapkan builder */
extern MazeNode *goalNode;  /* ditetapkan builder */

void playGame();

#endif /* MAZE_H */