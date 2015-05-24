TARGET		= spider
OBJS		= terminal.o api.o spiderPosition.o spider.o main.o
SRCS		= $(OBJS:.o=.c)
INCLUDEDIRS	+= -I../DXL_SDK_LINUX_v1_01/include
LIBDIRS		+= -L../DXL_SDK_LINUX_v1_01/lib
CFLAGS		= -Wall -W $(INCLUDEDIRS) -O2 -g 
LDFLAGS		= $(LIBDIRS) -lm -ldxl -lpthread

CC			= gcc

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)
	
.c.o:
	$(CC) -c $< $(CFLAGS)
	
clean:
	rm -f $(OBJS) $(TARGET)
	@echo "file deleted."

dep:
	gccmakedep $(SRCS)

