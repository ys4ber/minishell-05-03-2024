#include "minishell.h"




char *ft_set_spaces(char *input)
{
    int i;
    int j;
    char *new_input;

    i = 0;
    j = 0;
    new_input = ft_malloc(sizeof(char) * (ft_strlen(input) * 2 + 1));
    while (input[i])
    {
        if (ft_strchr("><|", input[i]))
        {
            new_input[j++] = ' ';
            new_input[j++] = input[i];
            if (input[i + 1] && ft_strchr("><|", input[i + 1]))
            {
                new_input[j++] = input[++i];
                new_input[j] = ' ';
            }
            else
                new_input[j] = ' ';
            j++;
        }
        else
            new_input[j++] = input[i];
        i++;
    }
    new_input[j] = '\0';
    return new_input;
}

t_redirect	*ft_hamza(char **args)
{
	t_redirect	*redirect;
	t_redirect	*current_redirect;
	t_redirect	*new_redirect;
	int i;

	redirect = NULL;
	current_redirect = NULL;
	i = 0;
	while(args[i])
	{
		if (ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0
			|| ft_strcmp(args[i], "<") == 0 || ft_strcmp(args[i], "<<") == 0)
		{
			new_redirect = ft_malloc(sizeof(t_redirect));
			if (args[i + 1] != NULL)
			{
				if (ft_strcmp(args[i], ">") == 0)
					new_redirect->type = 1;
				else if (ft_strcmp(args[i], ">>") == 0)
					new_redirect->type = 3;
				else if (ft_strcmp(args[i], "<") == 0)
					new_redirect->type = 2;
				else if (ft_strcmp(args[i], "<<") == 0)
					new_redirect->type = 4;
				new_redirect->file = args[++i];
				new_redirect->next = NULL;
				if (redirect == NULL)
				{
					redirect = new_redirect;
					current_redirect = new_redirect;
				}
				else
				{
					current_redirect->next = new_redirect;
					current_redirect = new_redirect;
				}
			}
		}
		i++;
	}
	return (redirect);
}

t_data* ft_set_in_pipe(char *input)
{
    int quote_flag = 0;
    int *pipe_indices = ft_malloc(sizeof(int) * strlen(input));
    int count = 0;

    char *str = ft_malloc(sizeof(char) * (strlen(input) + 1));
    strcpy(str, input);

    for (int i = 0; str[i]; i++)
    {
        if (str[i] == '\"')
        {
            quote_flag = !quote_flag;
        }
        else if (str[i] == '|' && quote_flag)
        {
            str[i] = SPECIAL_CHAR;
            pipe_indices[count++] = i;
        }
    }

   	t_data *data = ft_malloc(sizeof(t_data));
    data->str = str;
    data->pipe_indices = pipe_indices;
    data->count = count;

    return data;
}



t_quote *create_node(int index, int type, char *string)
{
    t_quote *node = (t_quote *)ft_malloc(sizeof(t_quote));
    node->index = index;
    node->type = type;
    node->string = strdup(string);
    node->next = NULL;
    return node;
}

void append_node(t_quote **head, t_quote *node)
{
    if (*head == NULL)
    {
        *head = node;
    }
    else
    {
        t_quote *current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = node;
    }
}