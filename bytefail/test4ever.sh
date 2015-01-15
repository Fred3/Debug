#!/bin/bash

# Adjust to 0 when this actually works.
EXPECTED_ERRORS=0

LOGFILE=$(mktemp)

i=0
while true; do
	echo -n .
	i=$[i+1]
	if [[ $[i % 40] -eq 0 ]] ; then echo ${i}; fi
#	./test.sh | tee ${LOGFILE} | grep -q "${EXPECTED_ERRORS} x FAILED" || break
	./test.sh | tee ${LOGFILE} | grep -q "PASSED" || break
done

echo
echo Failed after ${i} iterations.
echo Expected ${EXPECTED_ERRORS} errors but got:
echo ===========================================
cat ${LOGFILE}
echo ===========================================
echo

rm ${LOGFILE}

exit 1

