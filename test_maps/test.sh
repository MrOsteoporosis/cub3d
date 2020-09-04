#!/bin/bash
INVALID=$(ls | grep invalid);
VALID=$(ls | grep -v invalid | grep .cub);
DIR=${PWD##*/};

if [[ $1 == "leaks" ]]; then
	PREFIX="valgrind --tool=memcheck";
	echo "HI";
fi

cd ..;
echo -e "\n---------INVALID---------";
echo -e "\tShould print error for all\n";
for val in $INVALID; do
	echo "Testing: $val";
	echo -e -n "\t";
	$PREFIX ./cub3d $DIR/$val;
	echo ;
done

echo -e "\n----------VALID----------";
echo -e "\tShould launch cleanly for all\n";
for val in $VALID; do
	echo "Testing: $val";
	echo -e -n "\t";
	$PREFIX ./cub3d $DIR/$val;
	echo ;
done
