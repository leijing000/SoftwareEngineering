//#include<fstream>
#pragma warning(disable:4996)
#include"test.h"

int main()
{
	//�����������
	freopen("./output_condition.txt", "w", stdout);
	int stu_num = read_information();
	compute(stu_num);
	match(stu_num);
	print_match_result(stu_num, 21);
}
