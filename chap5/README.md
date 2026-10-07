# 5장 시스템 프로그래밍

실습 1~7과 연습 1~2의 Linux/POSIX C 프로그램입니다. 각 폴더에서 자료와 같이 `gcc -o main main.c`로 독립적으로 컴파일할 수 있습니다.

```sh
cd ~/syspro/chap5
make
make test
```

| 폴더 | 내용 | 폴더 안에서 실행 |
|---|---|---|
| prob1 | O_RDWR 파일 열기 | `./main test.txt` |
| prob2 | 512바이트 버퍼와 반복 read로 크기 계산 | `./main test.txt` |
| prob3 | 원본을 새 파일로 복사, 생성 권한 0600 | `./main test.txt copied.txt` |
| prob4 | creat와 dup, 공유 파일 위치 확인 | `./main` 다음 `cat myfile` |
| prob5 | 학번별 레코드 저장, 생성 권한 0640 | `./main student.txt` |
| prob6 | 레코드 조회 | `./main ../prob5/student.txt` |
| prob7 | 기존 학생의 점수 수정 | `./main ../prob5/student.txt` |
| exercise1 | 줄 번호, 목록, 범위, 전체 선택 | `./main test.txt` |
| exercise2 | 줄 순서 뒤집기 | `./main test.txt` |

prob5에는 `1401001 Kim 90`처럼 학번·이름·정수를 입력하고 Ctrl+D로 마칩니다. 이름은 공백 없이 최대 23바이트입니다. 학번 시작은 1401001이며 `(학번 - START) * sizeof(struct student)` 위치에 저장합니다. 학번 사이의 빈 레코드는 조회 결과에 표시하지 않습니다. prob6/7에서 Y 또는 y를 입력하면 계속합니다.

학생 파일은 확장자가 `.txt`여도 구조체를 저장한 바이너리 파일입니다. prob5/6/7을 같은 환경에서 빌드해서 사용해야 합니다. prob5는 기존 파일도 열어 입력한 학번의 레코드를 갱신합니다. 서로 다른 운영체제나 구조체 형식 사이의 파일 교환은 지원하지 않습니다.

prob2/3의 예제는 512바이트를 넘어 반복 읽기를 확인할 수 있습니다. 실제 실행으로 생성되는 실행파일·복사본·학생 파일은 Git에 포함하지 않습니다. 자료의 `create()`, `dub()`, `wrtie()`는 실제 시스템 함수 `creat()`, `dup()`, `write()`로 적용했습니다.

prob3는 같은 파일이나 하드링크를 복사 대상으로 지정해 원본을 지우는 일을 방지합니다. 이를 위해 대상 파일을 먼저 열고 원본과 구분한 뒤, 자료의 O_TRUNC와 같은 내용 초기화를 ftruncate로 수행합니다. exercise2는 원본의 마지막 줄바꿈 유무를 유지하며 줄 순서만 뒤집습니다.

검증 프로그램은 임시 폴더에서 동작하며 예제 파일과 사용자 레코드를 변경하지 않습니다. 단일 실행은 `make`, 전체 검증은 Python 3가 설치된 환경에서 `make test`를 사용합니다.
