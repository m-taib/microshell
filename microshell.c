#include "microshell.h"
#include <stdio.h>

int		count_elements(char **av)
{
	int		i;

	i = 0;
	while (av[i] && av[i][0] != '|' && av[i][0] != ';')
	{
		i++;
	}
	return (i);
}

int		ft_strlen(char *str)
{
	int		i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(char	*str)
{
	char	*dupstr;
	int		i;

	i = 0;
	dupstr = malloc(ft_strlen(str) + 1);
	if (!dupstr)
		return (0);
	i = 0;
	while (str[i])
	{
		dupstr[i] = str[i];
		i++;
	}
	dupstr[i] ='\0';
	return (dupstr);
}

void	reset(int	*prev, int *next)
{	
	if (prev[0])
	{
		close(prev[0]);
		close(prev[1]);
	}
	else
	{
		prev[0] = next[0];
		prev[1] = next[1];
	}
}
void	exec_cmds(char **str, char **env)
{
	int		i;
	int		count;
	int		prev[2];
	int		next[2];
	int		pid;

	prev[0] = 0;
	count = 1;
	i = 0;	
	while (str[i])
	{
		if (str[i][0] == '|')
			count++;
		i++;
	}
	i = -1;
	while (++i < count)
	{
		if (i < count - 1)
			pipe(next);
		pid = fork();
		if (pid < 0)
			exit(0);
		if (pid == 0)
		{
			if (i > 0)
			{
				dup2(prev[0], 0);
				close(prev[0]);
				close(prev[1]);
			}
			if (i < count - 1)
			{
				dup2(next[1], 1);
				close(next[0]);
				close(next[1]);
			}
			execve(str[0], str, env);
		}
		reset(prev, next);
	}	
}

int		main(int ac, char **av, char **env)
{
	int		i;
	char	***cmds;
	int		count;
	int		j;
	int		s;

	i = 1;
	count = 1;
	while (av[i])
	{
		if (av[i][0] == ';' && av[i + 1])
			count++;
		i++;
	}
	cmds = malloc(sizeof(char **) * (count + 1));
	if (!cmds)
		return (0);
	cmds[count] = NULL;
	i = 1;
	j = 0;
	while (av[i])
	{
		count = count_elements(&av[i]);
		cmds[j] = malloc(sizeof(char *) * (count + 1));
		if (!cmds[j])
			return (0);
		s = 0;
		while (av[i] && av[i][0] != '|' && av[i][0] != ';')
		{
			cmds[j][s] = ft_strdup(av[i]);
			printf("%s\n",cmds[j][s]);
			i++;
			s++;
		}
		printf("count = %d\n",count);
		printf("%d\n",s);
		cmds[j][s] = NULL;
		j++;
		if (av[i])
			i++;
	}
	s = 0;
	//printf("%s\n",cmds[0][s]);
	//exit(0);
	s = 0;
	while (cmds[0][s])
	{
		printf("%s\n",cmds[0][s]);
		s++;
	}
	exit(0);
	i = 1;
	/*while (av[i])
	{
		count = count_elements(&av[i]) + 1;
		printf("%d\n",count);
		exit(0);
		cmds[j] = malloc(sizeof(char *) * count);
		if (!cmds[j])
			return (0);
		while (av[i] && av[i][0] != '|' && av[i][0] != ';')
			i++;
		while (av[i] && av[i][0] != '|' && av[i][0] != ';')
		{
			cmds[count] = NULL;
			i++;
		}
		j++;

	}*/
	j = 0;
	i = 1;
	while (av[i])
	{
		s = 0;
		while (av[i] && av[i][0] != ';')
		{
			cmds[j][s] = ft_strdup(av[i]);
			i++;
			s++;
		}
		if (!av[i] || (av[i] && av[i][0] == ';'))
		{
			exec_cmds(cmds[j], env);
			j++;
		}
		if (av[i])
			i++;
		s++;
	}
}
