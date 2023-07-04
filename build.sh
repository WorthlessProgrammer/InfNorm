#!/bin/sh

set -xe

clang infnorm.c -Wall -Wextra -fsanitize=thread -O3 -lpthread -o main
