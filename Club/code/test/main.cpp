//#include<fstream>
#pragma warning(disable:4996)
#include"test.h"

int main()
{
	//������ļ�
	freopen("./output_condition.txt", "w", stdout);	

	int stu_num = read_information();          //��ȡ������ɵ�����
	int stu_depart = get_depart_num();         //��ȡ��������
	compute(stu_num);                          // ����Ȩ��ֵ
	match(stu_num);                            //ƥ���㷨
	print_match_result(stu_num, stu_depart);   //���ƥ����
}
