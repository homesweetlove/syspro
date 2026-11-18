# 과제 2 - 작업제어 실습

PuTTY의 **일반 Bash 터미널에서 직접** 실행한다.

```bash
(echo START1; sleep 10; echo DONE1) &
(echo START2; sleep 10; echo DONE2) &
jobs -l
```

두 작업이 후면에서 실행되므로 `START1`, `START2`가 출력되고, 10초가 지나 정상 종료하면 각각 `DONE1`, `DONE2`가 출력된다. 둘의 출력 순서는 실행 시점에 따라 달라진다.

## 신호별 결과

```bash
kill -KILL %1
```

첫 번째 작업을 강제로 종료하므로 해당 작업의 `DONE1`은 출력되지 않는다. 종료한 프로세스에는 `STOP`이나 `CONT`를 보낼 수 없다.

PPT에는 같은 첫 번째 프로세스에 `KILL`, `STOP`, `CONT`를 연달아 보내라고 되어 있지만, `KILL` 후에는 대상이 사라진다. 따라서 `STOP`, `CONT`는 **새로 실행한 작업**으로 확인한다.

```bash
(echo START1; sleep 30; echo DONE1) &
jobs -l
kill -STOP %3
jobs -l
kill -CONT %3
jobs -l
```

위 예시의 `%3`은 예시 번호다. 실제로는 `jobs -l`에 표시된 새 작업 번호를 사용한다. 두 번째 실행만 `sleep 30`으로 늘려서 신호를 입력할 시간을 확보했다.

- `kill -STOP` 후: 작업이 `Stopped` 상태가 되고 진행이 멈춘다.
- `kill -CONT` 후: 작업이 `Running` 상태로 돌아가고 남은 작업을 수행한다.
- `kill -KILL` 후: 작업이 즉시 종료되며, 다시 이어서 실행할 수 없다.

`jobs` 출력은 셸 세션에서 실행한 다른 작업과 경과 시간에 따라 달라질 수 있다.
