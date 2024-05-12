#!/usr/bin/bash
# chk.sh 
# chmod +x ./chk.sh

echo "compiling"
g++ make.cpp -o mk.out -Wall  -O2
g++ main.cpp -o tst.out -Wall -Wextra -std=c++20 
g++ bok.cpp -o std.out -Wall -O2
echo "compiled"

for ((i=1;;i++))
do
    ./del
    
    ./mk.out > in.txt
    echo "running on test" $i
    ./tst.out < in.txt > out.txt
    ./std.out < in.txt > ans.txt
    diff ans.txt out.txt -Z
    if [ $? -ne 0 ] ; then
        echo "Wrong Answer"
        break;
    fi 
    ./mk.out > in1.txt
    echo "running on test" $i
    ./tst.out < in1.txt > out1.txt
    ./std.out < in1.txt > ans1.txt  
    diff ans1.txt out1.txt -Z
    if [ $? -ne 0 ] ; then
        echo "Wrong Answer"
        break;
    fi 
done