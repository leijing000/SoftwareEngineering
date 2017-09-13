#pragma once
#include<iostream>
#include<ctime>
#define SIZE 9

using namespace std;

void init();//初始化棋盘
void print();//打印数独棋盘
bool isRightplace(int row, int col);//验证是否可以放置
bool generate(int mark);//生成数独,mark为当前位置标号（0-80） 