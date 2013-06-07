CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		glfw-thinger.o

LIBS =          -lGL -lGLU -lglfw

TARGET =	glfw-thinger

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
