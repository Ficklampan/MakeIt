NAME = makeit
LOCALEDIR = /usr/share/locale
MO_FILES = sv

$(NAME): $(MO_FILES)
	@echo "Making binaries of the .po files"

sv: sv.po
	@msgfmt -o $@.mo $< && cp -f $@.mo $(LOCALEDIR)/$@/LC_MESSAGES/$(NAME).mo

.PHONY: clean

clean:
	rm -f *.mo
