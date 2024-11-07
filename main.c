#include "lanParty.h"

int main (int argc, char* argv[]) 
{
    
    printf("Number of input files: %d\n", argc);
    printf("Exec name: %s\n", argv[0]);
    printf("File1: %s\n", argv[1]);
    printf("File2: %s\n", argv[2]);
    printf("File3: %s\n", argv[3]);
    // FILE *f_in = fopen( argv[1], "r" );

    //===============================================
    FILE* f_in = fopen(argv[1],"rt");
    if(f_in==NULL)
    {
        fprintf(stderr,"File not found!");
        return 0;
    } 
    int nr_echipe, cerinta1, cerinta2, cerinta3, cerinta4, cerinta5;

    fscanf(f_in, "%d %d %d %d %d", &cerinta1, &cerinta2, &cerinta3, &cerinta4, &cerinta5);
    fclose(f_in);
    
    List* list = createlist();

    if (cerinta1 == 1)
    {
        f_in = fopen(argv[2], "rt");
        if(f_in==NULL)
        {
            fprintf(stderr,"File not found!");
            return 0;
        } 

        fscanf(f_in, "%d", &nr_echipe);
        for (int i = 0; i < nr_echipe; i++)
        {
            Team echipa;
            processteam(&echipa, f_in);
            addtolist(list, echipa);
        }
        fclose(f_in);

        f_in = fopen(argv[3], "wt");
        if(f_in==NULL)
        {
            fprintf(stderr,"File not found!");
            return 0;
        } 

        printlistinoutput(f_in, list);
        fclose(f_in);

        if (cerinta2 == 1)
        {
            f_in = fopen(argv[3], "wt");
            if(f_in==NULL)
            {
                fprintf(stderr,"File not found!");
                return 0;
            } 

            int nr_max_teams = 1;
            do
            {
                nr_max_teams *= 2;
            } while (nr_max_teams < nr_echipe);
            nr_max_teams /= 2;
            
            if(nr_max_teams*2==nr_echipe)
                nr_max_teams*=2;

            for (int i = 1; i < nr_echipe - nr_max_teams+1; i++)
            {
                removeminfromlist(list);
            }

            printlistinoutput(f_in, list);
            fclose(f_in);
            if(cerinta3 == 1)
            {
                Matchqueue* queue = createqueue(); 
                processqueuefromlist(queue, list);
                freeList(list);
                int k = 1;
                FILE* f_in = fopen(argv[3], "at");
                if(f_in==NULL)
                {
                    fprintf(stderr,"File not found!");
                    return 0;
                }

                BST *tree = createBST();
                AVLtree *avltree=createAVLtree();

                do {
                    if(nr_max_teams==8)
                    {
                        processBST(tree,queue);
                        Matchqueue *queuesorted=createqueue();
                        inordertoqueue(tree->root,queuesorted);
                        processAVLtreefromsortedlist(avltree,queuesorted);
                        freeQueue(queuesorted);
                    }
                    fprintf(f_in, "\n--- ROUND NO:%d\n", k);
                    printqueueinoutput(f_in, queue);
                    fprintf(f_in,"\n");
                    Stack* winners = createstack();
                    Stack* losers = createstack();
                    winnersandlosers(queue, winners, losers);
                    freeStack(losers);
                    freeQueue(queue);
                    fprintf(f_in, "WINNERS OF ROUND NO:%d\n", k);
                    printstackinoutput(f_in, winners);
                    queue = createqueue();
                    createnewqueuefromstack(queue, winners);
                    freeStack(winners);
                    k++;
                    nr_max_teams /= 2;
                } while (nr_max_teams!=1);
                fclose(f_in);

                freeQueue(queue);//safety measure

                if(cerinta4==1)
                {
                    f_in = fopen(argv[3], "at");
                    if(f_in==NULL)
                    {
                        fprintf(stderr,"File not found!");
                        return 0;
                    }

                    fprintf(f_in,"\n");
                    fprintf(f_in,"TOP 8 TEAMS:\n");
                    printBSTinoutput(f_in,tree->root);
                    freeBST(tree);
                    fclose(f_in);
                    if(cerinta5==1)
                    {
                        f_in = fopen(argv[3], "at");
                        if(f_in==NULL)
                        {
                            fprintf(stderr,"File not found!");
                            return 0;
                        }

                        fprintf(f_in,"\n");
                        fprintf(f_in,"THE LEVEL 2 TEAMS ARE: \n");
                        printlevel2nodesrecursive(avltree->root,4,f_in);
                        freeAVL(avltree);
                        fclose(f_in);
                    }
                }
            }
        }
    }
}