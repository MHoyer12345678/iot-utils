LDLIBS = -lusb
PREFIX = /usr


all: hub-ctrl

.PHONY: install
install: hub-ctrl
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install $< $(DESTDIR)$(PREFIX)/bin/$<
	install power-reset-usb-port.sh $(DESTDIR)$(PREFIX)/bin

.PHONY: clean
clean:
	rm -f hub-ctrl
