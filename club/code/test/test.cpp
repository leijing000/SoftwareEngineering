
#include"test.h"
#include"json.h"

#include<ctime>
#include<queue>
#include<fstream>
#include<string>

using namespace std;

//ѧ���ṹ��
struct Student {
	int student_no;                  //ѧ�����1-300
	string available_schedules[49];  //����ʱ��
	string tags[16];                 //�ص��ǩ
	int want[5];                     //���־Ը   1-5
	double point;                    //����0-5

	int cur_want = 0;                //��cur_want��־Ը
	int department_no[5];			 //ѧ����������
	int choose_num;//����ѡ������

	int match_schedule[5];			 //��¼�ʱ��ƥ�����
	int match_tag[5];				 //��¼��ǩƥ�����
	double match_best[5];			 //��¼��Ȩ���ֵ���ʱ��*50% + ��Ȥ*30% + point*20%��

	int schedule_num = 0;            //ʱ��θ���
	int tag_num = 0;                 //��ǩ����

}stu[3000];

//���Žṹ��
struct Departments {
	int department_no;              //���ű��1-20
	int number_limit;               //��Ҫ��ѧ��������0-15
	string tags[16];                //�ص��ǩ
	string event_schedules[49];     //����ʱ���

	int choose_num;                 //���ڸò��ŵ�ѧ����
	int student_no[20];             //���ڸò��ŵ�ѧ�����

	int schedule_num = 0;           //ʱ��θ���
	int tag_num = 0;                //��ǩ����
}depart[200];

int read_information()
{

	Json::Reader reader;
	Json::Value root;

	//���ļ��ж�ȡ
	ifstream is;
	is.open("./import.txt", ios::binary);

	if (reader.parse(is, root))
	{
		for (int i = 0; i < root["department"].size(); i++)
		{

			//��ȡ���ű��
			int department_no = root["department"][i]["department_no"].asInt();
			depart[department_no - 1].department_no = department_no;

			//��ȡ������������
			depart[department_no - 1].number_limit = root["department"][i]["number_limit"].asInt();

			//��ȡ���Żʱ��
			for (int j = 0; j < root["department"][i]["event_schedules"].size(); j++)
			{
				depart[department_no - 1].event_schedules[j] = root["department"][i]["event_schedules"][j].asString();
				depart[department_no - 1].schedule_num++;
			}
			//��ȡ���ű�ǩ
			for (int j = 0; j < root["department"][i]["tags"].size(); j++)
			{
				depart[department_no - 1].tags[j] = root["department"][i]["tags"][j].asString();
				depart[department_no - 1].tag_num++;
			}
		}
		for (int i = 0; i < root["student"].size(); i++)
		{
			//��ȡѧ�����
			int student_no = root["student"][i]["student_no"].asInt();
			stu[student_no - 1].student_no = student_no;

			//��ȡѧ��־Ը
			for (int j = 0; j < 5; j++)
			{
				stu[student_no - 1].want[j] = root["student"][i]["want"][j].asInt();
			}

			//��ȡѧ������ʱ��
			for (int j = 0; j < root["student"][i]["free_time"].size(); j++)
			{
				stu[student_no - 1].available_schedules[j] = root["student"][i]["free_time"][j].asString();
				stu[student_no - 1].schedule_num++;
			}
			//��ȡѧ����Ȥ��ǩ
			for (int j = 0; j < root["student"][i]["tags"].size(); j++)
			{
				stu[student_no - 1].tags[j] = root["student"][i]["tags"][j].asString();
				stu[student_no - 1].tag_num++;
			}

			//��ȡ��������
		}

	}
	is.close();
	return root["student"].size();
}

//���ձ��ؼ����Ȩ���ֵ
void compute(int stu_num)
{
	for (int i = 0; i < stu_num; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			Student& student = stu[i];
			Departments& department = depart[stu[i].want[j] - 1];

			//����ʱ���غϴ���
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
			//������Ȥ��ǩ�غϴ���
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
			//��Ȩ �ʱ��ռ50%����Ȥռ%30������ռ20%
			student.match_best[j] = student.match_schedule[j] * (50 / 100) + student.match_tag[j] * (30 / 100) + student.point * (20 / 100);

		}
	}
}

//ʹ��Gale-Shapley�㷨����ƥ��
void match(int stu_num)
{
	//δ���䵽���ŵ�ѧ������
	queue<Student> que;
	for (int i = 0; i < stu_num; i++)
	{
		que.push(stu[i]);//��ʼ����δ����״̬�����ӽ�����
	}
	while (!que.empty())
	{
		bool next = false;
		Student& cur_student = stu[que.front().student_no - 1];
		que.pop();

		//����ѧ��cur_student�ĵ�cur_want��־Ը������Ϊdepartment)
		Departments& department = depart[cur_student.want[cur_student.cur_want] - 1];

		//������ڸò��ţ��͵���һ��
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
				//�������department����ʣ�ֱ࣬����ѡ
				department.student_no[department.choose_num++] = cur_student.student_no;
				cur_student.department_no[cur_student.choose_num++] = department.department_no;

			}
			else
			{
				int min_stu_no = -1;    //ѧ����Ȩ��ֵ��С��ѧ�����
				int position;           //Ȩ��ֵ��С��ѧ���ڲ�����ѡ�б��е��±�
				double min_best = 100;  //��ʱ��СȨ��ֵ

										//�ڲ���ѧ���б���ѡ�����Ȩ��ֵ��͵�ѧ�����
				for (int i = 0; i < department.choose_num; i++)
				{
					Student tmp = stu[department.student_no[i] - 1];
					if (min_best > tmp.match_best[cur_student.cur_want])
					{
						min_best = tmp.match_best[cur_student.cur_want];
						min_stu_no = tmp.student_no;//Ȩ��ֵ��С��ѧ�����
						position = i;               //Ȩ��ֵ��С��ѧ�����ڲ����б���±�
					}
				}
				//�������department������
				//���� ѧ��cur_student��Ȩ��ֵ�Ȳ���department�������Ѿ���ѡ��ѧ�����ͣ���ôѧ��cur_studentֻ�õ�����
				if (department.number_limit != 0 && cur_student.match_best[cur_student.cur_want] > min_best)
					//����ѧ��cur_student��ֱ���滻��Ȩ��ֵ��͵��Ǹ�ѧ��
				{
					//����̭��ѧ��
					Student& min_stu = stu[min_stu_no - 1];
					int pos_depart;
					for (int j = 0; j < min_stu.choose_num; j++)
					{
						if (min_stu.department_no[j] == department.department_no)
						{
							pos_depart = j;
						}
					}
					min_stu.department_no[pos_depart] = 0;//����̭���������ű��
					min_stu.choose_num--;
					/*
					//���滻����ѧ���ٷ���δ����Ķ�����ȥ
					if (min_stu.cur_want < 5)
					{
					que.push(min_stu);
					}*/
					department.student_no[position] = cur_student.student_no;
					cur_student.department_no[cur_student.choose_num++] = department.department_no;
				}
			}
		}
		cur_student.cur_want++;//��һ��־Ը
							   //cout << cur_student.cur_want << endl;
							   //������־Ը��û�п����꣬�������м����������
		if (cur_student.cur_want < 5)
		{
			que.push(cur_student);
		}
	}
}

void print_match_result(int stu_num, int depart_num)
{
	int total_num = 0; //Ԥ����������

	Json::Value root;

	Json::Value unlucky_student;
	Json::Value admitted;
	Json::Value unlucky_department;

	//admitted������
	Json::Value member;
	Json::Value department_no;
	Json::Value departnum;
	Json::Value stunum;

	//δ�����䵽���ŵ�ѧ��
	int unlucky_stu_num = 0;
	for (int i = 0; i < stu_num; i++)
	{
		if (stu[i].choose_num == 0)
		{
			unlucky_student[unlucky_stu_num++] = Json::Value(stu[i].student_no);
		}
	}
	root["unlucky_student"] = Json::Value(unlucky_student);

	
	//���벿�ŵ�ѧ��
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

	cout << "Ԥ������" << total_num << "�ˣ�" << endl;
	cout << "����" << unlucky_stu_num << "��δƥ�䣡" << endl;
	cout << "����" << (300 - unlucky_stu_num) << "�˳ɹ����벿�ţ�" << endl;

	//�������
	Json::StyledWriter sw;
	cout << sw.write(root) << endl << endl;
}