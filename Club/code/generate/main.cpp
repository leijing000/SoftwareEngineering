//#include<fstream>
#pragma warning(disable:4996)
#include<fstream>
#include"generate.h"

int main()
{
	//输出到文件
	freopen("./import.txt", "w", stdout);

	int depart_num, stu_num;
	cin >> depart_num;
	cin >> stu_num;

	//随机生成学生和部门信息
	generate(depart_num, stu_num);
	//输出随机生成的部门信息和学生信息
	print_generate_information(depart_num, stu_num);	
}
