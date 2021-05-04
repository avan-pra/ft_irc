int		char2hex(char input)
{
	if(input >= '0' && input <= '9')
		return input - '0';
	if(input >= 'A' && input <= 'F')
		return input - 'A' + 10;
	if(input >= 'a' && input <= 'f')
		return input - 'a' + 10;
	return -1;
}

char	hex2char(char input)
{
	if(input >= 0 && input <= 9)
		return input + '0';
	if(input >= 10 && input <= 15)
		return input + 'a' - 10;
	return -1;
}
