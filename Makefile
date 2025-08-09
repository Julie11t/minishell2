NAME        = minishell
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -g
RM          = rm -rf

LIBFT_DIR   = libft
LIBFT       = $(LIBFT_DIR)/libft.a
LIBFT_INC   = $(LIBFT_DIR)

READLINE_INC = -I/usr/include/readline
READLINE_LIB = -lreadline

SRC         = main.c tokenize.c expand.c split_token.c execute.c commands.c commands2.c export.c signals.c parsing.c redirection.c
OBJS        = $(SRC:.c=.o)

CFLAGS      += -I$(LIBFT_INC) $(READLINE_INC)
LDFLAGS     = $(LIBFT) $(READLINE_LIB)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(OBJS) $(LDFLAGS) -o $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re

