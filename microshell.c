#include "microshell.h"
#include <stdio.h>
#include <string.h>

void	 print_error(char *str)
{
    int		i;

    i = 0;
    while (str[i])
        write(2, &str[i], 1);
}

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

void	reset(int	*prev, int *next, int i, int count)
{	
	if (i > 0 && prev[0])
	{
		close(prev[0]);
		close(prev[1]);
	}
	if (i < count - 1)
	{
		prev[0] = next[0];
		prev[1] = next[1];
	}
}
void	exec_cmds(char **av, char **env)
{
	int		i;
	int		j;
	int		st;
	int		count;
	int		prev[2];
	int		next[2];
	int		pid;
	
	j = 0;
	st = 0;
	prev[0] = 0;
	count = 1;
	i = 0;
	while (av[i] && av[i][0] != ';')
	{
		if (av[i][0] == '|')
			count++;
		i++;
	}
	if (count == 1 && !strcmp(av[0], "cd"))
	{
		if (!av[1])
			print_error("error: cd: bad arguments\n");
		chdir("..");
		printf("%s\n",av[1]);
		return ;
	}
	i = 0;
	while (av && *av && (*av)[0] != ';')
	{	
		if (j < count - 1);
			pipe(next);
		pid = fork();
		if (pid == 0)
		{
			st = 0;
			while (av[st])
			{
				if (av[st][0] == '|' || av[st][0] == ';')
				{
					av[st] = NULL;
					break ;
				}
				st++;
			}
			/*i = 0;
			while (av[i])
			{
				printf("%s\n",av[i]);
				i++;
			}	
			printf("----------\n");
			exit(0);*/
			if (j > 0)
                        {
                                dup2(prev[0], 0);
                                close(prev[0]);
                                close(prev[1]);
                        }
                        if (j < count - 1)
                        {
                                dup2(next[1], 1);
                                close(next[0]);
                                close(next[1]);
                        }
			if (!strcmp(av[0], "cd"))
			{
				chdir(av[1]);
				exit(0);
			}
			execve(av[0], av, env);
			exit(0);
		}
		else
		{
			while (*av)
			{
				if ((*av)[0] == '|')
				{
					av++;
					break ;
				}
				av++;
			}
			reset(prev, next, j ,count);
			j++;
		}
	}
		
	/*while (str[i])
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
	}	*/
}

int		main(int ac, char **av, char **env)
{
	int	i;

	i = 1;
	while (av[i])
	{
		exec_cmds(&av[i], env);
		while (av[i])
		{
			if (av[i] && av[i][0] == ';')
			{
				i++;
				break ;
			}
			i++;
		}
	}
	return 0;
}
