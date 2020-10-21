# Use bash shell with pipefail option enabled so that the return status of a
# piped command is the value of the last (rightmost) commnand to exit with a
# non-zero status. This lets us pipe output into tee but still exit on test
# failures.
SHELL = /bin/bash
.SHELLFLAGS = -o pipefail -c

TEST_ARTIFACTS = pytest.stdout report.xml report.json cucumber.json pytest-logs.txt

all: test 

# Note $$ to let bash resolve the variable instead of make.
#
# Removed pytest coverage tests because they can't possibly work (we're testing a compiled C++ binary!)
test: install
	mkdir -p build
	PYTHONPATH=/app:/app/testing:/app/testing/post-deployment pytest $(if $(findstring all,$(MARK)),, -m $(MARK)) --disable-pytest-warnings | tee pytest.stdout; \
    status=$$?; \
    $(foreach artfct,$(TEST_ARTIFACTS),mv -f $(artfct) build/;) \
    exit $$status

.PHONY: all test

install:
	pip3 install -r test_requirements.txt
