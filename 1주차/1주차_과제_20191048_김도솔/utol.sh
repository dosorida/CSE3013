echo 'Working directory:' #/sogang/under/cse20191048/ULdata
read dirname #directory 이름 입력한 것을 저장

if [ -n "$dirname" ]; then
	if [ ! -d "$dirname" ]; then
		echo "Error message : cannot access directory."
		exit 0
	fi
	cd $dirname
fi

for dir in *
do
newname=`echo $dir | tr "[a-z] [A-Z]" "[A-Z] [a-z]"` #변수 newname은 dir의 대소문자를 tr을 이용해 바꾼 것
mv $dir $newname #$dir을 newname으로 바꾼다.
done
