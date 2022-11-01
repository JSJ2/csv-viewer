#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>
#include <conio.h>
#include <malloc.h> // malloc()
#include <io.h>		// access(), _find...()
#include <string.h> // strcmp()
#include "find.h"
#include "win.h"
#include "mkdir.h"
#include "check.h"
#include "assignment.h"
#include "del.h"
#include "sort.h"

#define MAX 128

int x, y;

typedef struct _finddata_t  FILE_SEARCH;

// csv ���丮 �ȿ� �ִ� csv������ �� ������ ã�´�
// �� ������ ��ȯ�Ѵ�.
int count_csv_file()
{
	int csv_cnt = 0;
	FILE_SEARCH fd;
	long handle;
	int result = 1;
	handle = _findfirst("./csv\\*.*", &fd);

	// . �׸��� .. dir ���h
	result = _findnext(handle, &fd);
	result = _findnext(handle, &fd);

	while (result != -1)
	{
		char line[260];
		line[0] = '\0';
		strcpy(line, fd.name);

		char* ptr = strtok(line, ".");
		ptr = strtok(NULL, ".");
		if (strcmp("csv", ptr) != 0)
		{
			result = _findnext(handle, &fd);
			continue;
		}
		result = _findnext(handle, &fd);
		csv_cnt++;
	}
	_findclose(handle);
	return csv_cnt;
}

// �迭�� �����ϸ� ���ڿ��� ����ش�.
void str_clear(char string[])
{
	int size = find_arr_size(string);
	for (int i = 0; i < size; i++)
		string[i] = '\0';
}

// ���͸� ������ csv������ ��θ� ��ȯ�Ѵ�.
char* select_csv_file(char** csv_file)
{
	// �ʱ�ȭ
	x = 3, y = 0;

	// ���� �̸� ����ü
	FILE_SEARCH fd;

	// ���� ����
	long handle;

	// csv ���� ���
	char filename[MAX];
	filename[0] = '\0';

	handle = _findfirst("./csv\\*.*", &fd);

	// ���� �Լ� ��ȯ��
	int result = 1;

	// csv_file�� size
	int size = 0;

	// . �׸��� .. dir ���h
	result = _findnext(handle, &fd);
	result = _findnext(handle, &fd);

	while (result != -1)
	{
		char line[MAX];
		line[0] = '\0';
		strcpy(line, fd.name);

		char* ptr = strtok(line, ".");
		ptr = strtok(NULL, ".");
		if (strcmp("csv", ptr) != 0)
		{
			result = _findnext(handle, &fd);
			continue;
		}

		gotoxy(0, y);
		printf("��");

		gotoxy(x, y);

		strcpy(csv_file[size], fd.name);
		printf("%s", csv_file[size]);

		result = _findnext(handle, &fd);
		y += 2;
		size++;
	}
	_findclose(handle);
	gotoxy(0, y);
	printf("�� Exit");
	int index = 0;

	int running = 1;
	char key = '\0';
	int move_x = 0, move_y = 0;
	textcolor(12);
	gotoxy(move_x, move_y);
	printf("��");
	while (running)
	{
		/*textcolor(15);
		gotoxy(20, 0);
		printf("                                                                                            ");

		gotoxy(20, 0);
		printf("x = %d, y = %d, move_x = %d, move_y = %d, index = %d", x, y, move_x, move_y, index);*/

		key = _getch();
		switch (key)
		{
		// Up
		case 72:
		{
			textcolor(15);
			gotoxy(move_x, move_y);
			printf("��");
			if (move_y == 0)
			{
				index = y / 2;
				move_y = y;
			}
			else if (move_y > 0)
			{
				index--;
				move_y -= 2;
			}
			textcolor(12);
			gotoxy(move_x, move_y);
			printf("��");
			break;
		}

		// Down
		case 80:
		{
			textcolor(15);
			gotoxy(move_x, move_y);
			printf("��");
			if (move_y == y)
			{
				index = 0;
				move_y = 0;
			}
			else if (move_y < y)
			{
				index++;
				move_y += 2;
			}
			textcolor(12);
			gotoxy(move_x, move_y);
			printf("��");
			break;
		}

		// ����
		case '\r':
		{
			textcolor(15);
			if (index == y / 2)
			{
				system("cls");
				exit(0);
			}
			system("cls");
			sprintf(filename, "./csv/%s", csv_file[index]);
			return filename;
		}
		}
	}
}

void input_data(char filename[], int column, int row, char* order_column_type, char** header_ptr, double** double_ptr, char*** string_ptr)
{
	// ���� ��θ� ���� csv������ �д´�
	FILE* fpR = fopen(filename, "r");
	if (fpR == NULL)
	{
		printf("%s�� �����ϴ�.\n\n", filename);
		exit(0);
	}
	char line[MAX];
	line[0] = '\0';
	// header ��������
	fgets(line, MAX, fpR);
	del_double_quotation_marks(line); // line�� �ִ� "�� _�� ��ȯ�Ѵ�.

	char* ptr = strtok(line, ",");
	for (int i = 0; i < column; i++)
	{
		strcpy(header_ptr[i], ptr);
		//printf("header[%d] = %s\n", i, header_ptr[i]);
		ptr = strtok(NULL, ",");
	}

	str_clear(line);
	int double_index = 0, string_index = 0, order_index = 0;
	// csv ��ü ������ ��������
	for (int i = 0; i < row; i++)
	{
		fgets(line, MAX, fpR);
		char* ptr = strtok(line, ",");
		while(ptr != NULL)
		{
			switch (order_column_type[order_index])
			{
				// ����
			case '0':
				sscanf(ptr, "%lf", &double_ptr[i][double_index]);
				double_index++;
				break;
				// ���ڿ�
			case '1':
				strcpy(string_ptr[i][string_index], ptr);
				string_index++;
				break;
			}
			order_index++;
			ptr = strtok(NULL, ",");
		}
		// �����ʹ� ���������� �����.
		/*printf("------------------------------------------------\n");
		for (int j = 0; j < double_index; j++)
			printf("double_ptr[%d][%d] = %lf\n", i, j, double_ptr[i][j]);
		for (int j = 0; j < string_index; j++)
			printf("string_ptr[%d][%d] = %s\n", i, j, string_ptr[i][j]);
		printf("------------------------------------------------\n");*/
		order_index = 0;
		double_index = 0;
		string_index = 0;
		str_clear(line);
	}

	fclose(fpR);
	fpR = NULL;
}

void print_data(char filename[], int column, int row, char* order_column_type, char** header_ptr, double** double_ptr, char*** string_ptr);

int main(void)
{
	fullscreen();
	while (1)
	{
		char* file_ptr;
		char filename[MAX];
		filename[0] = '\0';
		// csv �̸��� �޾ƿ� ��������
		char** csv_file = NULL;
		// header �̸����� �޾ƿ� ��������
		char** header_ptr = NULL;
		// �Ǽ� �����͸� �޾ƿ� ��������
		double** double_ptr = NULL;
		// ���ڿ� �����͸� �޾ƿ� ����������
		char*** string_ptr = NULL;
		// �Ǽ�,���ڿ��� �������� �޾ƿ� ������
		char* order_column_type = NULL;

		int double_cnt = 0, string_cnt = 0;
		int row, column;

		mkdir_csv();

		CursorView();

		// csv ���丮 �ȿ� �ִ� csv������ ����
		int csv_cnt = count_csv_file();

		// csv ���丮 �ȿ� �ִ� csv������ �̸��� ��ƿ� �迭 �����Ҵ�
		csv_file = assign_csv_arr(csv_cnt);

		// ���͸� ������ csv������ ��θ� ��ȯ�Ѵ�.
		file_ptr = select_csv_file(csv_file);

		// ������ csv���ϰ�θ� filename�� ����.
		strcpy(filename, file_ptr);

		// csv �Ҵ�����
		csv_free_assignment(csv_file, csv_cnt);

		// ������ csv������ row ��
		row = find_row(filename);
		// ������ csv������ column ��
		column = find_column(filename);

		// ���ڿ�, ���� ���� �迭 �����Ҵ�
		order_column_type = order_assignment(column);

		// header �����Ҵ�
		header_ptr = header_assignment(filename, column);

		// ������ csv���Ͽ� ���ڿ� ���ڿ��� ������ ��ġ�������� ���� �����´�.
		check_type(filename, order_column_type, &double_cnt, &string_cnt, column);

		// ���� �����Ҵ�
		double_ptr = double_assignment(row, double_cnt);

		// ���ڿ� �����Ҵ�, ���ڿ� �ִ� ũ�� 127
		string_ptr = string_assignment(row, string_cnt);

		// ������ ������ ���ڿ� ���ڿ��� ����, ��ġ �������� �̿��Ͽ� �����͸� �Է¹޴´�.
		input_data(filename, column, row, order_column_type, header_ptr, double_ptr, string_ptr);

		// �Է� ���� �����͸� ���.
		print_data(filename, column, row, order_column_type, header_ptr, double_ptr, string_ptr, double_cnt, string_cnt);

		// ���� ������ü
		header_free_assignment(header_ptr, column);
		double_free_assignment(double_ptr, row, double_cnt);
		string_free_assignment(string_ptr, row, string_cnt);
		system("cls");
	}
}

void print_data(char filename[], int column, int row, char* order_column_type, char** header_ptr, double** double_ptr, char*** string_ptr, int double_cnt, int string_cnt)
{
	int x = 0, y = 2;
	char key = '\0';

	printf("������ csv ��� : %s\n\n", filename);
	printf("�� ������ ����\n\n�� ������ ����");

	int selected_menu = 0;
	bool menu_loop = true;
	while (menu_loop)
	{
		gotoxy(x, y);
		textcolor(12);
		printf("��");
		textcolor(15);
		key = _getch();
		switch (key)
		{
		// Up
		case 72:
		{
			gotoxy(x, y);
			textcolor(15);
			printf("��");
			if (y > 2)
			{
				y -= 2;
				selected_menu = 0;
			}
			/*else if (y == 2)
			{
				y = 4;
			}*/
			break;
		}

		// Down
		case 80:
		{
			gotoxy(x, y);
			textcolor(15);
			printf("��");
			if (y < 4)
			{
				selected_menu = 1;
				y += 2;
			}
			/*else if (y == 4)
			{
				y = 2;
			}*/
			break;
		}

		// ����
		case '\r':
		{
			//printf("selected_column = %d", selected_column);
			menu_loop = false;
			break;
		}

		// ESC
		case 27:
		{
			return;
		}

		default:
			break;
		}
	}
	// �Ҵ�
	int** type_location = type_location_find(order_column_type, column);

	bool print_data_loop = true;
	int pp_x = 0, pp_y = 4, pp_index = 0;

	// ������ ��� �� ����� �������̽� �ݺ���
	while (print_data_loop)
	{
		x = 0, y = 2;
		int j = 0, double_index = 0, string_index = 0;
		system("cls");
		textcolor(8);
		printf("������ csv ��� : %s\n\n", filename);
		textcolor(15);

		switch (selected_menu)
		{
		// �� ������ ����
		case 0:
		{
			// ��� ���
			for (int i = 0; i < column; i++)
			{
				gotoxy(13 + (i * 15), 2);
				printf(" %s", header_ptr[i]); // ���� 10��
			}

			// �� �� ���
			gotoxy(0, 3);
			printf("������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������");
			gotoxy(6, 4);


			textcolor(15);
			printf("    ��");

			// csv ��� ������ ���
			for (int i = 0; i < row; i++)
			{

				// index ( ���� ) ���
				gotoxy(5, i + 4);
				textcolor(5);
				printf("%3d ", i);

				textcolor(15);
				printf(" �� ");

				// order_column_type�� ���� ���� ���� ���ڿ����� üũ�Ͽ�
				// ��� �����͸� ����Ѵ�.
				while (order_column_type[j] != '\0')
				{
					gotoxy(13 + (j * 15), i + 4);
					switch (order_column_type[j])
					{
						// ����
					case '0':
						printf("%-10.2lf\t\t", double_ptr[i][double_index]);
						double_index++;
						break;

						// ���ڿ�
					case '1':
						printf("%-10s\t\t", string_ptr[i][string_index]);
						string_index++;
						break;
					default:
						break;
					}
					j++;
				}
				printf("\n");
				j = 0, double_index = 0, string_index = 0;
			}
			
			// �Ʒ� �� ���
			printf("������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������\n");


			// �� menu_loop�� true�� �ٲپ� �ִ��� üũ
			menu_loop = true;

			char key = '\0';

			int pp_screen_move = 1;

			bool cursor_row_header_loop = true;

			int index_column = 0;

			// ����� ó�� �����ߴ���
			bool first_in = true;

			// ���� ����ڰ� ����� �ִ���
			// �⺻�� : ����.
			int Isheader = 0;

			// ���� ����ڰ� �࿡ �ִ���
			// �⺻�� : �ִ�.
			int Isrow = 1;

			// ����ڰ� �� �Ǵ� ���� �����͸� �����Ͽ�
			// ���� �Ǵ� ������ �� �� �ֵ��� ������.
			while (cursor_row_header_loop)
			{
				//gotoxy(130, 2);
				//printf("Isheader : %d, Isrow : %d", Isheader, Isrow);
				if (Isrow)
				{
					gotoxy(pp_x, pp_y);
					textcolor(12);
					printf(">>>");

					textcolor(15);
				}
				else
				{
					gotoxy(pp_x, pp_y);
					printf("   ");
				}

				// ����ڷκ��� �Է��� �޾ƿ´�.
				key = _getch();

				switch (key)
				{
				// Left
				case 75:
				{
					if (index_column > 0)
					{
						textcolor(15);
						gotoxy(13 + (index_column * 15), 2);
						printf(" %s", header_ptr[index_column]); // ���� 10��

						index_column--;

						textcolor(13);
						gotoxy(13 + (index_column * 15), 2);
						printf(" %s", header_ptr[index_column]); // ���� 10��

						textcolor(15);
					}
					else
					{
						index_column = 0;
						textcolor(15);
						gotoxy(13 + (index_column * 15), 2);
						printf(" %s", header_ptr[index_column]); // ���� 10��
						Isheader = 0;
						Isrow = 1;

						// ���� �������Ƿ� �ٽ� ������ �� ó�� ���Դٰ� �����Ѵٸ�
						// fisrt_in �� true�� �������־�� �Ѵ�.
						first_in = true;
					}
					break;
				}

				// Right
				case 77:
				{
					if (index_column < column - 1)
					{
						// �������� ������ �� index_column�� 0�̶��
						// ���� ��ġ�� �����Ƿ� ���� ���Ǹ� ���־�� ��.
						if (first_in)
						{
							Isheader = 1;
							Isrow = 0;
							textcolor(13);
							gotoxy(13 + (index_column * 15), 2);
							printf(" %s", header_ptr[index_column]); // ���� 10��
							first_in = false;

							textcolor(15);
						}
						else
						{
							// ���� ��ġ�� ������� ���
							textcolor(15);
							gotoxy(13 + (index_column * 15), 2);
							printf(" %s", header_ptr[index_column]); // ���� 10��

							// Right�̹Ƿ� 1�� ����
							index_column++;

							// ������ ��ġ�� ��������� ���
							textcolor(13);
							gotoxy(13 + (index_column * 15), 2);
							printf(" %s", header_ptr[index_column]); // ���� 10��

							// �� ���� ������� ����
							textcolor(15);
						}
					}
					break;
				}

				// Up
				case 72:
				{
					gotoxy(pp_x, pp_y);
					printf("   ");
					if (pp_y > 4)
					{
						pp_screen_move = 1;
						pp_y -= 1;
						pp_index--;
					}
					else if (pp_y == 4)
					{
						gotoxy(pp_x, pp_y - pp_screen_move);
						pp_screen_move++;
						printf("");
						/*
						pp_y = row + 3;
						pp_index = row - 1;*/
					}
					break;
				}

				//Down
				case 80:
				{
					gotoxy(pp_x, pp_y);
					printf("   ");
					if (pp_y < row + 3)
					{
						pp_screen_move = 1;
						pp_y += 1;
						pp_index++;
					}
					else if (pp_y == row + 3)
					{
						gotoxy(pp_x, pp_y + pp_screen_move);
						if (pp_screen_move < 3)
							pp_screen_move++;
						printf("");
						/*pp_y = 4;
						pp_index = 0;*/
					}
					break;
				}

				// ����
				case '\r':
				{
					// ��
					if (Isrow)
					{
						textcolor(15);
						system("cls");

						gotoxy(pp_x, pp_y);
						textcolor(12);
						printf(">>>");
						textcolor(15);
						for (int i = 0; i < column; i++)
						{
							gotoxy(13 + (i * 15), 2);
							printf(" %s", header_ptr[i]); // ���� 10��
						}
						gotoxy(0, 3);
						printf("������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������");
						gotoxy(6, 4);
						for (int i = 0; i < pp_index; i++)
						{
							gotoxy(5, i + 4);
							textcolor(15);
							printf("     �� ");
						}
						gotoxy(5, pp_index + 4);
						textcolor(5);
						printf("%3d ", pp_index);
						textcolor(15);
						printf(" �� ");

						while (order_column_type[j] != '\0')
						{
							textcolor(12);
							gotoxy(13 + (j * 15), pp_index + 4);
							switch (order_column_type[j])
							{
								// ����
							case '0':
								printf("%-10.2lf\t\t", double_ptr[pp_index][double_index]);
								double_index++;
								break;

								// ���ڿ�
							case '1':
								printf("%-10s\t\t", string_ptr[pp_index][string_index]);
								string_index++;
								break;
							default:
								break;
							}
							j++;
						}
						textcolor(15);
						printf("\n������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������\n");

						bool enter_row_loop = true;
						//printf("%d\n\n", pp_index);
						key = '\0';
						while (enter_row_loop)
						{
							key = _getch();
							switch (key)
							{
								// esc
							case 27:
								system("cls");
								enter_row_loop = false;
								cursor_row_header_loop = false;
								print_data_loop = true;
								break;

								// enter
							case '\r':
								system("cls");
								double** temp_double_ptr = double_ptr;
								Sequential_Sort(temp_double_ptr, double_cnt, row);
								enter_row_loop = false;
								cursor_row_header_loop = false;
								print_data_loop = false;

								break;

							default:
								break;
							}
						}
						break;
					}

					// ��
					else if (Isheader)
					{
						/*system("cls");
						printf("%-10s\n", header_ptr[index_column]);
						printf("index_column : %-10d\n", index_column);


						printf("order_column_type : %c\n", order_column_type[index_column]);
						printf("type_location[0][%d] : %d\n", index_column, type_location[0][index_column]);

						system("pause");*/

						
						double temp_double = 0.f;
						char temp_string[STRING_SIZE];	temp_string[0] = '\0';
						for (int i = 0; i < row; i++)
						{
							for (int j = i + 1; j < row; j++)
							{
								if (double_ptr[i][type_location[0][index_column]] > double_ptr[j][type_location[0][index_column]])
								{
									// �ϳ��� ��ġ�� �ٲپ� �־��ٸ� �� ���� �����͸� ���� �Űܾ� �Ѵ�.

									for (int k = 0; k < column; k++)
									{
										switch (order_column_type[k])
										{
										case '0':
										{
											temp_double = double_ptr[i][type_location[0][k]];
											double_ptr[i][type_location[0][k]] = double_ptr[j][type_location[0][k]];
											double_ptr[j][type_location[0][k]] = temp_double;
											break;
										}
										case '1':
										{
											strcpy(temp_string, string_ptr[i][type_location[1][k]]);
											strcpy(string_ptr[i][type_location[1][k]], string_ptr[j][type_location[1][k]]);
											strcpy(string_ptr[j][type_location[1][k]], temp_string);
											break;
										}
										default:
											break;
										}
										
									}
								}
							}
						}

						/*system("pause");
						for (int i = 0; i < row; i++)
							printf("double_ptr[%d][%d] : %-10lf\n", i, type_location[0][index_column], double_ptr[i][type_location[0][index_column]]);

						system("pause");*/
						
						
						print_data_loop = true;
						cursor_row_header_loop = false;
					}
					break;
				}

				// ESC
				case 27:
				{
					print_data_loop = false;
					cursor_row_header_loop = false;
					break;
				}

				default:
					break;
				}
			}
			break;
		}

		// �� ������ ����
		case 1:
		{
			for (int i = 0; i < column; i++)
			{
				printf("��  %s\n\n", header_ptr[i]);
			}
			int selected_column = 0;
			menu_loop = true;
			while (menu_loop)
			{
				gotoxy(x, y);
				textcolor(12);
				printf("��");
				key = _getch();
				switch (key)
				{
					// Up
				case 72:
					gotoxy(x, y);
					textcolor(15);
					printf("��");
					if (y > 2)
					{
						y -= 2;
						selected_column--;
					}
					else if (y == 2)
					{
						y = column * 2;
						selected_column = column - 1;
					}
					break;
					// Down
				case 80:
					gotoxy(x, y);
					textcolor(15);
					printf("��");
					if (y < column * 2)
					{
						y += 2;
						selected_column++;
					}
					else if (y == column * 2)
					{
						y = 2;
						selected_column = 0;
					}
					break;
				case '\r':
					textcolor(15);
					//printf("selected_column = %d", selected_column);
					menu_loop = false;
					break;
				default:
					break;
				}
			}

			// Enter�� ���� selected_column�� column��ġ�� �޾ƿ�.
			// �� ��ġ�� ���� �����͸� ����ϸ� ��
			switch (order_column_type[selected_column])
			{
				// ����
			case '0':
				/*system("cls");
				printf("0");*/
				break;

				// ���ڿ�
			case '1':
				/*system("cls");
				printf("1");*/
				break;
			}
			break;
		}

		default:
			break;
		}
	}

	// �Ҵ� ����
	type_location_free_assignment(type_location, column);
}	