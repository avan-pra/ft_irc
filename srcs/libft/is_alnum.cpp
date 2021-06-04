int		ft_isalnum(int c)
{
	if (c < 48 || c > 122 || (c > 57 && c < 65) || (c > 90 && c < 97))
		return (0);
	return (1);
}
