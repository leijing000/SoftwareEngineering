#pragma once
#include<iostream>

using namespace std;

int read_information();                                //��ȡ������ɵ�����
int get_depart_num();                                  //��ȡ��������
void compute(int stu_num);                             //����Ȩ��ֵ
void match(int stu_num);                               //ƥ���㷨
void print_match_result(int stu_num, int depart_num);  //���ƥ����
