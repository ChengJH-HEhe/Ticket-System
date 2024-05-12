rm disk Recycle root
g++ main.cpp -o test.out -Wall -Wextra -std=c++20 -fsanitize=address -fsanitize=undefined && time -p ./test.out  > output.txt