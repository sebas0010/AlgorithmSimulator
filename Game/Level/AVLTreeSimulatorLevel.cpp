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
	// 스페이스바 누르면 일시정지
	if (Input::Get().GetKeyDown(VK_SPACE))
	{
		pause = !pause;
	}

	if (pause) return;

	super::Tick(deltaTime);
	// esc 누르면 일시정지
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		dynamic_cast<Game*>(&Game::Get())->ToggleMenu();
	}

	// 노드가 삽입되는 과정
	if (isInserting)
	{
		// 입력시간 동안 deltaTime이 쌓이므로 한번 초기화
		if (isInserting == 1)
		{
			timer.Reset();
			timer.SetTargetTime(0.7f);
			isInserting++;
			return;
		}
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
		if (isRotating == 1)
		{
			timer.Reset();
			timer.SetTargetTime(2.0f);
			isRotating++;
			return;
		}
		timer.Tick(deltaTime);
		if(timer.IsTimeout()) DoRotation();
		return;
	}

	// 노드 삭제 과정
	if (isDeleting)
	{
		timer.Tick(deltaTime);
		if (isDeleting == 1)
		{
			timer.Reset();
			timer.SetTargetTime(1.0f);
			isDeleting++;
			return;
		}
		if (timer.IsTimeout())
		{
			if (isDeleting == 2)
			{
				DeleteNode();
				return;
			}

			if (!MarkUnbalancedFrom(rebalancingCur))
			{
				isDeleting = 0;
			}

		}
		return;
	}


	if (Input::Get().GetKeyDown('I') || Input::Get().GetKeyDown('i'))
	{
		// 모든 노드 흰색으로 초기화
		AllNodeWhite();

		// 숫자를 입력 받고 노드 추가
		GetInput(0);
	}

	if (Input::Get().GetKeyDown('a') || Input::Get().GetKeyDown('A'))
	{
		AllNodeWhite();
		StartInsertNode(12);
	}
	if (Input::Get().GetKeyDown('b') || Input::Get().GetKeyDown('B'))
	{
		AllNodeWhite();
		StartInsertNode(13);
	}
	if (Input::Get().GetKeyDown('c') || Input::Get().GetKeyDown('C'))
	{
		DeleteAllNodes();
	}
	if (Input::Get().GetKeyDown('d') || Input::Get().GetKeyDown('D'))
	{
		AllNodeWhite();
		// 숫자를 입력 받고 노드 삭제
		GetInput(1);
	}
}

void AVLTreeSimulator::Render()
{
	super::Render();

	if(pause) Game::Get().WriteToBuffer(Vector2::Zero, "Pause", Color::BlueIntensity);
	else Game::Get().WriteToBuffer(Vector2::Zero, "AVL Tree Simulator", Color::BlueIntensity);

	Game::Get().WriteToBuffer(Vector2(0, 3), "I : 노드 추가, D : 노드 삭제, C : 모든 노드 삭제", Color::White);

}

void AVLTreeSimulator::GetInput(int mode)
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
		if (mode == 0) StartInsertNode(data);
		else if (mode == 1) DeleteNode(data);
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

	isInserting++;
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
			addActor->SetParent(curActor);
			curActor = nullptr;
			isInserting = 0;

			// 회전이 필요하면 회전 단계로 넘어감
			if (MarkFirstUnbalancedTriplet(addActor))
			{
				Vector2 tmp = addActor->GetParent()->Position();
				tmp.x -= 4;
				if (tmp.x < 0) tmp.x = 0;
				tmp.y += 2;
				addActor->SetPosition(tmp);
			}
			else LocateTree();
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
			addActor->SetParent(curActor);
			curActor = nullptr;
			isInserting = 0;

			// 회전이 필요하면 회전 단계로 넘어감
			if (MarkFirstUnbalancedTriplet(addActor))
			{
				Vector2 tmp = addActor->GetParent()->Position();
				tmp.x += 4;
				tmp.y += 2;
				addActor->SetPosition(tmp);
			}
			else LocateTree();
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
		isInserting = 0;
		return;
	}
}

bool AVLTreeSimulator::MarkFirstUnbalancedTriplet(NodeActor* inserted)
{
	if (!inserted) return false;

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

			AllNodeWhite();
			z->ColorChange(Color::Yellow);            // 회전 기준 노드
			rotateNodeZ = z;
			if (y) y->ColorChange(Color::RedIntensity); // 자식
			rotateNodeY = y;
			if (x) x->ColorChange(Color::GreenIntensity);          // 손자
			rotateNodeX = x;

			// 회전 처리를 위한 타이머 설정
			isRotating++;
			timer.Reset();

			return true; // 첫 불균형만 처리하면 됨
		}
	}
	return false;
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
	// z: 첫 불균형 노드, y: z의 자식, x: y의 자식
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

	// 포인터 초기화
	rotateNodeX = rotateNodeY = rotateNodeZ = nullptr;
	isRotating = 0;
}

// 모든 노드 흰색으로
void AVLTreeSimulator::AllNodeWhite()
{
	AllNodeWhiteRecursive(treeRoot);
}

void AVLTreeSimulator::AllNodeWhiteRecursive(NodeActor* node)
{
	if (!node) return;

	node->ColorChange(Color::White);
	AllNodeWhiteRecursive(node->GetLeft());
	AllNodeWhiteRecursive(node->GetRight());
}

void AVLTreeSimulator::DeleteAllNodes()
{
	DeleteAllNodesRecursive(treeRoot);
	treeRoot = nullptr;
}

void AVLTreeSimulator::DeleteAllNodesRecursive(NodeActor* node)
{
	if (!node) return;

	DeleteAllNodesRecursive(node->GetLeft());
	DeleteAllNodesRecursive(node->GetRight());

	// 액터 삭제 예약 (엔진이 나중에 실제 제거)
	node->Destroy();
}

void AVLTreeSimulator::DeleteNode(int data)
{
	deleteNode = FindNode(data);
	if (!deleteNode) return;

	isDeleting++;
}

NodeActor* AVLTreeSimulator::FindNode(int data) const
{
	NodeActor* cur = treeRoot;
	while (cur)
	{
		if (data == cur->GetData())
		{
			cur->ColorChange(Color::RedIntensity);
			return cur; // 찾음
		}
		else if (data < cur->GetData())
		{
			cur = cur->GetLeft();
		}
		else
		{
			cur = cur->GetRight();
		}
	}
	return nullptr; // 없음
}

// 오른쪽 서브트리에서 최솟값(중위 후속자)
static NodeActor* MinNode(NodeActor* n) {
	while (n && n->GetLeft()) n = n->GetLeft();
	return n;
}

// 부모에서 oldChild 링크를 newChild로 교체
void AVLTreeSimulator::LinkFromParent(NodeActor* oldChild, NodeActor* newChild) {
	NodeActor* p = oldChild ? oldChild->GetParent() : nullptr;
	if (!p) {
		treeRoot = newChild;
		if (newChild) newChild->SetParent(nullptr);
		return;
	}
	if (p->GetLeft() == oldChild) p->SetLeft(newChild);
	else                          p->SetRight(newChild);
	if (newChild) newChild->SetParent(p);
}

// 삭제 후 위로 올라가며 AVL 리밸런싱 (여러 번 회전될 수 있음)
void AVLTreeSimulator::RebalanceUpwards(NodeActor* start) {
	for (NodeActor* z = start; z; z = z->GetParent()) {
		int lh = TreeHeight(z->GetLeft());
		int rh = TreeHeight(z->GetRight());
		int bf = lh - rh;

		if (bf > 1) {
			NodeActor* y = z->GetLeft();
			int ylh = TreeHeight(y ? y->GetLeft() : nullptr);
			int yrh = TreeHeight(y ? y->GetRight() : nullptr);
			if (ylh >= yrh) RotateRight(z);     // LL
			else             RotateLeftRight(z); // LR
		}
		else if (bf < -1) {
			NodeActor* y = z->GetRight();
			int ylh = TreeHeight(y ? y->GetLeft() : nullptr);
			int yrh = TreeHeight(y ? y->GetRight() : nullptr);
			if (yrh >= ylh) RotateLeft(z);      // RR
			else             RotateRightLeft(z); // RL
		}

		// 루트 안전 갱신
		while (treeRoot && treeRoot->GetParent()) treeRoot = treeRoot->GetParent();
	}
	// 회전(재배치)까지 끝난 뒤 좌표 갱신
	LocateTree();
	isDeleting = 0;
}

void AVLTreeSimulator::DeleteNode()
{
	if (!deleteNode) return;

	// 리밸런싱 시작 기준(삭제된 자리의 부모) 기억
	rebalancingCur = deleteNode->GetParent();

	// 1) 자식이 둘이면: 중위 후속자 s를 찾아 deleteNode와 값만 스왑 후, s(0/1자식)를 제거
	if (deleteNode->GetLeft() && deleteNode->GetRight()) {
		NodeActor* s = MinNode(deleteNode->GetRight());

		// --- 데이터 스왑 ---
		int tmp = deleteNode->GetData();
		deleteNode->SetData(s->GetData());
		s->SetData(tmp);

		// 실제로 제거할 노드를 후속자 s로 바꾸고, 그 부모를 리밸런스 시작점으로
		deleteNode = s;
		rebalancingCur = deleteNode->GetParent();
	}

	// 2) 여기로 내려오면 deleteNode는 자식이 0개 또는 1개
	NodeActor* child = deleteNode->GetLeft() ? deleteNode->GetLeft() : deleteNode->GetRight();

	// 부모 링크 교체
	LinkFromParent(deleteNode, child);

	// 노드 제거
	deleteNode->SetLeft(nullptr);
	deleteNode->SetRight(nullptr);
	deleteNode->SetParent(nullptr);
	deleteNode->Destroy();

	// 3) 위로 올라가며 리밸런싱
	if (rebalancingCur)
	{
		if (MarkUnbalancedFrom(rebalancingCur)) isDeleting++;
		else
		{
			LocateTree();
			isDeleting = 0;
		}
	}
	else {
		// 트리가 비었거나 루트만 남은 케이스
		LocateTree();
		isDeleting = 0;
	}
}

bool AVLTreeSimulator::MarkUnbalancedFrom(NodeActor* start)
{
	for (NodeActor* z = start; z; z = z->GetParent())
	{
		int lh = TreeHeight(z->GetLeft());
		int rh = TreeHeight(z->GetRight());
		int bf = lh - rh;

		if (std::abs(bf) > 1)
		{
			NodeActor* y = (bf > 1) ? z->GetLeft() : z->GetRight(); // 더 무거운 쪽
			NodeActor* x = nullptr;
			if (y) {
				int ylh = TreeHeight(y->GetLeft());
				int yrh = TreeHeight(y->GetRight());
				// y의 더 무거운 쪽 손자 선택
				x = (ylh >= yrh) ? y->GetLeft() : y->GetRight();
			}

			AllNodeWhite();
			z->ColorChange(Color::Yellow);
			rotateNodeZ = z;
			if (y) { y->ColorChange(Color::RedIntensity); rotateNodeY = y; }
			if (x) { x->ColorChange(Color::GreenIntensity); rotateNodeX = x; }

			rebalancingCur = z->GetParent();
			isRotating++;
			timer.Reset();
			return true;
		}
	}
	return false;
}