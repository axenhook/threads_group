CC = gcc
CXX = g++
AR = ar
LD = ld

CFLAGS = -g -Wall -I./ -I../ -std=gnu99

POBJS = 

TEST_LOG = test_log

TEST_LOG_SRC = test_log.c ../log.c threads_group.c test_module_a.c test_module_b.c

TARGET_ALL = $(TEST_LOG)

all: $(TARGET_ALL)

$(TEST_LOG):
	gcc -o $@ $(TEST_LOG_SRC) $(CFLAGS) -lpthread
	
clean:
	rm -f *.o ../*.o $(TARGET_ALL)
