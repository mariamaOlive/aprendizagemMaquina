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
				insertedValue= 0;

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



return 0;
}
