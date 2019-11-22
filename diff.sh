#!/bin/bash
rm -rf txt
letter="a"
max=100
multiplication_factor=45
function test_simple_letter {
	echo "===== Tests $1 - $max times with letter $letter  ====="
	sleep 1
	for (( i=1; i<=$max; i++ ))
	do
		echo "==== $i ===="
		echo -n "$letter" >> txt
		my="./ft_ssl $1 -q txt"
		if [ "$3" = "1" ]; then
			if [ "$1" = "sha256" ]; then
				valgrind --leak-check=full $my 2>&1 | grep "definitely lost:\|SHA256" | sed -E 's/^==([0-9]{5})== {4}//g'
			else
				valgrind --leak-check=full $my 2>&1 | grep "definitely lost:\|MD5" | sed -E 's/^==([0-9]{5})== {4}//g'
			fi
		fi
		forDiff=$($my)
		if [ "$1" = "sha256" ]; then
				or=$($2| cut -d ' ' -f1)
		else
				or=$($2)
		fi
		difference=$(diff <(echo "$forDiff") <(echo "$or"))
		if [ -z "$difference" ]
		then
			echo "OK"
		else
			echo "KO"
			echo $difference
			exit
		fi
		echo ""
	done
	rm -rf txt
}

function test_random_text {
	echo "===== Tests $1 - $max with random text ====="
	sleep 1
	for (( i=1; i<=$max; i++ ))
	do
		echo "==== $i ===="
		random=$(LC_CTYPE=C tr -dc A-Za-z0-9_\!\@\#\$\%\^\&\*\(\)-+= < /dev/urandom | head -c $(echo "$i*$multiplication_factor" | bc) | xargs)
		echo -n "$letter" >> txt
		my="./ft_ssl $1 -q txt"
		if [ "$3" = "1" ]; then
			if [ "$1" = "sha256" ]; then
				valgrind --leak-check=full $my 2>&1 | grep "definitely lost:\|SHA256" | sed -E 's/^==([0-9]{5})== {4}//g'
			else
				valgrind --leak-check=full $my 2>&1 | grep "definitely lost:\|MD5" | sed -E 's/^==([0-9]{5})== {4}//g'
			fi
		fi
		forDiff=$($my)
		if [ "$1" = "sha256" ]; then
			or=$($2| cut -d ' ' -f1)
		else
			or=$($2)
		fi
		difference=$(diff <(echo "$forDiff") <(echo "$or"))
		if [ -z "$difference" ]
		then
			echo "OK"
		else
			echo "KO"
			echo $difference
			exit
		fi
		echo ""
	done
	rm -rf txt
}

function generate_huge_file {
	path=/sgoinfre/goinfre/Perso/$3/file.txt
	touch $path
	echo "==== Test $1 - 5 Generate huge file in $path ===="
	sleep 1
	for (( i=1; i<=50; i++ ))
	do
		echo "==== $i ===="
		dd if=/dev/urandom of=$path bs=1048576 count=$i
		my="./ft_ssl $1 -q $path"
		if [ "$4" = "1" ]; then
			if [ "$1" = "sha256" ]; then
				valgrind --leak-check=full $my 2>&1 | grep "definitely lost:\|SHA256" | sed -E 's/^==([0-9]{5})== {4}//g'
			else
				valgrind --leak-check=full $my 2>&1 | grep "definitely lost:\|MD5" | sed -E 's/^==([0-9]{5})== {4}//g'
			fi

		fi
		forDiff=$($my)
		if [ "$1" = "sha256" ]; then
			or=$($2 $path | cut -d ' ' -f1)
		else
			or=$($2 $path)
		fi
		difference=$(diff <(echo "$forDiff") <(echo "$or"))
		if [ -z "$difference" ]
		then
			echo "OK"
		else
			echo "KO"
			echo $difference
			exit
		fi
		echo ""
	done
	rm -f $path
}

test_simple_letter "md5" "md5 -q txt" 0
test_simple_letter "sha256" "shasum -a 256 txt" 0
test_random_text "md5" "md5 -q txt" 0
test_random_text "sha256" "shasum -a 256 txt" 0
generate_huge_file "sha256" "shasum -a 256 " "vifonne" 0
generate_huge_file "md5" "md5 -q " "vifonne" 0
