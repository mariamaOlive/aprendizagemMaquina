#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

//Confidence Interval
void confidenceInterval(float sampleSize, float mean, float variance, FILE *resultsFile){
	//Confidence interval of 95% ->Z(α/2)= 1.96
	//With Normal
	//e=Z(α/2)*(σ/sqrt n)

	float sigma = sqrt(variance);
	float e= 1.96 * (sigma/sqrt(sampleSize));

	float intDown=mean-e;
	float intUp=mean+e;

	fprintf(resultsFile,"Confidence Interval:\n");
	fprintf(resultsFile,"(%f <= μ <= %f) = 95%%\n", intDown,intUp);
}

//Calculating Mean μ e Variance  σˆ2
void statistics(float sample[], int sampleSize, FILE *resultsFile){

	float mean;
	float variance;
	float sampleSum=0;



	//Mean
	for (int i = 0; i <sampleSize ; i++)
	{
		sampleSum+=sample[i];
	}

	mean=sampleSum/sampleSize;
	
	fprintf(resultsFile,"Mean: %f\n", mean);

	//Variance
	sampleSum=0;
	for (int i = 0; i < sampleSize; i++)
	{
		sampleSum+=pow((sample[i]-mean),2);
	}
	variance=sampleSum/sampleSize;

	confidenceInterval(sampleSize, mean, variance, resultsFile);
}

int main(){
//File variables
	FILE *fileErrorMLP;	
	fileErrorMLP= fopen("Results/errorMLP.txt", "r");
	float errorMLP[100];

	if(fileErrorMLP!=NULL){
		printf("File Error MLP successfully opened!\n");
		for (int i = 0; i < 100; i++)
		{
			fscanf(fileErrorMLP,"%f",&errorMLP[i]);
		}
		
	}else{
		printf("Error MLP file was not open!\n");
	}

	FILE *fileErrorSVM;
	char *fileErrorSVMName="Results/errorSVM0.txt";	
	fileErrorSVM= fopen(fileErrorSVMName, "r");
	float errorSVM[100];

	if(fileErrorSVM!=NULL){
		printf("File Error SVM successfully opened!\n");
		for (int i = 0; i < 100; i++)
		{
			fscanf(fileErrorSVM,"%f",&errorSVM[i]);
		}
		
	}else{
		printf("Error MLP file was not open!\n");
	}

	FILE *fileErrorBayes;
	char *fileErrorBayesName="Results/errorBayesian.txt";	
	fileErrorBayes= fopen(fileErrorBayesName, "r");
	float errorBayes[100];

	if(fileErrorBayes!=NULL){
		printf("File Error Bayesian successfully opened!\n");
		for (int i = 0; i < 100; i++)
		{
			fscanf(fileErrorBayes,"%f",&errorBayes[i]);
		}
		
	}else{
		printf("Error Bayesian file was not open!\n");
	}

	FILE *fileErrorKNN;	
	fileErrorKNN= fopen("Results/errorKNN15.txt", "r");
	float errorKNN[100];

	if(fileErrorKNN!=NULL){
		printf("File Error KNN successfully opened!\n");
		for (int i = 0; i < 100; i++)
		{
			fscanf(fileErrorKNN,"%f",&errorKNN[i]);
		}
		
	}else{
		printf("Error KNN file was not open!\n");
	}

	FILE *fileErrorSum;	
	fileErrorSum= fopen("Results/errorSum.txt", "r");
	float errorSum[100];

	if(fileErrorSum!=NULL){
		printf("File Error SUM successfully opened!\n");
		for (int i = 0; i < 100; i++)
		{
			fscanf(fileErrorSum,"%f",&errorSum[i]);
		}
		
	}else{
		printf("Error SUM file was not open!\n");
	}



	///////////////////////////STATISTICS//////////////////////////////
	//Calculating Mean and Confidence interval of the MLP
	FILE *resultsFile;
	resultsFile= fopen("Results/resultsTaks2.txt", "w");
	
	fprintf(resultsFile, "Bayesian Classifier\n");
	statistics(errorBayes,100, resultsFile);
	fprintf(resultsFile,"\n");

	fprintf(resultsFile, "KNN Classifier\n");
	statistics(errorKNN,100, resultsFile);
	fprintf(resultsFile,"\n");

	fprintf(resultsFile, "Sum Rule: Bayesian + KNN\n");
	statistics(errorSum,100, resultsFile);
	fprintf(resultsFile,"\n");

	fprintf(resultsFile, "MLP Classifier\n");
	statistics(errorMLP,100, resultsFile);
	fprintf(resultsFile,"\n");

	fprintf(resultsFile, "SVM Classifier\n");
	statistics(errorSVM,100, resultsFile);
	fprintf(resultsFile,"\n");

	fclose(resultsFile);
return 0;
}