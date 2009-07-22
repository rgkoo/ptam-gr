CC=gcc
CPPFLAGS = -DDATA_DIR= -DVERSION=\"0.0.1\"
CFLAGS=-O2 -Wall `pkg-config --cflags x11 xext`
LD=gcc
LDFLAGS=

TARGET=shadow

OBJS=shadow.o camera.o extension.o image.o md2.o shaders.o vector.o

LIBS=`pkg-config --libs x11 xext` -lGL -lGLU -lm

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) $(LIBS) -o $(TARGET)

clean:
	rm -f *.o $(TARGET)
	
.c.o:
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $<

shadow.o: shadow.c camera.h common.h extension.h image.h \
		md2.h shaders.h vector.h Makefile
camera.o: camera.c Makefile
extension.o: extension.c Makefile
image.o: image.c Makefile
md2.o: md2.c Makefile
shaders.o: shaders.c Makefile
vector.o: vector.c Makefile
