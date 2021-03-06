// 消消乐.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include "method.h"
#include <Windows.h>

int main()
{
	int K, M, N;
	int score;
	int step;
	double array[100];
	double number[100];
	LARGE_INTEGER freq;
	LARGE_INTEGER begintime;
	LARGE_INTEGER endtime;
	for (int i = 0; i < 100; i++)
		number[i] = array[i] = 0;
	cin >> K >> M >> N;
	/*for (int i = 0; i < 100; i++) {
		//Sleep(1000);
		Chessboard chess(K, M, N);
		chess.Matrix_random();
		chess.set_maxsum(0);
		chess.set_maxstep(10);
		chess.Road_init(10);
		chess.Exchange_more(0, 0);
		cout << chess.get_pairnum() << "  " << chess.get_maxsum() << endl;
		if (chess.get_maxsum()) {
			number[chess.get_pairnum()]++;
			array[chess.get_pairnum()] += chess.get_maxsum();
		}
	}
	
	for (int i = 0; i < 100; i++) {
		if (number[i] != 0) {
			array[i] = array[i] / number[i];
			if (array[i] >= 1)
				cout << i << ": " << array[i] << endl;
		}
	}*/
	
	Chessboard chess(K, M, N);
	Chessboard chess1(K, M, N);
	Chessboard chess2(K, M, N);
	// 随机生成棋盘
	chess.Matrix_random();
	chess.Matrix_print(chess.matrix);

	// 手动输入棋盘
	//chess.Matrix_init();
	chess1.copy(chess1.matrix, chess.matrix);
	chess1.calc_pair(chess1.matrix);
	chess2.copy(chess2.matrix, chess.matrix);
	// 一步操作
	//chess.Exchange_one();
	//cout << "一步的最大分数为：" << chess.get_maxsum() << endl;

	// 多步操作
	cout << "最大步数为：";
	cin >> step;
	
	// 未优化回溯法
	/*QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&begintime);
	chess.set_maxsum(0);
	chess.set_maxstep(step);
	chess.Road_init(step);
	score = chess.Exchange_more(0, 0);
	QueryPerformanceCounter(&endtime);
	//cout << "同色对数量为：" << chess.get_pairnum() << endl;
	cout
		<< "原回溯法总耗时为"
		<< (double)(endtime.QuadPart - begintime.QuadPart) * 1000 / freq.QuadPart
		<< "ms"
		<< endl;
	cout << "原回溯法移动" << step << "步的最大分数为：" << chess.get_maxsum() << endl;
	cout <<  "最大分数:"  <<  score << endl;
	cout << "原回溯法分支数：" << chess.get_branch() << endl;*/

	// map映射剪枝回溯法
	//chess1.Matrix_print(chess1.matrix);
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&begintime);
	chess1.set_maxsum(0);
	chess1.set_maxstep(step);
	chess1.Road_init(step);
	score = chess1.Exchange_more_map(0, 0);
	QueryPerformanceCounter(&endtime);
	//cout << "同色对数量为：" << chess1.get_pairnum() << endl;
	cout
		<< "map映射剪枝回溯法总耗时为"
		<< (double)(endtime.QuadPart - begintime.QuadPart) * 1000 / freq.QuadPart
		<< "ms"
		<< endl;
	cout << "map映射剪枝回溯法移动" << step << "步的最大分数为：" << chess1.get_maxsum() << endl;
	cout << "最大分数:" << score << endl;
	cout << "map映射剪枝回溯法分支数：" << chess1.get_branch() << endl;
	
	// 平均值剪枝回溯法
	//chess2.Matrix_print(chess2.matrix);
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&begintime);
	chess2.set_maxsum(0);
	chess2.set_maxstep(step);
	chess2.Road_init(step);
	score = chess2.Exchange_more_avg(0, 0);
	QueryPerformanceCounter(&endtime);
	cout
		<< "平均值剪枝回溯法总耗时为"
		<< (double)(endtime.QuadPart - begintime.QuadPart) * 1000 / freq.QuadPart
		<< "ms"
		<< endl;
	cout << "平均值剪枝回溯法移动" << step << "步的最大分数为：" << chess2.get_maxsum() << endl;
	cout << "最大分数:" << score << endl;
	cout << "平均值剪枝回溯法分支数：" << chess2.get_branch() << endl;

	//验证
	//chess.certify();
	return 0;
}

