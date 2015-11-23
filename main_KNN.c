#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

//Task1
void task1(int data[958][10]){

int nObjects=958;
int dMatrix[958][958]; // 713 o max
int nAtributes=9;

	//Dimilarity Matrix
	int i;
	for (i = 0; i < nObjects; i++)
	{
        int j;
		for (j = i; j < nObjects; ++j)
		{
			int cont=0;
            int k;
			for (k = 0; k < nAtributes; k++)
			{
				if(data[i][k]!=data[j][k]){
					cont++;
				}
			}
			dMatrix[i][j]=cont;
			printf("Dissimilarity between i:%d and j:%d = %d\n",i, j, dMatrix[i][j]);
			dMatrix[j][i]=cont;
		}
		printf("\n");
	}

int v=10; //Numero de vizinhos
int p; //Xp where p in [1..n]
int ClasseKnn[nObjects]; //Tabela de resultados
int Errors=0; //Contagem de error entre classificação a priori e com a método do k-NN vizinhos

for (p=0; p<nObjects; p++)
{
    int res[v][4]; // Tabela que vai contender os v vinzinhos mais proximos de Xp
    printf("\n\n**** p = %d *** \n\n", p);

    //Initialização da tabela dos vizinhos para Xp.
    for (i=0; i<v;  i++) res[i][0]=-1;

    srand(time(NULL));
    for (i=0; i<v; i++)
    {
        // Search for the first value posible
        int min = rand()%(nObjects-1);
        int r;

        int ok1=1;
        int ok2=1;
            while ((ok1 != 0) && (ok2!=0))
            {
                if (p==min)
                {
                    if (min == nObjects-1)
                        min = 0;
                    else min++;
                    ok1=0;
                }
                else
                    ok1=0;

                for (r=0; r<v; r++)
                {
                    if (min==res[r][0])
                    {
                        if (min == nObjects-1)
                            min = 0;
                        else min++;
                        ok2=1;
                        ok1=1;
                    }
                }
            }

        // Searching for the max dissimilarity between Xp and the others x
        int usado[nObjects];
        int b=0;

        // Aleatory way to choose the X to be compared with Xp.
        while (b<nObjects)
        {
            int selected = rand()%(nObjects-1);
            int r;
            int ok3=1;

            //Verification that the chosen value was not already compared.
            while ((ok3!=0))
            {
                ok3=0;
                for (r=0; r<b; r++)
                {
                    if (selected==usado[r])
                    {
                        if (selected == nObjects-1)
                            selected= 0;
                        else selected++;
                        ok3=1;
                        break;
                    }
                }
            }
            usado[b]=selected;

            int verif=0;

            // Verificação que o elemento j não ja apartene a tabela de resultados
            for (r=0; r<v; r++)
                if (res[r][0]==selected)
                {
                    verif++;
                    break;
                }

            // Si o elemento não esta na tabela de resultados
            if (verif==0) {
                if (dMatrix[selected][p] <+ dMatrix[min][p] && selected != p)
                {
                    min=selected;
                }
            }
            b++;
        }

        res[i][0]=min; //Saving of the indice
        res[i][1]=data[min][9]; //Saving of the class of the indice

        printf("Min = %d | Dissimilarity = %d | vinzinhos mais proximo numero %d  | classe = %d\n", res[i][0],dMatrix[min][p], i, res[i][1]);
    }

    //Counting of results
    int count1=0, count2=0;

    for (i=0; i<v; i++)
        if (res[i][1]==1)
            count1++;
        else
            count2++;

    printf("count1=%d, count2=%d \n", count1, count2);

    if (count1>count2)
        ClasseKnn[p]=1;
    else ClasseKnn[p]=2;

    printf("p=%d na classe %d", p, ClasseKnn[p]);

    if (ClasseKnn[p]!=data[p][9])
        Errors++;
}

printf("Percentage of errors between classification K-NN and classification a priori = %f", Errors/nObjects);
}



int main()
{

//File variables
FILE *ifp;
char *mode="r";
char *fileName="tic-tac-toe.data";

//Data Structure that will receive the file info
int data [958][10];


//////////////opening and reading file////////////////////

ifp= fopen(fileName, mode);

//Verifing if the file was open correctly
if(ifp!=NULL){
	printf("File successfully opened!\n");

	//Reading each character of the opened file
	char ch;
	int i=0;
	int j=0;
	int insertedValue;

	while( ( ch = fgetc(ifp) ) != EOF ){

		if(ch=='x' || ch=='o' || ch=='b' || ch=='p' || ch=='n'){ //ignore any other character

			//load the file into the selected data structure
			if(ch=='x'){
				insertedValue= 1;

			}else if(ch=='o'){
				insertedValue= 0;

			}else if(ch=='b'){
				insertedValue= -1;

			}else if(ch=='p'){
				insertedValue= 1;
				fgetc(ifp);

			}else{
				insertedValue= 2;

			}

			data[i][j]=insertedValue;
			j++;

			//adjusting i and j
			if(j==10){
				j=0;
				i++;
			}

		}

	}

	fclose(ifp);

}else{
	printf("The file was not open!\n");
}
//////////////////////data structure uploaded//////////////////

//Tasks

//Task 1
printf("tache 1 a faire\n");
task1(data);

printf("bb\n");

printf("tache 1 est faite \n");


//Task 2
//task2(data);




return 0;
}
