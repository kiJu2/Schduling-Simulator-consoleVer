#include <iostream>
#include <conio.h>
#include <vector>

using namespace std;

//---------------------퀵정렬---------------------
//정렬하실 때 필요하시면 사용하세요 ! (배열, 초기 인덱스 값, 끝 인덱스 값 입력하시면 됩니다.)
void quickSort(int *data, int start, int end) {
	if (start >= end) {
		return;
	}

	int pivot = start;
	int i = pivot + 1;
	int j = end;
	int temp;
	while (i <= j) {
		while (i <= end && data[i] <= data[pivot]) {
			i++;
		}
		while (j > start && data[j] >= data[pivot]) {
			j--;
		}
		if (i > j) {
			temp = data[j];
			data[j] = data[pivot];
			data[pivot] = temp;
		}
		else {
			temp = data[i];
			data[i] = data[j];
			data[j] = temp;
		}
	}

	quickSort(data, start, j - 1);
	quickSort(data, j + 1, end);
}

//call by reference
void reffrenceTimes(const int processes, int _arrivalTime[], const int *burstTime, const int timeQuantum,					//입력 변수
	int *waitingTime, int *turnAroundTime, double *normalizedTurnAroundTime,								//반환 변수
	vector<int> intervalRunTime, vector<int> runSequence, vector<vector <int> > queueLine, vector<int> queueLineName)	//구간 별 실행 시간, 실행 순서, 대기열(대기열은 push만하고 pop 하지 말아주세요 ! 기록용 대기열입니다)
{
	//queueLine은 대기열, queueLineName은 기다리는 프로세스의 이름입니다 예를 들어, queueLineName{1, 0, 1}, queueLine{4, 4, 4}라면 4(1번 프로세스) 4(0번 프로세스) 4(1번 프로세스)와 같이 기다리게 됩니다.
	//intervalRunTime은 구간별 실행시간이며 runSequence는 실행 순서입니다
	//ex : intervalRunTim[] = {3, 2}이고 runSequence[] = {1, 0} 이면 실행은 111 00 처럼 실행이 됩니다. (한 문자당 1개의 처리를 했다고 가정.)
	//이 함수만 수정해 주세요 ! 매개변수 수정 X
	//가급적 따로 외부함수를 만들지 않았음 좋겠습니다 ㅠㅠ! JS로 변환할 때 힘들어서요..
	//그리고 주석은 자세히 달아주시면 가독성에 도움 많이될 것 같아요 !
	//min : 도착시간 최소값		optionProcess : 선택된 프로세스		currentTime : 현재 실행시간

	//----------------------------------여기서부터 코딩 하시면 됩니다 ---------------------------------------------------
	int currentTime = 0;
	int exitProcessCnt = 0;
	int runProcess = -1;
	bool *isInQueue = new bool[processes];
	bool isEmptyTime = false;
	int runTimeSelecter = -1;
	vector<int> queue;
	int *cloneBurstTime = new int[processes];
	//@수정함
	vector<double> responseRatio;
	for (int i = 0; i < processes; i++) {
		//대기시간 초기화
		waitingTime[i] = 0;

		//BusrtTime 클론 생성.
		cloneBurstTime[i] = burstTime[i];
		isInQueue[i] = false;
	}

	while (exitProcessCnt < processes) {
		cout << "time : " << currentTime << endl;;
		//지금 시간대에 누군가 왔을 경우 Q 처리.
		for (int optionProcess = 0; optionProcess < processes; optionProcess++) {
			//해당 프로세스 도착시간이 지났으며, 프로세스 실행 시간이 남아있고, Q안에 존재하지 않을 경우.
			if (_arrivalTime[optionProcess] == currentTime && cloneBurstTime[optionProcess] > 0 && !isInQueue[optionProcess]) {
				isInQueue[optionProcess] = true;
				queue.push_back(optionProcess);
			}
		}

		//Queue의 대기열 상태를 바꿈 ->(WT + BT) / BT 기준.
		//@수정함.
		//생성
		responseRatio.clear();
		for (int queueSwaper = 0; queueSwaper < queue.size(); queueSwaper++) responseRatio.push_back(0);
		//대입
		for (int queueSwaper = 0; queueSwaper < queue.size(); queueSwaper++) {
			responseRatio[queueSwaper] = double(burstTime[queue[queueSwaper]] + waitingTime[queue[queueSwaper]]) / double(burstTime[queue[queueSwaper]]);
		}
		cout << "responseRatio[스왑 전] : ";
		for (int i = 0; i < responseRatio.size(); i++) cout << responseRatio[i] << " ";
		cout << endl;
		cout << "queue[스왑 전] : ";
		for (int i = 0; i < queue.size(); i++) cout << queue[i] << " ";
		cout << endl;
		//스왑
		for (int queueSwaper = 0; queueSwaper < queue.size(); queueSwaper++) {
			for (int queueInSwaper = queueSwaper + 1; queueInSwaper < queue.size(); queueInSwaper++) {
				if (responseRatio[queueSwaper] < responseRatio[queueInSwaper]) {
					double dtemp;
					int itemp;
					dtemp = responseRatio[queueSwaper];
					responseRatio[queueSwaper] = responseRatio[queueInSwaper];
					responseRatio[queueInSwaper] = dtemp;

					itemp = queue[queueSwaper];
					queue[queueSwaper] = queue[queueInSwaper];
					queue[queueInSwaper] = itemp;
				}
			}
		}
		//@여기까지
		cout << "responseRatio[스왑 후] : ";
		for (int i = 0; i < responseRatio.size(); i++) cout << responseRatio[i] << " ";
		cout << endl;
		cout << "queue[스왑 후] : ";
		for (int i = 0; i < queue.size(); i++) cout << queue[i] << " ";
		cout << endl;
		//프로세서 할당
		//대기열에 누군가 있냐 ? 그리고 실행중인 프로세스가 없냐 ?
		if (queue.size() > 0 && runProcess == -1) {
			runProcess = queue[0];
			queue.erase(queue.begin());

			runTimeSelecter++;
			intervalRunTime.push_back(0);
			runSequence.push_back(runProcess);
			isEmptyTime = false;
		}
		//대기열에도 없으면서 실행중이지도 않냐? ==> 공백시간이냐 ?
		else if (queue.size() <= 0 && runProcess == -1 && isEmptyTime == false) {
			runTimeSelecter++;
			intervalRunTime.push_back(0);
			runSequence.push_back(-1);
			isEmptyTime = true;
		}
		cout << "runProcess[실행 도중] : " << runProcess << endl;
		//실행 처리
		//누군가 실행중이냐?
		if (runProcess != -1) {
			//burstTime이 아직 남았냐?
			if (cloneBurstTime[runProcess] > 0) {
				intervalRunTime[runTimeSelecter]++;
				cloneBurstTime[runProcess] --;
				//burstTime이 더 이상 없냐?
				if (cloneBurstTime[runProcess] <= 0) {
					exitProcessCnt++;
					runProcess = -1;
				}
			}
		}
		//공백시간이냐?
		else if (runProcess == -1 && isEmptyTime == true) {
			intervalRunTime[runTimeSelecter]++;
		}

		//대기열에 남은 프로세스가 존재 하냐?(대기시간)
		for (int waitProcess = 0; waitProcess < queue.size(); waitProcess++) {
			waitingTime[queue[waitProcess]] ++;
		}
		//Queue저장.
		queueLine.push_back(queue);

		currentTime++;
		//----------------------------------------출력 부분----------------------------------------------
		cout << "runProcess : " << runProcess << endl;
		cout << "exitProcessCnt : " << exitProcessCnt << endl;

		cout << "runProcessCuurentBurstTime : ";
		for (int i = 0; i < processes; i++) cout << cloneBurstTime[i] << " ";
		cout << endl;

		cout << "queue : ";
		for (int i = 0; i < queue.size(); i++) cout << queue[i] << " ";
		cout << endl;

		cout << "isInQueue : ";
		for (int i = 0; i < processes; i++) cout << isInQueue[i] << " ";
		cout << endl;

		cout << "---------------------" << endl;
	}
	for (int i = 0; i < processes; i++) {
		turnAroundTime[i] = waitingTime[i] + burstTime[i];
		normalizedTurnAroundTime[i] = (double)turnAroundTime[i] / (double)burstTime[i];
	}
	cout << "---processState---" << endl;

	for (int i = 0; i < queueLine.size(); i++) {
		cout << i << " time has waitingRow : ";
		for (int j = 0; j < queueLine[i].size(); j++)
			cout << queueLine[i][j] << " ";
		cout << endl;
	}
	cout << "waitingTime : ";
	for (int i = 0; i < processes; i++) cout << waitingTime[i] << " ";
	cout << endl;

	cout << "turnaroundTime : ";
	for (int i = 0; i < processes; i++) cout << turnAroundTime[i] << " ";
	cout << endl;

	cout << "nomarlizeTurnaroundTime : ";
	for (int i = 0; i < processes; i++) cout << normalizedTurnAroundTime[i] << " ";
	cout << endl;

	cout << "\t" << endl;
	delete[] cloneBurstTime;
	////----------------------------------여기까지 코딩 하시면 됩니다 (나머지 수정 X)---------------------------------------------------
	////출력값은 PPT의 있는 것들과 vector<int> intervalRunTime, vector<int> runSequence, vector<int> queueLine, vector<int> queueLineName 가 필요합니다.
	////처리 순서 TUI 시각화
	////이하 부분은 지우지 말아주세요 결과값은 어떤 스케줄링을 사용하여도 이와 같은 형식으로 표시 되어야 합니다.

	cout << "TUI : ";
	for (int i = 0; i < intervalRunTime.size(); i++) {
		for (int j = 0; j < intervalRunTime[i]; j++) {
			cout << runSequence[i] << " ";
		}
	}
	cout << endl;
	for (int i = 0; i < intervalRunTime.size(); i++) {
		cout << "intervalRunTime[" << i << "] : " << intervalRunTime[i] << "\t";
	}
	cout << endl;

	for (int i = 0; i < runSequence.size(); i++) {
		cout << "runSequence[" << i << "] : " << runSequence[i] << "\t";
	}
	cout << endl;
}

void runScheduling() {
	//프로세스 수, 도착 시간, 실행 시간, 제한 시간(RR전용)			: 입력 받아야 할 변수
	int processes;
	int *arrivalTime; int *_arrivalTime;
	int *burstTime;
	int timeQuantum = 0;
	//대기 시간, 결과 시간, TT/WT									: 반환 받아야 할 변수
	//WT = TT - BT; NTT = TT / BT
	int *waitingTime;
	int *turnAroundTime;
	double *normalizedTurnAroundTime;
	//구간 별 실행시간												: 시각화에 필요한 변수
	vector<int> intervalRunTime;									//실행 시간 
	vector<int> runSequence;										//실행 순서
																	//프로세스 공백시간은 -1로 설정해주세요. 
																	//(ex : FCFS기준 두 프로세스의 arrival time이 0, 100 이고 burst time이 1, 1이라면 두번째 프로세스는 99의 공백시간을 갖게됩니다 이 공백시간을 intervalRunTime에 push하실 때, -1값으로 push해주세요)
	vector<vector <int> > queueLine;											//대기열 Q 대기시간
	vector<int> queueLineName;										//대기열 Q의 프로세스 순서(프로세스 네임);
																	//---------------------입력 구간-----------------
																	//		입력구간은 수정하지 않으셨으면 합니다 !

																	//프로세스 수 지정
	cout << "# of Processes : ";
	cin >> processes;
	//프로세스 수 만큼 동적할당
	arrivalTime = new int[processes]; _arrivalTime = new int[processes];
	burstTime = new int[processes];

	waitingTime = new int[processes];
	turnAroundTime = new int[processes];
	normalizedTurnAroundTime = new double[processes];

	//도착 시간 지정
	cout << "Arrival time : ";
	for (int i = 0; i < processes; i++) {
		cin >> arrivalTime[i];
		_arrivalTime[i] = arrivalTime[i];
	}
	//실행 시간 지정
	cout << "Burst time : ";
	for (int i = 0; i < processes; i++) {
		cin >> burstTime[i];
	}

	//RR전용
	//cout << "Time quantum for RR : ";
	//---------------------출력 구간-----------------
	reffrenceTimes(processes, _arrivalTime, burstTime, timeQuantum, waitingTime, turnAroundTime, normalizedTurnAroundTime, intervalRunTime, runSequence, queueLine, queueLineName);

	//동적할당 해제
	delete[] arrivalTime; delete[] burstTime; delete[] waitingTime; delete[] turnAroundTime; delete[] normalizedTurnAroundTime;

	cout << "끝" << endl;
	_getch();
}

void main() {
	runScheduling();
}