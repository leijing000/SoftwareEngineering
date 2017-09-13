#include"sudoku.h"

int shudu[SIZE][SIZE];

void init()
{
	for (int i = 0; i < SIZE; i++)
	{

		for (int j = 0; j < SIZE; j++)
		{
			shudu[i][j] = 0;
		}
	}
}

void print()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			cout << shudu[i][j] << " ";
		}
		cout << endl;
	}
}

bool isRightposition(int row, int col)
{
	int curNum = shudu[row][col];
	//验证行
	for (int i = 0; i < row; i++)
	{
		if (shudu[i][col] == curNum)
		{
			return false;
		}
	}
	//验证列
	for (int i = 0; i < col; i++)
	{
		if (shudu[row][i] == curNum)
		{
			return false;
		}
	}
	//验证九宫格 
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

bool generate(int mark)
{
	if (mark == 81)
	{
		return true;
	}

	int row = mark / 9;//行坐标
	int col = mark % 9;//列坐标

	//若当前位置已填入数字，则生成下一位置数字 
	if (shudu[row][col] != 0)
	{
		if (generate(mark + 1))
		{
			return true;
		}
	}
	else
	{
		int count = 0;
		int index;

		//arr用来标记数字是否被使用 
		int arr[9] = { 0 };

		for (int i = 0; i < 9; i++)
		{
			//生成随机数字，直到1到9全部生成完毕 
			while (1)
			{
				index = rand() % SIZE + 1;
				if (arr[index - 1] == 0)
				{
					arr[index - 1] = 1;
					count++;
					break;
				}
			}

			shudu[row][col] = index;

			//判断赋值是否合法，合法则继续生成下一位置
			if (isRightposition(row, col))
			{
				if (generate(mark + 1))
				{
					return true;
				}
			}
			else
			{
				//不合法则将当前位置赋值为0，回溯
				shudu[row][col] = 0;
			}
		}
	}
	return false;
}