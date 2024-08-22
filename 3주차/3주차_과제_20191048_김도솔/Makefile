cc = g++ # g++를 이용해 컴파일
target = main # 실행파일 이름
objects = main.o Str.o # 오브젝트 파일들

$(target) : $(objects) # 각 오브젝트 파일과 같은 이름의 .cpp 파일을 찾아 컴파일
	$(cc) -o $(target) $(objects)

$(objects) :  Str.h # 헤더 파일이 변경되어도 오브젝트 파일을 새로 생성

.PHONY : clean # 파일 이름과 겹치지 않기 위해 phony 사용
clean : # 현재 디렉토리에 있는 target 파일과 모든 object 파일 삭제
	rm $(target) $(objects)
