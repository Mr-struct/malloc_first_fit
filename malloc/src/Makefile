CC = gcc
CFLAGS = -W -Wall -Werror -c
NAME = libmy_malloc_$(shell uname).so
RM = rm -f
FILE = libmy_malloc.so
LINK = ln -s $(NAME) libmy_malloc.so
SRC= malloc.c\
	 free.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	gcc $(CFLAGS) $(SRC)
	ar rc $(NAME) $(OBJ)
	ranlib $(NAME)
	$(LINK)



clean:
	$(RM) $(OBJ) $(FILE)

fclean: clean
	$(RM) $(NAME) $(FILE)

re: fclean all

.PHONY : all clean fclean re
