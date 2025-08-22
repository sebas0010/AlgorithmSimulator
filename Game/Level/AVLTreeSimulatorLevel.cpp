#include "AVLTreeSimulatorLevel.h"
#include "Input.h"
#include "Game/Game.h"
#include "Actor/NodeActor.h"
#include "Utils/Utils.h"

#include <iostream>
#include <limits>
#include <math.h>
#include <string>
#include <cstdlib>


AVLTreeSimulator::AVLTreeSimulator()
{
	// 일시정지 레벨 생성
	dynamic_cast<Game*>(&Game::Get())->CreatePauseLevel();

	/*treeRoot = new NodeActor(15, Vector2(0, 5));
	AddActor(treeRoot);
	NodeActor* tmp = new NodeActor(10, Vector2(0, 5));
	treeRoot->SetLeft(tmp);
	AddActor(tmp);
	tmp = new NodeActor(20, Vector2(0, 5));
	treeRoot->SetRight(tmp);
	AddActor(tmp);
	
	tmp = new NodeActor(25, Vector2(0, 5));
	treeRoot->GetRight()->SetRight(tmp);
	AddActor(tmp);

	LocateTree();*/

	InsertNode(15);
	InsertNode(7);
	InsertNode(23);
	InsertNode(3);
	InsertNode(11);
	InsertNode(19);
	InsertNode(27);
}

AVLTreeSimulator::~AVLTreeSimulator()
{
}

void AVLTreeSimulator::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	// esc 누르면 일시정지
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		dynamic_cast<Game*>(&Game::Get())->ToggleMenu();
	}

	// 노드가 삽입되는 과정
	if (isInserting)
	{
		timer.Tick(deltaTime);
		if (timer.IsTimeout())
		{
			InsertNodeProcessing();
			timer.Reset();
		}
		return;
	}

	// 노드가 회전하는 과정
	if (isRotating)
	{
		DoRotation();
	}


	// 
	if (Input::Get().GetKeyDown('I') || Input::Get().GetKeyDown('i'))
	{
		// 명령 받는 동안 엔진 일시정지
		Game::Get().Pause();

		// 숫자를 입력 받고 노드 추가
		GetInput();
	
		// 엔진 동작
		Game::Get().Resume();
	}

	if (Input::Get().GetKeyDown('a') || Input::Get().GetKeyDown('A'))
	{
		StartInsertNode(12);
	}
	if (Input::Get().GetKeyDown('b') || Input::Get().GetKeyDown('B'))
	{
		StartInsertNode(13);
	}
}

void AVLTreeSimulator::Render()
{
	super::Render();
	Game::Get().WriteToBuffer(Vector2::Zero, "AVL Tree Simulator", Color::BlueIntensity);

	Game::Get().WriteToBuffer(Vector2(0, 3), "I : 노드 추가, D : 노드 삭제, C : 모든 노드 삭제", Color::White);

}

void AVLTreeSimulator::GetInput()
{

	// 커서 위치 값 생성 및 커서 이동
	//COORD coord{ 0, 4 };
	//Utils::SetConsolePosition(coord);
	static COORD coord{ 0, 4 };
	Engine::Get().SetCursorPosition(coord);

	// 입력 받음
	std::string inputData;
	std::cin >> inputData;


	try {
		int data = std::stoi(inputData);
		StartInsertNode(data);
		nodeCount++;
	}
	catch (...) {
		// 잘못된 입력이므로 아무것도 하지 않음
	}
}

void AVLTreeSimulator::LocateTree()
{
	int height = TreeHeight(treeRoot);

	// 루트 노드의 Y 좌표는 6, X 좌표는 아래의 식에 따라
	int xPosition = (1LL << (height + 2)) - 4;

	// 가장 왼쪽의 노드가 없다면 X 좌표 -= 4
	if (height >= 1)
	{
		int tmp = height;
		NodeActor* tmpNode = treeRoot;
		while (true)
		{
			tmpNode = tmpNode->GetLeft();
			tmp--;
			if (!tmpNode) break;
		}
		if (tmp >= 0) xPosition -= 4;

	}

	Vector2 nodePosition(xPosition ,6);
	
	// 재귀 함수를 통해 모든 노드를 위치시킴
	LocateTreeRecursive(treeRoot, height, nodePosition);
}

void AVLTreeSimulator::LocateTreeRecursive(NodeActor* node, int height, Vector2 nodePosition)
{
	// 노드를 해당 위치로 이동
	node->SetPosition(nodePosition);


	// 왼쪽 자손 노드가 있다면 그 노드도 위치를 정함
	if (node->GetLeft())
	{
		int tmp = (int)(1LL << (height + 1));
		Vector2 leftPosition(nodePosition.x - tmp, nodePosition.y + 2);
		LocateTreeRecursive(node->GetLeft(), height - 1, leftPosition);
	}

	// 오른쪽 자손 노드가 있다면 그 노드도 위치를 정함
	if (node->GetRight())
	{
		int tmp = (int)(1LL << (height + 1));
		Vector2 rightPosition(nodePosition.x + tmp, nodePosition.y + 2);
		LocateTreeRecursive(node->GetRight(), height - 1, rightPosition);
	}
}

int AVLTreeSimulator::TreeHeight(NodeActor* node)
{
	if (!node) return -1;
	return 1 + (std::max)(TreeHeight(node->GetLeft()), TreeHeight(node->GetRight()));
}

void AVLTreeSimulator::InsertNode(int data)
{
	// 1) 빈 트리면 루트를 만든다.
	if (!treeRoot)
	{
		treeRoot = new NodeActor(data, Vector2(0, 5), nullptr);
		AddActor(treeRoot);

		LocateTree();
		return;
	}


	// 2) 삽입 위치 탐색
	NodeActor* cur = treeRoot;
	NodeActor* parent = nullptr;

	while (cur)
	{
		parent = cur;
		if (data < cur->GetData())
			cur = cur->GetLeft();
		else if (data > cur->GetData())
			cur = cur->GetRight();
		else
		{
			// 중복 키는 삽입하지 않음
			return;
		}
	}

	// 3) 새 노드 생성 및 연결
	NodeActor* node = new NodeActor(data, Vector2(0, 5), parent);
	if (data < parent->GetData())
		parent->SetLeft(node);
	else
		parent->SetRight(node);

	AddActor(node);


	LocateTree();
}

void AVLTreeSimulator::StartInsertNode(int data)
{
	// 빈 트리면 루트를 만든다.
	if (!treeRoot)
	{
		treeRoot = new NodeActor(data, Vector2(0, 5), nullptr);
		AddActor(treeRoot);

		LocateTree();
		return;
	}

	// 빈 트리가 아니면 노드 별로 비교하며 갈 곳을 찾아간다.

	Vector2 newPosition = treeRoot->Position();
	newPosition.y++;

	addActor = new NodeActor(data, newPosition);
	addActor->ColorChange(Color::RedIntensity);
	AddActor(addActor);
	curActor = treeRoot;

	timer.Reset();
	timer.SetTargetTime(1.0f);
	isInserting = true;
}

void AVLTreeSimulator::InsertNodeProcessing()
{
	// 추가하려는 데이터가 비교하고 있는 데이터보다 작다면 왼쪽으로
	if (addActor->GetData() < curActor->GetData())
	{
		// 자리가 비어있으면 그 자리에 들어감
		if (!curActor->GetLeft())
		{
			curActor->SetLeft(addActor);
			addActor->ColorChange(Color::White);
			addActor->SetParent(curActor);
			LocateTree();
			curActor = nullptr;
			isInserting = false;

			// 회전이 필요하면 회전 단계로 넘어감
			MarkFirstUnbalancedTriplet(addActor);

			return;
		}

		curActor = curActor->GetLeft();
		Vector2 newPosition = curActor->Position();
		newPosition.y++;
		addActor->SetPosition(newPosition);
	}
	// 추가하려는 데이터가 비교하고 있는 데이터보다 크다면 오른쪽으로
	else if (addActor->GetData() > curActor->GetData())
	{
		// 자리가 비어있으면 그 자리에 들어감
		if (!curActor->GetRight())
		{
			curActor->SetRight(addActor);
			addActor->ColorChange(Color::White);
			addActor->SetParent(curActor);
			LocateTree();
			curActor = nullptr;
			isInserting = false;

			// 회전이 필요하면 회전 단계로 넘어감
			MarkFirstUnbalancedTriplet(addActor);

			return;
		}
		curActor = curActor->GetRight();
		Vector2 newPosition = curActor->Position();
		newPosition.y++;
		addActor->SetPosition(newPosition);
	}
	// 데이터가 같다면 중복이므로 추가하지 않음
	else
	{
		addActor->Destroy();
		addActor = nullptr;
		curActor = nullptr;
		isInserting = false;
		return;
	}
}

void AVLTreeSimulator::MarkFirstUnbalancedTriplet(NodeActor* inserted)
{
	if (!inserted) return;

	// 삽입된 노드에서 위로 올라가며 첫 불균형 노드(z)를 찾음
	for (NodeActor* z = inserted->GetParent(); z; z = z->GetParent())
	{
		int lh = TreeHeight(z->GetLeft());
		int rh = TreeHeight(z->GetRight());
		int bf = lh - rh;

		if (std::abs(bf) > 1)
		{
			// z = 불균형 노드
			NodeActor* y = (inserted->GetData() < z->GetData()) ? z->GetLeft() : z->GetRight();
			NodeActor* x = nullptr;
			if (y)
				x = (inserted->GetData() < y->GetData()) ? y->GetLeft() : y->GetRight();

			z->ColorChange(Color::Yellow);            // 회전 기준 노드
			rotateNodeZ = z;
			if (y) y->ColorChange(Color::RedIntensity); // 자식
			rotateNodeY = y;
			if (x) x->ColorChange(Color::Red);          // 손자
			rotateNodeX = x;
			isRotating = true;
			return; // 첫 불균형만 처리하면 됨
		}
	}
}

void AVLTreeSimulator::ReplaceParent(NodeActor* oldNode, NodeActor* newNode)
{
	NodeActor* p = oldNode->GetParent();
	if (!p) {
		treeRoot = newNode;
		if (newNode) newNode->SetParent(nullptr);
		return;
	}

	if (p->GetLeft() == oldNode) p->SetLeft(newNode);
	else                         p->SetRight(newNode);

	if (newNode) newNode->SetParent(p);
}

void AVLTreeSimulator::RotateRight(NodeActor* z)
{
	if (!z) return;

	NodeActor* y = z->GetLeft();
	if (!y) return; // 회전 불가

	NodeActor* C = y->GetRight();

	// 부모 교체: z의 자리에 y를 올림
	ReplaceParent(z, y);

	// y → z
	y->SetRight(z);
	z->SetParent(y);

	// z → C
	z->SetLeft(C);
	if (C) C->SetParent(z);

	// 루트 갱신
	if (!y->GetParent())
		treeRoot = y;
}

void AVLTreeSimulator::RotateLeft(NodeActor* z)
{
	if (!z) return;

	NodeActor* y = z->GetRight();
	if (!y) return; // 회전 불가

	NodeActor* B = y->GetLeft();

	// 부모 교체: z의 자리에 y를 올림
	ReplaceParent(z, y);

	// y → z
	y->SetLeft(z);
	z->SetParent(y);

	// z → B
	z->SetRight(B);
	if (B) B->SetParent(z);

	// 루트 갱신
	if (!y->GetParent())
		treeRoot = y;
}

// z의 왼쪽-오른쪽(LR) 불균형: y(=z->left) 먼저 Left, 그 다음 z에서 Right
void AVLTreeSimulator::RotateLeftRight(NodeActor* z)
{
	if (!z || !z->GetLeft()) return;
	RotateLeft(z->GetLeft());   // y에서 좌회전
	RotateRight(z);             // z에서 우회전
}

// z의 오른쪽-왼쪽(RL) 불균형: y(=z->right) 먼저 Right, 그 다음 z에서 Left
void AVLTreeSimulator::RotateRightLeft(NodeActor* z)
{
	if (!z || !z->GetRight()) return;
	RotateRight(z->GetRight()); // y에서 우회전
	RotateLeft(z);              // z에서 좌회전
}

void AVLTreeSimulator::DoRotation()
{
	// z: 첫 불균형 노드, y: z의 삽입 경로 방향 자식, x: y의 삽입 경로 방향 손자
	if (!rotateNodeZ || !rotateNodeY || !rotateNodeX) return;

	bool yIsLeft = (rotateNodeZ->GetLeft() == rotateNodeY);
	bool xIsLeft = (rotateNodeY->GetLeft() == rotateNodeX);

	if (yIsLeft && xIsLeft) {
		// LL
		RotateRight(rotateNodeZ);
	}
	else if (!yIsLeft && !xIsLeft) {
		// RR
		RotateLeft(rotateNodeZ);
	}
	else if (yIsLeft && !xIsLeft) {
		// LR
		RotateLeftRight(rotateNodeZ);
	}
	else { // (!yIsLeft && xIsLeft)
		// RL
		RotateRightLeft(rotateNodeZ);
	}

	// 시각화 재배치
	LocateTree();

	rotateNodeX->ColorChange(Color::White);
	rotateNodeY->ColorChange(Color::White);
	rotateNodeZ->ColorChange(Color::White);

	// 포인터 초기화
	rotateNodeX = rotateNodeY = rotateNodeZ = nullptr;
}
