FLAGS = -Wall -Wextra -O2

linear:
	gcc linear.c -o bin/linear $(FLAGS)

bin_tree:
	gcc bin_tree.c -o bin/bin_tree $(FLAGS)


