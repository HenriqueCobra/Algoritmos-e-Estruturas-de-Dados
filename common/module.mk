# Regras de build compartilhadas. Cada módulo tem um Makefile curto que define
# MODULE (o nome-base do par .c/.h) e inclui este arquivo:
#
#     MODULE := bubble_sort
#     include ../../common/module.mk
#
# Alvos:
#   make        compila e executa a demonstração (main.c)
#   make run    idem
#   make clean  remove o binário

CC     ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2

HEADER := $(wildcard $(MODULE).h)

.PHONY: all run clean
all: run

demo: $(MODULE).c $(HEADER) main.c
	$(CC) $(CFLAGS) -o demo $(MODULE).c main.c

run: demo
	./demo

clean:
	$(RM) demo demo.exe
