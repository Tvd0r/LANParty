#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include<limits.h>


//declarari de structuri
typedef struct
{
    char* firstname;
    char* secondname;
    int points;
} Player;

typedef struct
{
    char* team_name;
    Player* players;
    int number_of_players;
    float team_points;
}Team;

struct elem
{
    Team data;
    struct elem* urm;
};
typedef struct elem Node;

typedef struct
{
    Node* head;
}List;


struct Queuematch
{
    struct Queuematch *urm,*prev;
    Team team1,team2;
};
typedef struct Queuematch Queuenode;

typedef struct
{
    Queuenode *head,*tail;
}Matchqueue;

struct stivanode
{
    Team team;
    struct stivanode *urm;
};
typedef struct stivanode Stacknode;

typedef struct 
{
    Stacknode *head;
}Stack;

struct bstnode
{
    Team team;
    struct bstnode *left,*right;
};

typedef struct bstnode BSTnode;

typedef struct 
{
    BSTnode *root;
}BST;


struct aVLnode
{
    Team team;
    struct aVLnode *left;
    struct aVLnode *right;
    int height;
};
typedef struct aVLnode AVLnode;

typedef struct
{
    AVLnode *root;
}AVLtree;

//functi Cerinta 1
List* createlist();
void removespacesfrombuffer(char str[]);
float calcultatetotalpoints(Team* aux);
void processteam(Team* echipa, FILE* input);
void addtolist(List* l, Team team);
void printlistinoutput(FILE* output, List* l);


//functii Cerinta 2
Node* createnode(Team team);
int islistempty(List* l);
Node* minlist(List* list);
void removeminfromlist(List* list);

//functii cerinta 3
void freeList(List* list);
Matchqueue* createqueue();
Queuenode* createqueuenode(Node* team1info, Node* team2info);
void processqueuefromlist(Matchqueue* queue, List* list);
void printqueueinoutput(FILE* output, Matchqueue* queue);
Stack *createstack();
void pushstack(Stack* stack, Team team);
void winnersandlosers(Matchqueue* input, Stack* winners, Stack* losers);
Queuenode* createqueuenodefromstack(Stacknode* team1info, Stacknode* team2info);
void createnewqueuefromstack(Matchqueue* queue, Stack* stack);
void freeQueue(Matchqueue* queue);
void freeStack(Stack* stack);
void printstackinoutput(FILE* output, Stack *stack);

//functii cerinta 4
BST *createBST();
BSTnode* createbstnode(Team team);
void insertBSTnode(BSTnode **root, BSTnode *newnode);
void processBST(BST *tree, Matchqueue *queue);
void printBSTinoutput(FILE* output, BSTnode* root);
Queuenode* createqueuenodefromBST(BSTnode* root);
void inordertoqueue(BSTnode* root, Matchqueue* queue);
void freeBSTNode(BSTnode* node); 
void freeBST(BST* tree);


//functii cerinta 5
int max(int a, int b);
AVLtree  *createAVLtree();
AVLnode *rightRotate(AVLnode *y);
AVLnode *leftRotate(AVLnode *x);
AVLnode* createAVLnode(Team team);
void insertAVLnode(AVLnode **root, AVLnode *newnode);
void processAVLtreefromsortedlist(AVLtree *tree, Matchqueue *queue);
void printlevel2nodesrecursive(AVLnode* root, int currentLevel, FILE* output);
void freeAVLNode(AVLnode* node); 
void freeAVL(AVLtree* tree);


