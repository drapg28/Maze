/* ============================= main.c ============================== */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "maze.h"

static void builderMenu() {
    int choice;
    while (1) {
        printf("\n=== MAZE BUILDER ===\n");
        printf("1. Tambah node\n2. Hubungkan node\n3. Set start\n4. Set goal\n5. Lihat maze\n6. Selesai\n> ");
        if (scanf("%d", &choice)!=1) {
            while (getchar()!='\n');
            continue;
        }
        if (choice==1) {
            int id; printf("ID node: "); scanf("%d",&id);
            createNode(id);
        } else if (choice==2) {
            int from, to; char dir;
            printf("Dari node: "); scanf("%d", &from);
            printf("Ke node : "); scanf("%d", &to);
            printf("Arah [N/S/E/W]: "); scanf(" %c", &dir); dir = toupper(dir);
            connectNodes(from, to, dir);
        } else if (choice==3) {
            int id; printf("ID node start: "); scanf("%d", &id);
            startNode = findNode(id);
            if (startNode) printf("Start diset ke %d\n", id);
            else printf("Node tidak ditemukan!\n");
        } else if (choice==4) {
            int id; printf("ID node goal: "); scanf("%d", &id);
            goalNode = findNode(id);
            if (goalNode) { goalNode->isGoal=true; printf("Goal diset ke %d\n", id);} else printf("Node tidak ditemukan!\n");
        } else if (choice==5) {
            printMaze();
        } else if (choice==6) {
            if (!startNode||!goalNode) printf("Start/Goal belum diset!\n");
            else break;
        }
    }
}

int main() {
    builderMenu();
    playGame();
    destroyMaze();
    return 0;
}