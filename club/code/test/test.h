#pragma once
#include<iostream>
#include<string>

using namespace std;

int read_information();                                       //读取随机生成的数据
void compute(int stu_num);                                    //计算权重值函数
void match(int stu_num);                                      //匹配算法
void print_match_result(int stu_num, int depart_num);         //输出匹配结果
