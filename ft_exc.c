

#include "minishell.h"

static int	ft_strrch(char *str, char c)
{
	int	len;

	len = ft_strlen(str);
	while (len > 0)
	{
		if (str[len] == c)
			return (len);
		len--;
	}
	return (0);
}
static char *ft_substrr(char *str, int i)
{
	char	*retl;
	int		len;
	int		j;

	len = 0;
	j = 0; 
	len = ft_strlen(str);
	retl = malloc((len - i + 1) * sizeof(char));
	if (!retl)
		return (NULL);
	while (i < len)
	{
		retl[j] = str[i];
		i++;
		j++;
	}
	retl[j] = '\0';
	return (retl);

}

char **ft_env(t_env *envir)
{
	char **env;
	t_env *tmp;
	int	size;
	int	i;

	tmp = envir;
	size=0;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	env = malloc((size + 1)*sizeof(char *));
	if (!env)
		return (NULL);
	i = 0;
	tmp = envir;
	while (tmp)
	{
		env[i] = malloc(ft_strlen(tmp->name)+ft_strlen(tmp->value) + 1);
		if(!env[i])
			return (NULL);
		tmp = tmp->next;
		i++;
	}
	i = 0;
	while (envir)
    {
		env[i] = ft_strdup(envir->name);
        env[i] = ft_strjoin(env[i], "=");
        env[i] = ft_strjoin(env[i], envir->value);
        envir = envir->next;
		i++;
    }
	env[i] = NULL;
	return (env);
}

void ft_exec(t_cmd *tmp, char **env)
{
	char **cmdd;
	char	*str;
	int		i;

	i = 0;
	i = ft_strrch(tmp->cmd , '/');
	str = ft_substrr(tmp->cmd, i + 1);
    str = ft_strjoin(str, " ");
    str = ft_strjoin(str, tmp->flags);
    str = ft_strjoin(str, " ");
    str = ft_strjoin(str, tmp->files);
	cmdd = ft_split(str, ' ');
	if (execve(tmp->cmd, cmdd, env) == -1)
	{
		printf("\x1b[31m");
		printf("minishell: %s: command not found\n", *cmdd);
		printf("\033[0m");
		exit(127);
	}
}


void	new_pipe(t_cmd *data)
{
	int	i;
    int ac;

    ac = 0;
	t_cmd *tmp3 = data; 
    while (tmp3->next)
    {
        ac++;
		tmp3 = tmp3->next;
    }
	data->fd = malloc((ac) * sizeof(int *));
	if (!data->fd)
		return ;
	i = 0;
	while (i < ac)
	{
		data->fd[i] = malloc(2 * sizeof(int));
		i++;
	}
	i = 0;
	while (i < ac)
	{
		if (pipe(data->fd[i]) == -1)
		{
			perror("pipe");
			exit(1);
		}
		i++;
	}
	int **f = data->fd;
	tmp3 = data;
	while (tmp3)
    {
        tmp3->fd = f; 
		tmp3 = tmp3->next;
    }
}

void	ft_close(t_cmd *data, int	i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		close (data->fd[j][1]);
		close (data->fd[j][0]);
		j++;
	}
} 