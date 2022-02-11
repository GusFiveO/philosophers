SRC_DIR = src/
INC_DIR = include/
OBJ_DIR = bin/

SRCS = main.c utils.c process.c routine.c semaphores.c thread.c
OBJS = $(addprefix $(OBJ_DIR), $(SRCS:.c=.o))

NAME = philo

CC = gcc
CFLAGS = -Werror -Wextra -Wall -g #-fsanitize=thread#-fsanitize=address -fsanitize=thread
LINKS = -lpthread #-fsanitize=thread#-fsanitize=address -fsanitize=thread
INCPATH = -I$(INC_DIR)

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJS)
	$(CC) $(OBJS) $(LINKS) -o $(NAME)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	$(CC) $(CFLAGS) $(INCPATH) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
