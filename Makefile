all:

CFLAGS += -Wall -Wextra -pipe -O2 -g

QBIN2HEX := qbin2hex
QASM2 := qasm2
M4 := m4
RM := rm -f

all: main

main: main.o
main: LDLIBS += -lvc4vec
main.o: test.qhex

.PRECIOUS: %.qhex
%.qhex: %.qbin
	$(QBIN2HEX) <"$<" >"$@"

.PRECIOUS: %.qbin
%.qbin: %.qasm2
	$(QASM2) <"$<" >"$@"

.PRECIOUS: %.qasm2
%.qasm2: %.qasm2m4
	$(M4) <"$<" >"$@"

.PHONY: clean
clean:
	$(RM) main main.o
	$(RM) test.qhex test.qbin test.qasm2
