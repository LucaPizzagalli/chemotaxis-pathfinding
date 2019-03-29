#comandi utili (da liux):
#make all #per compilare
#./giochetto #per eseguire
#valgrind --leak-check=full --show-leak-kinds=all --suppressions=sdl.supp ./giochetto #per fare un test sulla gestione della memoria

#OBJS specifies which files to compile as part of the project
OBJS = *.cpp

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -Wall -pedantic

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -pthread

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = giochetto

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
