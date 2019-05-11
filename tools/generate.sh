#!/bin/bash
BASELINE=./bin/naamad_improved
TEST_NO=$1
OUT_DIR=$2
MAX_X=$3
MAX_N=$4
GEN_METHOD=$5
TMP_DIR=./tmp/
make
mkdir -p $OUT_DIR
mkdir -p $TMP_DIR
echo "generating tests to $OUT_DIR"

for i in `seq 1 $TEST_NO`
do
	echo "Generating test #$i"
	base="$OUT_DIR/test$i"
	input="$base.in"
	expected="$base.out"
	output="./tmp/test$i.baseline"
	./bin/gentest $MAX_X $MAX_N $GEN_METHOD > $input
	$BASELINE < $input > $output
	./bin/rect_to_area < $output > $expected
	echo "Saved to $base.in"
done
