#include <iostream>
#include <conio.h>
#include <vector>

using namespace std;

//---------------------������---------------------
//�����Ͻ� �� �ʿ��Ͻø� ����ϼ��� ! (�迭, �ʱ� �ε��� ��, �� �ε��� �� �Է��Ͻø� �˴ϴ�.)
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
void reffrenceTimes(const int processes, int _arrivalTime[], const int *burstTime, const int timeQuantum,					//�Է� ����
	int *waitingTime, int *turnAroundTime, double *normalizedTurnAroundTime,								//��ȯ ����
	vector<int> intervalRunTime, vector<int> runSequence, vector<vector <int> > queueLine, vector<int> queueLineName)	//���� �� ���� �ð�, ���� ����, ��⿭(��⿭�� push���ϰ� pop ���� �����ּ��� ! ��Ͽ� ��⿭�Դϴ�)
{
	//queueLine�� ��⿭, queueLineName�� ��ٸ��� ���μ����� �̸��Դϴ� ���� ���, queueLineName{1, 0, 1}, queueLine{4, 4, 4}��� 4(1�� ���μ���) 4(0�� ���μ���) 4(1�� ���μ���)�� ���� ��ٸ��� �˴ϴ�.
	//intervalRunTime�� ������ ����ð��̸� runSequence�� ���� �����Դϴ�
	//ex : intervalRunTim[] = {3, 2}�̰� runSequence[] = {1, 0} �̸� ������ 111 00 ó�� ������ �˴ϴ�. (�� ���ڴ� 1���� ó���� �ߴٰ� ����.)
	//�� �Լ��� ������ �ּ��� ! �Ű����� ���� X
	//������ ���� �ܺ��Լ��� ������ �ʾ��� ���ڽ��ϴ� �Ф�! JS�� ��ȯ�� �� ������..
	//�׸��� �ּ��� �ڼ��� �޾��ֽø� �������� ���� ���̵� �� ���ƿ� !
	//min : �����ð� �ּҰ�		optionProcess : ���õ� ���μ���		currentTime : ���� ����ð�

	//----------------------------------���⼭���� �ڵ� �Ͻø� �˴ϴ� ---------------------------------------------------
	int currentTime = 0;
	int exitProcessCnt = 0;
	int runProcess = -1;
	bool *isInQueue = new bool[processes];
	bool isEmptyTime = false;
	int runTimeSelecter = -1;
	vector<int> queue;
	int *cloneBurstTime = new int[processes];
	//@������
	vector<double> responseRatio;
	for (int i = 0; i < processes; i++) {
		//���ð� �ʱ�ȭ
		waitingTime[i] = 0;

		//BusrtTime Ŭ�� ����.
		cloneBurstTime[i] = burstTime[i];
		isInQueue[i] = false;
	}

	while (exitProcessCnt < processes) {
		cout << "time : " << currentTime << endl;;
		//���� �ð��뿡 ������ ���� ��� Q ó��.
		for (int optionProcess = 0; optionProcess < processes; optionProcess++) {
			//�ش� ���μ��� �����ð��� ��������, ���μ��� ���� �ð��� �����ְ�, Q�ȿ� �������� ���� ���.
			if (_arrivalTime[optionProcess] == currentTime && cloneBurstTime[optionProcess] > 0 && !isInQueue[optionProcess]) {
				isInQueue[optionProcess] = true;
				queue.push_back(optionProcess);
			}
		}

		//Queue�� ��⿭ ���¸� �ٲ� ->(WT + BT) / BT ����.
		//@������.
		//����
		responseRatio.clear();
		for (int queueSwaper = 0; queueSwaper < queue.size(); queueSwaper++) responseRatio.push_back(0);
		//����
		for (int queueSwaper = 0; queueSwaper < queue.size(); queueSwaper++) {
			responseRatio[queueSwaper] = double(burstTime[queue[queueSwaper]] + waitingTime[queue[queueSwaper]]) / double(burstTime[queue[queueSwaper]]);
		}
		cout << "responseRatio[���� ��] : ";
		for (int i = 0; i < responseRatio.size(); i++) cout << responseRatio[i] << " ";
		cout << endl;
		cout << "queue[���� ��] : ";
		for (int i = 0; i < queue.size(); i++) cout << queue[i] << " ";
		cout << endl;
		//����
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
		//@�������
		cout << "responseRatio[���� ��] : ";
		for (int i = 0; i < responseRatio.size(); i++) cout << responseRatio[i] << " ";
		cout << endl;
		cout << "queue[���� ��] : ";
		for (int i = 0; i < queue.size(); i++) cout << queue[i] << " ";
		cout << endl;
		//���μ��� �Ҵ�
		//��⿭�� ������ �ֳ� ? �׸��� �������� ���μ����� ���� ?
		if (queue.size() > 0 && runProcess == -1) {
			runProcess = queue[0];
			queue.erase(queue.begin());

			runTimeSelecter++;
			intervalRunTime.push_back(0);
			runSequence.push_back(runProcess);
			isEmptyTime = false;
		}
		//��⿭���� �����鼭 ������������ �ʳ�? ==> ����ð��̳� ?
		else if (queue.size() <= 0 && runProcess == -1 && isEmptyTime == false) {
			runTimeSelecter++;
			intervalRunTime.push_back(0);
			runSequence.push_back(-1);
			isEmptyTime = true;
		}
		cout << "runProcess[���� ����] : " << runProcess << endl;
		//���� ó��
		//������ �������̳�?
		if (runProcess != -1) {
			//burstTime�� ���� ���ҳ�?
			if (cloneBurstTime[runProcess] > 0) {
				intervalRunTime[runTimeSelecter]++;
				cloneBurstTime[runProcess] --;
				//burstTime�� �� �̻� ����?
				if (cloneBurstTime[runProcess] <= 0) {
					exitProcessCnt++;
					runProcess = -1;
				}
			}
		}
		//����ð��̳�?
		else if (runProcess == -1 && isEmptyTime == true) {
			intervalRunTime[runTimeSelecter]++;
		}

		//��⿭�� ���� ���μ����� ���� �ϳ�?(���ð�)
		for (int waitProcess = 0; waitProcess < queue.size(); waitProcess++) {
			waitingTime[queue[waitProcess]] ++;
		}
		//Queue����.
		queueLine.push_back(queue);

		currentTime++;
		//----------------------------------------��� �κ�----------------------------------------------
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
	////----------------------------------������� �ڵ� �Ͻø� �˴ϴ� (������ ���� X)---------------------------------------------------
	////��°��� PPT�� �ִ� �͵�� vector<int> intervalRunTime, vector<int> runSequence, vector<int> queueLine, vector<int> queueLineName �� �ʿ��մϴ�.
	////ó�� ���� TUI �ð�ȭ
	////���� �κ��� ������ �����ּ��� ������� � �����ٸ��� ����Ͽ��� �̿� ���� �������� ǥ�� �Ǿ�� �մϴ�.

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
	//���μ��� ��, ���� �ð�, ���� �ð�, ���� �ð�(RR����)			: �Է� �޾ƾ� �� ����
	int processes;
	int *arrivalTime; int *_arrivalTime;
	int *burstTime;
	int timeQuantum = 0;
	//��� �ð�, ��� �ð�, TT/WT									: ��ȯ �޾ƾ� �� ����
	//WT = TT - BT; NTT = TT / BT
	int *waitingTime;
	int *turnAroundTime;
	double *normalizedTurnAroundTime;
	//���� �� ����ð�												: �ð�ȭ�� �ʿ��� ����
	vector<int> intervalRunTime;									//���� �ð� 
	vector<int> runSequence;										//���� ����
																	//���μ��� ����ð��� -1�� �������ּ���. 
																	//(ex : FCFS���� �� ���μ����� arrival time�� 0, 100 �̰� burst time�� 1, 1�̶�� �ι�° ���μ����� 99�� ����ð��� ���Ե˴ϴ� �� ����ð��� intervalRunTime�� push�Ͻ� ��, -1������ push���ּ���)
	vector<vector <int> > queueLine;											//��⿭ Q ���ð�
	vector<int> queueLineName;										//��⿭ Q�� ���μ��� ����(���μ��� ����);
																	//---------------------�Է� ����-----------------
																	//		�Է±����� �������� ���������� �մϴ� !

																	//���μ��� �� ����
	cout << "# of Processes : ";
	cin >> processes;
	//���μ��� �� ��ŭ �����Ҵ�
	arrivalTime = new int[processes]; _arrivalTime = new int[processes];
	burstTime = new int[processes];

	waitingTime = new int[processes];
	turnAroundTime = new int[processes];
	normalizedTurnAroundTime = new double[processes];

	//���� �ð� ����
	cout << "Arrival time : ";
	for (int i = 0; i < processes; i++) {
		cin >> arrivalTime[i];
		_arrivalTime[i] = arrivalTime[i];
	}
	//���� �ð� ����
	cout << "Burst time : ";
	for (int i = 0; i < processes; i++) {
		cin >> burstTime[i];
	}

	//RR����
	//cout << "Time quantum for RR : ";
	//---------------------��� ����-----------------
	reffrenceTimes(processes, _arrivalTime, burstTime, timeQuantum, waitingTime, turnAroundTime, normalizedTurnAroundTime, intervalRunTime, runSequence, queueLine, queueLineName);

	//�����Ҵ� ����
	delete[] arrivalTime; delete[] burstTime; delete[] waitingTime; delete[] turnAroundTime; delete[] normalizedTurnAroundTime;

	cout << "��" << endl;
	_getch();
}

void main() {
	runScheduling();
}