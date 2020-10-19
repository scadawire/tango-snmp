# Use bash shell with pipefail option enabled so that the return status of a
# piped command is the value of the last (rightmost) commnand to exit with a
# non-zero status. This lets us pipe output into tee but still exit on test
# failures.
SHELL = /bin/bash
.SHELLFLAGS = -o pipefail -c

TEST_ARTIFACTS = snmpsimd.txt pytest.stdout 
#report.xml report.json cucumber.json pytest-logs.txt coverage.xml htmlcov

all: test 

# wait for the device to be available before beginning the test
# A temporary volume is mounted at /build when 'make test' is executing.
# These tests run from that location.
#
# Note $$ to let bash resolve the variable instead of make.
#
# Removed pytest coverage tests because they can't possibly work (we're testing a compiled C++ binary!)
test: install
	#snmpsimd.py --data-dir=. --agent-udpv4-endpoint=127.0.0.1:1024 | tee snmpsimd.txt &
	retry --max=10 -- tango_admin --ping-device ab/ab/simulator
	mkdir -p build && \
    find . -name "*.pyc" -type f -delete && \
    PYTHONPATH=/app:/app/testing:/app/testing/post-deployment pytest $(if $(findstring all,$(MARK)),, -m $(MARK)) --disable-pytest-warnings > pytest.stdout; \
    status=$$?; \
    $(foreach artfct,$(TEST_ARTIFACTS),mv -f $(artfct) build/;) \
    exit $$status
	# old Makefile test script below
	#for t in *.py; do \
	#	script --flush --quiet --return "$$t".stdout --command "pytest \"$$t\" "; \
	#done

.PHONY: all test

install:
	pip3 install -r test_requirements.txt



# old docker-compose simulator config:
#     command: >
#        sh -c "wait-for-it.sh ${TANGO_HOST} --timeout=30 --strict --
#        retry --max=5 &&
#        if [ \"${NETWORK_MODE}\" = \"host\" ]; then LISTEN=127.0.0.1; else LISTEN=\"${CONTAINER_NAME_PREFIX}tester\"; fi &&
#        echo SNMP simulator will listen at $${LISTEN} &&
#        ( (python -c 'import tango' && echo python has tango) || echo python no tango ) &&
#        snmpsimd.py --data-dir=. --agent-udpv4-endpoint=$${LISTEN}\:1024"
#
# notes for tester command:
#  double $ to prevent docker from trying to evaulate LISTEN
#  backslash-colon to prevent docker/yaml confusion (very cryptic errors)
#
