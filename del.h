// ���ڿ����� "(�ֵ���ǥ)�� �����Ѵ�.
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

// ���ڿ����� \n(�ٹٲ�)�� �����Ѵ�.
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