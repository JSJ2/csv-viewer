#include <stdio.h>
#include <Windows.h>
#define MAX 1024

// strlen() 을 쓰지 않는 이유
// 배열의 크기와 문자열의 크키는 다르기 때문이다.

// 배열의 크기를 반환한다.
int find_arr_size(char arr[])
{
	int i = 0;
	for (i; arr[i] != '\0'; i++);
	return i;
}

// csv파일 경로만 전달하면 csv의 row의 수를 반환함.
int find_row(char filename[])
{
	FILE* fpR = fopen(filename, "r");

	int row_cnt = 0;

	char line[MAX];
	line[0] = '\0';
	fgets(line, MAX, fpR); // header 버리기.
	int line_size;
	while (feof(fpR) == 0)
	{
		fgets(line, MAX, fpR);
		row_cnt++;
	}

	fclose(fpR);

	// feof()는 문자열 마지막이 \n로 끝나면 끝나지 않았다고 간주함.
	// 만약 csv마지막에 \n이 없다면 그대로 끝내고
	// 있다면 한 번더 반복해서 끝낸다.
	// 두 가지 경우의 수를 한 가지 가정으로 코드를 짜면
	// 두 가지 경우의 수를 동시에 충족시킬 수 없다.

	// 결론적으로 마지막에 줄에 \n이 있는지 없는지 체크하고 
	// 있다면 row_cnt를 하나 뺀다.
	line_size = find_arr_size(line);
	if (line[line_size - 1] == '\n')
		row_cnt--;
	else;


	return row_cnt;
}

// csv파일 경로만 전달하면 column수를 반환함.
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

// csv의 자료형의 위치를 찾아 2차원배열로 반환한다.
// order_column_type 배열로 찾는다.
// [0][] :   숫자 위치 배열
// [1][] : 문자열 위치 배열
int** type_location_find(char* order_column_type, int column)
{
	// [0][], [1][] 생성
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