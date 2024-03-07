#include "minishell.h"

char *ft_remove_duplicates_quotes(char *str)
{
    int i = 0;
    int j = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '\"' && str[i + 1] == '\"')
        {
            i += 2;
        }
        else
        {
            str[j] = str[i];
            i++;
            j++;
        }
    }
    str[j] = '\0';
    return str;
}

char *ft_set_and_save_advanced(char *new_inputl, t_quote **head) {
    int i = 0;
    int start = -1;
    int end = -1;

	new_inputl = ft_remove_duplicates_quotes(new_inputl);

    while (new_inputl[i] != '\0')
    {
        if (new_inputl[i] == '\"')
        {
            if (start == -1)
            {
                start = i;
            }
            else
            {
                end = i;
                if (end - start > 1) { // Check if the quotes are not consecutive
                    char *substring = strndup(new_inputl + start + 1, end - start - 1);
                    if (strlen(substring) > 0) { // Check if the substring is not empty
                        append_node(head, create_node(start, 1, substring));
                        for (int j = start + 1; j < end; j++) {
                            new_inputl[j] = 'x';
                        }
                    }
                    free(substring);
                }
                else if (end - start == 1) { // Check if the quotes are consecutive
                    for (int j = start; j <= end; j++) {
                        new_inputl[j] = ' ';
                    }
                }
                start = -1;
                end = -1;
            }
        }
        i++;
    }
    return new_inputl;
}


char **ft_restore_quotes(char **args, t_quote *quote)
{
    int i = 0;
    t_quote *tmp = quote;
    while (args[i])
    {
        if (args[i] != NULL && args[i][0] == '\"' && tmp != NULL)
        {
            char* new_arg = ft_strdup(tmp->string);
            free(args[i]); // Free the old string
            args[i] = new_arg; // Assign the new string
            tmp = tmp->next;
        }
        i++;
    }
    return args;
}


char	**ft_remove_redirection(char **args, char **new_args)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (args[i])
	{
		if ((args[i][0] == '<' && args[i][1] == '<') || (args[i][0] == '>'
				&& args[i][1] == '>'))
			i++;
		else if (args[i] == NULL)
			new_args[j] = NULL;
		else if (args[i][0] == '<' || args[i][0] == '>')
			i++;
		else
		{
			new_args[j] = args[i];
			i++;
			j++;
		}
	}
	new_args[j] = NULL;
	return (new_args);
}


t_cmd	*ft_set_arguments(char **args, t_cmd *cmd)
{
    int	i;
    int	j;

    cmd->cmd = args[0];
    cmd->flags = NULL;
    cmd->files = NULL;
    cmd->args = (char **)ft_malloc(sizeof(char *) * (ft_args_len(args) + 1)); // Allocate memory for args
    i = 1;
    j = 0;
    while (args[i])
    {
        if (args[i][0] == '-')
        {
            if (cmd->flags == NULL)
                cmd->flags = args[i];
            else
                cmd->flags = ft_strjoin2(cmd->flags, " ", args[i]);
        }
        else if (access(args[i], F_OK) == 0)
        {
            if (cmd->files == NULL)
                cmd->files = args[i];
            else
                cmd->files = ft_strjoin2(cmd->files, " ", args[i]);
        }
        else if (strcmp(args[i], ">") == 0 || strcmp(args[i], ">>") == 0
            || strcmp(args[i], "<") == 0 || strcmp(args[i], "<<") == 0)
        {
            if (args[i + 1])
            {
                i += 2;
                continue;
            }
            else
            {
                printf("Error: No file specified for redirection\n");
                return NULL;
            }
        }
        else
        {
            cmd->args[j] = args[i];
            j++;
        }
        i++;
    }
    cmd->args[j] = NULL;
    return (cmd);
}

void ft_execute(t_cmd *cmd , t_env *envir)
{
{
	t_cmd   *tmp=cmd;
    char    **env;
    int     pip=0;
    int     pid;
    env = ft_env(envir);
    
    while (tmp->next)
    {
        pip++;
        tmp = tmp->next;
    }
    t_cmd *cmd1 = cmd;
    t_cmd *cmd2 = cmd;
    new_pipe(cmd1);
    
	int i = 0;
	while (cmd)
	{
        pid = fork();
        if (pid == -1)
		{
			perror("fork");
			exit(1);
		}
        if (pid == 0)
        {
            if (!ft_builtins(cmd, envir))
                return ;
            if (pip > 0)
            {
                if (i == 0)
                {
                    dup2(cmd->fd[0][1], 1);
                    ft_close(cmd, pip);
                    ft_exec(cmd, env);
                }
                if (i != pip)
                {
                    dup2(cmd->fd[i - 1][0], 0);
                    dup2(cmd->fd[i][1], 1);
                    ft_close(cmd, pip);
                    ft_exec(cmd, env);
                }
                if (i == pip)
                {
                    dup2(cmd->fd[i -1][0], 0);
                    ft_close(cmd, pip);
                    ft_exec(cmd, env);
                }
                
            }
            else
            {
                 if (!ft_builtins(cmd, envir))
                    return ;
                ft_exec(cmd, env);
            }
        }
		cmd = cmd->next;
		i++;	
	}
    ft_close(cmd2, pip);
    waitpid(pid, NULL, 0);
}
}

int ft_check_redirect(char *str)
{
    if (ft_strcmp(str, ">") == 0 || ft_strcmp(str, ">>") == 0
        || ft_strcmp(str, "<") == 0 || ft_strcmp(str, "<<") == 0)
        return 1;
    return 0;
}


void ft_minishell(t_node *node, t_env *envir, char **env)
{
    t_cmd			*cmd;
    t_cmd			*current_cmd;
    t_cmd			*cmd_path;
    char			**args;
    (void)			env;
    char 			**quote_args;

    cmd = ft_malloc(sizeof(t_cmd));
    if (node->args == NULL)
        return;
    current_cmd = cmd;
    t_redirect *first_redirect = NULL;
    t_redirect *current_redirect = NULL;
    for (t_node *tmp1 = node; tmp1; tmp1 = tmp1->next)
    {
        args = ft_split(tmp1->args, ' ');
        if (tmp1->quote)
            quote_args = ft_restore_quotes(args, tmp1->quote);
        else
            quote_args = args;
        t_redirect *new_redirect = ft_hamza(quote_args); // ft set redirection

        if (new_redirect != NULL)
        {
            if (first_redirect == NULL)
            {
                first_redirect = new_redirect;
                current_redirect = new_redirect;
            }
            else
            {
                current_redirect->next = new_redirect;
                current_redirect = new_redirect;
            }
        }
        current_cmd = ft_set_arguments(quote_args, current_cmd);
        if (current_cmd != NULL) {
            current_cmd->redirect = first_redirect;
        } else {
        printf("Error: current_cmd is NULL\n");
            return;
        }
        first_redirect = NULL;
        if (current_cmd == NULL)
        {
            printf("Error: Failed to set arguments\n");
            return;
        }
        if (tmp1->next)
        {
            current_cmd->next = ft_malloc(sizeof(t_cmd));
            current_cmd = current_cmd->next;
        }
        else
            current_cmd->next = NULL;
    }
    cmd_path = ft_get_path(cmd, envir);
    ft_execute(cmd_path, envir);
}



void ft_parse_input(char *input, t_env *envir, char **env)
{
    if (input == NULL || ft_strlen(input) == 0)
        return ;
    (void)env;
    t_data *data = ft_set_in_pipe(input);
    char *str = ft_set_spaces(data->str);
    char **pipe = ft_split(str, '|');
    for (int i = 0; pipe[i]; i++)
    {
        for (int j = 0; pipe[i][j]; j++)
        {
            if (pipe[i][j] == SPECIAL_CHAR)
                pipe[i][j] = '|';
        }
    }
    t_node *node = ft_malloc(sizeof(t_node));
    t_node *tmp = node;
    for (int i = 0; pipe[i]; i++)
    {
        t_quote *head = NULL; // Create a new t_quote linked list for each node
        pipe[i] = ft_set_and_save_advanced(pipe[i], &head);
        tmp->quote = head; // Save the quotes in the node
        head = NULL; // Reset the head
        tmp->args = ft_strdup(pipe[i]);
        if (pipe[i + 1])
        {
            tmp->next = ft_malloc(sizeof(t_node));
            tmp = tmp->next;
        }
        else
            tmp->next = NULL;
    }
    ft_minishell(node, envir, env);
}


int	main(int ac, char **av, char **env)
{
	char	*input;
	t_env	*envir;

	(void)ac;
	(void)av;
	envir = ft_get_env(env);
	
	while (1)
	{
		input = readline("\033[0;34mminishell\n|___>$\033[0m");
		if (!input)
			printf("\n");
		else
			ft_parse_input(input, envir, env);
		add_history(input);
		if (ft_strncmp(input, "exit", 4) == 0)
		{
			free(input);
			break ;
		}
        free(input);
	}
    ft_free_all();
}

