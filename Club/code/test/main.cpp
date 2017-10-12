//#include<fstream>
#pragma warning(disable:4996)
#include"test.h"

int main()
{
	//输出到文件
	freopen("./output_condition.txt", "w", stdout);	

	int stu_num = read_information();          //读取随机生成的数据
	int stu_depart = get_depart_num();         //获取部门数量
	compute(stu_num);                          // 计算权重值
	match(stu_num);                            //匹配算法
	print_match_result(stu_num, stu_depart);   //输出匹配结果
}
