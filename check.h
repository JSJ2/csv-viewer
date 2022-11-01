#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX 1024

// ���ڿ� ���ڿ��� ������.
// ���ڸ�     0�� ��ȯ��.
// ���ڿ��̸� 1�� ��ȯ��.
int check_word(char ptr2[])
{
	bool IsString = false;
	for (unsigned int i = 0; i < strlen(ptr2); i++)
	{
		// ����, ',', '\n', '-' �̿��� ���ڴ� ���ڿ��� ������.
		if (ptr2[i] >= 48 && ptr2[i] <= 57)
			IsString = false;
		else if (ptr2[i] == '.')
			IsString = false;
		else if (ptr2[i] == '\n')
			IsString = false;
		else if (ptr2[i] == '-')
			IsString = false;
		else
		{
			IsString = true;
			break;
		}
	}

	if (IsString)
		return 1;
	else
		return 0;
}

void check_type(char filename[], char* order_column_type, int* double_cnt, int* string_cnt, int column)
{
	FILE* fpR = fopen(filename, "r");

	char line[MAX];
	line[0] = '\0';

	fgets(line, MAX, fpR); // header ����

	fgets(line, MAX, fpR);
	char* ptr2 = strtok(line, ",");
	for (int i = 0; i < column; i++)
	{
		int return_value = check_word(ptr2);
		switch (return_value)
		{
		case 0:
			order_column_type[i] = '0';
			(*double_cnt)++;
			break;
		case 1:
			order_column_type[i] = '1';
			(*string_cnt)++;
			break;
		default:
			break;
		}
		ptr2 = strtok(NULL, ",");
	}
	order_column_type[column] = '\0';
	fclose(fpR);
}