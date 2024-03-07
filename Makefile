NAME = minishell

CC = gcc

CFLAGS = -Wall -Wextra -Werror 


SRCS = main.c ft_split.c ft_store.c ft_utils.c ft_functions.c ft_exc.c ft_builtins.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)


$(NAME) : $(OBJS)
	$(CC)  $(CFLAGS)  $(SRCS) -o $(NAME)  -lreadline  -fsanitize=address -g3

clean:
	rm -rf $(OBJS)
	
fclean:
	rm -rf $(OBJS) $(NAME)

re: fclean all clean

.PHONY: all clean fclean re