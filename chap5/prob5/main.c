#define _FILE_OFFSET_BITS 64
#include <fcntl.h>
#include <stdio.h>
#include "student.h"

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "사용법: %s 레코드파일\n", argv[0]); return 1;
    }
    int fd = open(argv[1], O_WRONLY | O_CREAT, 0640);
    if (fd == -1) { perror(argv[1]); return 1; }
    char id_text[64], name[MAX], score_text[64];
    int count, failed = 0;
    printf("학번 이름 점수 입력 (Ctrl+D로 종료):\n");
    while ((count = scan_token(id_text, sizeof id_text)) != EOF) {
        if (count != 1 || scan_token(name, sizeof name) != 1 ||
            scan_token(score_text, sizeof score_text) != 1) {
            fprintf(stderr, "입력 오류: 학번 이름 점수를 완전하게 입력하세요.\n");
            failed = 1; break;
        }
        struct student s = {0};
        off_t offset;
        if (parse_int(id_text, &s.id) == -1 || parse_int(score_text, &s.score) == -1 ||
            record_offset(s.id, &offset) == -1) {
            fprintf(stderr, "입력 오류: 학번은 %d 이상, 점수는 정수여야 합니다.\n", START);
            failed = 1; break;
        }
        for (size_t i = 0; name[i]; ++i) s.name[i] = name[i];
        if (lseek(fd, offset, SEEK_SET) == (off_t)-1 || write_record(fd, &s) == -1) {
            perror("레코드 저장"); failed = 1; break;
        }
        printf("저장: %d %s %d\n", s.id, s.name, s.score);
    }
    if (ferror(stdin)) { perror("stdin"); failed = 1; }
    if (close(fd) == -1) { perror("close"); failed = 1; }
    return failed;
}
