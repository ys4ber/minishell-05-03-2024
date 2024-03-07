/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_store.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaber <ysaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 17:47:26 by ysaber            #+#    #+#             */
/*   Updated: 2024/03/07 19:01:19 by ysaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_env	*ft_store_environment(char *env)
{
	t_env	*environment;
	int		i;

	i = 0;
	while (env[i] != '=')
		i++;
	environment = (t_env *)ft_malloc(sizeof(t_env));
	environment->name = ft_substr(env, 0, i);
	environment->value = ft_substr(env, i + 1, ft_strlen(env) - i);
	environment->next = NULL;
	return (environment);
}

t_env	*ft_get_env(char **env)
{
	t_env	*environment;
	t_env	*head;
	int		i;

	i = 0;
	while (env[i])
	{
		if (i == 0)
		{
			environment = ft_store_environment(env[i]);
			head = environment;
		}
		else
		{
			environment->next = ft_store_environment(env[i]);
			environment = environment->next;
		}
		i++;
	}
	return (head);
}

static char	*ft_get_env_value(char *name, t_env *envir)
{
	t_env	*tmp;

	tmp = envir;
	while (tmp)
	{
		if (ft_strncmp(tmp->name, name, ft_strlen(name)) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

static char	*find_cmd_path(char **path_split, char *cmd)
{
	int		i;
	char	*cmd_path;

	i = 0;
	while (path_split[i])
	{
		cmd_path = ft_strjoin2(path_split[i], "/", cmd);
		if (access(cmd_path, F_OK) == 0)
		{
			return (cmd_path);
		}
		i++;
	}
	return (NULL);
}

t_cmd	*ft_get_path(t_cmd *cmd, t_env *envir)
{
	t_cmd	*tmp;
	char	*path;
	char	**path_split;
	char	*cmd_path;

	tmp = cmd;
	path = ft_get_env_value("PATH", envir);
	path_split = ft_split(path, ':');
	while (tmp)
	{
		if (access(tmp->cmd, F_OK) != 0)
		{
			cmd_path = find_cmd_path(path_split, tmp->cmd);
			if (cmd_path)
				tmp->cmd = cmd_path;
		}
		tmp = tmp->next;
	}
	return (cmd);
}
