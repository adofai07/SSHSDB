# SSHS Database Project

이 프로젝트는 설곽에서 이석 등 작업을 편리하게 할 수 있게 해주는 데에 목표가 있다.

## 설치/실행 방법

설치는 다음 명령어를 통해 가능하다.

```bash
git clone https://github.com/adofai07/SSHSDB
```

컴파일은 다음 명령어를 통해 가능하다.

```bash
gcc .\main.c .\getinput.c .\makeaccount.c .\u8string.c -o main -lregex
```

컴파일을 했다면 `main.exe`가 생성되었을 것이다. 이 `main.exe`를 실행하면 된다.

```bash
& .\main.exe
```

## 프로그래밍 스타일

#### 괄호는 K&R 스타일로 한다.

```c
int main() {
    return 0;
}
```

#### `while`, `for`, `switch`문 뒤에는 한 칸을 띄고 한다. 또한, 증감식에는 가능하다면 후위 연산자를 쓴다.

```c
while (int i = 0; i < 5; i++) {
    // do something
}
```

#### 포인터 변수를 선언할 때에는 `*`을 변수명에 붙인다.

```c
int *a, *b;
```

#### 주석은 가능하면 그 줄에 주석만 있도록 한다.

```c
// Do this
int a;

int b; // and not this
```

#### 변수, 함수, 구조체 등의 이름은 snake case로 적는다.

```c
// Good
int memory_limit;

// Bad
int memoryLimit;
```

#### 단항 연산자는 피연산자와 붙여 쓴다. 이항 연산자는 피연산자와 띄어 쓴다.

```c
int a = (3 + 5) ^ 5;
int b = ~a;
```

#### 헤더파일을 선언할 때는 함수 파리미터의 이름을 적지 않는다. 또한, `#define`아랫줄과 `#endif` 윗줄에는 한 줄을 띈다.

```c
#ifndef GETINPUT_H
#define GETINPUT_H

int is_number(char *, int);
int str_to_int(char *, int);
int get_input(int, int);

#endif
```