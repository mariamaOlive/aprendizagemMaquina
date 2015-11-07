#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


//Task1
void task1(int data[958][10]){

int nObjects=958;
int dMatrix[958][958]; 
int nAtributes=9;

	//Dimilarity Matrix
	for (int i = 0; i < 958; i++)
	{	

		for (int j = i; j < 958; j++)
		{
			int cont=0;

			for (int k = 0; k < nAtributes; k++)
			{
				if(data[i][k]!=data[j][k]){
					cont++;
				}
			}

			dMatrix[i][j]=cont;
			dMatrix[j][i]=cont;
		}
	}

	//Clustering Algorithm
	//Parameters
	int k=2; //number of clusters
	int m=2; //
	int T=150; //an iteration limit
	float e= pow(10, -10);
	int q=2;
	int t=0; //counts the number of iterations

	int proto[k][q];

	srand(time(NULL));
	//Selecting prototypes
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < q; j++)
		{
			int selected = rand()%(nObjects-1);
			proto[i][j]=selected;
			printf("selected: %i\n", proto[i][j]);
		}
		
	}

	float memberDregree[nObjects][k];

	for (int i = 0; i < nObjects; i++)
	{
		for (int j = 0; j < k; j++)
		{
			float sum0=0;
			for (int p = 0;  p< k; p++)
			{
					float sum1=0;
					float sum2=0;
					float sum3=0;

					for (int r = 0; r< q; r++)
					{
						//if(i!=proto[p][r]){
							sum2+=dMatrix[i][proto[j][r]];
							sum3+=dMatrix[i][proto[p][r]];
						//}else{
						//	sum2=1;
						//	sum3=1;
						//}
					}

					sum1=sum2/sum3;

					sum1=pow(sum1, (1/(m-1)));

					sum0+=sum1;
			}

			memberDregree[i][j]=(1/sum0);
		}
		
		if(i<10){
		printf("%i -> u1= %f ",i, memberDregree[i][0]);
		printf("u2= %f\n", memberDregree[i][1]);
		}
	}

	//Calculating J
	float J=0;
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < nObjects; j++)
		{	
			float sumQ=0;
			for (int n = 0; n < q; n++)
			{
				sumQ+=dMatrix[j][proto[i][n]];
			}

			J+= pow(memberDregree[j][i],m)*sumQ;
		}
	}
	printf("J= %f\n", J);

	//Cleaning prototype
	// for (int i = 0; i < k; i++)
	// {
	// 	for (int j = 0; j < q; j++)
	// 	{
	// 		proto[k][q]=-1;
	// 	}
	// }
	
	for (; (t < 150); t++) //Updating t
	{
	
		//Choosing again the prototypes
		 for (int r = 0; r < k; r++)
		 {
			float allPrototypes [nObjects][2];
				
				for (int j = 0; j < nObjects; j++) //=> h
				{
					float arg=0;
					for (int s = 0; s < nObjects; s++) //=> i
					{
						arg+= pow(memberDregree[s][r],m)*dMatrix[s][j];
					}

					allPrototypes[j][0]=arg;
					allPrototypes[j][1]=j;
				}

				qsort(allPrototypes, nObjects, 2*(sizeof(float)),cmpfunc);


				 // for (int i = 0; i <5; ++i)
				 // {
					// printf("%f %i\n",allPrototypes[i][0], (int) allPrototypes[i][1]);
				 // }

				for (int i = 0; i < q; i++)
				{
					proto[r][i]=(int)allPrototypes[i][1];
					printf("selected: %i\n",proto[r][i]);
				}
		}
	
		for (int i = 0; i < nObjects; i++)
		{
			for (int j = 0; j < k; j++)
			{
				float sum0=0;
				for (int p = 0;  p< k; p++)
				{
					// printf("%i,%i,%i\n",i,j,p);
					// printf("i:%i, p:%i\n",i, proto[p] );
					// printf("dis=>%i\n",dMatrix[i][proto[p]]);

						float sum1=0;
						float sum2=0;
						float sum3=0;

						for (int r = 0; r< q; r++)
						{
							//if(i!=proto[p][r]){
								sum2+=dMatrix[i][proto[j][r]];
								sum3+=dMatrix[i][proto[p][r]];
							// }else{
							// 	sum2=1;
							// 	sum3=1;
							// }
						}

						sum1=sum2/sum3;

						sum1=pow(sum1, (1/(m-1)));

						sum0+=sum1;
				}

				memberDregree[i][j]=(1/sum0);
			}


			
			 if(i<10){
			 	printf("%i -> u1= %f ",i, memberDregree[i][0]);
			 	printf("u2= %f\n", memberDregree[i][1]);
			 }
		}


		//Calculating J
		float J1=0;
		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < nObjects; j++)
			{	
				float sumQ=0;
				for (int n = 0; n < q; n++)
				{
					sumQ+=dMatrix[j][proto[i][n]];
				}

				J1+= pow(memberDregree[j][i],m)*sumQ;
			}
		}

		printf("e= %f\n", fabs(J1-J));
		printf("J= %f\n", J1);

		//Comparing J(t) with J(t-1)
		if(fabs(J1-J)<=e){
			break;
		}

		J=J1;
		
	}
	
	int class[nObjects];
	int numberErrors=0;

	for (int i = 0; i < nObjects; i++)
	{
	//printf("%i -> u1= %f ",i, memberDregree[i][0]);
	//printf("u2= %f\n", memberDregree[i][1]);

		if(memberDregree[i][0]>memberDregree[i][1]){
			class[i]=1;
		}else{
			class[i]=2;
		}
	}

	for (int i = 0; i < nObjects; i++)
	{
		if(class[i]!=data[i][9]){
			numberErrors++;
		}
	}
	
	printf("Porcentagem de Erro= %f\n", ((float)numberErrors/(float)nObjects));
}


//Task2
//change the return of the task after the function is complete
void task2(int data[958][10]){

	int nSample=958;
	float w1 [1][9][3];
	float w2 [1][9][3];

	//Calculating conditional probability p,q,r

	int nW1=626;
	int nW2=332;
	int nAtributes=9;

	//calculating the probability of class1
	for (int j = 0;  j< nAtributes; ++j)
	{
		//Calculating parameters p,q,r
		float sumP=0;
		float sumQ=0;
		float sumR=0;

		for (int p = 0; p <nW1 ; ++p)
		{
			sumP+= data[p][j]*(data[p][j]+1)/2;
		}

		for (int p = 0; p <nW1 ; ++p)
		{
			sumQ+= (1-((data[p][j])*(data[p][j])));
		}

		for (int p = 0; p <nW1 ; ++p)
		{
			sumR+= data[p][j]*(data[p][j]-1)/2;
		}


		/*printf("SumP= %f\n", sumP);
		printf("SumQ= %f\n", sumQ);
		printf("SumR= %f\n", sumR);*/

		w1[0][j][0]=(1.0/626.0)*sumP;

		w1[0][j][1]=(1.0/626.0)*sumQ;

		w1[0][j][2]=(1.0/626.0)*sumR;

	}


	//calculating the probability of c

	for (int j = 0;  j< nAtributes; ++j)
	{
		//Calculating parameters p,q,r
		float sumP=0;
		float sumQ=0;
		float sumR=0;

		for (int p = 0; p <nW2 ; ++p)
		{
			sumP+= data[p+nW1][j]*(data[p+nW1][j]+1)/2;
		}

		for (int p = 0; p <nW2 ; ++p)
		{
			sumQ+= (1-((data[p+nW1][j])*(data[p+nW1][j])));
		}

		for (int p = 0; p <nW2 ; ++p)
		{
			sumR+= data[p+nW1][j]*(data[p+nW1][j]-1)/2;
		}

		w2[0][j][0]=(1.0/332.0)*sumP;

		w2[0][j][1]=(1.0/332.0)*sumQ;

		w2[0][j][2]=(1.0/332.0)*sumR;

	}


//Calculating conditional probability
float cProbaility[958][2];
float a=0;
float b=0;

for (int i = 0; i < nSample; ++i)
{
	float cW1=1;

	//Case: P(X|w1)
	for (int j = 0; j < nAtributes; ++j)
	{
		cW1=cW1*(pow(w1[0][j][0],(data[i][j]*(data[i][j]+1)/2))*
			pow(w1[0][j][1],(1-((data[i][j])*(data[i][j]))))*
			pow(w1[0][j][2],(data[i][j]*(data[i][j]-1)/2)));


	}
	cProbaility[i][0]=cW1;
	


	float cW2=1;
	//Case: P(X|w2)
	for (int j = 0; j < nAtributes; ++j)
	{
		cW2=cW2*(pow(w2[0][j][0],(data[i][j]*(data[i][j]+1)/2))*
			pow(w2[0][j][1],(1-((data[i][j])*(data[i][j]))))*
			pow(w2[0][j][2],(data[i][j]*(data[i][j]-1)/2)));

	}
	cProbaility[i][1]=cW2;

	printf("w2= %f\n",cProbaility[i][1]);

}

/* Probability a priori */
float pW[2] = {(626.0/958.0), (332.0/958.0)};

/*Calculating probabilities a posteriori */
float pProbability[2][958];

for (i=0; i<2; ++i)
{
    for (j=0; j< nSample; ++j)
    {
        pProbability[i][j] = (cProbaility[j][i] * pW[i]) / ( (cProbaility[j][0] * pW[0]) + (cProbaility[j][1] * pW[1]));
    }
}

/* Classification of x respect with the rule given */
int Clasificador [nSample];

for (i=0; i<nSample; ++i)
{
    printf("Probability of w1 having x%d = %f \n", i, pProbability[0][i]);
    printf("Probability of w2 having x%d = %f \n", i, pProbability[1][i]);

    if (pProbability[0][i] >= pProbability[1][i])
    {
            Clasificador[i]=1;
    }
    else
    {
            Clasificador[i]=2;
    }

    printf("x%d in class %d\n", i, Clasificador[i]);
}

/*for (int i = 0; i <1; ++i)
{
	for (int j = 0; j < nAtributes; ++j)
	{
		printf("p= %f ", w1[i][j][0]);
		printf("q= %f ", w1[i][j][1]);
		printf("r= %f\n", w1[i][j][2]);
	}

}*/




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
task1(data);


//Task 2
task2(data);




return 0;
}
