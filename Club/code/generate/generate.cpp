#include"generate.h"
#include"json.h"
#include<ctime>
#include<queue>
#include<fstream>
#include<string>

using namespace std;

//学生结构体
struct Student {
	int student_no;                  //学生编号
	string available_schedules[49];  //空闲时间
	string tags[16];                 //兴趣标签
	int want[5];                     //五个志愿   
	double point;                    //绩点0-5

	int cur_want = 0;                //当前正在进行分配的志愿
	int department_no[25];			 //学生所属部门,0表示不在该部门，1表示已被该部门选中
	int choose_num;                  //已中选部门数

	int match_schedule[5];			 //记录活动时间匹配情况
	int match_tag[5];				 //记录标签匹配情况
	double match_best[5];			 //记录加权后的值（活动时间*50% + 兴趣*30% + point*20%）

	int schedule_num = 0;            //时间段个数
	int tag_num = 0;                 //标签个数

}stu[1000];

//部门结构体
struct Departments {
	int department_no;              //部门编号
	int number_limit;               //需要的学生数上限[0-15]
	string tags[16];                //兴趣标签
	string event_schedules[49];     //常规活动时间段

	int choose_num;                 //属于该部门的学生数
	int student_no[20];             //属于该部门的学生编号

	int schedule_num = 0;           //时间段个数
	int tag_num = 0;                //标签个数
}depart[100];

//兴趣标签
string tag[16] = {
	"reading","programming","film","English","music",
	"dance","basketball","football","table tennis",
	"chess","singing","volunteer","math","guitar","painting",
	"running"
};

//时间段
string free_time[49]{
	"Mon.8:00-10:00","Mon.10:00-12:00","Mon.12:00-14:00",
	"Mon.14:00-16:00","Mon.16:00-18:00","Mon.18:00-20:00",
	"Mon.20:00-22:00",
	"Tues.8:00-10:00","Tues.10:00-12:00","Tues.12:00-14:00",
	"Tues.14:00-16:00","Tues.16:00-18:00","Tues.18:00-20:00",
	"Tues.20:00-22:00",
	"Wed.8:00-10:00","Wed.10:00-12:00","Wed.12:00-14:00",
	"Wed.14:00-16:00","Wed.16:00-18:00","Wed.18:00-20:00",
	"Wed.20:00-22:00",
	"Thur.8:00-10:00","Thur.10:00-12:00","Thur.12:00-14:00",
	"Thur.14:00-16:00","Thur.16:00-18:00","Thur.18:00-20:00",
	"Thur.20:00-22:00",
	"Fri.8:00-10:00","Fri.10:00-12:00","Fri.12:00-14:00",
	"Fri.14:00-16:00","Fri.16:00-18:00","Fri.18:00-20:00",
	"Fri.20:00-22:00",
	"Sat.8:00-10:00","Sat.10:00-12:00","Sat.12:00-14:00",
	"Sat.14:00-16:00","Sat.16:00-18:00","Sat.18:00-20:00",
	"Sat.20:00-22:00",
	"Sun.8:00-10:00","Sun.10:00-12:00","Sun.12:00-14:00",
	"Sun.14:00-16:00","Sun.16:00-18:00","Sun.18:00-20:00",
	"Sun.20:00-22:00"
};

void generate(int depart_num, int stu_num)
{
	//初始化随机种子
	srand((unsigned)time(NULL));

	//部门随机数据生成
	for (int i = 0; i < depart_num; i++)
	{
		//时间状态，使用过的置为1
		int time_status[49] = { 0 };

		//标签的状态，使用过的置为1
		int tag_status[16] = { 0 };

		//部门编号[1-depart_num]
		depart[i].department_no = i + 1;

		//需要学生个数上限[0-15]
		depart[i].number_limit = rand() % 16;

		//随机生成兴趣标签[2-4个]
		for (int j = 0; j < rand() % 3 + 2; j++) {
			while (1)
			{
				int index = rand() % 16;
				if (tag_status[index] == 0)
				{
					depart[i].tags[depart[i].tag_num] = tag[index];
					tag_status[index] = 1;
					depart[i].tag_num++;
					break;
				}
			}
		}

		//随机生成部门活动时间[3-7个]
		for (int j = 0; j < rand() % 5 + 3; j++) {
			while (1)
			{
				int index = rand() % 49;
				if (time_status[index] == 0)
				{
					depart[i].event_schedules[depart[i].schedule_num] = free_time[index];
					time_status[index] = 1;
					depart[i].schedule_num++;
					break;
				}
			}
		}
	}

	//学生随机数据生成
	for (int i = 0; i < stu_num; i++)
	{
		//时间状态，使用过时间段的置为1
		int time_status[49] = { 0 };

		//标签的状态，使用过的标签置为1
		int tag_status[16] = { 0 };

		//学生编号[1-stu_num]
		stu[i].student_no = i + 1;

		//兴趣标签随机生成2-4个
		for (int j = 0; j < rand() % 3 + 2; j++) {
			while (1)
			{
				int index = rand() % 16;
				if (tag_status[index] == 0)
				{
					stu[i].tags[stu[i].tag_num] = tag[index];
					tag_status[index] = 1;
					stu[i].tag_num++;
					break;
				}
			}
		}

		//空闲时间随机生成3-7个
		for (int j = 0; j < rand() % 5 + 3; j++) {
			while (1)
			{
				int index = rand() % 49;
				if (time_status[index] == 0)
				{
					stu[i].available_schedules[stu[i].schedule_num] = free_time[index];
					time_status[index] = 1;
					stu[i].schedule_num++;
					break;
				}
			}
		}

		//志愿随机生成[对应部门号depart_num]
		for (int j = 0; j < 5; j++) {
			stu[i].want[j] = rand() % depart_num + 1;
		}

		//绩点随机生成
		stu[i].point = (rand() % 5000) / 1000.0;
	}
}



void print_generate_information(int depart_num, int stu_num)
{
	Json::Value root;
	Json::Value Stu;
	Json::Value Dep;
	for (int i = 0; i < depart_num; i++)
	{
		//定义department及各属性
		Json::Value department;
		Json::Value event_schedules;
		Json::Value tags;

		//给各属性赋值
		for (int j = 0; j < depart[i].schedule_num; j++)
		{
			event_schedules[j] = Json::Value(depart[i].event_schedules[j]);
		}
		for (int j = 0; j < depart[i].tag_num; j++)
		{
			tags[j] = Json::Value(depart[i].tags[j]);
		}

		department["department_no"] = Json::Value(depart[i].department_no);
		department["number_limit"] = Json::Value(depart[i].number_limit);
		department["event_schedules"] = Json::Value(event_schedules);
		department["tags"] = Json::Value(tags);

		Dep[i] = Json::Value(department);
	}
	for (int i = 0; i < stu_num; i++)
	{
		//定义student及各属性
		Json::Value student;
		Json::Value want;
		Json::Value free_time;
		Json::Value tags;

		//给student属性赋值
		for (int j = 0; j < 5; j++)
		{
			want[j] = Json::Value(stu[i].want[j]);
		}

		for (int j = 0; j < stu[i].schedule_num; j++)
		{
			free_time[j] = Json::Value(stu[i].available_schedules[j]);
		}

		for (int j = 0; j < stu[i].tag_num; j++)
		{
			tags[j] = Json::Value(stu[i].tags[j]);
		}

		student["student_no"] = Json::Value(stu[i].student_no);
		student["want"] = Json::Value(want);
		student["free_time"] = Json::Value(free_time);
		student["tags"] = Json::Value(tags);

		Stu[i] = Json::Value(student);
	}
	root["student"] = Json::Value(Stu);
	root["department"] = Json::Value(Dep);

	//缩进输出
	Json::StyledWriter sw;
	cout << sw.write(root);
}
