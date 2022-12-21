NAME = dico

OBJS = main.o

HEADS = 

CC = c++

FLAGS = -Wall -Wextra -Werror -O3

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CC) $(FLAGS) $(OBJS) -o $(NAME)

$(OBJS):	%.o:%.cpp $(HEADS)
			$(CC) $(FLAGS) -c $< -o $@

clean:
			rm -f $(OBJS)

fclean:		clean
			rm -f $(NAME)

re:			fclean all

.PHONY:		all clean fclean re
