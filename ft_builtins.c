/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_builtins.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ysaber <ysaber@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:21:16 by lkazaz            #+#    #+#             */
/*   Updated: 2024/03/07 23:16:41 by ysaber           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void ft_echo(t_cmd *tmp)
{
    if (tmp && tmp->args)
    {
        for (int i = 0; tmp->args[i]; i++)
        {
            printf("%s ", tmp->args[i]);
        }
    }
    
    printf("\n");
}


void ft_cd(t_cmd *tmp)
{
        if (!tmp->files)
            tmp->files = ft_strdup("/Users/ysaber");
        if (chdir(tmp->files) == -1)
            perror("cd"); 
}

void    ft_pwd(void)
{
    char s[256];

    getcwd(s, 256);
    printf("%s\n", s);
}

void    ft_affichenv(t_env *envir)
{
    char **env;

    env = ft_env(envir);
    while (env && *env)
    {
        printf("%s\n", *env);
        env++;
    }
}

void ft_unset(t_cmd *cmd, t_env *envir)
{
    t_env *p;
    t_env *tmp;
    (void)cmd;

    p = envir;
    while (p)
    {
        if (!ft_strcmp("PWD", p->name))
        {
            tmp = p->next;
			free(p->name);
	        free(p->value);
            p = tmp;
            break;
        }
        else
            p = p->next;
    }
}

t_env	*ft_lstlast(t_env *lst)
{
	if (lst == NULL)
		return (NULL);
	if (lst->next == NULL)
		return (lst);
	return (ft_lstlast(lst->next));
}

void	ft_lstadd_back(t_env **lst, t_env *new)
{
	t_env	*p;
    t_env   *tmp;
    
    p = *lst;
    while( p->next->next)
        p =p->next;
     tmp = p->next;
     p->next = new;
     p->next->next = tmp;
}

int ft_strnchr(const char *str, int a)
{
	char			*c;
	unsigned char	l;
    int             i;

	c = (char *)str;
    i = 0;
	l = (unsigned char )a;
	while (*c)
	{
		if (*c == l)
			return (i);
		c++;
        i++;
	}
	if (l != 0)
		return (0);
	return (0);
}

void    ft_export(t_cmd *cmd, t_env **envir)
{
    int len;
    t_env *last;
    t_env *new;
    //t_env *tmp;
    int i;

    i = 0;
    last = malloc(sizeof(t_env));
    if (!last)
        return ;
    //tmp = *envir;
    last = ft_lstlast(*envir);
    while (cmd->args[i])
    {
        if (ft_strnchr(cmd->args[i], '=')>0)
        {
            new=malloc(sizeof(t_env));
            if (new == NULL)
	        	return ;
            len = 0;
            while (cmd->args[i][len])
                 len++;
            new->name = ft_substr(cmd->args[i], 0, ft_strnchr(cmd->args[i], '='));
            new->value = ft_substr(cmd->args[i], ft_strnchr(cmd->args[i], '=') + 1, len);
	        new -> next = NULL;
            ft_lstadd_back(envir, new);
        }
        i++;
    }
    printf("lst: %s\n", last->name);
}

int ft_builtins(t_cmd *tmp, t_env *envir)
{
    char *command;

    if (tmp && tmp->cmd) // Check if tmp and tmp->cmd are not NULL
    {
        command = ft_last_world(tmp->cmd);
        if (command) // Check if command is not NULL
        {
            if (ft_strcmp(command, "echo") == 0)
            {
                ft_echo(tmp);
                return (0);
            }
            else if (ft_strcmp(command, "cd") == 0)
            {
                ft_cd(tmp);
                return (0);
            }
            else if (ft_strcmp(command, "pwd") == 0)
            {
                ft_pwd();
                return (0);
            }
            else if ((ft_strcmp(command, "env") == 0))
            {
                ft_affichenv(envir);
                return (0);
            }
            else if ((ft_strcmp(command, "unset") == 0))
            {
                ft_unset(tmp, envir);
                return (0);
            }
            else if (ft_strcmp(command, "export")==0)
            {
                ft_export(tmp, &envir);
                return (0);   
            }
        }
        else
            return (1);
    }
        return (-1);
}
