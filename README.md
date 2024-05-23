# SSHS Database Project

이 프로젝트는 설곽에서 이석 등 작업을 편리하게 할 수 있게 해주는 데에 목표가 있다.

## 설치/실행 방법

설치는 다음 명령어를 통해 가능하다.

```bash
git clone https://github.com/adofai07/SSHSDB
```

컴파일은 다음 명령어를 통해 가능하다.

```bash
gcc .\main.c .\getinput.c .\makeaccount.c .\u8string.c .\getsheet.c .\groups.c .\getenv.c -o main -lregex
```

컴파일을 했다면 `main.exe`가 생성되었을 것이다. 이 `main.exe`를 실행하면 된다.

```bash
& .\main.exe
```

## 역할 분담

|Name|File|
|:---|:---|
|최은호|`u8string.c`, `MoveSeat.py`|
|안준혁|Project Manager|
|고민재|`main.c`, `typing.h`|
|신믿음|`getsheet.c`, `makeaccount.c`|
|김유현|`groups.c`, `getinput.c`|
|남규영|None|

## 프로그래밍 스타일

#### 괄호는 One True Brace Style로 한다.

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

#### 주석은 가능하면 그 줄에 주석만 있도록 한다. `/*`와 `*/`는 사용하지 않는다.

```c
// Do this
int a;

int b; // and not this
```

#### 함수에 대한 주석은 `///` 이후에 `@brief`, `@param`, `@return`을 이용한다.

```c
/// @brief Checks is string is a positive integer
/// @param s string
/// @param len length of string
/// @return 0 if string is not an integer, 1 if it is
int is_number(char *s, int len) {
    int chk = 1;

    for (int i = 0; i < len; i++) {
        if (!('0' <= s[i] && s[i] <= '9')) {
            chk = 0;
            break;
        }
    }

    return chk;
}
```

#### 변수, 함수, 구조체 등의 이름은 snake case로 적는다.

```c
// Good
int memory_limit;

// Bad
int memoryLimit;
```

#### 구조체 자료형을 만들 때에는 구조체를 먼저 만들고 `typedef`한다. 또한, 자료형의 이름은 구조체의 이름에 `_t`를 붙인다.

```c
struct account {
    char email[50];
    char password[50];
    char name[50];

    int role;
    int student_no;
    int pos_pts;
    int neg_pts;
};

typedef struct account account_t;
```

#### 단항 연산자는 피연산자와 붙여 쓴다. 이항 연산자는 피연산자와 띄어 쓴다. 단, 예외도 존재한다.

```c
int a = (3 + 5) ^ 5;
int b = ~a;
```

* `()`, `[]`, `->`, `.`, `->*`, `.*`는 피연산자와 붙여 쓴다.
* 타입 캐스팅 연산자는 피연산자와 띄어 쓴다.

#### 컴마 앞에는 띄어쓰기를 하지 않는다. 컴마 뒤에는 한 칸을 띈다. `#define`문 안에서는 이 규칙이 적용되지 않는다.

```c
int a, b = 5, c;
```

#### `#define`문 안에서는 괄호를 많이 사용한다. 또한 같은 변수가 반복될 때에는 `__typeof__`를 사용한다.

```c
// Good
#define min(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b;       \
})

// Bad
#define min(a, b) (a < b ? a : b)
```

#### 중괄호를 같은 줄에 열고 닫을 때에는 앞뒤로 한 칸씩 띈다.

```c
int cnt[3] = { 0, 0, 0 };
```

#### 함수의 파라미터가 너무 많을 경우 아래와 같이 나눠 적는다. (연산자 뒤에 개행한다.)

```c
for (int i = 0; i < cnt; i++) {
    fprintf(fptr, "%s %s %s %d %d %d %d\n",
            accounts[i].email, accounts[i].password, accounts[i].name, accounts[i].role,
            accounts[i].student_no, accounts[i].pos_pts, accounts[i].neg_pts);
}
```

#### `if`문의 조건식 안에서 할당하지 않는다. `while`문 안에서의 할당은 괜찮다.

```c
// Good
new_string = (char *) malloc((len + n_bytes + 1) * sizeof(char));

if (new_string == NULL) {
    exit(EXIT_FAILURE);
}

// Bad
if ((new_string = (char *) malloc((len + n_bytes + 1) * sizeof(char))) == NULL) {
    exit(EXIT_FAILURE);
}
```

#### (중요) 주석이 아닌 이상 절대 코드에 한국어를 바로 적지 않는다. `\uXXXX`를 사용한다.

```c
// Good
const char *a = "\uAC00";

// Bad
const char *a = "가";
```

#### 문자열에 한국어를 포함할 때에는 한국어 1글자당 `char` 3글자를 잡는다.

```c
char s[7] = "\uC2DC\uBC1C";
```

#### 기본 헤더파일과 사용자 정의 헤더파일은 빈 줄로 분리한다.

```c
#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <wchar.h>
#include <locale.h>

#include "makeaccount.h"
#include "u8string.h"
#include "typing.h"
```

#### 헤더파일을 선언할 때는 함수 파라미터의 이름을 적지 않는다. 또한, `#define`아랫줄과 `#endif` 윗줄에는 한 줄을 띈다.

```c
#ifndef GETINPUT_H
#define GETINPUT_H

int is_number(char *, int);
int str_to_int(char *, int);
int get_input(int, int);

#endif
```

## Files

|Name|What it does|
|:---|:---|
|`admin_accountRequest.txt`|pending account log|
|`admin_recovery.txt`|account recovery requests|
|`{teacher name}_move.txt`|move request to {teacher name}|
|`{teacher name}_out.txt`|outing request to {teacher name}|
|`notifications_global.txt`|global notifications|
|`notifications_{student id}.txt`|personal notifications: {student id}|
|`notifications_{teacher name}.txt`|personal notifications: {teacher name}|
|`notifications_admin.txt`|personal notifications: admin|
|`alleviations.txt`|list of applicable alleviations|
|`accounts.txt`|registered account data|
|`groups.txt`|list of generated groups|
|`sheet.txt`|sheet data|
