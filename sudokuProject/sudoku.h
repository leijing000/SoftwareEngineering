#pragma once
#include<iostream>
#include<cstdio>
#include<ctime>
#define SIZE 9

using namespace std;

class sudoku {
public:
	int shudu[SIZE][SIZE];
	void init();//��ʼ������
	void print();//��ӡ��������
	bool isRightposition(int row, int col);//��֤�Ƿ���Է���
	bool generate(int mark);//��������,markΪ��ǰλ�ñ�ţ�0-80��
};
