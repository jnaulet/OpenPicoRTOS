# Helps uncrustifying and generating documentation

uncrustify:
	find . -name "*.[ch]" -exec uncrustify -c \
	  etc/uncrustify.cfg \
	  --replace --no-backup {} +

naturaldocs:
	naturaldocs -i . -xi docs \
	  -p etc/naturaldocs \
	  -o HTML docs

.PHONY: uncrustify naturaldocs
