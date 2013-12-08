CFLAGS := -I ../Engine -std=c++11
LDFLAGS := ../Engine/Engine.a -lGL -lglut -lGLEW
SOURCES := Invaders.cpp Missiles.cpp RedInvader.cpp StateGameover.cpp StateMenu.cpp \
	Obstacle.cpp Spaceship.cpp StateHighScore.cpp StatePlaying.cpp main.cpp
OBJECTS := $(SOURCES:.cpp=.o)
GAME := SpaceInvaders

compile: $(SOURCES) $(GAME)

$(GAME): $(OBJECTS)
	g++ $(OBJECTS) $(LDFLAGS) -o $(GAME)

.cpp.o:
	g++ -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(GAME)
