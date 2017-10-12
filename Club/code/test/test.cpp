
#include"test.h"
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

int department_num;//部门数量

//读文本中的数据
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
		}
	}
	is.close();
	department_num = root["department"].size();
	return root["student"].size();//返回学生数量
}

//获取部门数量
int get_depart_num()
{
	return department_num;
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
			student.match_best[j] = student.match_schedule[j] * 0.5 + student.match_tag[j] * 0.3 + student.point * 0.2;
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
		Student& cur_student = stu[que.front().student_no - 1];
		que.pop();

		//考虑学生cur_student的第cur_want个志愿（部门为department)
		Departments& department = depart[cur_student.want[cur_student.cur_want] - 1];

		//如果不在该部门，则继续参与分配，否则就等下一轮
		if (cur_student.department_no[department.department_no] == 0)
		{//如果部门department还有剩余，直接中选
			
			if (department.number_limit > department.choose_num)
			{
				//更新部门的学生列表
				department.student_no[department.choose_num++] = cur_student.student_no;
				//改变部门状态（已分配）
				cur_student.department_no[department.department_no] = 1;     
				//学生所属部门数加一
				cur_student.choose_num++;                                               
			}
			else
			{//名额已满则开始竞争，与权重值最小的学生进行比较

				int min_stu_no = -1;    //学生中权重值最小的学生编号
				int position;           //权重值最小的学生在部门中选列表中的下标
				double min_best = 100;  //临时最小权重值

				//在部门学生列表中查找权重值最低的学生编号
				for (int i = 0; i < department.choose_num; i++)
				{
					Student tmp = stu[department.student_no[i] - 1];
					if (min_best > tmp.match_best[cur_student.cur_want])
					{
						min_best = tmp.match_best[cur_student.cur_want];//更新最小值
						min_stu_no = tmp.student_no;					//权重值最小的学生编号
						position = i;									//权重值最小的学生所在部门列表的下标
					}
				}
				//如果部门department不纳新
				//或者 学生cur_student的权重值比部门department中所有已经中选的学生还低，那么学生cur_student只好等下轮
				if (department.number_limit != 0 && cur_student.match_best[cur_student.cur_want] > min_best)
				{//否则学生cur_student就直接替换掉权重值最低的那个学生

					//更新被淘汰学生的信息
					Student& min_stu = stu[min_stu_no - 1];             
					min_stu.department_no[department.department_no] = 0;//被淘汰后,将该部门对应的数组下标置0（未分配）
					min_stu.choose_num--;                               //被淘汰学生所选部门数减一

					//更新新加入的学生信息
					department.student_no[position] = cur_student.student_no;
					cur_student.department_no[department.department_no] = 1;
					cur_student.choose_num ++ ;
				}
			}
		}
		//下一个志愿
		cur_student.cur_want++;
		//如果五个志愿还没有考虑完，则放入队列继续参与分配
		if (cur_student.cur_want < 5)
		{
			que.push(cur_student);
		}
	}
}

//输出匹配结果
void print_match_result(int stu_num, int depart_num)
{
	int total_num = 0;             //预计招收人数		  
	int unlucky_stu_num = 0;       //未被分配到部门的学生数							   
	int department_num = 0;        //已分配的部门数
	int unlucky_depart_num = 0;    //未分配的部门数

	Json::Value root;
	Json::Value unlucky_student;   //未分配的学生
	Json::Value unlucky_department;//未分配的部门
	Json::Value Admitted;          //已分配的学生和部门

	//未分配的学生
	for (int i = 0; i < stu_num; i++)
	{
		if (stu[i].choose_num == 0)
		{
			unlucky_student[unlucky_stu_num++] = Json::Value(stu[i].student_no);
		}
	}

	//已分配的学生
	for (int i = 0; i < depart_num; i++)
	{
		
	
		total_num = total_num + depart[i].number_limit;

		if (depart[i].choose_num != 0)
		{
			Json::Value admitted;
			//admitted的属性
			Json::Value member;
			Json::Value department_no;

			department_no.append(depart[i].department_no);

			for (int j = 0; j < depart[i].choose_num; j++)
			{
				member.append(depart[i].student_no[j]);
			}	

			admitted["department_no"] = Json::Value(department_no);
			admitted["member"] = Json::Value(member);
			Admitted[department_num] = Json::Value(admitted);
		}
		department_num++;	
	}

	//未分配的部门
	for (int i = 0; i < depart_num; i++)
	{
		if (depart[i].choose_num == 0 && depart[i].number_limit != 0)
		{
			unlucky_department[unlucky_depart_num++] = Json::Value(depart[i].department_no);
		}
	}

    //根节点属性
	root["unlucky_student"] = Json::Value(unlucky_student);
	root["admitted"] = Json::Value(Admitted);
	root["unlucky_department"] = Json::Value(unlucky_department);

	cout << "预计招收" << total_num << "人！" << endl;
	cout << "共有" << unlucky_stu_num << "人未匹配到部门！" << endl;
	cout << "共有" << (300 - unlucky_stu_num) << "人成功加入部门！" << endl;

	//缩进输出
	Json::StyledWriter sw;
	cout << sw.write(root) << endl << endl;
}