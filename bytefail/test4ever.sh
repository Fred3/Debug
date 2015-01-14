#!/bin/bash

# Adjust to 0 when this actually works.
EXPECTED_ERRORS=2

LOGFILE=$(mktemp)

i=1
while true; do
	echo -n .
	if [[ $[i % 40] -eq 0 ]] ; then echo; fi
	i=$[i+1]
	./test.sh | tee ${LOGFILE} | grep -q "${EXPECTED_ERRORS} x FAILED" || break
done

echo
echo Failed after ${i} iterations.
echo Expected ${EXPECTED_ERRORS} erros but got:
echo ===========================================
cat ${LOGFILE}
echo ===========================================
echo

rm ${LOGFILE}

exit 1

