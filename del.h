// 문자열에서 "(쌍따옴표)를 제거한다.
void del_double_quotation_marks(char* s)
{
	for (int i = 0; s[i] != '\0'; i++)
	{
		switch (s[i])
		{
		case '\"':
			s[i] = '_';
			break;
		default:
			break;
		}
	}
}

// 문자열에서 \n(줄바꿈)를 제거한다.
void del_line_breaking(char* s)
{
	for (int i = 0; s[i] != '\0'; i++)
	{
		switch (s[i])
		{
		case '\n':
			s[i] = '\0';
			break;
		default:
			break;
		}
	}
}