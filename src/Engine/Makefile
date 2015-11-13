CFLAGS := -c -std=c++11 $(ADDITIONAL)
SOURCES := Audio.cpp Cursor.cpp CustomFont.cpp Entity.cpp Font.cpp FrameSequence.cpp Game.cpp GameState.cpp \
	Keyboard.cpp Layer.cpp Mouse.cpp Render.cpp Resource.cpp ResourceManager.cpp Scene.cpp Sprite.cpp \
	StateManager.cpp Texture.cpp Timer.cpp Utils.cpp
OBJECTS := $(SOURCES:.cpp=.o)
LIBRARY := Engine.a

compile: $(SOURCES) $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	ar rcs $@ $(OBJECTS)

.cpp.o:
	g++ $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -f $(LIBRARY)
