#include <stdlib.h>

void	ft_bzero(void *s, size_t n)
{
	int				i;
	unsigned char	*ptr;

	i = 0;
	ptr = (unsigned char*)s;
	while (n > 0)
	{
		ptr[i++] = 0;
		n--;
	}
}
