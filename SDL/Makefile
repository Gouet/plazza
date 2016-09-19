##
## Makefile for  in /home/veyrie_f/Tek2/cpp_plazza/SDL
##
## Made by Fernand Veyrier
## Login   <veyrie_f@epitech.net>
##
## Started on  Wed Apr 20 21:49:47 2016 Fernand Veyrier
## Last update Wed Apr 20 21:50:32 2016 Fernand Veyrier
##

CC		= clang++

RM		= rm -f

NAME		= sdl

SRCS		= Button.cpp \
  		  main.cpp \
  		  WindowManagerSDL.cpp \
  		  TextBox.cpp \
  		  ScrollBar.cpp \
  		  Panel.cpp \
  		  InfoBox.cpp \
  		  ../ExploreDirectory.cpp

OBJS		= $(SRCS:.cpp=.o)

CPPFLAGS	= -I./include -Wall -Wextra -W -std=c++11 -Wnon-virtual-dtor -I../

LIB		= -lSDL2 -lSDL2_ttf -lSDL2_image

$(NAME):	$(OBJS)

		$(CC) -o $(NAME) $(OBJS) $(LIB) 

all:		$(NAME)

clean:
		$(RM) $(OBJS)

fclean:		clean
		$(RM) $(NAME)

re:		fclean all

.PHONY:		all clean fclean re
