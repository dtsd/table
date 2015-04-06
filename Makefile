#SOURCES := main.cpp table.cpp field.cpp
SOURCES := main.cpp table.cpp page.cpp row.cpp
# Objs are all the sources, with .cpp replaced by .o
OBJS := $(SOURCES:.cpp=.o)

CC := g++ 
#CC := clang++
CFLAGS := -std=c++11
#LIBS := -lpthread
LIBS := 

all: table

# Compile the binary 't' by calling the compiler with cflags, lflags, and any libs (if defined) and the list of objects.
table: $(OBJS)
	$(CC) $(CFLAGS) -o table $(OBJS) $(LFLAGS) $(LIBS)

# Get a .o from a .cpp by calling compiler with cflags and includes (if defined)
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

