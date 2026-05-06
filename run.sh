#!/bin/bash

g++ main.cpp arithmetic.cpp read.cpp builtins.cpp if_while.cpp test.cpp -o minishell
g++ launch.cpp -o launch

./launch
