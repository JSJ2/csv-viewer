#include <string.h>
#include <Windows.h>
#include <stdio.h>
#include <io.h>

#define MAX 128

// 원하는 디렉토리의 이름을 전달하면
// 현재 위치에 만들어진다.
void mkdir_csv_csvname(char dirname[])
{
	char cmd[MAX];
	cmd[0] = '\0';
	char filename[MAX];
	filename[0] = '\0';

	sprintf(cmd, "mkdir %s", dirname);
	sprintf(filename, "./%s", dirname);

	int nResult = _access(filename, 0);

	switch (nResult)
	{
	case 0:
		return;
	case -1:
		system(cmd);
		break;
	}
}

// 현재 디렉토리에 csv 디렉토리가 만들어진다.
void mkdir_csv()
{
	int nResult = _access("./csv", 0);
	// csv 디렉토리가 없다면 만들고 1을 반환함
	// 있다면 0을 반환함.
	switch (nResult)
	{
	case 0:
		return;
	case -1:
		system("mkdir csv");
		printf("Success!! : Directory is created.\n\n");
		break;
	}
}

// 오른쪽에 있는 문자열을 이름으로 가지는 csv파일이름이 왼쪽으로 연결된다.
void connect_string_with_csv(char s1[], char* s2) {
	sprintf(s1, "./csv/%s.csv", s2);
}

// csv이름을 전달하면 그 파일이 csv디렉토리 안에 있는지 체크한다.
// 만약 있다면 1을 반환
// 없다면 0을 반환한다.
int exist_csv_file(char csv_name[]) {
	char* filename = NULL;

	connect_string_with_csv(filename, csv_name);

	FILE* fpR = fopen(filename, "r");
	if (fpR == NULL)
	{
		printf("\nFailed.. : We can't find '%s'..\n\n", filename);
		return 0;
	}
	else
	{
		fclose(fpR);

		printf("\nSuccess!! : Exist '%s'!\n\n", filename);
		return 1;
	}
}