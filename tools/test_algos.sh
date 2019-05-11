TEST_DIR=$1

SMALL=$(find $TEST_DIR -name 'small*')
MEDIUM=$(find $TEST_DIR -name 'medium*')
BIG=$(find $TEST_DIR -name 'big*')

FULL="$SMALL $MEDIUM $BIG"

NAAMAD="$SMALL $MEDIUM"
NAAMAD_IMPROVED=$FULL

NANCY=$FULL
for i in $NANCY
do
	sh tools/evaluate.sh nancy $i
done

#for i in $NAAMAD
#do
#	sh tools/evaluate.sh naamad $i
#done
#
#for i in $NAAMAD_IMPROVED
#do
#	sh tools/evaluate.sh naamad_improved $i
#done
