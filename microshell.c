#include "microshell.h"
#include <stdio.h>
#include <string.h>

void	print_error(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
		write(2, &str[i], 1);
}

int	count_elements(char **av)
{
	int	i;

	i = 0;
	while (av[i] && av[i][0] != '|' && av[i][0] != ';')
	{
		i++;
	}
	return (i);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

char	*ft_strdup(char *str)
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
	dupstr[i] = '\0';
	return (dupstr);
}

void	reset(int *prev, int *next, int i, int count)
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
int		exec_cmds(char **av, char **env)
{
	int	i;
	int	j;
	int	st;
	int	count;
	int	prev[2];
	int	next[2];
	int		exit_status;
	
	count = 1;	
	exit_status = 0;
	j = 0;
	st = 0;
	prev[0] = 0;
	i = 0;
	while (av[i] && av[i][0] != ';')
	{
		if (av[i][0] == '|')
			count++;
		i++;
	}
	int	pid[count];
	if (count == 1 && !strcmp(av[0], "cd"))
	{
		if (!av[1] || (av[1][0] == '|' || av[1][0] == ';'))
		{
			print_error("error: cd: bad arguments\n");
			return (1);
		}
		if (chdir(av[1]))
		{
			print_error("error: cd: cannot change directory to path_to_change\n");
			return (1);
		}
		return (0);
	}
	i = -1;
	while (++i < count && av && *av && (*av)[0] != ';')
	{
		if (j < count - 1)
		{
			if (pipe(next))
			{
				print_error("error: fatal\n");
				exit(1);
			}
		}
		pid[i] = fork();
		if (pid[i] == -1)
		{
			print_error("error: fatal\n");
			exit(1);
		}
		if (pid[i] == 0)
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
			if (execve(av[0], av, env))
			{
				print_error("error: cannot execute ");
				print_error(av[0]);
				print_error("\n");
				exit(1);
			}
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
			reset(prev, next, j, count);
			j++;
		}
	}
	i = -1;
	while (++i < count)
		waitpid(pid[i], &exit_status, 0);
	return (exit_status >> 8);
}


int	main(int ac, char **av, char **env)
{
	int	i;
	int	status;

	(void)ac;
	status = 0;
	i = 1;
	while (av[i])
	{
		status = exec_cmds(&av[i], env);
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
	return (status);
}
