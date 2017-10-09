
#include"test.h"
#include"json.h"

#include<ctime>
#include<queue>
#include<fstream>
#include<string>

using namespace std;

//学生结构体
struct Student {
	int student_no;                  //学生编号1-300
	string available_schedules[49];  //空闲时间
	string tags[16];                 //特点标签
	int want[5];                     //五个志愿   1-5
	double point;                    //绩点0-5

	int cur_want = 0;                //第cur_want个志愿
	int department_no[5];			 //学生所属部门
	int choose_num;//已中选部门数

	int match_schedule[5];			 //记录活动时间匹配情况
	int match_tag[5];				 //记录标签匹配情况
	double match_best[5];			 //记录加权后的值（活动时间*50% + 兴趣*30% + point*20%）

	int schedule_num = 0;            //时间段个数
	int tag_num = 0;                 //标签个数

}stu[3000];

//部门结构体
struct Departments {
	int department_no;              //部门编号1-20
	int number_limit;               //需要的学生数上限0-15
	string tags[16];                //特点标签
	string event_schedules[49];     //常规活动时间段

	int choose_num;                 //属于该部门的学生数
	int student_no[20];             //属于该部门的学生编号

	int schedule_num = 0;           //时间段个数
	int tag_num = 0;                //标签个数
}depart[200];

int read_information()
{

	Json::Reader reader;
	Json::Value root;

	//从文件中读取
	ifstream is;
	is.open("./import.txt", ios::binary);

	if (reader.parse(is, root))
	{
		for (int i = 0; i < root["department"].size(); i++)
		{

			//读取部门编号
			int department_no = root["department"][i]["department_no"].asInt();
			depart[department_no - 1].department_no = department_no;

			//读取部门人数上限
			depart[department_no - 1].number_limit = root["department"][i]["number_limit"].asInt();

			//读取部门活动时间
			for (int j = 0; j < root["department"][i]["event_schedules"].size(); j++)
			{
				depart[department_no - 1].event_schedules[j] = root["department"][i]["event_schedules"][j].asString();
				depart[department_no - 1].schedule_num++;
			}
			//读取部门标签
			for (int j = 0; j < root["department"][i]["tags"].size(); j++)
			{
				depart[department_no - 1].tags[j] = root["department"][i]["tags"][j].asString();
				depart[department_no - 1].tag_num++;
			}
		}
		for (int i = 0; i < root["student"].size(); i++)
		{
			//读取学生编号
			int student_no = root["student"][i]["student_no"].asInt();
			stu[student_no - 1].student_no = student_no;

			//读取学生志愿
			for (int j = 0; j < 5; j++)
			{
				stu[student_no - 1].want[j] = root["student"][i]["want"][j].asInt();
			}

			//读取学生空闲时间
			for (int j = 0; j < root["student"][i]["free_time"].size(); j++)
			{
				stu[student_no - 1].available_schedules[j] = root["student"][i]["free_time"][j].asString();
				stu[student_no - 1].schedule_num++;
			}
			//读取学生兴趣标签
			for (int j = 0; j < root["student"][i]["tags"].size(); j++)
			{
				stu[student_no - 1].tags[j] = root["student"][i]["tags"][j].asString();
				stu[student_no - 1].tag_num++;
			}

			//读取该生绩点
		}

	}
	is.close();
	return root["student"].size();
}

//按照比重计算加权后的值
void compute(int stu_num)
{
	for (int i = 0; i < stu_num; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			Student& student = stu[i];
			Departments& department = depart[stu[i].want[j] - 1];

			//计算活动时间重合次数
			for (int m = 0; m < student.schedule_num; m++)
			{
				for (int n = 0; n < department.schedule_num; n++)
				{
					if (department.event_schedules[n] == student.available_schedules[m])
					{
						student.match_schedule[j]++;
						//break;
					}
				}
			}
			//计算兴趣标签重合次数
			for (int m = 0; m < student.tag_num; m++)
			{
				for (int n = 0; n < department.tag_num; n++)
				{
					if (department.tags[n] == student.tags[m])
					{
						student.match_tag[j]++;
					}
				}
			}
			//加权 活动时间占50%，兴趣占%30，绩点占20%
			student.match_best[j] = student.match_schedule[j] * (50 / 100) + student.match_tag[j] * (30 / 100) + student.point * (20 / 100);

		}
	}
}

//使用Gale-Shapley算法进行匹配
void match(int stu_num)
{
	//未分配到部门的学生队列
	queue<Student> que;
	for (int i = 0; i < stu_num; i++)
	{
		que.push(stu[i]);//初始都是未分配状态，都加进队列
	}
	while (!que.empty())
	{
		bool next = false;
		Student& cur_student = stu[que.front().student_no - 1];
		que.pop();

		//考虑学生cur_student的第cur_want个志愿（部门为department)
		Departments& department = depart[cur_student.want[cur_student.cur_want] - 1];

		//如果已在该部门，就等下一轮
		for (int j = 0; j < cur_student.choose_num; j++)
		{
			if (cur_student.department_no[j] == department.department_no)
			{
				next = true;
				break;
			}
		}
		if (next == false)
		{
			if (department.number_limit > department.choose_num)
			{
				//如果部门department还有剩余，直接中选
				department.student_no[department.choose_num++] = cur_student.student_no;
				cur_student.department_no[cur_student.choose_num++] = department.department_no;

			}
			else
			{
				int min_stu_no = -1;    //学生中权重值最小的学生编号
				int position;           //权重值最小的学生在部门中选列表中的下标
				double min_best = 100;  //临时最小权重值

										//在部门学生列表中选择查找权重值最低的学生编号
				for (int i = 0; i < department.choose_num; i++)
				{
					Student tmp = stu[department.student_no[i] - 1];
					if (min_best > tmp.match_best[cur_student.cur_want])
					{
						min_best = tmp.match_best[cur_student.cur_want];
						min_stu_no = tmp.student_no;//权重值最小的学生编号
						position = i;               //权重值最小的学生所在部门列表的下标
					}
				}
				//如果部门department不纳新
				//或者 学生cur_student的权重值比部门department中所有已经中选的学生还低，那么学生cur_student只好等下轮
				if (department.number_limit != 0 && cur_student.match_best[cur_student.cur_want] > min_best)
					//否则学生cur_student就直接替换掉权重值最低的那个学生
				{
					//被淘汰的学生
					Student& min_stu = stu[min_stu_no - 1];
					int pos_depart;
					for (int j = 0; j < min_stu.choose_num; j++)
					{
						if (min_stu.department_no[j] == department.department_no)
						{
							pos_depart = j;
						}
					}
					min_stu.department_no[pos_depart] = 0;//被淘汰后所属部门编号
					min_stu.choose_num--;
					/*
					//被替换掉的学生再放入未分配的队列中去
					if (min_stu.cur_want < 5)
					{
					que.push(min_stu);
					}*/
					department.student_no[position] = cur_student.student_no;
					cur_student.department_no[cur_student.choose_num++] = department.department_no;
				}
			}
		}
		cur_student.cur_want++;//下一个志愿
							   //cout << cur_student.cur_want << endl;
							   //如果五个志愿还没有考虑完，则放入队列继续参与分配
		if (cur_student.cur_want < 5)
		{
			que.push(cur_student);
		}
	}
}

void print_match_result(int stu_num, int depart_num)
{
	int total_num = 0; //预计招收人数

	Json::Value root;

	Json::Value unlucky_student;
	Json::Value admitted;
	Json::Value unlucky_department;

	//admitted的属性
	Json::Value member;
	Json::Value department_no;
	Json::Value departnum;
	Json::Value stunum;

	//未被分配到部门的学生
	int unlucky_stu_num = 0;
	for (int i = 0; i < stu_num; i++)
	{
		if (stu[i].choose_num == 0)
		{
			unlucky_student[unlucky_stu_num++] = Json::Value(stu[i].student_no);
		}
	}
	root["unlucky_student"] = Json::Value(unlucky_student);

	
	//加入部门的学生
	int admitted_stu_num = 0;
	int department_num = 0;
	int member_num = 0;

	for (int i = 0; i < depart_num; i++)
	{
		total_num = total_num + depart[i].number_limit;
		if (depart[i].choose_num != 0)
		{
			department_no[department_num++] = Json::Value(depart[i].department_no);
			for (int j = 0; j < depart[i].choose_num; j++)
			{
				member[member_num].append(depart[i].student_no[j]);
			}
			member_num++;
		}
	}

	admitted["member"] = Json::Value(member);
	admitted["department_no"] = Json::Value(department_no);
	root["admitted"] = Json::Value(admitted);

	int unlucky_depart_num = 0;
	for (int i = 0; i < depart_num; i++)
	{
		if (depart[i].choose_num == 0 && depart[i].number_limit != 0)
		{
			unlucky_department[unlucky_depart_num++] = Json::Value(depart[i].department_no);
		}
	}

	root["unlucky_department"] = Json::Value(unlucky_department);

	cout << "预计招收" << total_num << "人！" << endl;
	cout << "共有" << unlucky_stu_num << "人未匹配！" << endl;
	cout << "共有" << (300 - unlucky_stu_num) << "人成功加入部门！" << endl;

	//缩进输出
	Json::StyledWriter sw;
	cout << sw.write(root) << endl << endl;
}