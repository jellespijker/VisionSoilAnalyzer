export LICENSELEN=`wc -l license.txt | cut -f1 -d ' '`  
find . -type f \(-name \*.cpp -o -name \*.h \) -print0 | xargs -0 ../Documentation/Copyright/addlicense.sh  
