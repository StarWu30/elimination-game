#pragma once

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <map>
using namespace std;

class Chessboard {
public:
	int K;			// 方块种类数量
	int M;			// 行数
	int N;			// 列数
	int pair;		// 连对
	int isUpdate;	// 更新标志
	int maxsum;		// 最大得分
	int maxstep;	// 最大步数
	int **matrix;	// 主棋盘
	int **sup;		// 辅助数组， 用于记录被消除的方块
	int **temp;		// 临时棋盘
	int **road;		// 路径
	int branch;		// 分支
	map<string, int> map;
	
	// 无参构造函数
	Chessboard() {
		K = 0;
		M = 0;
		N = 0;
		isUpdate = 0;
		maxsum = 0;
		branch = 0;
	}

	// 有参构造函数
	Chessboard(int k, int m, int n) {
		K = k;
		M = m;
		N = n;
		isUpdate = 0;
		maxsum = 0;
		branch = 0;
		matrix = new int*[M];
		sup = new int*[M];
		temp = new int*[M];
		for (int i = 0; i < M; i++) {
			matrix[i] = new int[N];
			sup[i] = new int[N];
			temp[i] = new int[N];
		}
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				temp[i][j] = matrix[i][j] = sup[i][j] = 0;
			}
		}
	
	}



	// 初始化棋盘（手动输入）
	void Matrix_init() {
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				cin >> matrix[i][j];
			}
		}
		calc_pair(matrix);
	}

	// 初始化棋盘（随机输入）
	void Matrix_random() {
		int i, j, random;
		srand((unsigned)time(NULL));
		for (i = 0; i < M; i++)
			for (j = 0; j < N; j++)
				matrix[i][j] = 0;
		for (i = 0; i < M; i++) {
			for (j = 0; j < N; j++) {
				matrix[i][j] = (rand()*rand()) % K + 1;
				if (check(i, j))
					j--;
			}
		}
		calc_pair(matrix);
	}

	// 初始化路径数组
	void Road_init(int step) {
		road = new int*[step];
		for (int i = 0; i < step; i++)
			road[i] = new int[4];
		for (int i = 0; i < step; i++)
			for (int j = 0; j < 4; j++)
				road[i][j] = -1;
	}

	// 打印棋盘
	void Matrix_print(int **a) {
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				cout << a[i][j] << " ";
			}
			cout << endl;
		}
	}

	// 打印路径
	void Road_print(int **road, int step) {
		cout << "exchange road is ";
		for (int p = 0; p < step; p++) {
			printf("(%d,%d)<->(%d,%d)  ", road[p][0], road[p][1], road[p][2], road[p][3]);
		}
		cout << endl;
	}

	// 复制棋盘
	void copy(int **a, int **b) {
		int i, j;
			for (i = 0; i < M; i++)
				for (j = 0; j < N; j++)
					a[i][j] = b[i][j];
	}
	
	// 复制路径
	void copy_road(int **a, int **b) {
		for(int i = 0; i < maxstep; i++)
			for (int j = 0; j < 4; j++) {
				a[i][j] = b[i][j];
			}
	}

	// 更新棋盘
	void update() {
		// 置零
		for (int i = 0; i < M; i++) {
			for (int j = 0; j < N; j++) {
				if (sup[i][j] == 1) {
					matrix[i][j] = 0;
					sup[i][j] = 0;
				}
			}
		}

		// 下落
		for (int j = 0; j < N; j++) {
			for (int i = M - 1; i > 0; i--) {
				if (matrix[i][j] == 0) {
					int k = i;
					while (k - 1 >= 0) {
						if (matrix[k - 1][j] != 0) {
							matrix[i][j] = matrix[k - 1][j];
							matrix[k - 1][j] = 0;
							break;
						}
						k--;
					}
				}
			}
		}
		// 打印更新棋盘过程
		/*cout << "update:" << endl;
		Matrix_print(matrix);
		cout << endl;*/
	}

	// 计算同色对数量
	void calc_pair(int **matrix) {
		pair = 0;
		for(int i = 0; i < M; i++)
			for (int j = 0; j < N; j++) {
				if (j < N - 1 && matrix[i][j] == matrix[i][j + 1])
					pair++;
				if (i < M - 1 && matrix[i][j] == matrix[i + 1][j])
					pair++;
			}
	}

	// 全部遍历检查
	int check_all() {
		int i, j, k, index, combo, sum;
		sum = 0;
		for (i = 0; i < M; i++) {
			combo = 1;
			for (j = 0; j < N - 1; j++) {
				if (matrix[i][j] != 0 && matrix[i][j] == matrix[i][j + 1]) {
					if (combo == 1)
						index = j;
					combo++;
				} else {
					if (combo >= 3) {
						sum += get_score(combo);
						isUpdate = 1;
						for (k = 0; k < combo; index++, k++) {
							sup[i][index] = 1;
						}
					}
					combo = 1;
				}
			}
			sum += get_score(combo);
			if (combo >= 3) {
				isUpdate = 1;
				for (k = 0; k < combo; index++, k++) {
					sup[i][index] = 1;
				}
			}
		}
		for (j = 0; j < N; j++) {
			combo = 1;
			for (i = 0; i < M - 1; i++) {
				if (matrix[i][j] != 0 && matrix[i][j] == matrix[i + 1][j]) {
					if (combo == 1)
						index = i;
					combo++;
				} else {
					if (combo >= 3) {
						sum += get_score(combo);
						isUpdate = 1;
						for (k = 0; k < combo; index++, k++) {
							sup[index][j] = 1;
						}
					}
					combo = 1;
				}
			}
			sum += get_score(combo);
			if (combo >= 3) {
				isUpdate = 1;
				for (k = 0; k < combo; index++, k++) {
					sup[index][j] = 1;
				}
			}
		}
		return sum;
	}

	// 单点检查
	int check(int x, int y) {
		int i, j, k, index, combo, sum;
		combo = 1;
		sum = 0;
		for (i = x - 2; i < x + 2; i++) {
			if (i < 0 || i >= M-1 ) {
				continue;
			}
			if (matrix[i][y] == 0) {
				continue;
			}
			if (matrix[i][y] == matrix[i + 1][y]) {
				if (combo == 1) {
					index = i;
				}
				combo++;
			} else {
				if (combo >= 3) {
					sum += get_score(combo);
					isUpdate = 1;
					for (k = 0; k < combo; index++, k++) 
						sup[index][y] = 1;
					
				}
				combo = 1;
			}
		}
		
		sum += get_score(combo);
		if (combo >= 3) {
			isUpdate = 1;
			for (k = 0; k < combo; index++, k++) {
				sup[index][y] = 1;
			}
		}
		combo = 1;
		for (j = y - 2; j < y + 2; j++) {
			if (j < 0 || j >= N-1 ) {
				continue;
			}
			if (matrix[x][j] == 0)
				continue;
			if (matrix[x][j] == matrix[x][j + 1]) {
				if (combo == 1) {
					index = j;
				}
				combo++;
			} else {
				if (combo >= 3) {
					sum += get_score(combo);
					isUpdate = 1;
					for (k = 0; k < combo; index++, k++) {
						sup[x][index] = 1;
					}
				}
				combo = 1;
			}
		}
		sum += get_score(combo);
		if (combo >= 3) {
			isUpdate = 1;
			for (k = 0; k < combo; index++, k++) {
				sup[x][index] = 1;
			}
		}
		return sum;
	}

	// 右移
	int move_right(int p, int q) {
		swap(matrix[p][q], matrix[p][q + 1]);
		return check(p, q) + check(p, q + 1);
	}

	// 下移
	int move_down(int p, int q) {
		swap(matrix[p][q], matrix[p + 1][q]);
		return check(p, q) + check(p + 1, q);
	}

	// 返回计算得分
	int get_score(int combo) {
		switch (combo) {
		case 1:
		case 2:
			return 0;
		case 3:
			return 1;
		case 4:
			return 4;
		default:
			return 10;
		}
	}

	// 设置最大得分
	void set_maxsum(int sum) {
		if (sum < 0)
			maxsum = 0;
		else
			maxsum = sum;
	}

	// 返回最大值
	int get_maxsum() {
		return maxsum;
	}

	// 返回同色对数量
	int get_pairnum() {
		return pair;
	}

	// 返回分支数量
	int get_branch() {
		return branch;
	}

	// 设置最大步数
	void set_maxstep(int step) {
		if (step <= 0)
			maxstep = 1;
		else
			maxstep = step;
	}

	// 一步操作
	void Exchange_one() {
		int i, j;
		int sum;
		copy(temp, matrix);
		for (i = 0; i < M; i++) {
			for (j = 0; j < N; j++) {
				sum = 0;
				// 列交换
				if (j < N - 1 && matrix[i][j] != 0 && matrix[i][j + 1] != 0
					&& matrix[i][j] != matrix[i][j + 1]) {
					/*cout << endl;
					cout << matrix[i][j] << " " << matrix[i][j + 1] << endl;
					Matrix_print(matrix);
					cout << endl;*/
					sum += move_right(i, j);
				}
				// 判断sup数组是否有1，有就更新棋盘，棋子下落
				// 全部遍历搜索，加分，再判断有没有下落
				while (isUpdate) {
					update();
					isUpdate = 0;
					sum += check_all();
				}
				// 更新最大分
				if (sum > maxsum) {
					maxsum = sum;
					Matrix_print(matrix);
					printf("(%d,%d)和(%d,%d)列交换，最大得分为%d\n", i, j, i, j+1, maxsum);
				}
				sum = 0;
				copy(matrix, temp);

				/*Matrix_print(matrix);
				cout << endl;*/
				// 行交换
				if (i < M - 1 && matrix[i][j] != 0 && matrix[i+1][j] != 0
					&& matrix[i][j] != matrix[i+1][j]){
					sum += move_down(i, j);
				}
				// 判断sup数组是否有1，有就更新棋盘，棋子下落
				// 全部遍历搜索，加分，再判断有没有下落
				while (isUpdate) {
					update();
					isUpdate = 0;
					sum += check_all();
				}
				// 更新最大分
				if (sum > maxsum) {
					maxsum = sum;
					Matrix_print(matrix);
					printf("(%d,%d)和(%d,%d)行交换，最大得分为%d\n", i, j, i+1, j, maxsum);
				}
				// 重置棋盘
				copy(matrix, temp);
			}
		}
	}

	// 多步操作：原回溯法
	int Exchange_more(int step, int current_sum) {
		if (step >= maxstep)
			return 0;
		int i, j;
		int score;
		int maxsum_temp = current_sum;
		int sum1, sum2, max;    // 
		int **matrix_t;
		int **road_t;
		branch++;
		max = 0;  // 
		matrix_t = new int*[M];
		road_t = new int*[maxstep];
		for (i = 0; i < maxstep; i++) {
			road_t[i] = new int[4];
		}
		for (i = 0; i < M; i++)
			matrix_t[i] = new int[N];
		copy(matrix_t, matrix);
		copy_road(road_t, road);

		for (i = 0; i < M; i++) {
			for (j = 0; j < N; j++) {
				// 列交换
				if (j < N - 1 && matrix[i][j] != 0 && matrix[i][j + 1] != 0
					&& matrix[i][j] != matrix[i][j + 1]) {
					score = move_right(i, j);
					if (score == 0)
						swap(matrix[i][j], matrix[i][j + 1]);		// 还原回移动前
					else {
						sum1 = 0;		//
						current_sum += score;
						sum1 += score;   //
						while (isUpdate) {
							update();
							isUpdate = 0;
							int t = check_all(); //
							current_sum += t;  //
							sum1 += t;   // 
						}
						road[step][0] = i;
						road[step][1] = j;
						road[step][2] = i;
						road[step][3] = j + 1;
						step++;
						//cout << "列变换，此时步数为" << step << endl;
						if (current_sum > maxsum) {
							maxsum = current_sum;
							/*cout << endl;
							Matrix_print(matrix);
							Road_print(road, step);
							cout << "current step:" << step << " maxsum:" << maxsum << endl;*/
						}
						sum1 += Exchange_more(step, current_sum);  //
						if (sum1 >= max)		//
							max = sum1;			//
						copy(matrix, matrix_t);
						copy_road(road, road_t);
						current_sum = maxsum_temp;
						step--;
					}
				}		
				// 行交换
				if (i < M - 1 && matrix[i][j] != 0 && matrix[i + 1][j] != 0
					&& matrix[i][j] != matrix[i + 1][j]) {
					score = move_down(i, j);
					if (score == 0)
						swap(matrix[i][j], matrix[i + 1][j]);		// 还原回移动前
					else {
						sum2 = 0;			//
						sum2 += score;		//
						current_sum += score;
						while (isUpdate) {
							update();
							isUpdate = 0;
							int t = check_all();	//
							current_sum += t;		//
							sum2 += t;
						}
						road[step][0] = i;
						road[step][1] = j;
						road[step][2] = i + 1;
						road[step][3] = j;
						step++;
						//cout << "行变换，此时步数为" << step << endl;
						if (current_sum > maxsum) {
							maxsum = current_sum;
							/*cout << endl;
							Matrix_print(matrix);
							Road_print(road, step);
							cout << "current step:" << step << " maxsum:" << maxsum << endl;*/
						}
							
						sum2 += Exchange_more(step, current_sum);
						if (sum2 >= max)
							max = sum2;
						copy(matrix, matrix_t);
						copy_road(road, road_t);
						current_sum = maxsum_temp;
						step--;
					}
				}	
			}
		}
		// 释放空间
		for (i = 0; i < M; i++) 
			delete matrix_t[i];
		delete matrix_t;
		for (i = 0; i < maxstep; i++)
			delete road_t[i];
		delete road_t;
		return max;
	}

	// 将二位数组转化为字符串
	string get_chess_string(int **matrix, int step) {
		string str = "0" + step;
		for (int i = 0; i < M; i++)
			for (int j = 0; j < N; j++)
				str += '0' + matrix[i][j];
		return str;
	}

	// map映射剪枝回溯法
	int Exchange_more_map(int step, int current_sum) {
		if (step >= maxstep)
			return 0;
		string s = get_chess_string(matrix, maxstep - step - 1);
		if (map.find(s) != map.end())
			return map[s];
		int i, j;
		int score;
		int maxsum_temp = current_sum;
		int sum1, sum2, max;    // 
		int **matrix_t;
		int **road_t;
		branch++;
		max = 0;  // 
		matrix_t = new int*[M];
		road_t = new int*[maxstep];
		for (i = 0; i < maxstep; i++) {
			road_t[i] = new int[4];
		}
		for (i = 0; i < M; i++)
			matrix_t[i] = new int[N];
		copy(matrix_t, matrix);
		copy_road(road_t, road);

		for (i = 0; i < M; i++) {
			for (j = 0; j < N; j++) {
				// 列交换
				if (j < N - 1 && matrix[i][j] != 0 && matrix[i][j + 1] != 0
					&& matrix[i][j] != matrix[i][j + 1]) {
					score = move_right(i, j);
					if (score == 0)
						swap(matrix[i][j], matrix[i][j + 1]);		// 还原回移动前
					else {
						sum1 = 0;		//
						current_sum += score;
						sum1 += score;   //
						while (isUpdate) {
							update();
							isUpdate = 0;
							int t = check_all(); //
							current_sum += t;  //
							sum1 += t;   // 
						}
						road[step][0] = i;
						road[step][1] = j;
						road[step][2] = i;
						road[step][3] = j + 1;
						step++;
						//cout << "列变换，此时步数为" << step << endl;
						if (current_sum > maxsum) {
							maxsum = current_sum;
							/*cout << endl;
							Matrix_print(matrix);
							Road_print(road, step);
							cout << "current step:" << step << " maxsum:" << maxsum << endl;*/
						}
						sum1 += Exchange_more_map(step, current_sum);  //
						if (sum1 >= max)		//
							max = sum1;			//
						copy(matrix, matrix_t);
						copy_road(road, road_t);
						current_sum = maxsum_temp;
						step--;
					}
				}
				// 行交换
				if (i < M - 1 && matrix[i][j] != 0 && matrix[i + 1][j] != 0
					&& matrix[i][j] != matrix[i + 1][j]) {
					score = move_down(i, j);
					if (score == 0)
						swap(matrix[i][j], matrix[i + 1][j]);		// 还原回移动前
					else {
						sum2 = 0;			//
						sum2 += score;		//
						current_sum += score;
						while (isUpdate) {
							update();
							isUpdate = 0;
							int t = check_all();	//
							current_sum += t;		//
							sum2 += t;
						}
						road[step][0] = i;
						road[step][1] = j;
						road[step][2] = i + 1;
						road[step][3] = j;
						step++;
						//cout << "行变换，此时步数为" << step << endl;
						if (current_sum > maxsum) {
							maxsum = current_sum;
							/*cout << endl;
							Matrix_print(matrix);
							Road_print(road, step);
							cout << "current step:" << step << " maxsum:" << maxsum << endl;*/
						}

						sum2 += Exchange_more_map(step, current_sum);
						if (sum2 >= max)
							max = sum2;
						copy(matrix, matrix_t);
						copy_road(road, road_t);
						current_sum = maxsum_temp;
						step--;
					}
				}
			}
		}
		// 释放空间
		for (i = 0; i < M; i++)
			delete matrix_t[i];
		delete matrix_t;
		for (i = 0; i < maxstep; i++)
			delete road_t[i];
		delete road_t;
		map[s] = max;
		return max;
	}

	// 平均值剪枝回溯法
	int Exchange_more_avg(int step, int current_sum) {
		if (step >= maxstep)
			return 0;
		int i, j;
		int score;
		int avg = 0;
		int num = 0;
		int maxsum_temp = current_sum;
		int sum, sum1, sum2, max;    // 
		int **matrix_t;
		int **road_t;
		branch++;
		sum = max = 0;  // 
		matrix_t = new int*[M];
		road_t = new int*[maxstep];
		for (i = 0; i < maxstep; i++) {
			road_t[i] = new int[4];
		}
		for (i = 0; i < M; i++)
			matrix_t[i] = new int[N];
		copy(matrix_t, matrix);
		copy_road(road_t, road);

		for (i = 0; i < M; i++) {
			for (j = 0; j < N; j++) {
				// 列交换
				if (j < N - 1 && matrix[i][j] != 0 && matrix[i][j + 1] != 0
					&& matrix[i][j] != matrix[i][j + 1]) {
					score = move_right(i, j);
					if (score == 0)
						swap(matrix[i][j], matrix[i][j + 1]);		// 还原回移动前
					else {
						sum1 = 0;		//
						current_sum += score;
						sum1 += score;   //
						while (isUpdate) {
							update();
							isUpdate = 0;
							int t = check_all(); //
							current_sum += t;  //
							sum1 += t;   // 
						}
						road[step][0] = i;
						road[step][1] = j;
						road[step][2] = i;
						road[step][3] = j + 1;
						step++;
						//cout << "列变换，此时步数为" << step << endl;
						if (current_sum > maxsum) {
							maxsum = current_sum;
							/*cout << endl;
							Matrix_print(matrix);
							Road_print(road, step);
							cout << "current step:" << step << " maxsum:" << maxsum << endl;*/
						}
						sum += sum1;
						num++;
						avg = sum / num;
						if(sum1 >= avg + (step*0.5 - 1) )
							sum1 += Exchange_more_avg(step, current_sum);
						if (sum1 >= max)		
							max = sum1;			
						copy(matrix, matrix_t);
						copy_road(road, road_t);
						current_sum = maxsum_temp;
						step--;
					}
				}
				// 行交换
				if (i < M - 1 && matrix[i][j] != 0 && matrix[i + 1][j] != 0
					&& matrix[i][j] != matrix[i + 1][j]) {
					score = move_down(i, j);
					if (score == 0)
						swap(matrix[i][j], matrix[i + 1][j]);		// 还原回移动前
					else {
						sum2 = 0;			
						sum2 += score;		
						current_sum += score;
						while (isUpdate) {
							update();
							isUpdate = 0;
							int t = check_all();	
							current_sum += t;		
							sum2 += t;
						}
						road[step][0] = i;
						road[step][1] = j;
						road[step][2] = i + 1;
						road[step][3] = j;
						step++;
						//cout << "行变换，此时步数为" << step << endl;
						if (current_sum > maxsum) {
							maxsum = current_sum;
							/*cout << endl;
							Matrix_print(matrix);
							Road_print(road, step);
							cout << "current step:" << step << " maxsum:" << maxsum << endl;*/
						}
						sum += sum2;
						num++;
						avg = sum / num;
						if(sum2 >= avg + (step * 0.5 - 1))
							sum2 += Exchange_more_avg(step, current_sum);
						if (sum2 >= max)
							max = sum2;
						copy(matrix, matrix_t);
						copy_road(road, road_t);
						current_sum = maxsum_temp;
						step--;
					}
				}
			}
		}
		// 释放空间
		for (i = 0; i < M; i++)
			delete matrix_t[i];
		delete matrix_t;
		for (i = 0; i < maxstep; i++)
			delete road_t[i];
		delete road_t;
		return max;
	}

	// 验证过程（手动输入每一步）
	void certify() {
		int i, j, p, q;
		int sum = 0;
		int t;
		cout << "共需要验证的步数：";
		cin >> t;
		for(int k = 1; k <= t; k++) {
			cout << "移动前：" << endl;
			Matrix_print(matrix);
			cout << "输入需要移动的两个坐标:";
			cin >> i >> j >> p >> q;
			swap(matrix[i][j], matrix[p][q]);
			cout << "移动后：" << endl;
			Matrix_print(matrix);
			cout << endl;
			sum += check(i, j) + check(p, q);
			while (isUpdate) {
				update();
				isUpdate = 0;
				sum += check_all();
			}
			printf("交换坐标(%d,%d)和(%d,%d)，目前得分为%d，已走了%d步\n", i, j, p, q, sum, k);
		}
		cout << "\n最终的格子布局状态为：" << endl;
		Matrix_print(matrix);
	}
};





