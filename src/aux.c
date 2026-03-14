#include "lem_ipc.h"

void	*ft_memset(void *str, int c, size_t n) {
	size_t			i;
	unsigned char	*s;

	i = 0;
	s = str;
	while (i < n)
	{
		s[i] = (unsigned char)c;
		i++;
	}
	s = str;
	return (str);
}

bool is_valid_team(const char *str) {
    int i = 0;
    if (str[i] == '+' || str[i] == '-') i++;
    while (str[i]) {
        if (str[i] < '0' || str[i] > '9')
            return false;
        i++;
    }
    return true;
}

bool check_args(int argc, char *argv[]) {
	if (argc != 2) {
        printf("Error: ./lem_ipc <team_id>");
        return false;
    }
	if (!is_valid_team(argv[1])){
		printf("Error: Team ID not valid, must be a digit between 0 and %i", MAX_TEAMS);
		return false;
	}
    int team_id = ft_atoi(argv[1]);
	if (team_id < 1 || team_id > MAX_TEAMS) {
		printf("Error: Team ID out of bounds, must be a digit between 0 and %i", MAX_TEAMS);
		return false;
	}
	return true;
}

static int ft_transform(const char *str, int i) {
    int    result;
    int    digit;
    result = 0;
    while (str[i] >= '0' && str[i] <= '9')
    {
        digit = str[i] - '0';
        result = result * 10 + digit;
        i++;
    }
    return (result);
}

int ft_atoi(const char *str) {
    int    i;
    int    sign;
    int    result;
    i = 0;
    sign = 1;
    result = 0;
    while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n'
        || str[i] == '\r' || str[i] == '\f' || str[i] == '\v')
    {
        i++;
    }
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
        {
            sign = -1;
        }
        i++;
    }
    result = ft_transform(str, i);
    return (sign * result);
}

void ft_putnbr(int n) {
    if (n >= 10)
        ft_putnbr(n / 10);
    ft_putchar('0' + n % 10);
}

void ft_putchar(char c) {
    write(1, &c, 1);
}

static unsigned int	ft_numlen(int n)
{
	unsigned int	count;

	count = 0;
	if (n == 0)
		return (1);
	if (n < 0)
		count++;
	while (n != 0)
	{
		n /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	unsigned int	len;
	char			*result;
	unsigned int	num;

	len = ft_numlen(n);
	result = (char *)malloc((len + 1) * sizeof(char));
	if (result == NULL)
		return (NULL);
	if (n < 0)
	{
		result[0] = '-';
		num = -n;
	}
	else
		num = n;
	if (num == 0)
		result[0] = '0';
	result[len] = '\0';
	while (num != 0)
	{
		result[len - 1] = '0' + (num % 10);
		num = num / 10;
		len--;
	}
	return (result);
}

int manhattan(t_position a, t_position b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}