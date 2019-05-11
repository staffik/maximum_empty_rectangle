#!/bin/bash
ALG=$1
TEST_DIR=$2
make
TESTS_NO=$(ls $TEST_DIR | grep '.in' | wc -l)
echo "testing $ALG algorithm on $TESTS_NO tests from $2"
TIME_SUM=0
RR_CNT=0

for i in $TEST_DIR/*.in
do
	base=$(basename $i .in)
	expected_area="$TEST_DIR/$base.out"
	algo_output="./tmp/$base.out"
	judge_log="./tmp/$base.log"
	profile_log="./tmp/$base.profile"
	/usr/bin/time -o $profile_log -p ./bin/$ALG < $i > $algo_output
	./bin/judge $i $expected_area $algo_output > $judge_log
	if [ -s $judge_log ]
	then
		echo "wrong answer at test $i"
		cat $judge_log
		break
	else
		echo "test $i passed"
	fi
	TIME_ELAPSED=$(cat $profile_log | grep user | awk '{print $2}')
	echo "Time elapsed: $TIME_ELAPSED"
	TIME_SUM="$TIME_SUM+$TIME_ELAPSED"
	RR_CONSIDERED=$(cat $algo_output | awk '{print $5}')
	echo "RRs considered: $RR_CONSIDERED"
	RR_CNT=$(echo "$RR_CNT + $RR_CONSIDERED" | bc)
done
TIME_SUM=$(echo $TIME_SUM | bc)
echo "ALL TESTS PASSED!"
echo "TOTAL TIME: $TIME_SUM"
echo "TOTAL RR CONSIDERED: $RR_CNT"
ALGTESTRES="./tmp/$ALG$(echo $TEST_DIR | sed 's/\///g').res"
echo "$TIME_SUM $RR_CNT" > $ALGTESTRES
echo "Results saved to $ALGTESTRES"

