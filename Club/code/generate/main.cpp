//#include<fstream>
#pragma warning(disable:4996)
#include<fstream>
#include"generate.h"

int main()
{
	//������ļ�
	freopen("./import.txt", "w", stdout);

	int depart_num, stu_num;
	cin >> depart_num;
	cin >> stu_num;

	//�������ѧ���Ͳ�����Ϣ
	generate(depart_num, stu_num);
	//���������ɵĲ�����Ϣ��ѧ����Ϣ
	print_generate_information(depart_num, stu_num);	
}
