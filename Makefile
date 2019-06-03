ECHO  = /bin/echo
EXEC  = lrg
DIRS  = bin


all: directory

directory:
	@for dir in $(DIRS); \
	do \
		printf "Checking $$dir ..."; \
		if [ ! -d $$dir ]; then \
			mkdir $$dir; \
			echo "Creating directory \"$$dir\" ..."; \
		else \
			echo "Directory exists."; \
		fi; \
	done
	@cd src; make -j4 -f Makefile.src --no-print-directory EXEC=$(EXEC);
	@ln -fs bin/$(EXEC)

clean:
	@cd src; make -f Makefile.src --no-print-directory clean;
	@rm -f $(EXEC)
