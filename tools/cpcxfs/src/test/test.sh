rm -f test.dsk
rm -f test2.dsk
rm -f test3.dsk
rm -f test4.dsk
echo '--- cpcxfs'
../cpcxfs
echo '--- cpcxfs -?'
../cpcxfs -?
echo '--- cpcxfs -h'
../cpcxfs -h
echo '--- cpcxfs -nd test.dsk'
../cpcxfs -nd test.dsk
echo '--- cpcxfs -f -e format -f DATA test.dsk'
../cpcxfs -f -e format -f DATA test.dsk
echo '--- cpcxfs -f -e format -d test2.dsk'
../cpcxfs -f -e format -d test2.dsk
echo '--- cpcxfs -f -e format -s test3.dsk'
../cpcxfs -f -e format -s test3.dsk
echo '--- cpcxfs -nd "test2.dsk"'
../cpcxfs -nd "test2.dsk"
echo '--- cpcxfs -ns "test3.dsk"'
../cpcxfs -ns "test3.dsk"
echo '--- cpcxfs -nv "test4.dsk"'
../cpcxfs -nv "test4.dsk"
echo '--- cpcxfs test.dsk'
../cpcxfs test.dsk
echo '--- cpcxfs "test2.dsk"'
../cpcxfs "test2.dsk"
echo '--- cpcxfs -o'
../cpcxfs -o 
echo '--- cpcxfs "test2.dsk"'
../cpcxfs -o DATA test.dsk
../cpcxfs -o DATA "test2.dsk"
../cpcxfs "test.dsk" -p smalldoc.txt
../cpcxfs "test.dsk" -b -f -p smalldoc.txt
../cpcxfs "test.dsk" -b -f -p "smalldoc.txt" "small1"
../cpcxfs "test.dsk" -t -f -p "smalldoc.txt" "small2"
../cpcxfs "test.dsk" -t -f -p "smalldoc.txt" "small.txt"
../cpcxfs -o DATA "test2.dsk" -p "smalldoc.txt"
../cpcxfs -o DATA "test2.dsk" -d
../cpcxfs -o DATA "test2.dsk" -s
../cpcxfs -o DATA "test2.dsk" -x "instruct.txt"
../cpcxfs -o DATA "test2.dsk" -o
echo '--- cpcxfs "test2.dsk" -f -mp "smalldoc*"'
../cpcxfs "test2.dsk" -f -mp "smalldoc*"
echo '--- cpcxfs "test2.dsk" -f -mp smalldoc*'
../cpcxfs "test2.dsk" -f -mp smalldoc*
echo '--- cpcxfs "test2.dsk" -f -g "small.txt"'
../cpcxfs "test2.dsk" -f -p "smalldoc.txt" "small.txt"
../cpcxfs "test2.dsk" -f -p "smalldoc.txt" "small"
../cpcxfs "test2.dsk" -f -p "smalldoc.txt" "small."
../cpcxfs "test2.dsk" -f -g "small.txt" smallfile.txt
echo '--- cpcxfs "test2.dsk" -f -g "small"'
../cpcxfs "test2.dsk" -f -g "small" smallfile.txt
echo '--- cpcxfs "test2.dsk" -f -g "small."'
../cpcxfs "test2.dsk" -f -g "small." smallfile.txt
echo '--- cpcxfs "test2.dsk" -mg "small*"'
../cpcxfs "test2.dsk" -f -mg "small*"
echo '--- cpcxfs "test2.dsk" -e dir;put "smalldoc.txt" "small.txt";get "small.txt";dir;close;quit'
../cpcxfs "test2.dsk" -e 'dir;put "smalldoc.txt" "small.txt";get "small.txt";dir;close;quit'

