# Percorre todos os módulos (cada um com seu próprio Makefile).
#
#   make / make run   compila e executa a demonstração de cada módulo
#   make clean         remove os binários
#   make list          lista os módulos

MODULES := $(sort $(dir $(wildcard */*/Makefile)))

.PHONY: all run clean list

all: run

run:
	@status=0; \
	for m in $(MODULES); do \
		printf '\n== %s\n' "$$m"; \
		$(MAKE) --no-print-directory -C "$$m" run || status=1; \
	done; \
	exit $$status

clean:
	@for m in $(MODULES); do $(MAKE) --no-print-directory -C "$$m" clean; done

list:
	@for m in $(MODULES); do echo "$$m"; done
