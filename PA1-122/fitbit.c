#include"fitbit.h"

//Find Patient
char* findPatient(FILE* infile) {
	char line[400] = {'\0'};
	char target[10] = { '\0' };
	char garbage[15] = "";
	fgets(line, 400, infile);
	strcpy(garbage, strtok(line, ","));
	strcpy(target, strtok(NULL, ","));
	return target;
}

//Reads in cleans and parses every line
void readData(FILE* infile, FitbitData datastruct[], char* targetpatient) {
	char line[400] = {'\0'};
	char lastline[400] = { '\0' };
	char fullline[400] = { '\0' };
	char garbage[400] = { '\0' };
	
	//fgets(garbage, 400, infile);
	//fgets(garbage, 400, infile);
	int index = 0;
	while ( !feof(infile)){
		strcpy(lastline, fullline);
		fgets(line, 400, infile);
		strcpy(fullline, line);

		int i = 0;
		while (line[i] != '\n') {
			int ii = i;
			if (line[ii] == ',' && line[ii + 1] == ',') {
				shiftStrings(line, ii);
				line[ii + 1] = '-';
				line[ii + 2] = '1';
			}
			i++;
		}
		if (strcmp(lastline, fullline) == 0) {
			continue;
		}

		char* target = strtok(line, ",");
		if (strcmp(target,targetpatient) == 0) {
			strcpy(datastruct[index].patient, target);
			strcpy(datastruct[index].minute, strtok(NULL, ","));
			datastruct[index].calories = atof(strtok(NULL, ","));
			datastruct[index].distance = atof(strtok(NULL, ","));
			datastruct[index].floors = atoi(strtok(NULL, ","));
			datastruct[index].heartRate = atoi(strtok(NULL, ","));
			datastruct[index].steps = atoi(strtok(NULL, ","));
			datastruct[index].sleepLevel = atoi(strtok(NULL, ""));
			index++;
		}
		
	}


}

//does all calculations and records data
void recordData(FILE* outfile, FitbitData datastruct[]) {
	double cal = totalCalories(datastruct);
	double dist = totalDistance(datastruct);
	unsigned int floors = floorsWalked(datastruct);
	unsigned int steps = totalSteps(datastruct);
	unsigned int heartrate = computeHeart(datastruct);
	unsigned int maxstep = maxsteps(datastruct);
	char start[20] = {'\0'}, end[20] = {'\0'};
	unsigned int worstsleep = badSleep(datastruct, start, end);

	fprintf(outfile, "Total Calories, Total Distance, Total Floors, Total Steps, Avg Heartrate, Max Steps, Worst Sleep, Start Time Bad Sleep, End Time Worst Sleep\n %lf, %lf, %d, %d, %d, %d, %d, %s, %s\n", cal, dist, floors, steps, heartrate, maxstep, worstsleep, start, end);
	for (int i = 0; i < 1440; i++) {
		fprintf(outfile, "%s,%s,%lf,%lf,%d,%d,%d,%d\n", datastruct[i].patient, datastruct[i].minute, datastruct[i].calories, datastruct[i].distance, datastruct[i].floors, datastruct[i].heartRate, datastruct[i].steps, datastruct[i].sleepLevel);
	}
}

//functions for calculations
double totalCalories(FitbitData datastruct[]) {
	double totalCal = 0;
	int arrsize = 1440;
	unsigned int flag = -1;
	for (int i = 0; i < arrsize; i++) {
		if (datastruct[i].calories == flag) {
			continue;
		}
		totalCal += datastruct[i].calories;
	}
	return totalCal;
}

double totalDistance(FitbitData datastruct[]) {
	double totalDist = 0;
	int arrsize = 1440;
	unsigned int flag = -1;
	for (int i = 0; i < arrsize; i++) {
		if (datastruct[i].distance == flag) {
			continue;
		}
		totalDist += datastruct[i].distance;
	}
	return totalDist;
}

unsigned int computeHeart(FitbitData datastruct[]) {
	int sum = 0, ave = 0;
	int arrsize = 1440;
	unsigned int flag = -1;
	for (int i = 0; i < arrsize; i++) {
		if (datastruct[i].heartRate == flag) {
			arrsize--;
			continue;
		}
		sum += datastruct[i].heartRate;
	}
	ave = sum / arrsize;
	return ave;
}

unsigned int floorsWalked(FitbitData datastruct[]) {
	int floors = 0;
	int arrsize = 1440;
	unsigned int flag = -1;
	for (int i = 0; i < arrsize; i++) {
		if (datastruct[i].floors == flag) {
			continue;
		}
		floors += datastruct[i].floors;
	}
	
	return floors;
}

unsigned int totalSteps(FitbitData datastruct[]) {
	int steps = 0;
	int arrsize = 1440;
	unsigned int flag = -1;
	for (int i = 0; i < arrsize; i++) {
		if (datastruct[i].steps == flag) {
			continue;
		}
		steps += datastruct[i].steps;
	}

	return steps;
}

unsigned int maxsteps(FitbitData datastruct[]) {
	int max = 0;
	int arrsize = 1440;
	unsigned int flag = -1;
	for (int i = 0; i < arrsize; i++) {
		if (datastruct[i].steps > max && datastruct[i].steps != flag) {
			max = datastruct[i].steps;
		}
	}
	return max;
}

//kickass badsleep function
unsigned int badSleep(FitbitData datastruct[], char* start, char* end) {
	int badSleep = 0, maxBadsleep = 0, lastrange = 0;
	int startrange = 0, endrange = 0;
	int finalstartrange = 0, finalendrange = 0;

	for (int i = 0; i < 1440;) {
		if (datastruct[i].sleepLevel > 1) {
			startrange = i;
			while (datastruct[i].sleepLevel > 1)
			{
				badSleep += datastruct[i].sleepLevel;
				i++;
			}
			endrange = i;
			if (badSleep > maxBadsleep) {
				maxBadsleep = badSleep;
				finalstartrange = startrange;
				finalendrange = endrange;
			}
		}
		else {
			i++;
		}
	}
	
	strcpy(start, datastruct[finalstartrange].minute);
	strcpy(end, datastruct[finalendrange].minute);

	return maxBadsleep;
}


//TA helped with
void shiftStrings(char str[], int start) {
	int end = 0;
	for (end = 0; str[end] != '\0'; end++);
	while(end>start){
		str[end + 2] = str[end];
		end--;
	}
}