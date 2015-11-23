#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


//Auxiliary functions
long double fat(long double n){
	if(n==0){
		return 1;
	}

	return fat(n-1)*n;
}

long double comb(int n, int k){
	return fat((long double)n)/(fat((long double)k)*fat((long double)n-(long double)k));
}

//Comparte two elements
int cmpfunc (const void * a, const void * b)
{
   return ( *(float*)a - *(float*)b );
}


//Swap Elements in an array
void swap(int *a, int *b){
	int temp=*a;
	*a=*b;
	*b=temp;
}

//Radomize an array (Complexity=O(n) o/)
void randArray(int *array, int nElements){
	srand(time(NULL));

	for(int i=nElements-1; i>0; i--){
		int j= rand()%(i+1);
		swap(&array[i],&array[j]);
	}
}

//Confidence Interval
void confidenceInterval(float sampleSize, float mean, float variance){
	//Confidence interval of 95% ->Z(α/2)= 1.96
	//With Normal
	//e=Z(α/2)*(σ/sqrt n)

	float sigma = sqrt(variance);
	float e= 1.96 * (sigma/sqrt(sampleSize));

	float intDown=mean-e;
	float intUp=mean+e;

	printf("Confidence Interval:\n");
	printf("(%f <= μ <= %f) = 95%%\n", intDown,intUp);
}

//Calculating Mean μ e Variance  σˆ2
void statistics(float sample[], int sampleSize){

	float mean;
	float variance;
	float sampleSum=0;


	//Mean
	for (int i = 0; i <sampleSize ; i++)
	{
		sampleSum+=sample[i];
	}

	mean=sampleSum/sampleSize;

	//Variance
	sampleSum=0;
	for (int i = 0; i < sampleSize; i++)
	{
		sampleSum+=pow((sample[i]-mean),2);
	}
	variance=sampleSum/sampleSize;

	confidenceInterval(sampleSize, mean, variance);
}


//Task1
void task1(int data[958][10]){

float totalU[100][958][2];
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
	int q=2; //Cardinality of the prototype set
	int t=0; //counts the number of iterations

	int proto[k][q];

	float bestJ=100000000;
	int bestJIndex=1;
	float J, J1;
	int bestProto[k][q];

	for(int l=0; l<3; l++){
		srand(time(NULL));
		//Selecting prototypes
		for (int i = 0; i < k; i++)
		{
			for (int j = 0; j < q; j++)
			{
				int selected = rand()%(nObjects-1);
				proto[i][j]=selected;
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
		}

		//Calculating J
		J=0;
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

					for (int i = 0; i < q; i++)
					{
						proto[r][i]=(int)allPrototypes[i][1];
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
			}


			//Calculating J
			J1=0;
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


			//TotalU= all membership matrix 
			for (int i = 0; i < nObjects; i++)
			{
				for (int j = 0; j < k; j++)
				{
					totalU[l][i][j]=memberDregree[i][j];
				}
			}

			//Comparing J(t) with J(t-1)
			if(fabs(J1-J)<=e){

				break;
			}

			J=J1;
			
		}

		//Gets minimum J 
		if(J<bestJ){
			bestJ=J;
			bestJIndex=l;
			for (int i = 0; i < k; ++i)
			{
				for (int j = 0; j < q; ++j)
				{
					bestProto[i][j]=proto[i][j];
				}
			}
		}
	}

	printf("bestJ= %f\n", bestJ);
	printf("bestJIndex= %i\n", bestJIndex);
	for (int i = 0; i < k; ++i)
	{
		for (int j = 0; j < q; ++j)
		{
			printf("%i\n",bestProto[i][j] );
		}
	}

	printf("Melhor Particao= %d\n", bestJIndex);
	for (int i = 0; i < 958; ++i)
	{
		for (int j = 0; j <2; ++j)
		{
			printf("%f ", totalU[bestJIndex][i][j]);
		}
		printf("\n");

	}

	//Dada a melhor partição Fuzzy gerar partição Hard 
	int class1[nObjects];
	int class2[nObjects];
	int* classFinal;
	int numberErrors1=0;
	int numberErrors2=0;

	//Class1 -> class 1 | class 2
	//Class2 -> class 2 | class 1
		for (int i = 0; i < nObjects; i++)
		{
			if(totalU[bestJIndex][i][0]>totalU[bestJIndex][i][1]){
				class1[i]=1;
				class2[i]=2;
			}else{
				class1[i]=2;
				class2[i]=1;
			}
		}

		for (int i = 0; i < nObjects; i++)
		{
			if(class1[i]!=data[i][9]){
				numberErrors1++;
			}
			if(class2[i]!=data[i][9]){
				numberErrors2++;
			}
		}

		//Escolher a partição Hard que apresenta menor erro de classificação

		if(numberErrors1<numberErrors2){
			classFinal=class1;
			printf("Porcentagem de Erro= %f\n", ((float)numberErrors1/(float)nObjects));
			for (int i = 0; i < k; ++i)
			{
				printf("Medoids da classe%i\n", i+1);
				for (int j = 0; j < q; ++j)
				{
					printf("%i\n", bestProto[i][j]);
				}
			}
		}else{
			classFinal=class2;
			printf("Porcentagem de Erro= %f\n", ((float)numberErrors2/(float)nObjects));
			for (int i = k; i >0; --i)
			{
				printf("Medoids da classe%i\n", i);
				for (int j = 0; j < q; ++j)
				{
					printf("%i\n", bestProto[i-1][j]);
				}
			}
		}

		for (int i = 0; i < nObjects; ++i)
		{
			printf("%i\n",classFinal[i]);
		}


	//Indice de Rand Corrigido
	int n12=0;
	int n21=0;
	int n11=0;
	int n22=0;
	int u1=0;
	int u2=0;
	int v1=626;
	int v2=332;


	for (int i = 0; i < nObjects; i++)
	{
		if(data[i][9]==1 && classFinal[i]==1){
			u1++;
			n11++;
		}else if(data[i][9]==2 && classFinal[i]==2){
			u2++;
			n22++;
		}else if(data[i][9]==1 && classFinal[i]==2){
			u2++;
			n12++;
		}else if(data[i][9]==2 && classFinal[i]==1){
			u1++;
			n21++;
		}
	}


	long double num=(comb(n11,2)+comb(n12,2)+comb(n21,2)+comb(n22,2)-pow((comb(nObjects,2)),2)-(1/comb(nObjects,2))*(comb(u1,2)+comb(u2,2))*(comb(v1,2)+comb(v2,2)));

	printf("%Lf\n",num);
	long double quo=((((comb(u1,2)+comb(u2,2))+(comb(v1,2)+comb(v2,2)))/2)-(1/comb(nObjects,2))*(comb(u1,2)+comb(u2,2))*(comb(v1,2)+comb(v2,2)));

	printf("%Lf\n",quo);
	long double CR= num/quo;
	printf("%Lf\n", CR);
			  

}


//Task2
//change the return of the task after the function is complete
float task2(int data[950][10], int startValidation, int nPositive, int nNegative, int nTotal, int nFold){

	
	float w1 [1][9][3];
	float w2 [1][9][3];

	//Calculating conditional probability p,q,r

	int nW1=(nPositive*nFold)-nPositive;
	int nW2=(nNegative*nFold)-nNegative;
	int nSample=nW1+nW2;
	int nAtributes=9;

	//calculating the probability of class1
	for (int j = 0;  j< nAtributes; j++)
	{
		//Calculating parameters p,q,r
		float sumP=0;
		float sumQ=0;
		float sumR=0;

		int cont=0;

		for (int p = 0; p < nTotal; p++)
		{ 
			//Only calculates values relative to class 1 and skips validation folder
			if((data[p][9])==1 && (p<startValidation || p>=(startValidation+(nPositive+nNegative))))
			{
				sumP+= data[p][j]*(data[p][j]+1)/2;
				sumQ+= (1-((data[p][j])*(data[p][j])));
				sumR+= data[p][j]*(data[p][j]-1)/2;
				cont++;
			}
		}

		// printf("SumP= %f\n", sumP);
		// printf("SumQ= %f\n", sumQ);
		// printf("SumR= %f\n", sumR);


		w1[0][j][0]=(1.0/(float)nW1)*sumP;
		w1[0][j][1]=(1.0/(float)nW1)*sumQ;
		w1[0][j][2]=(1.0/(float)nW1)*sumR;
	}


	//calculating the probability of class 2

	for (int j = 0;  j< nAtributes; j++)
	{
		//Calculating parameters p,q,r
		float sumP=0;
		float sumQ=0;
		float sumR=0;

		
		for (int p = 0; p < nTotal; p++)
		{ 
			//Only calculates values relative to class 2 and skips validation folder
			if((data[p][9])==2 && (p<startValidation || p>=(startValidation+(nPositive+nNegative))))
			{
				sumP+= data[p][j]*(data[p][j]+1)/2;
				sumQ+= (1-((data[p][j])*(data[p][j])));
				sumR+= data[p][j]*(data[p][j]-1)/2;
			}
		}

		w2[0][j][0]=(1.0/(float)nW2)*sumP;
		w2[0][j][1]=(1.0/(float)nW2)*sumQ;
		w2[0][j][2]=(1.0/(float)nW2)*sumR;
	}

//Checking the value found with the validation set
//Calculating Evidence of both classes (P|wj)
float a=0;
float b=0;
int tValidation= nPositive+nNegative;
float cProbaility[tValidation][2];

int k=0;
int right=0;
int wrong=0;
int Clasificador [tValidation];
for (int i = startValidation; i < (startValidation+tValidation); i++, k++)
{
	float cW1=1;
	
	//Case: P(X|w1)
	for (int j = 0; j < nAtributes; j++)
	{
		cW1=cW1*(pow(w1[0][j][0],(data[i][j]*(data[i][j]+1)/2))*
			pow(w1[0][j][1],(1-((data[i][j])*(data[i][j]))))*
			pow(w1[0][j][2],(data[i][j]*(data[i][j]-1)/2)));
	}

	float cW2=1;
	//Case: P(X|w2)
	for (int j = 0; j < nAtributes; j++)
	{
		cW2=cW2*(pow(w2[0][j][0],(data[i][j]*(data[i][j]+1)/2))*
			pow(w2[0][j][1],(1-((data[i][j])*(data[i][j]))))*
			pow(w2[0][j][2],(data[i][j]*(data[i][j]-1)/2)));

	}
	
	//printf("w2= %f\n",cW1);
	//printf("w2= %f\n",cW2);



/* Probability a priori */
	float pW[2] = {(626.0/958.0), (332.0/958.0)}; //A gente tem que colocar essa probabilidade de acordo com o k-fold

	/*Calculating probabilities a posteriori */
	//float pProbability[2][958];
	float pProbability1 = (cW1 * pW[0]) / ( (cW1 * pW[0]) + (cW2 * pW[1]));
	float pProbability2 = (cW2 * pW[1]) / ( (cW1 * pW[0]) + (cW2 * pW[1]));
	

	//printf("Probability of w1 having x%d = %f \n", k, pProbability1);
	//printf("Probability of w2 having x%d = %f \n", k, pProbability2);

    if (pProbability1 >= pProbability2)
    {
            Clasificador[k]=1;
    }
    else
    {
            Clasificador[k]=2;
    }

    //printf("x%d in class %d\n", k, Clasificador[k]);
	

	//verifies if the pattern was correctly classified
	if(Clasificador[k]==data[i][9]){
		right++;
	}else{
		wrong++;
	}
}

	return ((float)wrong/(float)tValidation)*100; //returns the percentage of wrongly classified
}

float task2b(int data[950][10], int startValidation, int nPositive, int nNegative, int nTotal, int nFold){

	int nObjects=950;
	int dMatrix[950][950]; // 713 o max
	int nAtributes=9;
	int tValidation= nPositive+nNegative;

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
			//printf("Dissimilarity between i:%d and j:%d = %d\n",i, j, dMatrix[i][j]);
			dMatrix[j][i]=cont;
		}
		//printf("\n");
	}

	int v=10; //Numero de vizinhos
	int p; //Xp where p in [1..n]
	int ClasseKnn[nObjects]; //Tabela de resultados
	int Errors=0; //Contagem de error entre classificação a priori e com a método do k-NN vizinhos

	 int res[v][4];

	for (p=startValidation; p<(startValidation+tValidation); p++)
	{
	    // Tabela que vai contender os v vinzinhos mais proximos de Xp
	    //printf("\n\n**** p = %d *** \n\n", p);

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

	        //printf("Min = %d | Dissimilarity = %d | vinzinhos mais proximo numero %d  | classe = %d\n", res[i][0],dMatrix[min][p], i, res[i][1]);
	    }

	    //Counting of results
	    int count1=0, count2=0;

	    for (i=0; i<v; i++)
	        if (res[i][1]==1)
	            count1++;
	        else
	            count2++;

	   // printf("count1=%d, count2=%d \n", count1, count2);

	    if (count1>count2)
	        ClasseKnn[p]=1;
	    else ClasseKnn[p]=2;

	  //  printf("p=%d na classe %d", p, ClasseKnn[p]);

	    if (ClasseKnn[p]!=data[p][9])
	        Errors++;
	}

printf("Percentage of errors between classification K-NN and classification a priori = %f\n", (float)Errors/(float)tValidation);

	return Errors/nObjects;
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
//Task 1
	//task1(data);

//Task 2

//Tasks
/*Algorithm:
	-Create folder randomically (positive and negative)
		-do 10x	
			-do 62x {fill the array with positives}
			-do 33x {fill the array with negatives}
	-Do 10x
		-Create a new array of trainingData and validationData
		-Train with 9 folders and validate with 1
			-Enter function: kData, startValidationData, numberPositive, numberNegative, numberFolder
			-Inside the tasks when training ask to skip the range of the validationData
			-and when validating just use the range of the validationData
		-Update startValidationData 
*/

		//With cross validation

		//Randomize input data
		//Each folder will contain 95 samples (62 positive / 33 negative) that will be chosen randomly
		int nFold=10;
		int nPositive=62;
		int nNegative=33;
		int nTotal=nPositive+nNegative;
		

		int kData [nTotal*nFold][10];
		int n[958];
		for(int i=0; i<958; i++){
				n[i]=i;
		}

		//Calling tasks to execute with cross validation

		int executeKFold=10;
		float meansTask2 [100];
		float meansTask2b[100];
		int cont=0;

		//Execute k-fold 10x... in the end we are going to have 100 samples
		for (int j = 0; j < executeKFold ; j++)
		{
			//Creating randomized array as base
			randArray(n,626);
			randArray(&n[626], 332);

			int P=0;
			int N=626;
			int i=0;

			//Filling folders
			for (int f = 0; f < nFold ; f++){
				
				int pAux= P+nPositive;
				for (; P < pAux ; P++, i++)
				{	
					for (int j = 0; j < 10; j++)
					{
						kData[i][j]=data[n[P]][j];
					}
				}

				int nAux= N+nNegative;
				for (; N < nAux; N++, i++)
				{
					for (int j = 0; j < 10; j++)
					{
						kData[i][j]=data[n[N]][j];
					}
				}
			}

			int startValidation=0;
			int startValidation2=nPositive+nNegative;

			for (int i = 0; i < nFold; i++)
			{
				//Writing k-fold data on files
				//Writing on file for the SVM classifier
				//Label used= +1 for positive cases and -1 for negative cases

				char *path= "/Users/mariamaoliveira/Documents/UFPE/2015-2/IF699_AM/Projeto/aprendizagemMaquina"; //I did not manage to use relative pahth
				//SVM input
				char trainingNameSVM[1000];
				//Path to the project folder
				sprintf(trainingNameSVM,"%s/data_kfold/SVM/%i/SVMtraining%i.csv",path,j,i);
				//printf("%s\n", trainingNameSVM);
				char testNameSVM[1000];
				sprintf(testNameSVM,"%s/data_kfold/SVM/%i/SVMtest%i.csv",path,j,i);
				char *modeW="w";

				FILE *fTraining;
				FILE *fTest;
				fTraining= fopen(trainingNameSVM, modeW);
				fTest=fopen(testNameSVM,modeW);

				if(fTraining!=NULL || fTest!=NULL){
					printf("SVM File successfully opened for Writing!\n");
					
					for (int l = 0; l < 950; l++)
					{	//Label
						if(l<startValidation || l>=(startValidation+(nPositive+nNegative))){
							
							for (int j = 0; j < 9; j++)
							{	
								fprintf(fTraining, "%d,",kData[l][j]);
							}
							fprintf(fTraining, "%d", kData[l][9]);
							fprintf(fTraining, "\n");
						}else{
							
							for (int j = 0; j < 9; j++)
							{	
								fprintf(fTest, "%d,",kData[l][j]);
							}
							fprintf(fTest, "%d", kData[l][9]);
							fprintf(fTest, "\n");
						}
					}

				}else{
					printf("The SVM file was not open!\n");
				}

				fclose(fTraining);
				fclose(fTest);

				if(i==0){
				//MLP input
				char trainingNameMLP[1000];
				sprintf(trainingNameMLP,"%s/data_kfold/MLP/MLP%i.csv",path,j);
				

				FILE *fTrainingMLP;
				
				fTrainingMLP= fopen(trainingNameMLP, modeW);
				

				if(fTrainingMLP!=NULL){
					printf("MLP File successfully opened for Writing!\n");

					for (int l = 0; l < 950; l++)
					{
							
							for (int j = 0; j < 9; j++)
							{	
								fprintf(fTrainingMLP, "%d,",kData[l][j]);
							}

							if(kData[l][9]==2){
								fprintf(fTrainingMLP, "%d,", 0);
								fprintf(fTrainingMLP, "%d", 1);
							}else{
								fprintf(fTrainingMLP, "%d,", 1);
								fprintf(fTrainingMLP, "%d", 0);
							}
							fprintf(fTrainingMLP, "\n");
						
					}

				}else{
					printf("The MLP file was not open!\n");
				}
				
				fclose(fTrainingMLP);
				}

				//Task 2: Naive Bayesian Classifier
				meansTask2[cont]=task2(kData, startValidation,nPositive,nNegative,nTotal*nFold,nFold);
				//Task 2b: KNN
				meansTask2b[cont]=task2b(kData, startValidation,nPositive,nNegative,nTotal*nFold,nFold);
				
				cont++;
				
				//Updating trainingData and validationData
				startValidation+=(nPositive+nNegative);
				if(i<8){
					startValidation2+=(nPositive+nNegative);
				}else{
					startValidation2=0;
				}
				
			}
		}
		
	


//File variables
	// FILE *fileErrorMLP;
	// char *fileErrorMLPName="MATLAB/errorMLP.txt";	
	// fileErrorMLP= fopen(fileErrorMLPName, "r");
	// float errorMLP[100];

	// if(fileErrorMLPName!=NULL){
	// 	printf("File Error MLP successfully opened!\n");
	// 	for (int i = 0; i < 100; i++)
	// 	{
	// 		fscanf(fileErrorMLP,"%f",&errorMLP[i]);
	// 	}
		
	// }else{
	// 	printf("Error MLP file was not open!\n");
	// }

	// FILE *fileErrorSVM;
	// char *fileErrorSVMName="libsvm-3.20/matlab/errorMLP.txt";	
	// fileErrorSVM= fopen(fileErrorSVMName, "r");
	// float errorSVM[100];

	// if(fileErrorSVMName!=NULL){
	// 	printf("File Error MLP successfully opened!\n");
	// 	for (int i = 0; i < 100; i++)
	// 	{
	// 		fscanf(fileErrorSVM,"%f",&errorSVM[i]);
	// 	}
		
	// }else{
	// 	printf("Error MLP file was not open!\n");
	// }

	// ///////////////////////////STATISTICS//////////////////////////////
	// //Calculating Mean and Confidence interval of the MLP
	// statistics(meansTask2,100);
	// //statistics(meansTask2b,100);
	// statistics(errorMLP,100);
	// statistics(errorSVM,100);




	//	task2(kData, startValidation,nPositive,nNegative,nTotal*nFold,nFold);
	


		// for (int i = 0; i < nTotal*nFold; ++i)
		// {printf("%i=", i);
		// 	for (int j = 0; j < 10; j++)
		// 	{
		// 		printf("%i ",kData[i][j]);
		// 	}
		// 	printf("\n");
		// }

return 0;
}
