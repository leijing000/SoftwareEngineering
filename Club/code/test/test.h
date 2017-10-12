#pragma once
#include<iostream>

using namespace std;

int read_information();                                //读取随机生成的数据
int get_depart_num();                                  //获取部门数量
void compute(int stu_num);                             //计算权重值
void match(int stu_num);                               //匹配算法
void print_match_result(int stu_num, int depart_num);  //输出匹配结果
