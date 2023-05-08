# Helps uncrustifying and generating documentation

uncrustify:
	find . -name "*.[ch]" -exec uncrustify -c \
	  etc/uncrustify.cfg \
	  --replace --no-backup {} +

naturaldocs:
	naturaldocs -i . \
	  -xi docs -xi demo -xi staging \
	  -p etc/naturaldocs \
	  -o HTML docs

cloc:
	cloc picoRTOS.c
	cloc picoRTOS-SMP.c

check:
	cd test && ./run_tests.sh

DEMO := $(shell find demo -mindepth 1 -maxdepth 1)

distcheck:
	@for demo in $(DEMO); do \
	  echo $$demo " : "; \
	  $(MAKE) -C $$demo splint || exit 1; \
	  $(MAKE) -C $$demo cppcheck || exit 1; \
	done
	@echo ""
	@echo "Demo Test Complete"

.PHONY: uncrustify naturaldocs clock check distcheck
