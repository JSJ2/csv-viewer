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

// csv 蛤滓饜葬 寰縑 氈朝 csvだ橾檜 賃 偃檣雖 瓊朝棻
// 斜 偎熱蒂 奩�納挬�.
int count_csv_file()
{
	int csv_cnt = 0;
	FILE_SEARCH fd;
	long handle;
	int result = 1;
	handle = _findfirst("./csv\\*.*", &fd);

	// . 斜葬堅 .. dir 蝶痛
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

// 寡翮擊 瞪殖ж賊 僥濠翮擊 綠錶遽棻.
void str_clear(char string[])
{
	int size = find_arr_size(string);
	for (int i = 0; i < size; i++)
		string[i] = '\0';
}

// 縛攪蒂 援腦賊 csvだ橾曖 唳煎蒂 奩�納挬�.
char* select_csv_file(char** csv_file)
{
	// 蟾晦��
	x = 3, y = 0;

	// だ橾 檜葷 掘褻羹
	FILE_SEARCH fd;

	// だ橾 滲熱
	long handle;

	// csv だ橾 唳煎
	char filename[MAX];
	filename[0] = '\0';

	handle = _findfirst("./csv\\*.*", &fd);

	// だ橾 л熱 奩�秣�
	int result = 1;

	// csv_file曖 size
	int size = 0;

	// . 斜葬堅 .. dir 蝶痛
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
		printf("﹤");

		gotoxy(x, y);

		strcpy(csv_file[size], fd.name);
		printf("%s", csv_file[size]);

		result = _findnext(handle, &fd);
		y += 2;
		size++;
	}
	_findclose(handle);
	gotoxy(0, y);
	printf("﹤ Exit");
	int index = 0;

	int running = 1;
	char key = '\0';
	int move_x = 0, move_y = 0;
	textcolor(12);
	gotoxy(move_x, move_y);
	printf("﹥");
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
			printf("﹤");
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
			printf("﹥");
			break;
		}

		// Down
		case 80:
		{
			textcolor(15);
			gotoxy(move_x, move_y);
			printf("﹤");
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
			printf("﹥");
			break;
		}

		// 縛攪
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
	// だ橾 唳煎蒂 鱔п csvだ橾擊 檗朝棻
	FILE* fpR = fopen(filename, "r");
	if (fpR == NULL)
	{
		printf("%s陛 橈蝗棲棻.\n\n", filename);
		exit(0);
	}
	char line[MAX];
	line[0] = '\0';
	// header 陛螳螃晦
	fgets(line, MAX, fpR);
	del_double_quotation_marks(line); // line縑 氈朝 "蒂 _煎 滲�納挬�.

	char* ptr = strtok(line, ",");
	for (int i = 0; i < column; i++)
	{
		strcpy(header_ptr[i], ptr);
		//printf("header[%d] = %s\n", i, header_ptr[i]);
		ptr = strtok(NULL, ",");
	}

	str_clear(line);
	int double_index = 0, string_index = 0, order_index = 0;
	// csv 瞪羹 等檜攪 陛螳螃晦
	for (int i = 0; i < row; i++)
	{
		fgets(line, MAX, fpR);
		char* ptr = strtok(line, ",");
		while(ptr != NULL)
		{
			switch (order_column_type[order_index])
			{
				// 璋濠
			case '0':
				sscanf(ptr, "%lf", &double_ptr[i][double_index]);
				double_index++;
				break;
				// 僥濠翮
			case '1':
				strcpy(string_ptr[i][string_index], ptr);
				string_index++;
				break;
			}
			order_index++;
			ptr = strtok(NULL, ",");
		}
		// 等檜攪朝 撩奢瞳戲煎 盪濰脾.
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
		// csv 檜葷擊 嫡嬴螢 癱ん檣攪
		char** csv_file = NULL;
		// header 檜葷菟擊 嫡嬴螢 癱ん檣攪
		char** header_ptr = NULL;
		// 褒熱 等檜攪蒂 嫡嬴螢 癱ん檣攪
		double** double_ptr = NULL;
		// 僥濠翮 等檜攪蒂 嫡嬴螢 噙葬ん檣攪
		char*** string_ptr = NULL;
		// 褒熱,僥濠翮曖 牖憮紫蒂 嫡嬴螢 ん檣攪
		char* order_column_type = NULL;

		int double_cnt = 0, string_cnt = 0;
		int row, column;

		mkdir_csv();

		CursorView();

		// csv 蛤滓饜葬 寰縑 氈朝 csvだ橾曖 偎熱
		int csv_cnt = count_csv_file();

		// csv 蛤滓饜葬 寰縑 氈朝 csvだ橾曖 檜葷擊 氬嬴螢 寡翮 翕瞳й渡
		csv_file = assign_csv_arr(csv_cnt);

		// 縛攪蒂 援腦賊 csvだ橾曖 唳煎蒂 奩�納挬�.
		file_ptr = select_csv_file(csv_file);

		// 摹鷗и csvだ橾唳煎蒂 filename縑 盪濰.
		strcpy(filename, file_ptr);

		// csv й渡п薯
		csv_free_assignment(csv_file, csv_cnt);

		// 摹鷗и csvだ橾曖 row 熱
		row = find_row(filename);
		// 摹鷗и csvだ橾曖 column 熱
		column = find_column(filename);

		// 僥濠翮, 璋濠 牖憮 寡翮 翕瞳й渡
		order_column_type = order_assignment(column);

		// header 翕瞳й渡
		header_ptr = header_assignment(filename, column);

		// 摹鷗и csvだ橾縑 璋濠諦 僥濠翮曖 偎熱諦 寡纂牖憮紫蒂 陝陝 陛螳螞棻.
		check_type(filename, order_column_type, &double_cnt, &string_cnt, column);

		// 璋濠 翕瞳й渡
		double_ptr = double_assignment(row, double_cnt);

		// 僥濠翮 翕瞳й渡, 僥濠翮 譆渠 觼晦 127
		string_ptr = string_assignment(row, string_cnt);

		// 嬪縑憮 陛螳螞 璋濠諦 僥濠翮曖 偎熱, 寡纂 牖憮紫蒂 檜辨ж罹 等檜攪蒂 殮溘嫡朝棻.
		input_data(filename, column, row, order_column_type, header_ptr, double_ptr, string_ptr);

		// 殮溘 嫡擎 等檜攪蒂 轎溘.
		print_data(filename, column, row, order_column_type, header_ptr, double_ptr, string_ptr, double_cnt, string_cnt);

		// 瞪睡 翕瞳п羹
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

	printf("摹鷗и csv 唳煎 : %s\n\n", filename);
	printf("﹤ 等檜攪 爾晦\n\n﹤ 等檜攪 啗骯晦");

	int selected_menu = 0;
	bool menu_loop = true;
	while (menu_loop)
	{
		gotoxy(x, y);
		textcolor(12);
		printf("﹥");
		textcolor(15);
		key = _getch();
		switch (key)
		{
		// Up
		case 72:
		{
			gotoxy(x, y);
			textcolor(15);
			printf("﹤");
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
			printf("﹤");
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

		// 縛攪
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
	// й渡
	int** type_location = type_location_find(order_column_type, column);

	bool print_data_loop = true;
	int pp_x = 0, pp_y = 4, pp_index = 0;

	// 等檜攪 轎溘 塽 餌辨濠 檣攪む檜蝶 奩犒僥
	while (print_data_loop)
	{
		x = 0, y = 2;
		int j = 0, double_index = 0, string_index = 0;
		system("cls");
		textcolor(8);
		printf("摹鷗и csv 唳煎 : %s\n\n", filename);
		textcolor(15);

		switch (selected_menu)
		{
		// ﹤ 等檜攪 爾晦
		case 0:
		{
			// ④渦 轎溘
			for (int i = 0; i < column; i++)
			{
				gotoxy(13 + (i * 15), 2);
				printf(" %s", header_ptr[i]); // 奢寥 10偃
			}

			// 嶺 還 轎溘
			gotoxy(0, 3);
			printf("式式式式式式式式式式成式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式");
			gotoxy(6, 4);


			textcolor(15);
			printf("    弛");

			// csv 賅萇 等檜攪 轎溘
			for (int i = 0; i < row; i++)
			{

				// index ( 牖憮 ) 轎溘
				gotoxy(5, i + 4);
				textcolor(5);
				printf("%3d ", i);

				textcolor(15);
				printf(" 弛 ");

				// order_column_type縑 評塭 璋濠 檣雖 僥濠翮檣雖 羹觼ж罹
				// 賅萇 等檜攪蒂 轎溘и棻.
				while (order_column_type[j] != '\0')
				{
					gotoxy(13 + (j * 15), i + 4);
					switch (order_column_type[j])
					{
						// 璋濠
					case '0':
						printf("%-10.2lf\t\t", double_ptr[i][double_index]);
						double_index++;
						break;

						// 僥濠翮
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
			
			// 嬴概 還 轎溘
			printf("式式式式式式式式式式扛式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式\n");


			// 謁 menu_loop蒂 true煎 夥紱橫 輿朝雖 羹觼
			menu_loop = true;

			char key = '\0';

			int pp_screen_move = 1;

			bool cursor_row_header_loop = true;

			int index_column = 0;

			// ④渦縑 籀擠 霞殮ц朝雖
			bool first_in = true;

			// ⑷營 餌辨濠陛 ④渦縑 氈朝雖
			// 晦獄高 : 橈棻.
			int Isheader = 0;

			// ⑷營 餌辨濠陛 ч縑 氈朝雖
			// 晦獄高 : 氈棻.
			int Isrow = 1;

			// 餌辨濠陛 ч 傳朝 翮曖 等檜攪蒂 摹鷗ж罹
			// 薑溺 傳朝 蹺轎擊 й 熱 氈紫煙 掘⑷л.
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

				// 餌辨濠煎睡攪 殮溘擊 嫡嬴螞棻.
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
						printf(" %s", header_ptr[index_column]); // 奢寥 10偃

						index_column--;

						textcolor(13);
						gotoxy(13 + (index_column * 15), 2);
						printf(" %s", header_ptr[index_column]); // 奢寥 10偃

						textcolor(15);
					}
					else
					{
						index_column = 0;
						textcolor(15);
						gotoxy(13 + (index_column * 15), 2);
						printf(" %s", header_ptr[index_column]); // 奢寥 10偃
						Isheader = 0;
						Isrow = 1;

						// 緒螳 釭鬲戲嘎煎 棻衛 霞殮й 陽 籀擠 菟橫諮棻堅 儅陝и棻賊
						// fisrt_in 擎 true煎 滲唳п輿橫撿 и棻.
						first_in = true;
					}
					break;
				}

				// Right
				case 77:
				{
					if (index_column < column - 1)
					{
						// 螃艇薹擊 揚毓擊 陽 index_column檜 0檜塭賊
						// 檜瞪 嬪纂陛 橈戲嘎煎 評煎 薑曖蒂 п輿橫撿 л.
						if (first_in)
						{
							Isheader = 1;
							Isrow = 0;
							textcolor(13);
							gotoxy(13 + (index_column * 15), 2);
							printf(" %s", header_ptr[index_column]); // 奢寥 10偃
							first_in = false;

							textcolor(15);
						}
						else
						{
							// 檜瞪 嬪纂蒂 �羃鶺虞� 轎溘
							textcolor(15);
							gotoxy(13 + (index_column * 15), 2);
							printf(" %s", header_ptr[index_column]); // 奢寥 10偃

							// Right檜嘎煎 1擊 渦л
							index_column++;

							// 遺霜檣 嬪纂蒂 爾塭儀戲煎 轎溘
							textcolor(13);
							gotoxy(13 + (index_column * 15), 2);
							printf(" %s", header_ptr[index_column]); // 奢寥 10偃

							// 斜 檜�� �羃鶺虞� 滲唳
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

				// 縛攪
				case '\r':
				{
					// ч
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
							printf(" %s", header_ptr[i]); // 奢寥 10偃
						}
						gotoxy(0, 3);
						printf("式式式式式式式式式式成式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式");
						gotoxy(6, 4);
						for (int i = 0; i < pp_index; i++)
						{
							gotoxy(5, i + 4);
							textcolor(15);
							printf("     弛 ");
						}
						gotoxy(5, pp_index + 4);
						textcolor(5);
						printf("%3d ", pp_index);
						textcolor(15);
						printf(" 弛 ");

						while (order_column_type[j] != '\0')
						{
							textcolor(12);
							gotoxy(13 + (j * 15), pp_index + 4);
							switch (order_column_type[j])
							{
								// 璋濠
							case '0':
								printf("%-10.2lf\t\t", double_ptr[pp_index][double_index]);
								double_index++;
								break;

								// 僥濠翮
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
						printf("\n式式式式式式式式式式扛式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式式\n");

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

					// 翮
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
									// ж釭曖 嬪纂蒂 夥紱橫 輿歷棻賊 斜 ч曖 等檜攪蒂 瞪睡 衡啖撿 и棻.

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

		// ﹤ 等檜攪 啗骯晦
		case 1:
		{
			for (int i = 0; i < column; i++)
			{
				printf("﹤  %s\n\n", header_ptr[i]);
			}
			int selected_column = 0;
			menu_loop = true;
			while (menu_loop)
			{
				gotoxy(x, y);
				textcolor(12);
				printf("﹥");
				key = _getch();
				switch (key)
				{
					// Up
				case 72:
					gotoxy(x, y);
					textcolor(15);
					printf("﹤");
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
					printf("﹤");
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

			// Enter蒂 揚楝 selected_column縑 column嬪纂蒂 嫡嬴褥.
			// 斜 嬪纂縑 評塭 等檜攪蒂 轎溘ж賊 脾
			switch (order_column_type[selected_column])
			{
				// 璋濠
			case '0':
				/*system("cls");
				printf("0");*/
				break;

				// 僥濠翮
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

	// й渡 п薯
	type_location_free_assignment(type_location, column);
}	