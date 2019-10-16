CXX       := g++
CPPFLAG   := -g -O2 -Wall -Iinclude -std=c++11
CPPLDFLAG := -lavcodec -lavformat -lswresample -lavutil 

TARGAET   := mp3emulate
OBJ       := AudioPlayer.o main.o 

#USE_SDL2  = 1
USE_ALSA  = 1

ifdef USE_ALSA
CPPLDFLAG += -lasound
CPPFLAG   += -D USE_ALSA
OBJ       += ALSADriver.o AudioRecord.o
endif

ifdef USE_SDL2
CPPLDFLAG += -lSDL2
CPPFLAG   += -D USE_SDL2 
OBJ       += SDL2Driver.o
endif

all:$(OBJ)
	$(CXX) -o $(TARGAET) $^ $(CPPLDFLAG)

clean:
	rm $(OBJ) $(TARGAET)

%.o:%.cpp
	$(CXX) $(CPPFLAG) -c -o $@ $<
