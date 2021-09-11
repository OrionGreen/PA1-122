#include"fitbit.h"

int main (void) {

	FitbitData daysData[1440] = { {"","",0,0,0,0,0,0} };
	
	FILE* infile = fopen("FitbitData.csv", "r");
	FILE* outfile = fopen("Results.csv", "w");


	if (infile != NULL) {
		printf("Opened Data File Successfully!\n");
		char target[10] = { '\0' };
		strcpy(target, findPatient(infile));
		readData(infile, daysData, target);
	}

	recordData(outfile, daysData);

	fclose(infile);
	fclose(outfile);

	return 0;
}