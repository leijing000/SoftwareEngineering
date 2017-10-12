#include"generate.h"
#include"json.h"
#include<ctime>
#include<queue>
#include<fstream>
#include<string>

using namespace std;

//ѧ���ṹ��
struct Student {
	int student_no;                  //ѧ�����
	string available_schedules[49];  //����ʱ��
	string tags[16];                 //��Ȥ��ǩ
	int want[5];                     //���־Ը   
	double point;                    //����0-5

	int cur_want = 0;                //��ǰ���ڽ��з����־Ը
	int department_no[25];			 //ѧ����������,0��ʾ���ڸò��ţ�1��ʾ�ѱ��ò���ѡ��
	int choose_num;                  //����ѡ������

	int match_schedule[5];			 //��¼�ʱ��ƥ�����
	int match_tag[5];				 //��¼��ǩƥ�����
	double match_best[5];			 //��¼��Ȩ���ֵ���ʱ��*50% + ��Ȥ*30% + point*20%��

	int schedule_num = 0;            //ʱ��θ���
	int tag_num = 0;                 //��ǩ����

}stu[1000];

//���Žṹ��
struct Departments {
	int department_no;              //���ű��
	int number_limit;               //��Ҫ��ѧ��������[0-15]
	string tags[16];                //��Ȥ��ǩ
	string event_schedules[49];     //����ʱ���

	int choose_num;                 //���ڸò��ŵ�ѧ����
	int student_no[20];             //���ڸò��ŵ�ѧ�����

	int schedule_num = 0;           //ʱ��θ���
	int tag_num = 0;                //��ǩ����
}depart[100];

//��Ȥ��ǩ
string tag[16] = {
	"reading","programming","film","English","music",
	"dance","basketball","football","table tennis",
	"chess","singing","volunteer","math","guitar","painting",
	"running"
};

//ʱ���
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
	//��ʼ���������
	srand((unsigned)time(NULL));

	//���������������
	for (int i = 0; i < depart_num; i++)
	{
		//ʱ��״̬��ʹ�ù�����Ϊ1
		int time_status[49] = { 0 };

		//��ǩ��״̬��ʹ�ù�����Ϊ1
		int tag_status[16] = { 0 };

		//���ű��[1-depart_num]
		depart[i].department_no = i + 1;

		//��Ҫѧ����������[0-15]
		depart[i].number_limit = rand() % 16;

		//���������Ȥ��ǩ[2-4��]
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

		//������ɲ��Żʱ��[3-7��]
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

	//ѧ�������������
	for (int i = 0; i < stu_num; i++)
	{
		//ʱ��״̬��ʹ�ù�ʱ��ε���Ϊ1
		int time_status[49] = { 0 };

		//��ǩ��״̬��ʹ�ù��ı�ǩ��Ϊ1
		int tag_status[16] = { 0 };

		//ѧ�����[1-stu_num]
		stu[i].student_no = i + 1;

		//��Ȥ��ǩ�������2-4��
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

		//����ʱ���������3-7��
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

		//־Ը�������[��Ӧ���ź�depart_num]
		for (int j = 0; j < 5; j++) {
			stu[i].want[j] = rand() % depart_num + 1;
		}

		//�����������
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
		//����department��������
		Json::Value department;
		Json::Value event_schedules;
		Json::Value tags;

		//�������Ը�ֵ
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
		//����student��������
		Json::Value student;
		Json::Value want;
		Json::Value free_time;
		Json::Value tags;

		//��student���Ը�ֵ
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

	//�������
	Json::StyledWriter sw;
	cout << sw.write(root);
}
