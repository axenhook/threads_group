CC = gcc
CXX = g++
AR = ar
LD = ld

CFLAGS = -g -Wall -I./ -I../ -std=c99

POBJS = 

TEST_THREADS_GROUP = test_threads_group

TEST_THREADS_GROUP_SRC = test_threads_group.c ../threads_group.c

TARGET_ALL = $(TEST_THREADS_GROUP)

all: $(TARGET_ALL)

$(TEST_THREADS_GROUP):
	gcc -o $@ $(TEST_THREADS_GROUP_SRC) $(CFLAGS)
	
clean:
	rm -f *.o ../*.o $(TARGET_ALL)
