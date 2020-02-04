TARGET = pa3
GCC = gcc
CFLAGS = -g -std=c99 -Wall -Wshadow -Wvla -pedantic -lm
#CFLAGS2 = -g -std=c99 -Wall -Wshadow -Wvla -pedantic -lm


CC = $(GCC) $(CFLAGS)
#CC2 = $(GCC) $(CFLAGS2)

SRCS = mcm_main.c mcm.c 

OBJS = $(SRCS:%.c=%.o)
VALGRIND = valgrind --tool=memcheck --leak-check=full --show-reachable=yes --track-origins=yes

.c.o:
	$(CC) $(CFLAGS) -c $*.c

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

t: $(TARGET)
	./pa3 7_8.gr out_file1 out_file2
	diff 7_8.mcm 7_8.mcm

t2: $(TARGET)
	./pa3 10_40.gr out_file1 out_file2

	diff 10_40.mcm 10_40.mcm

t3: $(TARGET)
	./pa3 neg7_8.gr out_file1 out_file2
	diff neg7_8.mcm neg7_8.mcm

t1000: $(TARGET)
	./pa3 1000_8000.gr out_file1 out_file2

t2k: $(TARGET)
	./pa3 2000_16000.gr out_file1 out_file2
	
t5k: $(TARGET)
	./pa3 5000_40000.gr out_file1 out_file2


mem: $(TARGET)
	$(VALGRIND) ./pa3 7_8.gr out_file1 out_file2

mem2: $(TARGET)
	$(VALGRIND) ./pa3 ex/10_40.gr out/10_40.mcm out/10_40.cycle

mem3: $(TARGET)
	$(VALGRIND) ./pa3 ex/neg7_8.gr out/neg7_8.mcm out/neg7_8.cycle

mem4: $(TARGET)
	$(VALGRIND) ./pa3 ex/1000_8000.gr out/1000_8000.mcm out/1000_8000.cycle

mem5: $(TARGET)
	$(VALGRIND) ./pa3 ex/5000_40000.gr out/5k.mcm out/5k.cycle

clean:
	rm -f $(OBJS) $(TARGET) out/*
	clear
zip: 
	zip pa3.zip *.c *.h
