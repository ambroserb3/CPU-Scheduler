#include "stdafx.h"
#include "Header.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;



ProcessClass::ProcessClass(int _id, double _BurstTime) {
	id = _id;
	BurstTime = _BurstTime;
	RemaingTime = _BurstTime;
}//end ProcessClass Constructor

int ProcessClass::get_id() { return id; };
double ProcessClass::get_BurstTime() { return BurstTime; };
double ProcessClass::get_RemaingTime() { return RemaingTime; };

void ProcessClass::ExecutTime(double _time) {
	RemaingTime = RemaingTime - _time;
}//end ProcessClass ExecuTime



RRScheduling::RRScheduling(int _processMAXNumber) {

	//2-D Address to pointer
	ptrExecutionProcess = new double*[_processMAXNumber];

	for (int i = 0; i < _processMAXNumber; i++) {
		ptrExecutionProcess[i] = new double[3];
	}
	executionIndex = 0;
}//end RRScheduling Constructor

RRScheduling::RRScheduling(int _processMAXNumber, double _quantum) {

	//2-D Address to pointer
	ptrExecutionProcess = new double*[_processMAXNumber];

	for (int i = 0; i < _processMAXNumber; i++) {
		ptrExecutionProcess[i] = new double[3];
	}
	executionIndex = 0;
	quantum = _quantum;
}//end RRScheduling Constructor

RRScheduling::~RRScheduling() {}//解構子

void RRScheduling::setQuantum(double _quantum) { quantum = _quantum; }

void RRScheduling::setScheduling(int _process_id, double _startTime, double _endTime) {
	ptrExecutionProcess[executionIndex][0] = _process_id;
	ptrExecutionProcess[executionIndex][1] = _startTime;
	ptrExecutionProcess[executionIndex][2] = _endTime;
	executionIndex++;
}

double * RRScheduling::get_ExecutionProcess(int _index) { return ptrExecutionProcess[_index]; };
int RRScheduling::get_executionIndex() { return executionIndex; };
double RRScheduling::get_quantum() { return quantum; };



void ReadProcessInfo(const char *_FileName, ProcessClass **_processArray, int *_proceeArray_Index, const int MAX_PROCESS_NUMBER, RRScheduling *_RRSch) {
	//Open File
	try {
		fstream fin;
		fin.open(_FileName, ios::in);
		if (fin.fail()) {
			fin.close();
			cout << "\n#01-Read file opening failed.\n";
			system("pause");
			exit(1);
		}//end if
		cout << "\n#01-Read File Success" << endl;

		//Save Process Information
		const int BUFFER_SIZE = 20;
		char buffer[BUFFER_SIZE] = { NULL };
		char process_id[BUFFER_SIZE] = { NULL };
		char burst_time[BUFFER_SIZE] = { NULL };
		char quantum[BUFFER_SIZE] = { NULL };
		
		while (*_proceeArray_Index < MAX_PROCESS_NUMBER) {
			fin.getline(buffer, BUFFER_SIZE,' ');
			if (fin.eof()) {

				//遇到檔案結尾了
				for (int i = 0; i < BUFFER_SIZE; i++) {
					quantum[i] = buffer[i];
				}//end for

				_RRSch -> setQuantum( atof( quantum ));
				cout << "Quantum = " << _RRSch->get_quantum()<<endl;

				break;
			}
			else {
				//非檔案結尾
				for (int i = 0; i < BUFFER_SIZE; i++) {
					process_id[i] = buffer[i];
				}//end for
				fin.getline(burst_time, BUFFER_SIZE, '\n');
			}//end if

			//Save process information to processArray
			_processArray[*_proceeArray_Index] = new ProcessClass(atoi( process_id ), atof( burst_time ));

			//Check Save Success
			cout << "Process Id = "<< _processArray[*_proceeArray_Index]->get_id();
			cout << ", Burst Time = "<< _processArray[*_proceeArray_Index]->get_BurstTime();
			cout << endl;

			//Reset char array
			resetCharArray(process_id, BUFFER_SIZE);
			resetCharArray(burst_time, BUFFER_SIZE);
			resetCharArray(quantum, BUFFER_SIZE);

			*_proceeArray_Index += 1;

		}//end while

		fin.close();
	}
	catch (exception e) {
		cout << "\n#01-ReadProcessInfo : " << e.what() << endl;
	}//end try

}//end ReadProcessInfo



void Scheduling(ProcessClass **_processArray, int*_processArray_Index, RRScheduling *_RRSch) {

	double currentTime = 0.0;
	int NotFinshProcessNumber = *_processArray_Index; //未處理完的Process數量

	while (NotFinshProcessNumber>0) {
		for (int i = 0; i<*_processArray_Index; i++) {
			if (_processArray[i]->get_RemaingTime() > 0) {

				if (_processArray[i]->get_RemaingTime() > _RRSch->get_quantum()) {

					//如果process剩餘時間大於Quantum，則最大只能執行Quantum Time
					_RRSch->setScheduling(_processArray[i]->get_id(), currentTime, currentTime + _RRSch->get_quantum());
					_processArray[i]->ExecutTime(_RRSch->get_quantum());
					currentTime += _RRSch->get_quantum();
				}
				else {

					//如果process剩餘時間小於Quantum，則執行剩餘時間即可
					_RRSch->setScheduling(_processArray[i]->get_id(), currentTime, currentTime + _processArray[i]->get_RemaingTime());
					currentTime += _processArray[i]->get_RemaingTime();
					_processArray[i]->ExecutTime(_processArray[i]->get_RemaingTime());
					NotFinshProcessNumber -= 1;
				}//end if
			}//end if
		}//end for
	}//end while
}//end Scheduling



void WirteExecutionResult(RRScheduling *_RRSch, const char *_READ_FILE_NAME, const char *_WRITE_FILE_NAME) {

	fstream fin(_READ_FILE_NAME, ios::in);
	fstream fout(_WRITE_FILE_NAME, ios::out);

	const int BUFFER_SIZE = 20;
	char buffer[BUFFER_SIZE];

	//複製題目
	while (true) {
		fin.getline(buffer, BUFFER_SIZE, '\n');
		fout << buffer << endl;
		resetCharArray(buffer, BUFFER_SIZE);
		if (fin.eof()) { break; }
	}//end while
	fout << endl;

	for (int i = 0; i < _RRSch->get_executionIndex(); i++) {
		cout << "pId = " << _RRSch->get_ExecutionProcess(i)[0];
		cout << ", start = " << _RRSch->get_ExecutionProcess(i)[1];
		cout << ", end = " << _RRSch->get_ExecutionProcess(i)[2];
		cout << endl;

		fout << _RRSch->get_ExecutionProcess(i)[0];
		fout << " " << _RRSch->get_ExecutionProcess(i)[1];
		fout << " ~ " << _RRSch->get_ExecutionProcess(i)[2];
		fout << endl;
	}//end for

	fin.close();
	fout.close();

}//end WirteExecutionResult



void showCharArray(char *_showchar, int _charsize) {

	for (int i = 0; i < _charsize; i++) {
		if (_showchar[i] == NULL) { break; }
		cout << _showchar[i];
	}//end for

}//end showCharArray



void resetCharArray(char *_resetchar, int _charsize) {
	for (int i = 0; i < _charsize; i++) {
		_resetchar[i] = NULL;
	}//end for
}//end resetCharArray

