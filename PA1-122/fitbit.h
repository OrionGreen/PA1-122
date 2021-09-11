
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

typedef enum sleep
{
    NONE = 0, ASLEEP = 1, AWAKE = 2, REALLYAWAKE = 3

} Sleep;

//I didn't change any struct stuff yay!
typedef struct fitbit
{
    char patient[10];
    char minute[9];
    double calories;
    double distance;
    unsigned int floors;
    unsigned int heartRate;
    unsigned int steps;
    Sleep sleepLevel;
} FitbitData;


//function declarations
char* findPatient(FILE* infile);
void readData(FILE* infile, FitbitData datastruct[], char* targetpatient);
void recordData(FILE* outfile, FitbitData datastruct[]);
double totalCalories(FitbitData datastruct[]);
double totalDistance(FitbitData datastruct[]);
unsigned int computeHeart(FitbitData datastruct[]);
unsigned int floorsWalked(FitbitData datastruct[]);
unsigned int totalSteps(FitbitData datastruct[]);
void shiftStrings(char str[], int start);
unsigned int badSleep(FitbitData datastruct[], char* start, char* end);