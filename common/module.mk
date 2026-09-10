# Regras de build compartilhadas. Cada módulo tem um Makefile curto que define
# MODULE (o nome-base do par .c/.h) e inclui este arquivo:
#
#     MODULE := bubble_sort
#     include ../../common/module.mk
#
# Alvos:
#   make        compila e executa a demonstração (main.c)
#   make run    idem
#   make clean  remove o diretório de build

CC     ?= cc
CFLAGS ?= -std=c11 -Wall -Wextra -Wpedantic -O2

ifeq ($(OS),Windows_NT)
    EXE := .exe
endif

HEADER  := $(wildcard $(MODULE).h)
BUILD   := _build
BIN     := $(BUILD)/$(MODULE)$(EXE)

.PHONY: all run clean
all: run

$(BIN): $(MODULE).c $(HEADER) main.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -o $@ $(MODULE).c main.c

run: $(BIN)
	./$(BIN)

clean:
	$(RM) -r $(BUILD)
