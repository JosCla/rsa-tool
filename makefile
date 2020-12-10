CXX = g++
CFLAGS = -g -Wall
LIBS = -lgmpxx -lgmp

CPPFILES = src/main.cpp src/rsafuncs.cpp src/rsaobj.cpp src/rsacont.cpp
OBJFILES = $(CPPFILES:.cpp=.o)

TARGET = rsa

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CXX) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LIBS)

%.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $^

clean:
	$(RM) $(OBJFILES)
	$(RM) $(TARGET)
