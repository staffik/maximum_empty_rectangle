#!/bin/bash
TEST_DIR=$1
sh tools/generate.sh 1000 $TEST_DIR/small_uniform 100 100 0
sh tools/generate.sh 100 $TEST_DIR/medium_uniform 10000 10000 0 
sh tools/generate.sh 10 $TEST_DIR/big_uniform 1000000 1000000 0

sh tools/generate.sh 1000 $TEST_DIR/small_normal 100 100 1
sh tools/generate.sh 100 $TEST_DIR/medium_normal 10000 10000 1
sh tools/generate.sh 10 $TEST_DIR/big_normal 1000000 1000000 1

sh tools/generate.sh 1000 $TEST_DIR/small_mixed 100 100 2
sh tools/generate.sh 100 $TEST_DIR/medium_mixed 10000 10000 2 
sh tools/generate.sh 10 $TEST_DIR/big_mixed 1000000 1000000 2
