# 임베디드 시스템 설계실습 - 커널 모듈 이해 실습

## 1. 실습 개요
본 실습은 교재 9장 **모듈 프로그래밍**의 내용을 직접 코딩 과제로 확장하기보다, Raspberry Pi OS에서 실제 커널 모듈을 빌드하고 적재/제거하면서 교재의 핵심 개념을 확인하는 것을 목표로 한다.

학생은 제공된 예제 코드를 수정하지 않는다. 대신 명령어 실행 결과를 관찰하고, 교재의 용어와 실제 시스템 출력이 어떻게 연결되는지 기록한다.

이번 실습은 다음 순서로 진행한다.
1. `scripts/check_env.sh` - 커널 버전, 빌드트리, 컴파일러 환경 확인
2. `hello_module.c` - 가장 기본적인 kernel module의 load/unload 관찰
3. `module_info.c` - `MODULE_*` 메타데이터와 `modinfo` 확인
4. `module_param.c` - `module_param()`과 `insmod name=value` 형식 확인
5. `usage_provider.c`, `usage_consumer.c` - usage count와 모듈 의존성 확인
6. `worksheets/ch09_observation.md` - 관찰 기록 및 제출

---

## 2. 실습 운영 원칙
- 본 실습은 **최신 Raspberry Pi OS가 설치된 일반 Raspberry Pi**를 기준으로 진행한다.
- Bookworm/Trixie 여부보다 중요한 것은 `/lib/modules/$(uname -r)/build`가 정상인지이다.
- Achro-EM Kit의 벤더 커널 환경에서는 빌드트리가 없을 수 있으므로, 본 레포의 빌드 실습은 일반 Raspberry Pi에서 수행한다.
- 학생은 커널 모듈 코드를 새로 작성하지 않는다.
- 제공된 예제 코드를 읽고, 빌드/적재/제거/로그 확인 결과를 해석한다.
- `insmod`로 올린 모듈은 실습이 끝나면 반드시 `rmmod`로 제거한다.
- 커널 로그는 `dmesg -T`로 확인한다.

---

## 3. 실습 환경 및 준비물
- Raspberry Pi 4 또는 Raspberry Pi 5
- Raspberry Pi OS
- SSH 접속 가능 환경
- `sudo` 권한이 있는 사용자 계정
- 커널 헤더 및 빌드 도구가 설치된 환경

레포 다운로드:
```bash
git clone <repo-url> embweek11
cd embweek11
```

필요 패키지 설치:
```bash
sudo apt update
sudo apt install -y build-essential
```

64-bit Raspberry Pi OS:
```bash
sudo apt install -y linux-headers-rpi-v8
```

32-bit Raspberry Pi OS:
```bash
sudo apt install -y linux-headers-rpi-{v6,v7,v7l}
```

환경 확인:
```bash
bash scripts/check_env.sh
```

다음과 같이 출력되면 실습을 진행할 수 있다.
```text
OK: /lib/modules/<kernel-version>/build is ready.
Result: ready for Chapter 9 kernel module practice.
```

파일 디렉토리 구조:
```text
embweek11/
├── hello_module.c
├── module_info.c
├── module_param.c
├── usage_provider.c
├── usage_consumer.c
├── Makefile
├── scripts/
│   ├── check_env.sh
│   ├── clean_loaded_modules.sh
│   └── run_smoke_test.sh
├── worksheets/
│   └── ch09_observation.md
├── .gitignore
└── README.md
```

---

## 4. 실습 1 - 커널 모듈 빌드 환경 확인

### 4.1 목표
- 현재 실행 중인 커널 버전을 확인한다.
- 외부 커널 모듈 빌드에 필요한 build tree가 있는지 확인한다.
- `uname -r`과 `/lib/modules/$(uname -r)/build`의 관계를 이해한다.

### 4.2 실행
```bash
bash scripts/check_env.sh
```

직접 확인:
```bash
uname -r
uname -m
ls -ld /lib/modules/$(uname -r)/build
test -e /lib/modules/$(uname -r)/build/Makefile && echo OK || echo MISSING
```

### 4.3 확인 사항
- 내 Raspberry Pi의 커널 버전은 무엇인가?
- 내 Raspberry Pi는 32-bit인가, 64-bit인가?
- `/lib/modules/$(uname -r)/build`가 존재하는가?
- 이 환경에서 제공된 `.ko` 예제를 빌드할 수 있는가?

---

## 5. 실습 2 - 제공된 모듈 빌드

### 5.1 목표
- 교재의 Makefile 흐름을 실제 `make` 출력으로 확인한다.
- `.c` 파일이 `.ko` 커널 모듈 파일로 만들어지는 과정을 관찰한다.

### 5.2 실행
```bash
make clean
make
ls -l *.ko
```

### 5.3 확인 사항
- 생성된 `.ko` 파일은 몇 개인가?
- `make` 출력에서 `CC [M]`, `MODPOST`, `LD [M]` 단계가 보이는가?
- `Makefile`의 `KDIR := /lib/modules/$(shell uname -r)/build`는 어떤 경로를 가리키는가?

---

## 6. 실습 3 - `hello_module.c` 생명주기 관찰

### 6.1 목표
- `module_init()`과 `module_exit()`이 언제 실행되는지 확인한다.
- `insmod`, `lsmod`, `rmmod`, `dmesg` 명령의 흐름을 익힌다.

### 6.2 소스 확인
```bash
sed -n '1,120p' hello_module.c
```

### 6.3 적재
```bash
sudo insmod ./hello_module.ko
lsmod | grep hello_module
sudo dmesg -T | tail -n 20
```

### 6.4 제거
```bash
sudo rmmod hello_module
sudo dmesg -T | tail -n 20
```

### 6.5 확인 사항
- `insmod` 직후 `module_begin()`의 메시지가 출력되는가?
- `rmmod` 직후 `module_end()`의 메시지가 출력되는가?
- `printk()` 출력은 왜 터미널이 아니라 `dmesg`에서 확인하는가?

---

## 7. 실습 4 - `modinfo`와 `vermagic`

### 7.1 목표
- `MODULE_LICENSE`, `MODULE_AUTHOR`, `MODULE_DESCRIPTION`, `MODULE_VERSION`의 역할을 확인한다.
- `modinfo` 명령으로 모듈 메타데이터를 읽는다.
- `vermagic`과 현재 커널 버전의 관계를 이해한다.

### 7.2 실행
```bash
modinfo module_info.ko
modinfo -F vermagic module_info.ko
uname -r
```

### 7.3 load/unload
```bash
sudo insmod ./module_info.ko
sudo dmesg -T | tail -n 20
sudo rmmod module_info
sudo dmesg -T | tail -n 20
```

### 7.4 확인 사항
- `modinfo`에서 author, description, license, version이 보이는가?
- `vermagic` 안의 커널 버전이 `uname -r`과 일치하는가?
- 커널 버전이 맞지 않는 모듈은 왜 적재할 수 없는가?

---

## 8. 실습 5 - `module_param()` 관찰

### 8.1 목표
- `module_param()`으로 모듈 로드 시 값을 전달하는 흐름을 확인한다.
- `insmod module.ko name=value` 형식을 익힌다.
- `modinfo -F parm`으로 모듈이 받는 파라미터를 확인한다.

### 8.2 파라미터 정보 확인
```bash
modinfo module_param.ko
modinfo -F parm module_param.ko
```

### 8.3 실행
```bash
sudo insmod ./module_param.ko name=group01 debug=1
sudo dmesg -T | tail -n 20
sudo rmmod module_param
```

다른 값으로 다시 확인:
```bash
sudo insmod ./module_param.ko name=group02 debug=0
sudo dmesg -T | tail -n 20
sudo rmmod module_param
```

### 8.4 확인 사항
- `name`, `debug` 파라미터는 각각 어떤 역할을 하는가?
- `modinfo -F parm module_param.ko`에서 어떤 정보가 출력되는가?
- `insmod` 때 전달한 `name` 값이 `dmesg`에 반영되는가?

---

## 9. 실습 6 - usage count와 모듈 의존성

### 9.1 목표
- `lsmod` 출력의 세 번째 열인 usage count의 의미를 확인한다.
- 한 모듈이 다른 모듈의 symbol을 사용할 때 의존성이 생기는 것을 관찰한다.
- 사용 중인 모듈을 먼저 제거하려고 하면 실패하는 것을 확인한다.

### 9.2 provider 모듈만 적재
```bash
sudo insmod ./usage_provider.ko
lsmod | grep usage_
```

이때 `usage_provider`의 usage count는 보통 `0`이다.

### 9.3 consumer 모듈 적재
```bash
sudo insmod ./usage_consumer.ko
lsmod | grep usage_
sudo dmesg -T | tail -n 20
```

`usage_consumer`가 `usage_provider`의 exported symbol을 사용하므로, `usage_provider`의 usage count가 증가한다.

예상 형태:
```text
usage_consumer    ...    0
usage_provider    ...    1 usage_consumer
```

### 9.4 제거 순서 확인
먼저 잘못된 순서로 제거해 본다.

```bash
sudo rmmod usage_provider
```

`usage_provider`는 `usage_consumer`가 사용 중이므로 제거에 실패해야 한다.

올바른 순서:
```bash
sudo rmmod usage_consumer
sudo rmmod usage_provider
```

### 9.5 확인 사항
- `lsmod`의 usage count 값은 어느 열에 나타나는가?
- `usage_consumer`를 적재한 뒤 `usage_provider`의 usage count가 어떻게 바뀌는가?
- 왜 `usage_provider`를 먼저 제거할 수 없는가?
- 모듈 의존성이 있을 때 올바른 제거 순서는 무엇인가?

---

## 10. 실습 7 - 실수 상황 분석

### 10.1 같은 모듈을 두 번 적재
```bash
sudo insmod ./hello_module.ko
sudo insmod ./hello_module.ko
```

확인 후 정리:
```bash
sudo rmmod hello_module
```

### 10.2 없는 모듈 제거
```bash
sudo rmmod hello_module
```

### 10.3 확인 사항
- 같은 모듈을 두 번 올리면 어떤 에러가 발생하는가?
- 이미 제거된 모듈을 다시 제거하면 어떤 에러가 발생하는가?
- 이런 상황에서 `lsmod`와 `dmesg` 중 무엇을 먼저 확인해야 하는가?

---

## 11. 관찰 기록지

실습 결과는 아래 파일을 기준으로 정리한다.

```bash
less worksheets/ch09_observation.md
```

제출 시에는 `worksheets/ch09_observation.md`의 질문에 대한 답을 별도 문서로 작성하거나, 같은 형식을 복사해서 채워 제출한다.

---

## 12. 정리 명령
실습 중 모듈이 남아 있으면 다음 명령으로 정리한다.

```bash
bash scripts/clean_loaded_modules.sh
make clean
```

TA가 전체 동작을 한 번에 검증할 때는 다음 스크립트를 사용할 수 있다.

```bash
bash scripts/run_smoke_test.sh
```

---

## 13. 제출물
1. 환경 확인 결과
   - `uname -a`
   - `/lib/modules/$(uname -r)/build` 확인 결과
2. `make` 결과
   - 생성된 `.ko` 파일 목록
   - `CC [M]`, `MODPOST`, `LD [M]` 단계 관찰
3. `hello_module.c` 실습 결과
   - `insmod`, `lsmod`, `dmesg`, `rmmod` 결과
4. `module_info.c` 분석 결과
   - `modinfo` 출력
   - `vermagic`과 `uname -r` 비교
5. `module_param.c` 실습 결과
   - `modinfo -F parm` 출력
   - `name=...`, `debug=...` 입력에 따른 `dmesg` 결과
6. usage count 실습 결과
   - `usage_provider`만 적재했을 때의 `lsmod` 결과
   - `usage_consumer`까지 적재했을 때의 `lsmod` 결과
   - `usage_provider`를 먼저 제거했을 때의 실패 결과
   - 올바른 제거 순서 설명
7. 짧은 설명
   - 커널 모듈이 일반 C 프로그램과 다른 점
   - `vermagic`이 필요한 이유
   - `insmod`와 `rmmod`의 역할
   - usage count가 필요한 이유
   - Achro-EM Kit 벤더 커널 환경과 일반 Raspberry Pi OS 환경의 차이

