//#include<fstream>
#pragma warning(disable:4996)
#include<fstream>
#include"generate.h"

int main()
{
	//生成随机数据

	freopen("./import.txt", "w", stdout);
	int depart_num, stu_num;
	cin >> depart_num >> stu_num;
	generate(depart_num, stu_num);
	print_generate_information(depart_num, stu_num); 
}
