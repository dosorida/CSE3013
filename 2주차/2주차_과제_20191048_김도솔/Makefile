cc = gcc # gcc를 이용해 컴파일
target = ForStarWars # Makefile의 최종 목표가 되는 파일 이름
objects = main.o initarr.o count.o printarr.o # make에서 사용되는 object file들

$(target): $(objects) # 각 object file과 같은 이름의 .c 파일을 찾아 모두 컴파일
	$(cc) -o $(target) $(objects)

$(objects) : ForStarWars.h # 헤더 파일이 변경되어도 오브젝트 파일을 새로 생성

.PHONY : clean # 파일 이름과 겹치지 않기 위해 phony 사용
clean : # 현재 디렉토리에 있는 target 파일과 모든 object 파일 삭제
	rm $(target) $(objects)
