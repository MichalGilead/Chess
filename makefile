CC = gcc
OBJS = SPChessGUIManager.o SPChessMainWin.o SPChessGameWin.o SPChessLoadWin.o SPChessGameModeWin.o SPChessDifficultyLevelWin.o SPChessUserColorWin.o SPMiniMaxNode.o SPMiniMax.o SPChessInit.o SPChessGame.o SPArrayList.o SPChessMove.o SPChessParser.o SPMainAux.o main.o
EXEC = chessprog
COMP_FLAG = -std=c99 -Wall -Wextra \
-Werror -pedantic-errors
SDL_COMP_FLAG = -I/usr/local/lib/sdl_2.0.5/include/SDL2 -D_REENTRANT
SDL_LIB = -L/usr/local/lib/sdl_2.0.5/lib -Wl,-rpath,/usr/local/lib/sdl_2.0.5/lib -Wl,--enable-new-dtags -lSDL2 -lSDL2main

all: $(EXEC)
$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(SDL_LIB) -o $@
SPArrayList.o: SPArrayList.h SPArrayList.c SPChessMove.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessGame.o: SPChessGame.c SPChessGame.h SPArrayList.h SPChessInit.h SPChessMove.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessInit.o: SPChessInit.c SPChessInit.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessMove.o: SPChessMove.c SPChessMove.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessParser.o: SPChessParser.c SPChessParser.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPMiniMaxNode.o: SPMiniMaxNode.c SPMiniMaxNode.h SPChessGame.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPMiniMax.o: SPMiniMax.c SPMiniMax.h SPMiniMaxNode.h SPChessGame.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPMainAux.o: SPMainAux.c SPMainAux.h SPChessGame.h SPMiniMax.h SPChessParser.h SPChessInit.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessMainWin.o: SPChessMainWin.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessGameWin.o: SPMainAux.h SPChessGame.h SPChessGameWin.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessLoadWin.o: SPChessLoadWin.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessGameModeWin.o: SPChessGameModeWin.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessDifficultyLevelWin.o: SPChessDifficultyLevelWin.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessUserColorWin.o: SPChessUserColorWin.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
SPChessGUIManager.o: SPMainAux.h SPChessGame.h SPChessMainWin.h SPChessGameWin.h SPChessLoadWin.h SPChessGameModeWin.h SPChessDifficultyLevelWin.h SPChessUserColorWin.h SPChessGUIManager.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c
main.o: main.c SPMainAux.h SPChessGame.h SPChessParser.h SPMiniMax.h SPChessGUIManager.h
	$(CC) $(COMP_FLAG) $(SDL_COMP_FLAG) -c $*.c

clean:
	rm -f *.o $(EXEC) $(UNIT_TESTS)
