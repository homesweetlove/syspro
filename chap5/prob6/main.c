#define _FILE_OFFSET_BITS 64
#include <fcntl.h>
#include <stdio.h>
#include "student.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "사용법: %s 레코드파일\n", argv[0]); return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) { perror(argv[1]); return 1; }
    char token[64], answer[64] = "N";
    int failed = 0;
    do {
        int id;
        off_t offset;
        printf("검색할 학생의 학번 입력 : "); fflush(stdout);
        int input = scan_token(token, sizeof token);
        if (input == EOF) break;
        if (input != 1 || parse_int(token, &id) == -1) {
            printf("입력 오류\n");
        } else if (record_offset(id, &offset) == -1) {
            printf("레코드 [%d] 없음\n", id);
        } else {
            struct student s = {0};
            if (lseek(fd, offset, SEEK_SET) == (off_t)-1) {
                perror("lseek"); failed = 1; break;
            }
            ssize_t n = read_record(fd, &s);
            if (n == -1) { perror("read"); failed = 1; break; }
            if (n != 0 && n != (ssize_t)sizeof s) {
                fprintf(stderr, "불완전한 레코드\n"); failed = 1; break;
            }
            if (n == (ssize_t)sizeof s && s.id == id) {
                printf("학번: %d, 이름: %.*s, 점수: %d\n", s.id, MAX, s.name, s.score);
            } else printf("레코드 [%d] 없음\n", id);
        }
        printf("계속하시겠습니까?(Y/N) "); fflush(stdout);
        if (scan_token(answer, sizeof answer) != 1) break;
    } while ((answer[0] == 'Y' || answer[0] == 'y') && answer[1] == '\0');
    if (ferror(stdin)) { perror("stdin"); failed = 1; }
    if (close(fd) == -1) { perror("close"); failed = 1; }
    return failed;
}
