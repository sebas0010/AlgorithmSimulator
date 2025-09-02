#include "AVLTreeSimulatorLevel2.h"
#include "Game/Game.h"

AVLTreeSimulator2::AVLTreeSimulator2()
{
	// 일시정지 레벨 생성
	dynamic_cast<Game*>(&Game::Get())->CreatePauseLevel();
}

AVLTreeSimulator2::~AVLTreeSimulator2()
{

}

void AVLTreeSimulator2::Tick(float deltaTime)
{
	// 입력 중
	if (isInputting)
	{
		for (int i = 0; i < 10; i++)
		{
			if (Input::Get().GetKeyDown(0x30 + i))
			{
				// 두 자리수 제한
				if (inputValue > 10) return;
				inputValue = inputValue * 10 + i;
				return;
			}
			// esc 키를 누르면 취소
			if (Input::Get().GetKeyDown(VK_ESCAPE))
			{
				inputValue = 0;
				isInputting = false;
				return;
			}
			// backspace 키를 누르면 한글자 지움
			if (Input::Get().GetKeyDown(VK_BACK))
			{
				inputValue /= 10;
				return;
			}
			// 엔터 키를 누르면 입력 끝
			if (Input::Get().GetKeyDown(VK_RETURN))
			{
				if (inputValue == 0)
				{
					isInputting = false;
					inputValue = 0;
					return;
				}

				// 입력 완료
				if (mode) InsertStart(inputValue);
				else DeleteStart(inputValue);
				isInputting = false;
				inputValue = 0;
				return;
			}
		}
	}

	// 스페이스바 누르면 일시정지
	if (Input::Get().GetKeyDown(VK_SPACE))
	{
		pause = !pause;
	}

	if (pause) return;

	super::Tick(deltaTime);
	// esc 누르면 일시정지메뉴로 이동
	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		dynamic_cast<Game*>(&Game::Get())->ToggleMenu();
	}

	// 노드 삽입중
	if(isInserting)
	{
		if (isInserting == 1)
		{
			InsertProcess();
		}
		else if (isInserting == 2)
		{
			InsertPost();
			return;
		}
		else if (isInserting == 3)
		{
			// LL
			RotateRight(z);
			if (rotateLevel == 2)
			{
				isInserting = 0;

				z->ChangeColor(Color::White);
				y->ChangeColor(Color::White);
				x->ChangeColor(Color::White);
			}
		}
		else if (isInserting == 4)
		{
			if (rotateLevel == 0 || rotateLevel == 1)
			{
				// LR z의 왼쪽 자손 기준 기준 RotateLeft, z 기준 RotateRight
				if (z && z->GetLeft()) RotateLeft(z->GetLeft());
			}
			else if (rotateLevel == 2)
			{
				isInserting = 3;
				rotateLevel = 0;
				return;
			}
		}
		else if (isInserting == 5)
		{
			if (rotateLevel == 0 || rotateLevel == 1)
			{
				// RL z의 오른쪽 RotateRight, z 기준 RotateLeft
				if (z && z->GetRight()) RotateRight(z->GetRight());
			}
			else if (rotateLevel == 2)
			{
				rotateLevel = 0;
				isInserting = 6;
				return;
			}
		}
		else if (isInserting == 6)
		{
			// RR
			RotateLeft(z);
			if (rotateLevel == 2) {
				isInserting = 0;

				z->ChangeColor(Color::White);
				y->ChangeColor(Color::White);
				x->ChangeColor(Color::White);
			}
		}
		return;
	}

	if (isDeleting)
	{
		// 노드 안보이게 하고 깜빡임 신호
		if (isDeleting == 1)
		{
			DeleteProcess();
			return;
		}
		// 노드 깜빡임 멈추기 기다렸다가 다음단계로 이동
		else if (isDeleting == 2)
		{
			if (!deletingNode->visible) isDeleting++;
			return;
		}
		else if (isDeleting == 3)
		{
			// 자식 노드가 없는 경우
			if (!deletingNode->GetLeft() && !deletingNode->GetRight())
			{
				ReplaceChild(deletingNode->GetParent(), deletingNode, nullptr);
				z = deletingNode->GetParent();
				deletingNode->Destroy();
				deletingNode = nullptr;
				isDeleting = 5;
				return;
			}
			// 자식 노드가 1개인 경우
			else if (!deletingNode->GetLeft() || !deletingNode->GetRight())
			{
				AVLNode* child = deletingNode->GetLeft() ? deletingNode->GetLeft() : deletingNode->GetRight();
				ReplaceChild(deletingNode->GetParent(), deletingNode, child);
				MoveNode(child, deletingNode->Position());
				z = child->GetParent();
				deletingNode->Destroy();
				deletingNode = nullptr;
				isDeleting = 4;
				return;
			}
			// 자식 노드가 2개인 경우
			else
			{
				AVLNode* succ = deletingNode->GetRight();

				while (succ->GetLeft()) succ = succ->GetLeft();

				AVLNode* parent = deletingNode->GetParent();
				AVLNode* left = deletingNode->GetLeft();
				AVLNode* right = deletingNode->GetRight();
				AVLNode* succParent = succ->GetParent();
				AVLNode* succRight = succ->GetRight();

				if (right == succ) {
					// succ이 deletingNode의 '직접 오른쪽 자식' ──
					ReplaceChild(parent, deletingNode, succ);

					succ->SetLeft(left);
					if (left) left->SetParent(succ);

					deletingNode->SetRight(succRight);
					if (succRight) succRight->SetParent(deletingNode);
					succ->SetRight(deletingNode); deletingNode->SetParent(succ);

					deletingNode->SetLeft(nullptr);
				}
				else {
					// succ이 오른쪽 서브트리 더 아래(왼쪽 사슬) ──
					ReplaceChild(succParent, succ, deletingNode);   // succ 자리에는 deletingNode가 올라감
					deletingNode->SetLeft(nullptr);                 // succ 자리는 왼쪽 없음
					deletingNode->SetRight(succRight);
					if (succRight) succRight->SetParent(deletingNode);

					ReplaceChild(parent, deletingNode, succ);       // deletingNode 자리에는 succ이 올라감
					succ->SetLeft(left);
					if (left) left->SetParent(succ);
					succ->SetRight(right);
					if (right) right->SetParent(succ);
				}


				// deletingNode를 트리에서 먼저 분리한 뒤 파괴
				AVLNode* parentDel = deletingNode->GetParent();
				AVLNode* childDel = deletingNode->GetRight(); // 후계자 자리 특성상 왼쪽은 없음
				ReplaceChild(parentDel, deletingNode, childDel);

				MoveNode(root, rootPosition);

				z = parentDel; // 불균형 탐색 시작점

				deletingNode->Destroy();
				deletingNode = nullptr;

				isDeleting = 4;
				return;
			}
		}
		else if (isDeleting == 4)
		{
			if (IsAllNodeStop())
			{
				// 불균형 검사로 이동
				isDeleting = 5;
				return;
			}
		}
		else if (isDeleting == 5)
		{
			// 불균형 검사
			while (z && std::abs(BalanceFactor(z)) <= 1) {
				z = z->GetParent();
			}

			if (!z) {
				// 불균형 노드 없음
				isDeleting = 0;
				z = nullptr;
				y = nullptr;
				x = nullptr;
				return;
			}

			y = (BalanceFactor(z) > 0) ? z->GetLeft() : z->GetRight();

			if (y)
			{
				x = (BalanceFactor(y) > 0) ? y->GetLeft() : y->GetRight();
			}
			else
			{
				x = nullptr;
			}
			
			if (z) z->ChangeColor(Color::RedIntensity);
			if (y) y->ChangeColor(Color::GreenIntensity);
			if (x) x->ChangeColor(Color::BlueIntensity);

			// 회전 단계로 이동
			isDeleting = 6;
			return;
		}
		else if (isDeleting == 6)
		{
			// 불균형 형태 검사
			isDeleting += DetectCase() + 1;
		}
		else if (isDeleting == 7)
		{
			// LL
			RotateRight(z);
			if (rotateLevel == 2)
			{
				z->ChangeColor(Color::White);
				y->ChangeColor(Color::White);
				x->ChangeColor(Color::White);

				// 불균형 탐색으로 돌아감
				isDeleting = 5;
			}
		}
		else if (isDeleting == 8)
		{
			// LR
			if (rotateLevel == 0 || rotateLevel == 1)
			{
				// LR z의 왼쪽 자손 기준 기준 RotateLeft, z 기준 RotateRight
				if (z && z->GetLeft()) RotateLeft(z->GetLeft());
			}
			else if (rotateLevel == 2)
			{
				isDeleting = 7;
				rotateLevel = 0;
				return;
			}
		}
		else if (isDeleting == 9)
		{
			// RL
			if (rotateLevel == 0 || rotateLevel == 1)
			{
				// RL z의 오른쪽 RotateRight, z 기준 RotateLeft
				if (z && z->GetRight()) RotateRight(z->GetRight());
			}
			else if (rotateLevel == 2)
			{
				rotateLevel = 0;
				isDeleting = 10;
				return;
			}
		}
		else if (isDeleting == 10)
		{
			// RR
			RotateLeft(z);
			if (rotateLevel == 2) {
				z->ChangeColor(Color::White);
				y->ChangeColor(Color::White);
				x->ChangeColor(Color::White);

				// 불균형 탐색으로 돌아감
				isDeleting = 5;
			}
		}
	}

	if (allClear)
	{
		if (!root) { allClear = false; return; }      // 널 가드
		if (root->visible) return;                    // 깜빡임 끝날 때까지 대기

		DeleteAllRecursive(root);
		root = nullptr;                               
		z = y = x = deletingNode = insertingNode = comparingNode = nullptr;
		isInserting = 0; isDeleting = 0;
		allClear = false;
		return;
	}

	// i 키 누르면 노드 삽입을 위한 입력을 받음
	if (Input::Get().GetKeyDown('i') || Input::Get().GetKeyDown('I'))
	{
		if (nodeCount == 31)
		{
			return;
		}
		isInputting = true;
		mode = true;
	}
	// d 키 누르면 노드 삭제를 위한 입력을 받음
	if (Input::Get().GetKeyDown('d') || Input::Get().GetKeyDown('D'))
	{
		if (nodeCount == 0)
		{
			return;
		}
		isInputting = true;
		mode = false;
	}
	// c 키 누르면 모든 노드 삭제
	if (Input::Get().GetKeyDown('c') || Input::Get().GetKeyDown('C'))
	{
		if (nodeCount == 0)
		{
			return;
		}
		nodeCount = 0;
		DeleteAll();
	}
}

void AVLTreeSimulator2::Render()
{
	super::Render();

	if (pause) Game::Get().WriteToBuffer(Vector2::Zero, "Pause", Color::RedIntensity);
	else if(isInputting) Game::Get().WriteToBuffer(Vector2::Zero, "Inputting", Color::BlueIntensity);
	else if(isInserting) Game::Get().WriteToBuffer(Vector2::Zero, "Inserting", Color::RedIntensity);
	else if(isDeleting) Game::Get().WriteToBuffer(Vector2::Zero, "Deleting", Color::RedIntensity);
	else if(allClear) Game::Get().WriteToBuffer(Vector2::Zero, "Clearing", Color::RedIntensity);
	else Game::Get().WriteToBuffer(Vector2::Zero, "AVL Tree Simulator 2", Color::BlueIntensity);

	Game::Get().WriteToBuffer(Vector2(0, 3), "I : 노드 추가, D : 노드 삭제, C : 모든 노드 삭제", Color::White);

	if (isInputting)
	{
		Vector2 drawPosition(0, 5);
		if (inputValue != 0)
		{
			int num1 = inputValue / 10;
			int num2 = inputValue % 10;

			if (num1 != 0)
			{
				for (int i = 0; i < 5; i++)
				{
					Game::Get().WriteToBuffer(drawPosition, bigNumber[num1][i].c_str(), Color::White);
					drawPosition.y++;
				}
				drawPosition.y -= 5;
				drawPosition.x += num1 == 1 ? 2 : 6;
			}
			if (inputValue != 0)
			{
				for (int i = 0; i < 5; i++)
				{
					Game::Get().WriteToBuffer(drawPosition, bigNumber[num2][i].c_str(), Color::White);
					drawPosition.y++;
				}
			}
		}
	}
}

void AVLTreeSimulator2::InsertStart(int data)
{
	insertingNode = new AVLNode(data, spawnSpot);
	AddActor(insertingNode);
	if (root != nullptr) comparingNode = root;
	locationLevel = 0;
	isInserting++;
}

void AVLTreeSimulator2::InsertProcess()
{
	// 트리가 비어있으면 루트노드로 설정
	if (root == nullptr)
	{
		root = insertingNode;
		root->MoveStart(rootPosition, 100.0f);
		isInserting = 0;
		nodeCount++;
		insertingNode->ChangeColor(Color::White);
		return;
	}

	if (insertingNode->IsMoving())
	{
		return;
	}

	// 삽입이 종료되면 삽입 단계 종료
	if (comparingNode == nullptr)
	{
		isInserting++;
		nodeCount++;
		insertingNode->ChangeColor(Color::White);
		return;
	}
	
	// 비교하고 있는 노드보다 작다면 왼쪽으로
	if (comparingNode->GetData() > insertingNode->GetData())
	{
		if (comparingNode->GetLeft() == nullptr)
		{
			comparingNode->SetLeft(insertingNode);
			insertingNode->SetParent(comparingNode);
			insertingNode->MoveStart(Vector2(comparingNode->Position().x - (8 * (1 << (4 - locationLevel))), comparingNode->Position().y + 30), 130.0f);
			comparingNode = nullptr;
		}
		else
		{
			comparingNode = comparingNode->GetLeft();
			insertingNode->MoveStart(Vector2(comparingNode->Position().x, comparingNode->Position().y + 15), 130.0f);
		}
	}
	// 비교하고 있는 노드보다 크다면 오른쪽으로
	else if (comparingNode->GetData() < insertingNode->GetData())
	{
		if (comparingNode->GetRight() == nullptr)
		{
			comparingNode->SetRight(insertingNode);
			insertingNode->SetParent(comparingNode);
			insertingNode->MoveStart(Vector2(comparingNode->Position().x + (8 * (1 << (4 - locationLevel))), comparingNode->Position().y + 30), 130.0f);
			comparingNode = nullptr;
		}
		else
		{
			comparingNode = comparingNode->GetRight();
			insertingNode->MoveStart(Vector2(comparingNode->Position().x, comparingNode->Position().y + 15), 130.0f);
		}
	}
	// 비교하고 있는 노드와 같다면 취소
	else
	{
		insertingNode->Destroy();
		isInserting = 0;
		comparingNode = nullptr;
		return;
	}
	locationLevel++;
}

void AVLTreeSimulator2::InsertPost()
{
	// 불균형 노드 탐색 후 회전 대상 x, y, z에 저장
	for (z = insertingNode->GetParent(); z; z = z->GetParent())
	{
		if (std::abs(BalanceFactor(z)) > 1)
		{
			// z = 불균형 노드
			y = (insertingNode->GetData() < z->GetData()) ? z->GetLeft() : z->GetRight();
			x = nullptr;
			if (y) x = (insertingNode->GetData() < y->GetData()) ? y->GetLeft() : y->GetRight();
			
			z->ChangeColor(Color::RedIntensity);
			y->ChangeColor(Color::GreenIntensity);
			x->ChangeColor(Color::BlueIntensity);

			isInserting = 3 + DetectCase();
			return;
		}
	}
	isInserting = 0;
}

int AVLTreeSimulator2::DetectCase()
{
	const bool yIsLeft = (z->GetLeft() == y);
	const bool xIsLeft = (y->GetLeft() == x);
	const bool yIsRight = (z->GetRight() == y);
	const bool xIsRight = (y->GetRight() == x);

	rotateLevel = 0;

	if (yIsLeft && xIsLeft)
	{
		// LL
		return 0;
	}
	if (yIsLeft && xIsRight)
	{
		// LR
		return 1;
	}
	if (yIsRight && xIsLeft)
	{
		// RL
		return 2;
	}
	if (yIsRight && xIsRight)
	{
		// RR
		return 3;
	}
}

void AVLTreeSimulator2::ReplaceChild(AVLNode* parent, AVLNode* oldChild, AVLNode* newChild)
{
	if (!parent) { // oldChild 가 루트였던 경우
		root = newChild;
		if (newChild) newChild->SetParent(nullptr);
		return;
	}
	if (parent->GetLeft() == oldChild) {
		parent->SetLeft(newChild);
	}
	else if (parent->GetRight() == oldChild) {
		parent->SetRight(newChild);
	}
	if (newChild) newChild->SetParent(parent);
}

// 구조를 먼저 바꾸고 노드를 움직여야 올바른 자리로 감
void AVLTreeSimulator2::MoveNode(AVLNode* targetNode, Vector2 position)
{
	int level = 0;
	AVLNode* tmp = targetNode;
	while (tmp != root)
	{
		tmp = tmp->GetParent();
		level++;
	}
	targetNode->MoveStart(position, 80.0f);
	if (targetNode->GetLeft()) MoveNode(targetNode->GetLeft(), Vector2(position.x - (8 * (1 << (4 - level))), position.y + 30));
	if (targetNode->GetRight()) MoveNode(targetNode->GetRight(), Vector2(position.x + (8 * (1 << (4 - level))), position.y + 30));
}

void AVLTreeSimulator2::RotateRight(AVLNode* a)
{
	if (rotateLevel == 0)
	{
		b = a->GetLeft();
		c = b ? b->GetRight() : nullptr;

		ReplaceChild(a->GetParent(), a, b);

		// a를 b의 오른쪽 자식으로
		if (b) b->SetRight(a);
		a->SetParent(b);

		// c를 a의 왼쪽으로
		a->SetLeft(c);
		if (c) c->SetParent(a);


		MoveNode(b, a->Position());
		rotateLevel = 1;
	}
	else if (rotateLevel == 1)
	{
		if (IsAllNodeStop()) rotateLevel = 2;
	}
}

void AVLTreeSimulator2::RotateLeft(AVLNode* a)
{
	if (rotateLevel == 0)
	{
		b = a->GetRight();             // 새 축
		c = b ? b->GetLeft() : nullptr;

		ReplaceChild(a->GetParent(), a, b);

		// a를 b의 왼쪽 자식으로
		if (b) b->SetLeft(a);
		a->SetParent(b);

		// c를 a의 오른쪽으로
		a->SetRight(c);
		if (c) c->SetParent(a);


		MoveNode(b, a->Position());
		rotateLevel = 1;
	}
	else if (rotateLevel == 1)
	{
		if (IsAllNodeStop()) rotateLevel = 2;
	}
}

bool AVLTreeSimulator2::IsAllNodeStop()
{
	return IsAllNodeStopRecursive(root);
}

bool AVLTreeSimulator2::IsAllNodeStopRecursive(AVLNode* node)
{
	if (!node) return true;

	// 현재 노드가 움직이는 중이면 바로 false
	if (node->IsMoving()) return false;

	// 왼쪽, 오른쪽 모두 정지 상태여야 true
	return IsAllNodeStopRecursive(node->GetLeft()) &&
		IsAllNodeStopRecursive(node->GetRight());
}

// 삭제할 노드를 찾고 없다면 함수 종료, 있다면 삭제
void AVLTreeSimulator2::DeleteStart(int data)
{
	deletingNode = FindNode(data);
	if (!deletingNode) return;
	nodeCount--;
	isDeleting++;
}

void AVLTreeSimulator2::DeleteProcess()
{
	deletingNode->Blink();
	isDeleting++;
}

void AVLTreeSimulator2::DeleteAll()
{
	BlinkAll(root);
	allClear = true;
}

void AVLTreeSimulator2::DeleteAllRecursive(AVLNode* node)
{
	if (!node) return;
	DeleteAllRecursive(node->GetLeft());
	DeleteAllRecursive(node->GetRight());
	node->Destroy();
}

void AVLTreeSimulator2::BlinkAll(AVLNode* node)
{
	if (!node) return;
	BlinkAll(node->GetLeft());
	BlinkAll(node->GetRight());
	node->Blink();
}

AVLNode* AVLTreeSimulator2::FindNode(int key)
{
	AVLNode* cur = root;
	while (cur) {
		if (key < cur->GetData()) cur = cur->GetLeft();
		else if (key > cur->GetData()) cur = cur->GetRight();
		else return cur;
	}
	return nullptr;
}
