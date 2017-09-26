#pragma once
#include<iostream>
#include<cstdio>
#include<ctime>
#define SIZE 9

using namespace std;

class sudoku {
public:
	int shudu[SIZE][SIZE];
	void init();//初始化棋盘
	void print();//打印数独棋盘
	bool isRightposition(int row, int col);//验证是否可以放置
	bool generate(int mark);//生成数独,mark为当前位置标号（0-80）
};
