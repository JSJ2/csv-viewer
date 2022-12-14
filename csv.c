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

// csv 디렉토리 안에 있는 csv파일이 몇 개인지 찾는다
// 그 갯수를 반환한다.
int count_csv_file()
{
	int csv_cnt = 0;
	FILE_SEARCH fd;
	long handle;
	int result = 1;
	handle = _findfirst("./csv\\*.*", &fd);

	// . 그리고 .. dir 스킾
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

// 배열을 전달하면 문자열을 비워준다.
void str_clear(char string[])
{
	int size = find_arr_size(string);
	for (int i = 0; i < size; i++)
		string[i] = '\0';
}

// 엔터를 누르면 csv파일의 경로를 반환한다.
char* select_csv_file(char** csv_file)
{
	// 초기화
	x = 3, y = 0;

	// 파일 이름 구조체
	FILE_SEARCH fd;

	// 파일 변수
	long handle;

	// csv 파일 경로
	char filename[MAX];
	filename[0] = '\0';

	handle = _findfirst("./csv\\*.*", &fd);

	// 파일 함수 반환값
	int result = 1;

	// csv_file의 size
	int size = 0;

	// . 그리고 .. dir 스킾
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
		printf("□");

		gotoxy(x, y);

		strcpy(csv_file[size], fd.name);
		printf("%s", csv_file[size]);

		result = _findnext(handle, &fd);
		y += 2;
		size++;
	}
	_findclose(handle);
	gotoxy(0, y);
	printf("□ Exit");
	int index = 0;

	int running = 1;
	char key = '\0';
	int move_x = 0, move_y = 0;
	textcolor(12);
	gotoxy(move_x, move_y);
	printf("■");
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
			printf("□");
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
			printf("■");
			break;
		}

		// Down
		case 80:
		{
			textcolor(15);
			gotoxy(move_x, move_y);
			printf("□");
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
			printf("■");
			break;
		}

		// 엔터
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
	// 파일 경로를 통해 csv파일을 읽는다
	FILE* fpR = fopen(filename, "r");
	if (fpR == NULL)
	{
		printf("%s가 없습니다.\n\n", filename);
		exit(0);
	}
	char line[MAX];
	line[0] = '\0';
	// header 가져오기
	fgets(line, MAX, fpR);
	del_double_quotation_marks(line); // line에 있는 "를 _로 변환한다.

	char* ptr = strtok(line, ",");
	for (int i = 0; i < column; i++)
	{
		strcpy(header_ptr[i], ptr);
		//printf("header[%d] = %s\n", i, header_ptr[i]);
		ptr = strtok(NULL, ",");
	}

	str_clear(line);
	int double_index = 0, string_index = 0, order_index = 0;
	// csv 전체 데이터 가져오기
	for (int i = 0; i < row; i++)
	{
		fgets(line, MAX, fpR);
		char* ptr = strtok(line, ",");
		while(ptr != NULL)
		{
			switch (order_column_type[order_index])
			{
				// 숫자
			case '0':
				sscanf(ptr, "%lf", &double_ptr[i][double_index]);
				double_index++;
				break;
				// 문자열
			case '1':
				strcpy(string_ptr[i][string_index], ptr);
				string_index++;
				break;
			}
			order_index++;
			ptr = strtok(NULL, ",");
		}
		// 데이터는 성공적으로 저장됨.
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
		// csv 이름을 받아올 투포인터
		char** csv_file = NULL;
		// header 이름들을 받아올 투포인터
		char** header_ptr = NULL;
		// 실수 데이터를 받아올 투포인터
		double** double_ptr = NULL;
		// 문자열 데이터를 받아올 쓰리포인터
		char*** string_ptr = NULL;
		// 실수,문자열의 순서도를 받아올 포인터
		char* order_column_type = NULL;

		int double_cnt = 0, string_cnt = 0;
		int row, column;

		mkdir_csv();

		CursorView();

		// csv 디렉토리 안에 있는 csv파일의 갯수
		int csv_cnt = count_csv_file();

		// csv 디렉토리 안에 있는 csv파일의 이름을 담아올 배열 동적할당
		csv_file = assign_csv_arr(csv_cnt);

		// 엔터를 누르면 csv파일의 경로를 반환한다.
		file_ptr = select_csv_file(csv_file);

		// 선택한 csv파일경로를 filename에 저장.
		strcpy(filename, file_ptr);

		// csv 할당해제
		csv_free_assignment(csv_file, csv_cnt);

		// 선택한 csv파일의 row 수
		row = find_row(filename);
		// 선택한 csv파일의 column 수
		column = find_column(filename);

		// 문자열, 숫자 순서 배열 동적할당
		order_column_type = order_assignment(column);

		// header 동적할당
		header_ptr = header_assignment(filename, column);

		// 선택한 csv파일에 숫자와 문자열의 갯수와 배치순서도를 각각 가져온다.
		check_type(filename, order_column_type, &double_cnt, &string_cnt, column);

		// 숫자 동적할당
		double_ptr = double_assignment(row, double_cnt);

		// 문자열 동적할당, 문자열 최대 크기 127
		string_ptr = string_assignment(row, string_cnt);

		// 위에서 가져온 숫자와 문자열의 갯수, 배치 순서도를 이용하여 데이터를 입력받는다.
		input_data(filename, column, row, order_column_type, header_ptr, double_ptr, string_ptr);

		// 입력 받은 데이터를 출력.
		print_data(filename, column, row, order_column_type, header_ptr, double_ptr, string_ptr, double_cnt, string_cnt);

		// 전부 동적해체
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

	printf("선택한 csv 경로 : %s\n\n", filename);
	printf("□ 데이터 보기\n\n□ 데이터 계산기");

	int selected_menu = 0;
	bool menu_loop = true;
	while (menu_loop)
	{
		gotoxy(x, y);
		textcolor(12);
		printf("■");
		textcolor(15);
		key = _getch();
		switch (key)
		{
		// Up
		case 72:
		{
			gotoxy(x, y);
			textcolor(15);
			printf("□");
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
			printf("□");
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

		// 엔터
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
	// 할당
	int** type_location = type_location_find(order_column_type, column);

	bool print_data_loop = true;
	int pp_x = 0, pp_y = 4, pp_index = 0;

	// 데이터 출력 및 사용자 인터페이스 반복문
	while (print_data_loop)
	{
		x = 0, y = 2;
		int j = 0, double_index = 0, string_index = 0;
		system("cls");
		textcolor(8);
		printf("선택한 csv 경로 : %s\n\n", filename);
		textcolor(15);

		switch (selected_menu)
		{
		// □ 데이터 보기
		case 0:
		{
			// 헤더 출력
			for (int i = 0; i < column; i++)
			{
				gotoxy(13 + (i * 15), 2);
				printf(" %s", header_ptr[i]); // 공백 10개
			}

			// 윗 줄 출력
			gotoxy(0, 3);
			printf("──────────┬────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────");
			gotoxy(6, 4);


			textcolor(15);
			printf("    │");

			// csv 모든 데이터 출력
			for (int i = 0; i < row; i++)
			{

				// index ( 순서 ) 출력
				gotoxy(5, i + 4);
				textcolor(5);
				printf("%3d ", i);

				textcolor(15);
				printf(" │ ");

				// order_column_type에 따라 숫자 인지 문자열인지 체크하여
				// 모든 데이터를 출력한다.
				while (order_column_type[j] != '\0')
				{
					gotoxy(13 + (j * 15), i + 4);
					switch (order_column_type[j])
					{
						// 숫자
					case '0':
						printf("%-10.2lf\t\t", double_ptr[i][double_index]);
						double_index++;
						break;

						// 문자열
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
			
			// 아랫 줄 출력
			printf("──────────┴────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────\n");


			// 왜 menu_loop를 true로 바꾸어 주는지 체크
			menu_loop = true;

			char key = '\0';

			int pp_screen_move = 1;

			bool cursor_row_header_loop = true;

			int index_column = 0;

			// 헤더에 처음 진입했는지
			bool first_in = true;

			// 현재 사용자가 헤더에 있는지
			// 기본값 : 없다.
			int Isheader = 0;

			// 현재 사용자가 행에 있는지
			// 기본값 : 있다.
			int Isrow = 1;

			// 사용자가 행 또는 열의 데이터를 선택하여
			// 정렬 또는 추출을 할 수 있도록 구현함.
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

				// 사용자로부터 입력을 받아온다.
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
						printf(" %s", header_ptr[index_column]); // 공백 10개

						index_column--;

						textcolor(13);
						gotoxy(13 + (index_column * 15), 2);
						printf(" %s", header_ptr[index_column]); // 공백 10개

						textcolor(15);
					}
					else
					{
						index_column = 0;
						textcolor(15);
						gotoxy(13 + (index_column * 15), 2);
						printf(" %s", header_ptr[index_column]); // 공백 10개
						Isheader = 0;
						Isrow = 1;

						// 빠져 나갔으므로 다시 진입할 때 처음 들어왔다고 생각한다면
						// fisrt_in 은 true로 변경해주어야 한다.
						first_in = true;
					}
					break;
				}

				// Right
				case 77:
				{
					if (index_column < column - 1)
					{
						// 오른쪽을 눌렀을 때 index_column이 0이라면
						// 이전 위치가 없으므로 따로 정의를 해주어야 함.
						if (first_in)
						{
							Isheader = 1;
							Isrow = 0;
							textcolor(13);
							gotoxy(13 + (index_column * 15), 2);
							printf(" %s", header_ptr[index_column]); // 공백 10개
							first_in = false;

							textcolor(15);
						}
						else
						{
							// 이전 위치를 흰색으로 출력
							textcolor(15);
							gotoxy(13 + (index_column * 15), 2);
							printf(" %s", header_ptr[index_column]); // 공백 10개

							// Right이므로 1을 더함
							index_column++;

							// 움직인 위치를 보라색으로 출력
							textcolor(13);
							gotoxy(13 + (index_column * 15), 2);
							printf(" %s", header_ptr[index_column]); // 공백 10개

							// 그 이후 흰색으로 변경
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

				// 엔터
				case '\r':
				{
					// 행
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
							printf(" %s", header_ptr[i]); // 공백 10개
						}
						gotoxy(0, 3);
						printf("──────────┬────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────");
						gotoxy(6, 4);
						for (int i = 0; i < pp_index; i++)
						{
							gotoxy(5, i + 4);
							textcolor(15);
							printf("     │ ");
						}
						gotoxy(5, pp_index + 4);
						textcolor(5);
						printf("%3d ", pp_index);
						textcolor(15);
						printf(" │ ");

						while (order_column_type[j] != '\0')
						{
							textcolor(12);
							gotoxy(13 + (j * 15), pp_index + 4);
							switch (order_column_type[j])
							{
								// 숫자
							case '0':
								printf("%-10.2lf\t\t", double_ptr[pp_index][double_index]);
								double_index++;
								break;

								// 문자열
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
						printf("\n──────────┴────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────\n");

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

					// 열
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
									// 하나의 위치를 바꾸어 주었다면 그 행의 데이터를 전부 옮겨야 한다.

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

		// □ 데이터 계산기
		case 1:
		{
			for (int i = 0; i < column; i++)
			{
				printf("□  %s\n\n", header_ptr[i]);
			}
			int selected_column = 0;
			menu_loop = true;
			while (menu_loop)
			{
				gotoxy(x, y);
				textcolor(12);
				printf("■");
				key = _getch();
				switch (key)
				{
					// Up
				case 72:
					gotoxy(x, y);
					textcolor(15);
					printf("□");
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
					printf("□");
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

			// Enter를 눌러 selected_column에 column위치를 받아옴.
			// 그 위치에 따라 데이터를 출력하면 됨
			switch (order_column_type[selected_column])
			{
				// 숫자
			case '0':
				/*system("cls");
				printf("0");*/
				break;

				// 문자열
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

	// 할당 해제
	type_location_free_assignment(type_location, column);
}	