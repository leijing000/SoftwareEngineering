#include"sudoku.h"

void sudoku::init()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			shudu[i][j] = 0;
		}
	}
	return;
}

void sudoku::print()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			//printf("%d ",shudu[i][j]);
			cout << shudu[i][j] << " ";
		}
		cout << endl;
	}
}

bool sudoku::isRightposition(int row, int col)
{
	int curNum = shudu[row][col];
	//��֤��
	for (int i = 0; i < row; i++)
	{
		if (shudu[i][col] == curNum)
		{
			return false;
		}
	}
	//��֤��
	for (int i = 0; i < col; i++)
	{
		if (shudu[row][i] == curNum)
		{
			return false;
		}
	}
	//��֤�Ź��� 
	int rowStart = row / 3 * 3;
	int rowEnd = rowStart + 2;
	int colStart = col / 3 * 3;
	int colEnd = colStart + 2;
	for (int i = rowStart; i <= rowEnd; i++)
	{
		for (int j = colStart; j <= colEnd; j++)
		{
			if (shudu[i][j] == curNum && i != row && j != col)
			{
				return false;
			}
		}
	}
}

bool sudoku::generate(int mark)
{
	shudu[0][0] = (1 + 9) % 9 + 1;
	if (mark == 81)
	{
		return true;
	}

	int row = mark / 9;//������
	int col = mark % 9;//������

					   //����ǰλ�����������֣���������һλ������ 
	if (shudu[row][col] != 0)
	{
		if (generate(mark + 1))
		{
			return true;
		}
	}
	else
	{
		int index;

		//arr������������Ƿ�ʹ�� 
		int arr[9] = { 0 };
		for (int i = 0; i < 9; i++)
		{
			//����������֣�ֱ��1��9ȫ��������� 
			while (1)
			{
				index = rand() % SIZE + 1;
				if (arr[index - 1] == 0)
				{
					arr[index - 1] = 1;
					break;
				}
			}

			shudu[row][col] = index;

			//�жϸ�ֵ�Ƿ�Ϸ����Ϸ������������һλ��
			if (isRightposition(row, col))
			{
				if (generate(mark + 1))
				{
					return true;
				}
			}
		}
		//���Ϸ��򽫵�ǰλ�ø�ֵΪ0������
		shudu[row][col] = 0;
	}
	return false;
}