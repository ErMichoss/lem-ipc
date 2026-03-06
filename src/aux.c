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