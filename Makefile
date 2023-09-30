all:
	g++ main.cc -std=c++20 -Iinclude/ -ggdb3 -fsanitize=address 

gcc:
	g++ main.cc -std=c++20 -Iinclude/ -Wall -Wextra -pedantic

clang:
	g++ main.cc -std=c++20 -Iinclude/ -Weverything \
		-Wno-c++98-compat \
		-Wno-c++20-compat \
		-Wno-poison-system-directories \
		-Wno-padded
