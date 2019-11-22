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
		my=$(./ft_ssl $1 -q txt)
		or=$($2)
		difference=$(diff <(echo "$my") <(echo "$or"))
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
		my=$(./ft_ssl $1 -q txt)
		or=$($2)
		difference=$(diff <(echo "$my") <(echo "$or"))
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
		my=$(./ft_ssl $1 -q $path)
		if [ "$1" = "sha256" ]; then
			or=$($2 $path | cut -d ' ' -f1)
		else
			or=$($2 $path)
		fi
		difference=$(diff <(echo "$my") <(echo "$or"))
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

test_simple_letter "md5" "md5 -q txt"
test_random_text "md5" "md5 -q txt"
#generate_huge_file "sha256" "shasum -a 256 " "vifonne"
