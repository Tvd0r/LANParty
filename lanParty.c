#include "lanParty.h"

void removeTrailingcharactersfrombuffer(char *str) 
{
    int i = strlen(str) - 1;
    while (i >= 0 && (isspace(str[i]) || str[i] == '\n')) 
    {
        str[i] = '\0'; 
        i--;
    }
}

void removespacesfrombuffer(char s[])
{
    int i, j = 0;
    int len = strlen(s);
    while (isspace(s[j])) {
        j++;
    }
    for (i = 0; i < len - j; i++) {
        s[i] = s[i + j];
    }
    s[i] = '\0';
}

float calcultatetotalpoints(Team* aux)
{
    float s = 0;
    for (int i = 0; i < aux->number_of_players; i++)
    {
        s += aux->players[i].points;
    }
    float m = (float)s / aux->number_of_players;
    return m;
}

void processteam(Team* echipa, FILE* input)
{
    int numar_jucatori;
    char buffer[100];
    fscanf(input, "%d", &numar_jucatori);
    echipa->number_of_players = numar_jucatori;

    fgets(buffer, sizeof(buffer), input);
    //buffer[strcspn(buffer, "\n")] = '\0';
    removeTrailingcharactersfrombuffer(buffer);
    removespacesfrombuffer(buffer);
    echipa->team_name = strdup(buffer);

    echipa->players = (Player*)malloc(numar_jucatori * sizeof(Player));
    if(echipa->players==NULL)
    {
        fprintf(stderr, "Memory error.\n");
        exit(0);
    }
    for (int i = 0; i < numar_jucatori; i++)
    {
        Player player;
        int puncte;
        fscanf(input, "%s", buffer);
        buffer[strcspn(buffer, "\n")] = '\0';
        removespacesfrombuffer(buffer);
        player.firstname = strdup(buffer);

        fscanf(input, "%s", buffer);
        buffer[strcspn(buffer, "\n")] = '\0';
        removespacesfrombuffer(buffer);
        player.secondname = strdup(buffer);

        fscanf(input, "%d", &puncte);

        player.points = puncte;

        echipa->players[i] = player;
    }
    echipa->team_points = calcultatetotalpoints(echipa);
}

Node* createnode(Team team)
{
    Node* newnode = (Node*)malloc(sizeof(Node));
    if(newnode==NULL)
    {
        fprintf(stderr, "Memory error.\n");
        return NULL;
    }
    newnode->data = team;
    newnode->urm = NULL;
    return newnode;

}

List* createlist()
{
    List* l = (List*)malloc(sizeof(List));
    if(l==NULL)
    {
        fprintf(stderr, "Memory error.\n");
        return NULL;
    }
    l->head = NULL;
    return l;
}

int islistempty(List* l)
{
    return (l->head == NULL);
}

void addtolist(List* l, Team team)
{
    Node* newnode = createnode(team);
    if (islistempty(l))
        l->head = newnode;
    else
    {
        newnode->urm = l->head;
        l->head = newnode;
    }
}

void printlistinoutput(FILE* output, List* l)
{
    Node* aux = l->head;
    while (aux != NULL)
    {
        fprintf(output, "%s\n", aux->data.team_name);
        aux = aux->urm;
    }
}


Node* minlist(List* list)
{
    Node* minimum = NULL;
    float total_pointsmin = 100000000.00;
    Node* aux = list->head;
    while (aux != NULL)
    {
        if (aux->data.team_points < total_pointsmin)
        {
            minimum = aux;
            total_pointsmin = aux->data.team_points;
        }
        aux = aux->urm;
    }
    return minimum;
}

void removeminfromlist(List* list)
{
    Node* minimum = minlist(list);
    if (minimum == list->head)
    {
        Node* aux = list->head;
        list->head = list->head->urm;
        free(aux->data.team_name);
        free(aux->data.players);
        free(aux);
    }
    else
    {
        Node* current = list->head->urm;
        Node* previous = list->head;

        while (current != NULL)
        {
            if (current == minimum)
            {
                previous->urm = current->urm;
                free(current->data.team_name);
                free(current->data.players);
                free(current);
                break;
            }
            previous = current;
            current = current->urm;
        }
    }
}

Matchqueue* createqueue()
{
    Matchqueue* queue=(Matchqueue*)malloc(sizeof(Matchqueue));
    if(queue==NULL)
    {
        fprintf(stderr, "Memory error.\n");
        return NULL;
    }
    queue->head = queue->tail = NULL;
    return queue;
}

Queuenode* createqueuenode(Node* team1info, Node* team2info)
{
    Queuenode* newnode = (Queuenode*)malloc(sizeof(Queuenode));
    if(newnode==NULL)
    {
        fprintf(stderr, "Memory error.\n");
        return NULL;
    }

    newnode->urm = NULL;
    newnode->prev = NULL;

    newnode->team1.number_of_players = team1info->data.number_of_players;
    newnode->team1.team_points = team1info->data.team_points;
    newnode->team1.team_name = strdup(team1info->data.team_name);
    newnode->team1.team_name[strcspn(newnode->team1.team_name, "\n")] = '\0';
    newnode->team1.players = NULL;//nu mai sunt relevanti problemei


    newnode->team2.number_of_players = team2info->data.number_of_players;
    newnode->team2.team_points = team2info->data.team_points;
    newnode->team2.team_name = strdup(team2info->data.team_name);
    newnode->team2.team_name[strcspn(newnode->team2.team_name, "\n")] = '\0';
    newnode->team2.players = NULL;//nu mai sunt relevanti problemei

    return newnode;

}

void processqueuefromlist(Matchqueue* queue, List* list)
{
    Node* current = list->head;
    while (current != NULL && current->urm != NULL) {
        Queuenode* newnode = createqueuenode(current, current->urm);
        if (queue->head == NULL) 
        {
            queue->head = queue->tail = newnode;
        }
        else 
        {
            queue->tail->urm = newnode;
            newnode->prev = queue->tail;
            queue->tail = newnode;
        }
        current = current->urm->urm;
    }
}

void freeList(List* list)
{
    if (list == NULL) 
    {
        return;
    }

    Node* current = list->head;
    while (current != NULL) 
    {
        Node* temp = current;
        current = current->urm;
        free(temp->data.team_name);
        free(temp->data.players);
        free(temp);
    }

    free(list);
}

Stack* createstack() 
{
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    if(stack==NULL)
    {
        fprintf(stderr, "Memory error.\n");
        return NULL;
    }
    stack->head = NULL;
    return stack;
}

void pushstack(Stack* stack, Team team) 
{
    Stacknode* newnode = (Stacknode*)malloc(sizeof(Stacknode));
    if(newnode==NULL)
    {
        fprintf(stderr, "Memory error.\n");
        exit(0);
    }
    newnode->team.team_name = strdup(team.team_name);
    newnode->team.players = NULL;
    newnode->team.number_of_players = team.number_of_players;
    newnode->team.team_points = team.team_points+1;

    newnode->urm = stack->head;
    stack->head = newnode;
}

void winnersandlosers(Matchqueue* input, Stack* winners, Stack* losers) 
{
    Queuenode* temp = input->head;
    while (temp != NULL) 
    {
        if (temp->team1.team_points >temp->team2.team_points) 
        {
            pushstack(winners, temp->team1);
            pushstack(losers, temp->team2);
        }
        else if (temp->team1.team_points < temp->team2.team_points) 
        {
            pushstack(losers, temp->team1);
            pushstack(winners, temp->team2);
        }
        else if(temp->team1.team_points == temp->team2.team_points)
        {
            pushstack(losers, temp->team1);
            pushstack(winners, temp->team2);
        }
        temp = temp->urm;
    }
}

Queuenode* createqueuenodefromstack(Stacknode* team1info, Stacknode* team2info)
{
    Queuenode* newnode = (Queuenode*)malloc(sizeof(Queuenode));
    if(newnode==NULL)
    {
        fprintf(stderr, "Memory error.\n");
        return NULL;
    }

    newnode->urm = NULL;
    newnode->prev = NULL;

    newnode->team1.number_of_players = team1info->team.number_of_players;
    newnode->team1.team_points = team1info->team.team_points;
    newnode->team1.team_name = strdup(team1info->team.team_name);
    newnode->team1.team_name[strcspn(newnode->team1.team_name, "\n")] = '\0';
    newnode->team1.players = NULL;


    newnode->team2.number_of_players = team2info->team.number_of_players;
    newnode->team2.team_points = team2info->team.team_points;
    newnode->team2.team_name = strdup(team2info->team.team_name);
    newnode->team2.team_name[strcspn(newnode->team2.team_name, "\n")] = '\0';
    newnode->team2.players = NULL;

    return newnode;

}

void createnewqueuefromstack(Matchqueue* queue, Stack* stack)
{
    Stacknode* current = stack->head;
    while (current != NULL && current->urm != NULL) {
        Queuenode* newnode = createqueuenodefromstack(current, current->urm);
        if (queue->head == NULL) {
            queue->head = queue->tail = newnode;
        }
        else {
            queue->tail->urm = newnode;
            newnode->prev = queue->tail;
            queue->tail = newnode;
        }
        current = current->urm->urm;
    }
}

void freeStack(Stack* stack) 
{
    Stacknode* current = stack->head;
    while (current != NULL) 
    {
        Stacknode* temp = current;
        current = current->urm;
        free(temp->team.team_name);
        free(temp->team.players);
        free(temp);
    }
    free(stack);
}

void freeQueue(Matchqueue* queue) 
{
    Queuenode* current = queue->head;
    while (current != NULL) {
        Queuenode* temp = current;
        current = current->urm;
        free(temp->team1.team_name);
        free(temp->team1.players);
        free(temp->team2.team_name);
        free(temp->team2.players);
        free(temp);
    }
    free(queue);
}

void printstackinoutput(FILE* output, Stack *stack)
{
    Stacknode *temp=stack->head;
    while(temp!=NULL)
    {
        fprintf(output,"%s",temp->team.team_name);
        for(int i=1; i<=34-strlen(temp->team.team_name); i++)
            fprintf(output," ");
        fprintf(output,"-  %.2f",temp->team.team_points);
        fprintf(output,"\n");
        temp=temp->urm;
    }
}

void printqueueinoutput(FILE* output, Matchqueue* queue)
{
    Queuenode* aux = queue->head;
    while (aux != NULL)
    {
        fprintf(output,"%s",aux->team1.team_name);

        for (int i = 1; i <= 33 - strlen(aux->team1.team_name); i++)
            fprintf(output, " ");

        fprintf(output, "-");

        for (int i = 1; i <= 33 - strlen(aux->team2.team_name); i++)
            fprintf(output, " ");

        fprintf(output,"%s",aux->team2.team_name);
            
        fprintf(output, "\n");
        aux = aux->urm;
    }
}



BST *createBST()
{
    BST *tree=(BST*)malloc(sizeof(BST));
    if(tree==NULL)
    {
        fprintf(stderr, "Memory error.\n");
        return NULL;
    }
    tree->root=NULL;
    return tree;
}

BSTnode* createbstnode(Team team) 
{
    BSTnode *newnode = (BSTnode*)malloc(sizeof(BSTnode));
    if(newnode==NULL)
    {
        fprintf(stderr, "Memory error.\n");
        return NULL;
    }
    if (newnode == NULL) 
    {
        fprintf(stderr, "Eroare la alocarea memoriei pentru un nod BST.\n");
        exit(EXIT_FAILURE); 
    }
    
    newnode->team.team_name = strdup(team.team_name);
    newnode->team.team_points = team.team_points;
    newnode->team.players = NULL; 
    newnode->team.number_of_players = team.number_of_players;
    
    newnode->left = newnode->right = NULL;

    return newnode;
}

void insertBSTnode(BSTnode **root, BSTnode *newnode) 
{
    if (*root == NULL) 
    {
        *root = newnode;
    } 
    else 
    {
        if (newnode->team.team_points > (*root)->team.team_points) 
        {
            insertBSTnode(&((*root)->left), newnode);
        } 
        else if (newnode->team.team_points < (*root)->team.team_points) 
        {
            insertBSTnode(&((*root)->right), newnode);
        } 
        else 
        {
            if (strcmp(newnode->team.team_name, (*root)->team.team_name) > 0) 
            {
                insertBSTnode(&((*root)->left), newnode);
            } else 
            {
                insertBSTnode(&((*root)->right), newnode);
            }
        }
    }
}

void processBST(BST *tree, Matchqueue *queue)
{
    Queuenode *temp = queue->head;
    while (temp != NULL)
    {
        BSTnode *newnode1 = createbstnode(temp->team1);
        BSTnode *newnode2 = createbstnode(temp->team2);
        insertBSTnode(&(tree->root), newnode1);
        insertBSTnode(&(tree->root), newnode2);
        temp = temp->urm;
    }
}

void printBSTinoutput(FILE* output, BSTnode* root)
{
    if (root != NULL) 
    {
        printBSTinoutput(output, root->left);
        fprintf(output, "%s", root->team.team_name);
        for(int i=1; i<=34-strlen(root->team.team_name); i++)
            fprintf(output, " ");
        fprintf(output, "-  %.2f\n",root->team.team_points);
        printBSTinoutput(output, root->right);
    }
}

void freeBSTNode(BSTnode* node) 
{
    if (node == NULL) 
    {
        return;
    }
    free(node->team.team_name);
    freeBSTNode(node->left);
    freeBSTNode(node->right);
    free(node);
}

void freeBST(BST* tree) 
{
    if (tree == NULL) 
    {
        return;
    }
    freeBSTNode(tree->root);
    free(tree);
}


Queuenode* createqueuenodefromBST(BSTnode* root) 
{
    Queuenode* newnode = (Queuenode*)malloc(sizeof(Queuenode));

    if(newnode==NULL)
    {
        fprintf(stderr, "Memory error.\n");
        return NULL;
    }

    newnode->urm = NULL;
    newnode->prev = NULL;

    newnode->team1.number_of_players = root->team.number_of_players;
    newnode->team1.team_points = root->team.team_points;
    newnode->team1.team_name = strdup(root->team.team_name);
    newnode->team1.team_name[strcspn(newnode->team1.team_name, "\n")] = '\0';
    newnode->team1.players = NULL;

    newnode->team2.number_of_players = 0;
    newnode->team2.team_points = 0.0;
    newnode->team2.team_name = NULL;
    newnode->team2.players = NULL;

    return newnode;
}

void inordertoqueue(BSTnode* root, Matchqueue* queue) 
{
    if (root != NULL) 
    {
        inordertoqueue(root->left, queue);
        Queuenode* newnode = createqueuenodefromBST(root);
        if (queue->head == NULL) 
        {
            queue->head = queue->tail = newnode;
        } else 
        {
            queue->tail->urm = newnode;
            newnode->prev = queue->tail;
            queue->tail = newnode;
        }
        inordertoqueue(root->right, queue);
    }
}

int max(int a, int b) 
{
    return (a > b) ? a : b;
}

AVLtree  *createAVLtree()
{
    AVLtree *tree=(AVLtree*)malloc(sizeof(AVLtree));
    if(tree==NULL)
    {
        fprintf(stderr, "Memory error.\n");
        return NULL;
    }
    tree->root=NULL;
    return tree;
}

int getHeight(AVLnode *node) 
{
    if (node == NULL)
        return 0;
    return node->height;
}


AVLnode* leftRotate(AVLnode* z) 
{
    AVLnode *y = z->right;
    AVLnode *T2 = y->left;
    
    y->left = z;
    z->right = T2;
    
    z->height = max(getHeight(z->left), getHeight(z->right))+1;
    y->height = max(getHeight(y->left), getHeight(y->right))+1;
    
    return y;
}


AVLnode* rightRotate(AVLnode* z) 
{
    AVLnode *y = z->left;
    AVLnode *T3 = y->right;
   
    y->right = z;
    z->left = T3;
    
    z->height = max(getHeight(z->left), getHeight(z->right))+1;
    y->height = max(getHeight(y->left), getHeight(y->right))+1;
    
    return y;
}


AVLnode* createAVLnode(Team team) 
{
    AVLnode *newnode = (AVLnode*)malloc(sizeof(AVLnode));
    if(newnode==NULL)
    {
        fprintf(stderr, "Memory error.\n");
        return NULL;
    }
    newnode->team.team_name = strdup(team.team_name);
    newnode->team.team_points = team.team_points;
    newnode->team.players = NULL; 
    newnode->team.number_of_players = team.number_of_players;
    newnode->left = newnode->right = NULL;
    return newnode;
}

void insertAVLnode(AVLnode **root, AVLnode *newnode){
    if (*root == NULL){
        *root = newnode;
    } 
    else{
        if (newnode->team.team_points < (*root)->team.team_points){
            insertAVLnode(&((*root)->left), newnode);
        } 
        else if (newnode->team.team_points > (*root)->team.team_points){
            insertAVLnode(&((*root)->right), newnode);
        } 
        else{
            if (strcmp(newnode->team.team_name, (*root)->team.team_name) < 0)
                insertAVLnode(&((*root)->left), newnode);
            else if (strcmp(newnode->team.team_name, (*root)->team.team_name) > 0)
                insertAVLnode(&((*root)->right), newnode);
        }
    }
    (*root)->height = 1 + max(getHeight((*root)->left), getHeight((*root)->right));
    int balance= (getHeight((*root)->left) - getHeight((*root)->right));
    if (balance > 1){
        if (newnode->team.team_points < (*root)->left->team.team_points)
            *root = rightRotate(*root);
        else if (newnode->team.team_points > (*root)->left->team.team_points){
            (*root)->left = leftRotate((*root)->left);
            *root = rightRotate(*root);
        } else{
            *root = rightRotate(*root);
        }
    }
    if (balance < -1){
        if (newnode->team.team_points > (*root)->right->team.team_points)
            *root = leftRotate(*root);
        else if (newnode->team.team_points < (*root)->right->team.team_points){
            (*root)->right = rightRotate((*root)->right);
            *root = leftRotate(*root);
        } else{
            *root = leftRotate(*root);
        }
    }
}

void processAVLtreefromsortedlist(AVLtree *tree, Matchqueue *queue) 
{
    Queuenode *temp = queue->head;
    while (temp != NULL) {
        AVLnode *newnode1 = createAVLnode(temp->team1);
        insertAVLnode(&(tree->root), newnode1);
        temp = temp->urm;
    }
}

void printlevel2nodesrecursive(AVLnode* root, int currentLevel, FILE* output) 
{
    if (root == NULL) 
    {
        return;
    }
    if (currentLevel == 2) 
    {
        fprintf(output, "%s\n", root->team.team_name);
    } 
    else if (currentLevel > 2) 
    {
        printlevel2nodesrecursive(root->right, currentLevel - 1, output);
        printlevel2nodesrecursive(root->left, currentLevel - 1, output);
    }
}

void freeAVLNode(AVLnode* node) 
{
    if (node == NULL) 
    {
        return;
    }
    free(node->team.team_name);
    freeAVLNode(node->left);
    freeAVLNode(node->right);
    free(node);
}

void freeAVL(AVLtree* tree) 
{
    if (tree == NULL)
    {
        return;
    }
    freeAVLNode(tree->root);
    free(tree);
}
