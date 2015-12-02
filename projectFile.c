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

	int (*a2)[2]=a;
	int (*b2)[2]=b;
   return ( (*a2)[0] - (*b2)[0]);
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

//Task1
void task1(int data[958][10], FILE *resultsFile){

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
	float m=1.5; //
	int T=150; //an iteration limit
	float e= pow(10, -10);
	int q=2; //Cardinality of the prototype set

	int proto[k][q];

	float bestJ=100000000;
	int bestJIndex=1;
	float J, J1;
	int bestProto[k][q];
	srand(time(NULL));

	for(int l=0; l<100; l++){
		printf("Fuzzy Iteration: %i\n", l);
		int t=0; //counts the number of iterations

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
						//	if(i!=proto[p][r]){
								sum2+=dMatrix[i][proto[j][r]];
								sum3+=dMatrix[i][proto[p][r]];
						//	}else{
						//		sum2=1;
						//		sum3=1;
						//	}
						}

						sum1=sum2/sum3;

						sum1=pow(sum1, (1/(m-1)));

						sum0+=sum1;
				}

				memberDregree[i][j]=(1/sum0);
				//printf("%f ",memberDregree[i][j]);
			}
			//printf("\n");
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
						float sum1=0;
						float sum2=0;
						float sum3=0;

						for (int r = 0; r< q; r++)
						{
						//	if(i!=proto[p][r]){
								sum2+=dMatrix[i][proto[j][r]];
								sum3+=dMatrix[i][proto[p][r]];
						//	}else{
						//		sum2=1;
						//		sum3=1;
						//	}
						}

						sum1=sum2/sum3;

						sum1=pow(sum1, (1/(m-1)));

						sum0+=sum1;
				}

				memberDregree[i][j]=(1/sum0);
			//	printf("%f \n", memberDregree[i][j]=(1/sum0));
				
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
				J=J1;
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

	fprintf(resultsFile,"Melhor Particao encontrada na iteraçao: %i\n", bestJIndex);
	fprintf(resultsFile,"-------------------------------\n");
	for (int i = 0; i < 958; ++i)
	{
		fprintf(resultsFile, "U%i= ", i);
		for (int j = 0; j <2; ++j)
		{
			fprintf(resultsFile,"%f ", totalU[bestJIndex][i][j]);
		}
		fprintf(resultsFile,"\n");
	}
	fprintf(resultsFile,"-------------------------------\n");

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
			fprintf(resultsFile,"Porcentagem de Erro= %f\n", (100*(float)numberErrors1/(float)nObjects));
			for (int i = 0; i < k; ++i)
			{
				fprintf(resultsFile,"Medoids da classe%i\n", i+1);
				for (int j = 0; j < q; ++j)
				{
					fprintf(resultsFile,"%i\n", bestProto[i][j]);
				}
			}
		}else{
			classFinal=class2;
			fprintf(resultsFile,"Porcentagem de Erro= %f\n", ((float)numberErrors2/(float)nObjects));
			for (int i = k; i >0; --i)
			{
				fprintf(resultsFile,"Medoids da classe%i\n", i);
				for (int j = 0; j < q; ++j)
				{
					fprintf(resultsFile,"%i\n", bestProto[i-1][j]);
				}
			}
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

	long double num= (comb(n11,2)+comb(n22,2)+comb(n12,2)+comb(n21,2))-(comb(u1,2)+comb(u2,2))*((comb(v1,2)+comb(v2,2))/comb(nObjects,2));

	long double quo=(((comb(u1,2)+comb(u2,2))+(comb(v1,2)+comb(v2,2)))/2)-((comb(u1,2)+comb(u2,2))/(comb(nObjects,2)));

	long double CR= num/quo;
	fprintf(resultsFile,"Indice de Rand Corrigido= %Lf\n", CR);


	fprintf(resultsFile,"Erro classe1= %f\n", (float)n12/(float)626);	
	fprintf(resultsFile,"Erro classe2= %f\n", (float)n21/(float)332);	

	fprintf(resultsFile,"\nParticao Hard\n");

	for (int i = 0; i < nObjects; ++i)
	{
		fprintf(resultsFile,"Objeto %i: %i\n",i,classFinal[i]);
	}		  

}


//Task2
//change the return of the task after the function is complete
float task2(int data[950][10], int startValidation, int nPositive, int nNegative, int nTotal, int nFold){

	FILE *bayesFile;
	bayesFile= fopen("posterioriBayes", "w");

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
	
	/* Probability a priori */
		float pW[2] = {(626.0/958.0), (332.0/958.0)}; //A gente tem que colocar essa probabilidade de acordo com o k-fold

		/*Calculating probabilities a posteriori */
		float pProbability1 = (cW1 * pW[0]) / ( (cW1 * pW[0]) + (cW2 * pW[1]));
		float pProbability2 = (cW2 * pW[1]) / ( (cW1 * pW[0]) + (cW2 * pW[1]));

		fprintf(bayesFile, "%f %f\n",pProbability1, pProbability2);
		

	    if (pProbability1 >= pProbability2)
	    {
	            Clasificador[k]=1;
	    }
	    else
	    {
	            Clasificador[k]=2;
	    }

		//verifies if the pattern was correctly classified
		if(Clasificador[k]==data[i][9]){
			right++;
		}else{
			wrong++;
		}
	}

	fclose(bayesFile);

	return ((float)wrong/(float)tValidation); //returns the percentage of wrongly classified
}

float task2b(int data[950][10], int startValidation, int nPositive, int nNegative, int nTotal, int nFold){

	FILE *knnFile;
	knnFile= fopen("posterioriKNN", "w");

	int nObjects=950;
	int dMatrix[950][950]; 
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
			dMatrix[j][i]=cont;
			//printf("Dissimilarity between i:%d and j:%d = %d\n",i, j, dMatrix[i][j]);
		}
		//printf("\n");
	}

	int v=15; //Number of neighbors
	int p; //Xp where p in [1..n]
	int ClasseKnn[nObjects]; //Tabela de resultados
	int Errors=0; //Contagem de error entre classificação a priori e com a método do k-NN vizinhos

	int distP[958][2];
	
	for (int i = 0; i < nObjects; ++i)
	distP[i][0]=1000;

	int res[v][4];

	for (p=startValidation; p<(startValidation+tValidation); p++)
	{

		for (int i = 0; i < nObjects; i++)
		{
			if((i<startValidation || i>=(startValidation+tValidation))&& i!=p){
			distP[i][0]=dMatrix[p][i];
			distP[i][1]=data[i][9];
			}
		}

		//Ordernar a matriz dispP para pegar todos as distâncias menores
		qsort(distP, nObjects, 2*(sizeof(int)),cmpfunc);

		// for (int i = 0; i < nObjects; ++i)
		// {
		//  	printf("%i %i\n", distP[i][0], distP[i][1]);
		// }

		int check=1;
		int v2=v-1;
		while(check){
			if(distP[v2][0]!=distP[v2+1][0]){
				check=0;
			}else{
				v2++;
			}
		}

		check=1;
		int v3=v-1;
		while(check){
			if((distP[v3][0]!=distP[v3+1][0])|| v3==0){
				check=0;
			}else{
				v3--;
			}
		}

		//Trick to make things right!!!
		if(v3!=0){
			v3++;
		}

		int auxArray[v2-v3+1];
		int j=v3;
		for (int i = 0 ; i <v2-v3+1 ; i++, j++)
		{
			auxArray[i]=j;
		}
		randArray(auxArray, v2-v3+1);

		//Counting of results
	    int count1=0, count2=0;

	    for (i=0; i<v3; i++)
	        if (distP[i][1]==1)
	            count1++;
	        else
	            count2++;

	    int k=0;
	   	for (int i = v3; i < v; i++,k++)
	   	{
	   		if (distP[auxArray[k]][1]==1)
	            count1++;
	        else
	            count2++;
	   	}


	   // printf("count1=%d, count2=%d \n", count1, count2);

	    if (count1>count2)
	        ClasseKnn[p]=1;
	    else ClasseKnn[p]=2;

	    //Posteriori probability
	    float P1=(float)count1/(float)v;
	    float P2=(float)count2/(float)v;

	    fprintf(knnFile, "%f %f\n",P1, P2);

	  //  printf("p=%d na classe %d", p, ClasseKnn[p]);

	    if (ClasseKnn[p]!=data[p][9])
	        Errors++;
}
	    
	 fclose(knnFile);
	//printf("Percentage of errors between classification K-NN and real classification = %f\n", (float)Errors/(float)tValidation);

	return ((float)Errors/(float)tValidation);
}


float task2SumRule(int data[950][10], int startValidation, int nPositive, int nNegative, int nTotal, int nFold){
	
	FILE *knnFile;
	knnFile= fopen("posterioriKNN", "r");
	FILE *bayesFile;
	bayesFile= fopen("posterioriBayes", "r");
	int tValidation= nPositive+nNegative;


	//Sum rule applied here after all posteriori probability of test set have been calculated
	int L=2; //number of classifiers
	float prioriP1=0.653;
	float prioriP2=0.347;
	int finalClass[958];
	int nErrors=0;


	if(knnFile!=NULL &&  bayesFile!=NULL){
	
	float postBayes[958][2];
	float postKNN [958][2];

	for (int i = startValidation; i < (startValidation+tValidation); i++)
	{
		fscanf(bayesFile,"%f",&postBayes[i][0]);
		fscanf(bayesFile,"%f",&postBayes[i][1]);
		fscanf(knnFile,"%f",&postKNN[i][0]);
		fscanf(knnFile,"%f",&postKNN[i][1]);
	}

	for (int i = startValidation; i < (startValidation+tValidation); i++)
	{
		float sum1=0;
		float sum2=0;

		sum1+=postBayes[i][0];
		sum1+=postKNN[i][0];
		sum2+=postBayes[i][1];
		sum2+=postKNN[i][1];

		float term1=(1-L)*prioriP1+sum1;
		//printf("term1: %f\n", term1);
		float term2=(1-L)*prioriP2+sum2;
		//printf("term2: %f\n", term2);

		if(term1>term2){
			finalClass[i]=1;
		}else{
			finalClass[i]=2;
		}

		//printf("%i\n", finalClass[i]);
		//Comparing with the original data in order to calculate the error
		if(finalClass[i]!=data[i][9]){
			nErrors++;
		}
			
	}

	fclose(knnFile);
	fclose(bayesFile);
	
	}else{
		printf("It was not possible to proceed! Error Sum Rule!\n");
	}


	return ((float)nErrors/(float)tValidation);
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


//Writing file with numerical data
	//Label used= 1 for positive cases and 2 for negative cases

FILE *ofp;
ofp= fopen("tic-tac-toe-numerical.csv", "w");

if(ofp!=NULL){
	printf("File successfully opened for Writing!\n");

	for (int i = 0; i < 958; i++)
	{	//Label

		for(int j = 0; j < 9; j++)
		{	
		fprintf(ofp, " %d,",data[i][j]);
		}
		fprintf(ofp, "%d\n", data[i][9]);
	}

}else{
	printf("The file was not open!\n");
}
fclose(ofp);
 	





//////////////////////data structure uploaded//////////////////
//Task 1
	// FILE *resultsFuzzyFile;
	// resultsFuzzyFile= fopen("Results/resultsTaks1.txt", "w");

	// //Executes the algorith of task1 and prints its results into the file "Results/resultsTaks1.txt"
	// task1(data,resultsFuzzyFile);

	// fclose(resultsFuzzyFile);
	

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
// 		//Each folder will contain 95 samples (62 positive / 33 negative) that will be chosen randomly
		FILE *resultsBayesFile;
		resultsBayesFile= fopen("Results/errorBayesian.txt", "w");
		FILE *resultsKNNFile;
		resultsKNNFile= fopen("Results/errorKNN15.txt", "w");
		FILE *resultsSumFile;
		resultsSumFile= fopen("Results/errorSum.txt", "w");

		char kfoldName[1000];
		FILE *filekfold;

		char kfoldRead[1000];
		FILE *filekfoldRead;


		int nFold=10;
		int nPositive=62;
		int nNegative=33;
		int nTotal=nPositive+nNegative;
		

		int kData [nTotal*nFold][10];
		int n[950];
		for(int i=0; i<950; i++){
				n[i]=i;
		}

		//Calling tasks to execute with cross validation

		int executeKFold=10;
		float meansTask2 [100];
		float meansTask2b[100];
		float meansTask2SumRule[100];
		int cont=0;


		//Execute k-fold 10x... in the end we are going to have 100 samples

		for (int j = 0; j < executeKFold ; j++)
		{
			

		//ATTENTION: Just run this part of the experiment once in ordet to create the k-fold files
		//If you want to create a new k-fold data uncomment the code below

		// sprintf(kfoldName,"data_kfold/OTHERS/%i", j);
		// filekfold=fopen(kfoldName,"w");
			
		// //Creating randomized array as base
		// randArray(n,626);
		// randArray(&n[626], 332);

		// int P=0;
		// int N=626;
		// int i=0;

		// 	//Filling folders
		// 	for (int f = 0; f < nFold ; f++){
				
		// 		int pAux= P+nPositive;
		// 		for (; P < pAux ; P++, i++)
		// 		{	
		// 			for (int j = 0; j < 9; j++)
		// 			{
		// 				kData[i][j]=data[n[P]][j];
		// 				fprintf(filekfold, "%i ", kData[i][j]);
		// 			}
		// 			kData[i][9]=data[n[P]][9];
		// 			fprintf(filekfold, "%i\n", kData[i][9]);
		// 		}

		// 		int nAux= N+nNegative;
		// 		for (; N < nAux; N++, i++)
		// 		{
		// 			for (int j = 0; j < 9; j++)
		// 			{
		// 				kData[i][j]=data[n[N]][j];
		// 				fprintf(filekfold, "%i ", kData[i][j]);
		// 			}
		// 			kData[i][9]=data[n[N]][9];
		// 			fprintf(filekfold, "%i\n", kData[i][9]);
		// 		}
		// 	}
		// fclose(filekfold);

		//Loading k-fold data
		//If the code above is uncommented, comment this part below the LOAD 
		sprintf(kfoldRead,"data_kfold/OTHERS/%i", j);
		printf("%s\n", kfoldRead);
		filekfoldRead=fopen(kfoldRead,"r");

		for (int i = 0; i < 950; ++i)
		{
			for (int j = 0; j < 10; ++j)
			{
				fscanf(filekfoldRead,"%i",&kData[i][j]);
			}
		}
		
		fclose(filekfoldRead);
		//////////////////////////////////////////////////////


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
				printf("Bayesian: %f\n", meansTask2[cont]);
				fprintf(resultsBayesFile, "%f\n", meansTask2[cont]);
				//Task 2b: KNN
				meansTask2b[cont]=task2b(kData, startValidation,nPositive,nNegative,nTotal*nFold,nFold);
				printf("KNN: %f\n", meansTask2b[cont]);
				fprintf(resultsKNNFile, "%f\n", meansTask2b[cont]);

				meansTask2SumRule[cont]=task2SumRule(kData, startValidation,nPositive,nNegative,nTotal*nFold,nFold);
				printf("SumRule: %f\n",meansTask2SumRule[cont]);
				fprintf(resultsSumFile, "%f\n", meansTask2SumRule[cont]);

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

		fclose(resultsBayesFile);
		fclose(resultsKNNFile);
		fclose(resultsSumFile);
		

return 0;
}
