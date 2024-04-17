# Variables
CC = gcc
SRC_DIR = ./src/
OBJ_DIR = ./obj/
CFLAGS = -Wall -Wextra -Werror -Ilibft/include -Iinclude -Llibft
SRC_FILES = main \
			syntax_checking/check_syntax syntax_checking/syntax_utils \
			signals/signal_handling \
			builtins/ft_pwd builtins/ft_cd builtins/ft_echo builtins/ft_env builtins/ft_export\
			builtins/ft_exit \
			init/env_init \
			utils/ft_free_args
SRC = $(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FILES)))
OBJ = $(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))
NAME = minishell
MAKE := make
VAL_HIDE	:= readline.hide
VAL_FLAGS	:= --leak-check=full --show-leak-kinds=all -s --suppressions=$(VAL_HIDE)


GREEN=\033[0;32m
YELLOW=\033[0;33m
BLUE=\033[0;34m
MAGENTA=\033[0;35m
NC=\033[0m

# Phony targets
.PHONY: all clean fclean re val

# Rules
all: $(NAME)

val: $(NAME)
	valgrind $(VAL_FLAGS) ./$(NAME)

$(NAME): $(OBJ)
	@cd ./libft && $(MAKE) > /dev/null && $(MAKE) bonus > /dev/null && $(MAKE) printf > /dev/null
	@echo "$(GREEN)Built Libft ✅ $(NC)"
	@$(CC) $(CFLAGS) -o $@ $^ -lreadline -lft
	@echo "$(BLUE)Compiled $(NAME) 💻 $(NC)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) -rf $(OBJ_DIR)
	@echo "$(MAGENTA)Cleaned object files ✅ $(NC)"

fclean: clean
	@cd ./libft && $(MAKE) fclean > /dev/null
	@echo "$(MAGENTA)Cleaned libft ❎ $(NC)"
	@$(RM) -f $(NAME)
	@echo "$(MAGENTA)Cleaned $(NAME) ❎ $(NC)"

re: fclean all