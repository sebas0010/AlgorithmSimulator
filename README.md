# AlgorithmSimulator

Win32 콘솔 환경에서 동작하는 `C++` 알고리즘 시각화 프로젝트입니다.  
공용 엔진 레이어와 시뮬레이터 로직 레이어를 분리해, `A*`, `Quad Tree`, `AVL Tree`를 직접 조작하며 동작 과정을 확인할 수 있도록 구성했습니다.

## 프로젝트 소개

이 프로젝트는 단순히 알고리즘을 구현하는 데서 끝나지 않고,  
알고리즘과 자료구조의 상태 변화를 화면에 드러내는 데 초점을 둔 콘솔 시뮬레이터입니다.

구조는 크게 두 부분으로 나뉩니다.

- `Engine`
  - 콘솔 더블 버퍼 렌더링
  - 입력 처리
  - `Actor` / `Level` 기반 생명주기 관리
- `Game`
  - 알고리즘별 시뮬레이터 화면
  - 메인 메뉴 / 일시정지 메뉴
  - 사용자 입력을 통한 상호작용 로직

메인 메뉴에는 다음 시뮬레이터가 포함되어 있습니다.

- `A Star Simulator`
- `Quad Tree Simulator`
- `AVL Tree Simulator`
- `AVL Tree Simulator 2`

## 개발 목적

이 프로젝트는 다음 역량을 보여주기 위한 목적으로 만들었습니다.

- 알고리즘 결과뿐 아니라 중간 과정을 시각적으로 표현하는 능력
- 작은 엔진 구조 위에 독립적인 시뮬레이터 화면을 구성하는 능력
- 입력, 상태 전환, 시간 기반 애니메이션을 콘솔 환경에서 처리하는 능력
- 같은 주제를 서로 다른 방식으로 시각화하며 구조를 비교하는 능력

## 시뮬레이터 구성

### 1. A* 시뮬레이터

사용자가 직접 시작점, 목표 지점, 벽을 배치한 뒤 경로 탐색을 실행하는 시뮬레이터입니다.

- 시작 지점 지정
- 목표 지점 지정
- 벽 배치 및 제거
- 경로 탐색 시작
- 탐색 과정을 프레임 단위로 시각화

구현 포인트:

- `priority_queue` 기반 open set
- `gScore`, `closed` 집합 관리
- 4방향 이웃 탐색
- 한 번에 한 노드씩 확장하는 시뮬레이션 흐름

관련 파일:

- `Game/Level/AStarSimulatorLevel.cpp`
- `Game/Actor/AStarPlayer.cpp`
- `Game/Actor/AStarNode.h`

### 2. Quad Tree 시뮬레이터

공간 분할 자료구조인 Quad Tree를 콘솔 화면에서 직접 확인할 수 있는 시뮬레이터입니다.  
사용자가 삽입 모드에 들어가 점을 배치하면, 트리 노드가 분할되며 오브젝트가 하위 사분면으로 재배치됩니다.

구현 포인트:

- 재귀적인 Quad Tree 노드 구조
- 분할 전 이펙트 처리
- 화면 좌표 기반 영역 분할
- 커서 이동 후 위치 확정 방식의 오브젝트 삽입

관련 파일:

- `Game/Level/QuadTreeSimulatorLevel.cpp`
- `Game/Actor/QuadPlayer.cpp`
- `Game/Actor/QuadNode.cpp`

### 3. AVL Tree 시뮬레이터

첫 번째 AVL 시뮬레이터는 AVL 트리의 기본 삽입, 삭제, 회전 로직을 비교적 단순한 형태로 보여주는 버전입니다.

구현 포인트:

- BST 삽입 / 삭제 흐름
- 첫 번째 불균형 `z-y-x` 삼중 노드 탐색
- LL / RR / LR / RL 회전 처리
- 구조 변화 후 전체 트리 재배치

관련 파일:

- `Game/Level/AVLTreeSimulatorLevel.cpp`
- `Game/Actor/NodeActor.cpp`

### 4. AVL Tree 시뮬레이터 2

두 번째 AVL 시뮬레이터는 같은 개념을 더 시각적으로 표현한 버전입니다.  
삽입, 삭제, 회전 과정이 단계별 애니메이션으로 진행되며, 노드 자체도 더 크게 렌더링됩니다.

첫 번째 버전과 비교했을 때의 차이:

- 노드 이동 애니메이션
- 큰 원형 노드 렌더링
- 삽입 / 삭제 / 회전의 단계적 처리
- 삭제 전 깜빡임, 이동 완료 대기, 리밸런싱 시각화

관련 파일:

- `Game/Level/AVLTreeSimulatorLevel2.cpp`
- `Game/Actor/AVLNode.cpp`

## 핵심 구현 포인트

### 콘솔 엔진 구조 분리

솔루션은 `Engine` DLL과 `Game` 실행 프로젝트로 분리되어 있습니다.

- `Engine`은 DLL로 빌드됩니다.
- 공개 헤더는 `Includes/`로 복사됩니다.
- `Engine.lib`, `Engine.dll`은 `Lib/Engine/`으로 복사됩니다.
- `Game`은 해당 라이브러리를 링크하고, 빌드 후 `Engine.dll`을 출력 폴더로 복사합니다.

이 구조 덕분에 렌더링 / 입력 / 레벨 관리는 엔진에 두고, 실제 알고리즘 시뮬레이터는 게임 레이어에 집중할 수 있습니다.

### 레벨 기반 화면 전환

프로젝트는 메뉴와 시뮬레이터를 각각 독립적인 레벨로 구성합니다.

- 메인 메뉴
- 알고리즘 시뮬레이터 레벨
- 일시정지 메뉴

이 구조 덕분에 각 알고리즘 화면이 서로 분리되어 있고, 새로운 시뮬레이터를 추가하기도 쉽습니다.

### 결과가 아니라 과정의 시각화

이 프로젝트의 핵심은 계산 결과만 출력하는 것이 아니라,  
알고리즘이 내부적으로 어떻게 진행되는지를 보이게 만든 점입니다.

- A*는 노드를 단계적으로 확장합니다.
- Quad Tree는 분할 과정을 화면에 드러냅니다.
- AVL은 불균형 노드와 회전 과정을 눈에 보이게 표현합니다.

즉, 구현 자체보다도 "중간 상태를 어떻게 보여줄 것인가"가 중요한 프로젝트입니다.

## 프로젝트 구조

```text
AlgorithmSimulator
├─ Engine/          # 콘솔 엔진 코드
├─ Game/            # 시뮬레이터 및 메뉴 로직
├─ Includes/        # Engine 공개 헤더
├─ Lib/Engine/      # Engine 빌드 결과물
└─ AlgorithmSimulator.sln
```

## 빌드 방법

### 요구 사항

- Windows
- Visual Studio 2022
- MSVC v143 Toolset

### 빌드 순서

1. `AlgorithmSimulator.sln`을 엽니다.
2. `Game` 프로젝트를 시작 프로젝트로 설정합니다.
3. `Debug|x64` 또는 `Release|x64`로 빌드합니다.
4. `Game` 프로젝트를 실행합니다.

## 조작 방법

### 공통

- 메뉴 이동: `Up`, `Down`
- 메뉴 선택: `Enter`
- 시뮬레이터 내 일시정지 토글: `Space`
- 일시정지 메뉴 열기: `Esc`

### A* 시뮬레이터

- 커서 이동: `Arrow Keys`
- 시작 지점 지정: `S`
- 벽 배치: `W`
- 목표 지점 지정: `G`
- 현재 위치 오브젝트 제거: `D`
- 전체 초기화: `C`
- 시뮬레이터 시작: `Enter`

### Quad Tree 시뮬레이터

- 삽입 모드 진입: `I`
- 배치 커서 이동: `Arrow Keys`
- 삽입 확정: `Enter`
- 삽입 취소: `Esc`

### AVL Tree 시뮬레이터 / AVL Tree 시뮬레이터 2

- 노드 삽입: `I`
- 노드 삭제: `D`
- 전체 삭제: `C`
- 숫자 입력: `0-9`
- 입력 수정: `Backspace`
- 입력 확정: `Enter`
- 입력 취소: `Esc`

## 주요 소스 위치

- 엔진 메인 루프: `Engine/Core/Engine.cpp`
- 입력 처리: `Engine/Core/Input.cpp`
- 레벨 컨테이너: `Engine/Level/Level.cpp`
- 시작 지점: `Game/Main.cpp`
- 메인 메뉴: `Game/Level/MainMenuLevel.cpp`
- 일시정지 메뉴: `Game/Level/PauseLevel.cpp`

## 한 줄 요약

콘솔 환경에서 알고리즘의 실행 과정을 직접 조작하고 시각적으로 확인할 수 있도록 만든 `C++` 기반 알고리즘 시뮬레이터 프로젝트입니다.
