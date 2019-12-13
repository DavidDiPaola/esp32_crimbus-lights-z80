# 2019 David DiPaola, licensed CC0 (public domain worldwide)

SRC = test.s
BIN = test.bin

COFF = $(BIN:.bin=.coff)
PREFIX ?= z80-unknown-coff-

.phony: all
all: $(BIN)

.phony: send
send: $(BIN)
	cat $< | nc -w 10 10.11.12.129 8080

.phony: clean
clean:
	rm -f $(COFF)
	rm -f $(BIN)

.phony: dump
dump: $(COFF)
	$(PREFIX)objdump --disassemble-all $<

.phony: dump_bin
dump_bin: $(BIN)
	$(PREFIX)objdump --target=binary --architecture=z80 --disassemble-all $<

$(COFF): $(SRC)
	$(PREFIX)as -o $@ $<

$(BIN): $(COFF)
	$(PREFIX)objcopy -O binary $< $@

