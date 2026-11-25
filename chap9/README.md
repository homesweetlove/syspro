# 시스템 프로그래밍 9장 - 프로세스 제어

PPT의 실습 문제 13개와 과제 4개를 정리한 파일이다. Linux(우분투)에서 GCC로 컴파일한다.

## 폴더 구성

| 경로 | 내용 |
| --- | --- |
| `prob1-1/`, `prob1-2/`, `prob1-3/` | 1-1, 1-2, 1-3 프로세스 생성 |
| `prob2-1/`, `prob2-2/` | 2-1, 2-2 프로세스 기다리기 |
| `prob3-1/`, `prob3-2/`, `prob3-3/` | 3-1, 3-2, 3-3 프로그램 실행 |
| `prob4/` | 4 system() 함수의 원리 구현 |
| `prob5-1/`, `prob5-2/` | 5-1, 5-2 입출력 재지정 |
| `prob6-1/`, `prob6-2/` | 6-1, 6-2 프로세스 그룹 |
| `hw1/`~`hw4/` | 과제 1~2 실행 명령과 설명, 과제 3~4 쉘 프로그램 |

## 컴파일

`syspro`의 `chap9` 폴더 안에서 실행한다.

```bash
cd ~/syspro/chap9
make
```

컴파일 후 실행 파일은 `build` 폴더에 생긴다. 예를 들어 `prob1-1/main.c`는 `build/prob1-1/main`로 실행한다.
`build/`는 `.gitignore`에 등록되어 있으므로 GitHub에 실행 파일이 올라가지 않는다.

`make`를 사용할 수 없으면 아래처럼 개별 컴파일할 수 있다.

```bash
gcc -std=c11 -Wall -Wextra prob1-1/main.c -o main
./main
```

## 실습 문제 실행

```bash
./build/prob1-1/main
./build/prob1-2/main
./build/prob1-3/main

./build/prob2-1/main
./build/prob2-2/main

./build/prob3-1/main
./build/prob3-2/main
./build/prob3-3/main ls -l

./build/prob4/main

./build/prob5-1/main output.txt
cat output.txt
./build/prob5-2/main result.txt wc output.txt
cat result.txt

./build/prob6-1/main
./build/prob6-2/main
```

## 과제 실행

- **과제 1, 2:** `hw1/assignment1_jobs.md`, `hw2/assignment2_jobcontrol.md`의 명령을 PuTTY Bash 터미널에서 직접 실행하고 출력 결과를 확인한다.
- **과제 3:** `./build/hw3/main`을 실행한다.
- **과제 4:** `./build/hw4/main`을 실행한다.

과제 3의 입력 예시는 다음과 같다.

```text
[shell] ls -l
[shell] pwd; date; whoami
[shell] sleep 2 &
[shell] echo hello > test.txt
[shell] wc -w < test.txt
[shell] exit
```

과제 4의 입력 예시는 다음과 같다.

```text
[Shell] ls -al
[Shell] sleep 2 &
[Shell] date
[Shell] exit
```

두 쉘은 문제에서 요구한 기능에 맞춘 기초 구현이다. 따옴표 처리나 파이프(`|`) 등은 구현 범위에 포함되지 않는다. 두 쉘 모두 `exit` 명령으로 종료할 수 있다.

## 실행 결과를 볼 때 주의할 점

1. 프로세스 PID와 출력 순서는 실행할 때마다 달라질 수 있다.
2. 실습 2-2의 PPT에 `sleep()을 이용해 1ms`라고 되어 있지만, 실제 `sleep(1)`은 **1초**다. 슬라이드의 `sleep(1)`, `sleep(2)` 의도에 따라 각각 1초, 2초로 구현했다.
3. 실습 4에서는 표준 라이브러리 `system()`과 이름이 충돌하지 않도록 사용자 함수 `mysystem()`을 만들었다. 내부 구현은 `fork()`, `execl()`, `waitpid()`를 사용한다.
4. `wait()`의 `status`는 그대로 종료 코드가 되는 것이 아니다. `WIFEXITED()`와 `WEXITSTATUS()`를 이용해 실제 종료 코드를 읽는다.

## GitHub main 브랜치로 푸시

`chap9` 폴더의 파일을 `~/syspro/chap9`에 복사한 뒤, **`syspro` 저장소 루트**에서 실행한다.

```bash
cd ~/syspro
git status
git switch main
git add chap9
git commit --allow-empty-message -m ""
git push origin main
```

`git commit --allow-empty-message -m ""`는 커밋 메시지를 비워 놓는다. 단, 실제 커밋 기록(해시)은 GitHub에 남는다. 기존에 변경된 파일이 없는 경우에는 새 커밋이 생성되지 않는다.

> **주의:** `git status`로 업로드할 파일을 확인한다. 이 파일 묶음에는 개인 키나 계정 정보가 들어 있지 않지만, 저장소의 다른 파일은 별도로 점검해야 한다.
