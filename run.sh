#!/bin/bash

g++ main.cpp arithmetic.cpp read.cpp command.cpp if_while.cpp test.cpp subsidiary.cpp -o minishell
g++ launch.cpp subsidiary.cpp -o launch

./launch

rm minishell
rm launch
