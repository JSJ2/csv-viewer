#include <stdio.h>
#include <Windows.h>
#define MAX 1024

// strlen() �� ���� �ʴ� ����
// �迭�� ũ��� ���ڿ��� ũŰ�� �ٸ��� �����̴�.

// �迭�� ũ�⸦ ��ȯ�Ѵ�.
int find_arr_size(char arr[])
{
	int i = 0;
	for (i; arr[i] != '\0'; i++);
	return i;
}

// csv���� ��θ� �����ϸ� csv�� row�� ���� ��ȯ��.
int find_row(char filename[])
{
	FILE* fpR = fopen(filename, "r");

	int row_cnt = 0;

	char line[MAX];
	line[0] = '\0';
	fgets(line, MAX, fpR); // header ������.
	int line_size;
	while (feof(fpR) == 0)
	{
		fgets(line, MAX, fpR);
		row_cnt++;
	}

	fclose(fpR);

	// feof()�� ���ڿ� �������� \n�� ������ ������ �ʾҴٰ� ������.
	// ���� csv�������� \n�� ���ٸ� �״�� ������
	// �ִٸ� �� ���� �ݺ��ؼ� ������.
	// �� ���� ����� ���� �� ���� �������� �ڵ带 ¥��
	// �� ���� ����� ���� ���ÿ� ������ų �� ����.

	// ��������� �������� �ٿ� \n�� �ִ��� ������ üũ�ϰ� 
	// �ִٸ� row_cnt�� �ϳ� ����.
	line_size = find_arr_size(line);
	if (line[line_size - 1] == '\n')
		row_cnt--;
	else;


	return row_cnt;
}

// csv���� ��θ� �����ϸ� column���� ��ȯ��.
int find_column(char filename[]) 
{
	FILE* fpR = fopen(filename, "r");
	if (fpR == NULL)
	{
		system("cls");
		printf("Error!!! %s is NULL. Check \"find.h\" int auto_find_column()");
		exit(0);
	}

	int column_cnt = 0;

	char line[MAX];
	line[0] = '\0';

	fgets(line, MAX, fpR);

	char* ptr = strtok(line, ",");

	while (ptr != NULL)
	{
		column_cnt++;
		ptr = strtok(NULL, ",");
	}
	fclose(fpR);

	return column_cnt;
}

// csv�� �ڷ����� ��ġ�� ã�� 2�����迭�� ��ȯ�Ѵ�.
// order_column_type �迭�� ã�´�.
// [0][] :   ���� ��ġ �迭
// [1][] : ���ڿ� ��ġ �迭
int** type_location_find(char* order_column_type, int column)
{
	// [0][], [1][] ����
	int** type_location = (int**)malloc(sizeof(int*) * 2);
	for (int i = 0; i < 2; i++)
		type_location[i] = (int*)malloc(sizeof(int) * column);

	int double_index = 0, string_index = 0;

	for (int j = 0; j < column; j++)
	{
		switch (order_column_type[j])
		{
		case '0':
			type_location[0][j] = double_index;
			type_location[1][j] = -1;
			double_index++;
			break;

		case '1':
			type_location[0][j] = -1;
			type_location[1][j] = string_index;
			string_index++;
			break;
		}
	}
	return type_location;
}