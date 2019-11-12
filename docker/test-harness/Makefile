# Use bash shell with pipefail option enabled so that the return status of a
# piped command is the value of the last (rightmost) commnand to exit with a
# non-zero status. This lets us pipe output into tee but still exit on test
# failures.
SHELL = /bin/bash
.SHELLFLAGS = -o pipefail -c

all: test 

# wait for the device to be available before beginning the test
# A temporary volume is mounted at /build when 'make test' is executing.
# These tests run from that location.
#
# Note $$ to let bash resolve the variable instead of make.
#
test:
	retry --max=10 -- tango_admin --ping-device ab/ab/48v
	echo `pwd`
	ls
	for t in *.py; do \
		python "$$t" | tee "$$t".stdout ; \
	done
	ls *.stdout


.PHONY: all test
