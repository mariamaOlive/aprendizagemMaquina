#include <stdio.h>


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


float w1 [626][9][3];
float w2 [332][9][3];

//Calculating conditional probability W1

int nW1=626;
int nW2=332;
int nAtributes=9;

for (int i = 0; i < nW1; ++i)
{
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
			//printf("%i\n",data[p][j]);
			sumQ+= (1-((data[p][j])*(data[p][j])));

			//printf("%f\n",sumQ);
		}

		for (int p = 0; p <nW1 ; ++p)
		{
			sumR+= data[p][j]*(data[p][j]-1)/2;
		}


		/*printf("SumP= %f\n", sumP);
		printf("SumQ= %f\n", sumQ);
		printf("SumR= %f\n", sumR);*/
		
		w1[i][j][0]=(1.0/626.0)*sumP;
		
		w1[i][j][1]=(1.0/626.0)*sumQ;
		
		w1[i][j][2]=(1.0/626.0)*sumR;	
		
	}
}

for (int i = 0; i < nW2; ++i)
{
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
			//printf("%i\n",data[p][j]);
			sumQ+= (1-((data[p+nW1][j])*(data[p+nW1][j])));

			//printf("%f\n",sumQ);
		}

		for (int p = 0; p <nW2 ; ++p)
		{
			sumR+= data[p+nW1][j]*(data[p+nW1][j]-1)/2;
		}


		/*printf("SumP= %f\n", sumP);
		printf("SumQ= %f\n", sumQ);
		printf("SumR= %f\n", sumR);*/
		
		w2[i][j][0]=(1.0/332.0)*sumP;
		
		w2[i][j][1]=(1.0/332.0)*sumQ;
		
		w2[i][j][2]=(1.0/332.0)*sumR;	
		
	}
}

for (int i = 0; i <nW2; ++i)
{
	for (int j = 0; j < nAtributes; ++j)
	{
		printf("p= %f ", w2[i][j][0]);
		printf("q= %f ", w2[i][j][1]);
		printf("r= %f\n", w2[i][j][2]);
	}

}



return 0;
}
