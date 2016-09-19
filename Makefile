##
## Makefile for  in /home/veyrie_f/Tek2/cpp_plazza
##
## Made by Fernand Veyrier
## Login   <veyrie_f@epitech.net>
##
## Started on  Fri Apr  8 19:42:47 2016 Fernand Veyrier
## Last update Sun Apr 24 13:06:19 2016 veyrie_f
##

CC		= g++

RM		= rm -f

NAME		= plazza

SRCS		= NamedPipe.cpp \
		  Mutex.cpp \
		  Regex.cpp \
		  PlazzaCore.cpp \
		  Process.cpp \
		  Order.cpp \
		  OrdersDispatcher.cpp \
		  OrderResolver.cpp \
		  FileDecryptor.cpp \
		  CaesarDecrypt.cpp \
		  CondVar.cpp \
		  ResolvedData.cpp \
		  Semaphore.cpp \
		  XORDecrypt.cpp \
		  Pipe.cpp

SHELLMODE   =   $(SRCS) \
		main.cpp

UIMODE      =   $(SRCS) \
		UIManager.cpp \
		MainWindow.cpp \
		MenuNCurses.cpp \
		ProcessInfo.cpp \
		NcursesDisplayProcess.cpp \
		NcursesLoading.cpp \
		NCurses.cpp \
		ExploreDirectory.cpp \
		mainUI.cpp \
		SDL/WindowManagerSDL.cpp \
		SDL/Button.cpp \
		SDL/InfoBox.cpp \
		SDL/Panel.cpp \
		SDL/ScrollBar.cpp \
		SDL/TextBox.cpp

OBJSSH		= $(SHELLMODE:.cpp=.o)

OBJSUI      	= $(UIMODE:.cpp=.o)

CPPFLAGS	= -I./include -I./ -Wall -Wextra -W -std=c++11

LDFLAGS		= -lpthread

UIFLAGS     =  -I./SDL -lncurses -lSDL2 -lSDL2_ttf -lSDL2_image

$(NAME):	$(OBJSSH)
	$(CC) -o $(NAME) $(OBJSSH) $(LDFLAGS)

ui:		$(OBJSUI)
	$(CC) -o $(NAME) $(OBJSUI) $(LDFLAGS) $(UIFLAGS)

all:		$(NAME)

clean:
	$(RM) $(OBJSSH)
	$(RM) $(OBJSUI)

fclean:	clean
	$(RM) $(NAME)

re:	fclean all

reui:	fclean ui

.PHONY:		all clean fclean re
