# 9장 커널 모듈 이해 실습 관찰 기록지

## 1. 실습 환경

```bash
uname -a
uname -r
uname -m
ls -ld /lib/modules/$(uname -r)/build
test -e /lib/modules/$(uname -r)/build/Makefile && echo OK || echo MISSING
```

기록:

```text
커널 버전:
아키텍처:
build tree 경로:
build tree 존재 여부:
```

질문:

```text
왜 커널 모듈을 빌드할 때 현재 커널과 맞는 build tree가 필요한가?
```

## 2. make 결과 관찰

```bash
make clean
make
ls -l *.ko
```

기록:

```text
생성된 .ko 파일 목록:
make 출력에서 CC [M], MODPOST, LD [M]가 의미하는 단계:
```

질문:

```text
교재의 Makefile에서 KDIR 또는 /lib/modules/$(uname -r)/build가 하는 역할은 무엇인가?
```

## 3. hello_module 생명주기

```bash
sudo insmod ./hello_module.ko
lsmod | grep hello_module
sudo dmesg -T | tail -n 20
sudo rmmod hello_module
sudo dmesg -T | tail -n 20
```

기록:

```text
insmod 후 lsmod 결과:
load 시 dmesg 메시지:
unload 시 dmesg 메시지:
```

질문:

```text
module_init()과 module_exit()는 각각 언제 실행되는가?
printk() 출력은 왜 터미널이 아니라 dmesg에서 확인하는가?
```

## 4. modinfo와 vermagic

```bash
modinfo module_info.ko
modinfo -F vermagic module_info.ko
uname -r
```

기록:

```text
license:
author:
description:
version:
vermagic:
uname -r:
```

질문:

```text
vermagic 안의 커널 버전이 현재 커널과 다르면 어떤 문제가 생기는가?
MODULE_LICENSE("GPL") 정보는 modinfo에서 어떻게 보이는가?
```

## 5. module_param 관찰

```bash
modinfo -F parm module_param.ko
sudo insmod ./module_param.ko name=group01 debug=1
sudo dmesg -T | tail -n 20
sudo rmmod module_param
```

기록:

```text
modinfo -F parm 출력:
name=group01이 반영된 dmesg 메시지:
debug=1이 반영된 dmesg 메시지:
```

질문:

```text
module_param()으로 등록된 값은 insmod 명령에서 어떤 형식으로 전달하는가?
```

## 6. usage count와 모듈 의존성

```bash
sudo insmod ./usage_provider.ko
lsmod | grep usage_

sudo insmod ./usage_consumer.ko
lsmod | grep usage_

sudo rmmod usage_provider

sudo rmmod usage_consumer
sudo rmmod usage_provider
```

기록:

```text
provider만 적재했을 때 usage_provider 행:
consumer까지 적재했을 때 usage_provider 행:
usage_provider를 먼저 제거했을 때 에러 메시지:
올바른 제거 순서:
```

질문:

```text
lsmod의 세 번째 열 usage count는 무엇을 의미하는가?
왜 usage_consumer를 먼저 제거해야 하는가?
```

## 7. 정리 확인

```bash
bash scripts/clean_loaded_modules.sh
lsmod | grep -E 'hello_module|module_info|module_param|usage_provider|usage_consumer'
```

기록:

```text
남아 있는 실습 모듈:
```

## 8. 제출 전 요약

```text
이번 실습에서 확인한 9장 핵심 개념 3가지:

1.
2.
3.

Achro-EM Kit의 벤더 커널 환경과 일반 Raspberry Pi OS 환경의 차이:
```

