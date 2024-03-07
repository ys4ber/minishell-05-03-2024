#pragma once

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define SPECIAL_CHAR '#'

typedef struct s_quote
{
	int					start;
	int					end;
	int					index;
	int					type;
	char				*string;
	struct s_quote		*next;
}						t_quote;

typedef struct s_node
{
	char				*args;
	t_quote				*quote;
	struct s_node		*next;
}						t_node;

typedef struct s_redirect
{
	char				*file;
	int					type;
	struct s_redirect	*next;
}						t_redirect;

/*
1 s_redirect output >
2 s_redirect input <
3 s_redirect output append >>
4 s_redirect here document <<
*/

typedef struct s_pipe
{
	char				*cmd;
	int					index;
	char				*string;
	struct s_pipe		*next;
}						t_pipe;

typedef struct s_cmd
{
	char				**args;
	char				*cmd;
	int 				**fd;
	int					num_pip;
	char				*flags;
	char				*files;
	t_redirect			*redirect;
	struct s_cmd		*next;
}						t_cmd;

typedef struct s_mini
{
	t_cmd				*cmd;
	t_cmd				*current_cmd;
	t_cmd				*cmd_path;
	t_redirect			*redirect;
	t_node				*tmp;
	char				**args;
	char				**new_args;
}						t_mini;

typedef struct s_env
{
	char				*name;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_list
{
	void				*ptr;
	struct s_list		*next;
}						t_list;

typedef struct s_data
{
	char				*str;
	int					*pipe_indices;
	int					count;
}						t_data;



// ft_split.c
char					**ft_split(char const *s, char c);

// ft_utils.c
int						ft_strncmp(const char *s1, const char *s2, size_t n);
char					*ft_strchr(const char *s, int c);
int						ft_strlen(char *str);
char					*ft_strdup(char *str);
int						ft_strcmp(char *s1, char *s2);
char					*ft_substr(char *s, int start, int len);
char					*ft_strjoin2(char *s1, char *s2, char *s3);
char					*ft_last_world(char *str);
char					*ft_strjoin(char *s1, char *s2);
int						ft_args_len(char **args);
char					*ft_strnstr(const char *haystack, const char *needle,
							size_t len);
int						ft_strlen_2d(char **str);
void					*ft_malloc(size_t size);
void					ft_free_all(void);

// ft_store.c
t_env					*ft_get_env(char **env);
t_cmd					*ft_get_path(t_cmd *cmd, t_env *envir);

// ft_functions.c
char					*ft_set_spaces(char *input);
t_redirect				*ft_hamza(char **args);
t_data					*ft_set_in_pipe(char *input);
t_quote					*create_node(int index, int type, char *string);
void					append_node(t_quote **head, t_quote *node);

//exic
void ft_exec(t_cmd *tmp, char **env);
void	new_pipe(t_cmd *data);
void	ft_close(t_cmd *data, int	i);
char 	**ft_env(t_env *envir);
char	*ft_strstr(char *str, char *to_find);
void 	ft_echo(t_cmd *tmp);
void 	ft_cd(t_cmd *tmp);
int 	ft_builtins(t_cmd *tmp, t_env *envir);