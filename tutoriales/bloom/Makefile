local: compile tests
	rm bloom

compile:
	gcc -g -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror -o bloom *.c

tests:
	./bloom

unoptimized_compile:
	gcc -O0 -std=c99 -Wall -Wconversion -Wno-sign-conversion -Werror -o bloom_bench *.c

measure_time:
	/usr/bin/time -f "Measurements: time=%e maxmem=%M" ./bloom_bench 2>&1 1>/dev/null

clean_time:
	rm bloom_bench

time: unoptimized_compile measure_time clean_time
