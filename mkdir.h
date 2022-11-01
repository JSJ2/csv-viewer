#include <string.h>
#include <Windows.h>
#include <stdio.h>
#include <io.h>

#define MAX 128

// ���ϴ� ���丮�� �̸��� �����ϸ�
// ���� ��ġ�� ���������.
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

// ���� ���丮�� csv ���丮�� ���������.
void mkdir_csv()
{
	int nResult = _access("./csv", 0);
	// csv ���丮�� ���ٸ� ����� 1�� ��ȯ��
	// �ִٸ� 0�� ��ȯ��.
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

// �����ʿ� �ִ� ���ڿ��� �̸����� ������ csv�����̸��� �������� ����ȴ�.
void connect_string_with_csv(char s1[], char* s2) {
	sprintf(s1, "./csv/%s.csv", s2);
}

// csv�̸��� �����ϸ� �� ������ csv���丮 �ȿ� �ִ��� üũ�Ѵ�.
// ���� �ִٸ� 1�� ��ȯ
// ���ٸ� 0�� ��ȯ�Ѵ�.
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