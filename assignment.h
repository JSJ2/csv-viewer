#include <malloc.h>
#include <Windows.h>
#include <stdio.h>

#define CSV_FILENAME 1024
#define STRING_SIZE 128

char** assign_csv_arr(int csv_cnt)
{
	char** csv_file = (char**)malloc(sizeof(char*) * csv_cnt);
	for (int i = 0; i < csv_cnt; i++)
		csv_file[i] = (char*)malloc(sizeof(char) * (CSV_FILENAME + 1));
	return csv_file;
}

// �Լ� ������ �����Ҵ��� �ϰ�
// �Ҵ�� �޸𸮸� �� �����ͷ� ��ȯ�Ѵ�.
double** double_assignment(int row, int double_cnt)
{
	double** ptr = (double**)malloc(sizeof(double*) * row);

	for (int i = 0; i < row; i++)
	{
		ptr[i] = (double*)malloc(sizeof(double) * double_cnt);
	}
	return ptr;
}

// ���ڿ��� �ִ� ũ��� 127�̴�.
char*** string_assignment(int row, int string_cnt)
{
	char*** ptr = (char***)malloc(sizeof(char**) * row);

	for (int i = 0; i < row; i++)
	{
		ptr[i] = (char**)malloc(sizeof(char*) * string_cnt);
		for (int j = 0; j < string_cnt; j++)
			ptr[i][j] = (char*)malloc(sizeof(char) * STRING_SIZE); // ���ڿ��� �ִ� ũ��� 127�̴�.
	}
	return ptr;
}

char** header_assignment(char filename[], int column)
{
	int max_header_size = 0;
	FILE* fpR = fopen(filename, "r");
	if (fpR == NULL)
	{
		printf("Error");
		exit(0);
	}

	char** header_ptr = (char**)malloc(sizeof(char*) * column);
	for (int i = 0; i < column; i++)
	{
		header_ptr[i] = (char*)malloc(sizeof(char) * 128); // ����Ʈ 128
	}
	return header_ptr;
}

char* order_assignment(int column)
{
	char* order = (char*)malloc(sizeof(char) * column + 1);
	return order;
}

void csv_free_assignment(char** csv_file, int csv_cnt)
{
	for (int i = 0; i < csv_cnt; i++)
		free(csv_file[i]);
	free(csv_file);
}

void header_free_assignment(char** ptr, int column)
{
	for (int i = 0; i < column; i++)
		free(ptr[i]);
	free(ptr);
}

void double_free_assignment(double** ptr, int row, int double_cnt)
{
	for (int i = 0; i < row; i++)
		free(ptr[i]);
	free(ptr);
}

void string_free_assignment(char*** ptr, int row, int string_cnt)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < string_cnt; j++)
			free(ptr[i][j]);
		free(ptr[i]);
	}
	free(ptr);
}

void type_location_free_assignment(int** type_location, int column)
{
	for (int i = 0; i < 2; i++)
		free(type_location[i]);
	free(type_location);
}