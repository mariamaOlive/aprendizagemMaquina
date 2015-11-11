#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


//Auxiliary functions
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

	// for (int i = 0; i < 958; ++i)
	// {
	// 	for (int j = 0; j < 958; ++j)
	// 	{
	// 		printf("%i ",dMatrix[i][j]);
	// 	}
	// 	printf("\n");
	// }

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
	float J, J1;
	int bestProto[k][q];

for(int l=0; l<100; l++){
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
		
		//if(i<5){
		//printf("%i -> u1= %f ",i, memberDregree[i][0]);
		//printf("u2= %f\n", memberDregree[i][1]);
		//}
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


				//  for (int i = 0; i <5; ++i)
				//  {
				//	 printf("%f %i\n",allPrototypes[i][0], (int) allPrototypes[i][1]);
				//  }

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


			
			// if(i<5){
			// 	printf("%i -> u1= %f ",i, memberDregree[i][0]);
			// 	printf("u2= %f\n", memberDregree[i][1]);
			// }
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

		printf("e= %f\n", fabs(J1-J));
		printf("J= %f\n", J1);

		//Comparing J(t) with J(t-1)
		if(fabs(J1-J)<=e){

			break;
		}

		J=J1;
		
	}

	if(J<bestJ){
		bestJ=J;
		for (int i = 0; i < k; ++i)
	{
		for (int j = 0; j < q; ++j)
		{
			bestProto[i][j]=proto[i][j];
		}
	}
	}
	
	// int class[nObjects];
	// int numberErrors=0;


	// for (int i = 0; i < nObjects; i++)
	// {
	// //printf("%i -> u1= %f ",i, memberDregree[i][0]);
	// //printf("u2= %f\n", memberDregree[i][1]);

	// 	if(memberDregree[i][0]>memberDregree[i][1]){
	// 		class[i]=1;
	// 	}else{
	// 		class[i]=2;
	// 	}
	// }

	// for (int i = 0; i < nObjects; i++)
	// {
	// 	if(class[i]!=data[i][9]){
	// 		numberErrors++;
	// 	}
	// }

	

	
	//printf("Porcentagem de Erro= %f\n", ((float)numberErrors/(float)nObjects));
}

printf("bestJ= %f\n", bestJ);
	for (int i = 0; i < k; ++i)
	{
		for (int j = 0; j < q; ++j)
		{
			printf("%i\n",bestProto[i][j] );
		}
	}
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

	// for (int i = 0; i <1; ++i)
	// {
	// 	for (int j = 0; j < nAtributes; ++j)
	// 	{
	// 		printf("p= %f ", w1[i][j][0]);
	// 		printf("q= %f ", w1[i][j][1]);
	// 		printf("r= %f\n", w1[i][j][2]);
	// 	}

	// }

	//Mean 

	//Variance

	//Confidence interval
	// printf("correctly classified %i\n",right);
	// printf("wrongly classified %i\n",wrong);
	// printf("\n");

	return ((float)wrong/(float)tValidation)*100; //returns the percentage of wrongly classified
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

	//Writing on file for the SVM classifier
	//Label used= +1 for positive cases and -1 for negative cases
	// char *modeSVM="w";
	// char *fileNameSVM="tic-tac-toeSVM";
	// FILE *ofp;
	// ofp= fopen(fileNameSVM, modeSVM);

	// if(ofp!=NULL){
	// 	printf("SVM File successfully opened for Writing!\n");

	// 	for (int i = 0; i < 958; i++)
	// 	{	//Label
	// 		fprintf(ofp, "%d", data[i][9]);
	// 		for (int j = 0; j < 9; j++)
	// 		{	
	// 			fprintf(ofp, " %d:%d",(j),data[i][j]);
	// 		}
	// 		fprintf(ofp, "\n");
	// 	}

	// }else{
	// 	printf("The SVM file was not open!\n");
	// }

	// fclose(ofp);
	fclose(ifp);

}else{
	printf("The file was not open!\n");
}
//////////////////////data structure uploaded//////////////////
//Task 1
	task1(data);
//Task 2
	//task2(data);


int kFold=1;

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

			for (int i = 0; i < nFold; i++)
			{
				//Task 2: Naive Bayesian Classifier
				meansTask2[cont]=task2(kData, startValidation,nPositive,nNegative,nTotal*nFold,nFold);
				cont++;
				//Updating trainingData and validationData
				startValidation+=(nPositive+nNegative);
			}
		}
		
		statistics(meansTask2,100);
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
