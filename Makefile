CC ?= gcc
CFLAGS_common ?= -O0 -Wall -std=gnu99

EXEC = phonebook_orig phonebook_opt
all: $(EXEC)

SRCS_common = main.c

phonebook_orig: $(SRCS_common) phonebook_orig.c phonebook_orig.h
	$(CC) $(CFLAGS_common) -DIMPL="\"$@.h\"" -o $@ \
		$(SRCS_common) $@.c

phonebook_opt: $(SRCS_common) phonebook_opt.c phonebook_opt.h
	$(CC) $(CFLAGS_common) -DOPT -DIMPL="\"$@.h\"" -o $@ \
		$(SRCS_common) $@.c

run: $(EXEC)
	watch -d -t "./phonebook_orig && echo 3 > /proc/sys/vm/drop_caches && ./phonebook_opt"

cache-test: phonebook_orig phonebook_opt
	perf stat --repeat 100 -e cache-misses,cache-references,instructions,cycles ./phonebook_orig
	perf stat --repeat 100 -e cache-misses,cache-references,instructions,cycles ./phonebook_opt
	
.PHONY: calculate clean plot

plot:
	gnuplot plot.gp
	eog run-time.png

calculate: calculate.c
	$(CC) $^ -o $@
	./$@
	rm -f $@

clean:
	$(RM) $(EXEC) *.o perf.* *.txt
