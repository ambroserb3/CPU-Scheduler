#pragma once
#include <string>
using namespace std;



//Save Process Information
class ProcessClass {
private:
	int id;
	double BurstTime;
	double RemaingTime;
public:
	ProcessClass(int _id, double _BurstTime);
	int get_id();
	double get_BurstTime();
	double get_RemaingTime();
	void ExecutTime(double _time);
};



//Save Scheduling Information
class RRScheduling {
private:
	double **ptrExecutionProcess; //double point double[MAX_PROCESS_NUMBER] point double[3]
	double quantum;
	int executionIndex;
public:
	RRScheduling() {};
	RRScheduling(int);
	RRScheduling(int _proccessMAXNumber, double _quantum);
	~RRScheduling();
	void setQuantum(double);
	void setScheduling(int _process_id, double _startTime, double _endTime);
	double * get_ExecutionProcess(int);
	int get_executionIndex();
	double get_quantum();
};



//Max proccess number
const int MAX = 100;



//Function
void ReadProcessInfo(const char *FileName, ProcessClass **, int *, const int, RRScheduling *);
void Scheduling(ProcessClass **proccesArrayAddress, int *, RRScheduling *Scheduling);
void WirteExecutionResult(RRScheduling *, const char *, const char *);
void showCharArray(char *, int);
void resetCharArray(char *, int);



