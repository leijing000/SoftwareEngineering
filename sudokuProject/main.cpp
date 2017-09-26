#include"sudoku.h"

#pragma warning(disable:4996)

int main(int argc, char* argv[])
{
	freopen("sudoku.txt", "w", stdout);
	int n;
	//输入不合法的判断
	if (*argv[2] < '0' || *argv[2] > '9')
	{
		cout << "Input Error!" << endl;
	}
	else
	{
		n = atoi(argv[2]);

		sudoku shudu;
		for (int i = 0; i < n; i++)
		{
			shudu.init();
			if (shudu.generate(0))
			{
				shudu.print();
			}
			cout << endl;
		}
	}
	return 0;
}