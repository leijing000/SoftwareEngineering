#pragma once
#include<iostream>
#include<ctime>
#define SIZE 9

using namespace std;

void init();//��ʼ������
void print();//��ӡ��������
bool isRightplace(int row, int col);//��֤�Ƿ���Է���
bool generate(int mark);//��������,markΪ��ǰλ�ñ�ţ�0-80�� 