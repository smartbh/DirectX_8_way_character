#include "stdafx.h"

Player::Player()
{
	col = new ObRect();
	col->scale = Vector2(96.0f, 96.0f) / 2.0f;
	col->SetWorldPos(Vector2(-400.0f, -400.0f));
	col->isFilled = false;

	walk = new ObImage(L"Walk.png");
	walk->scale = Vector2(96.0f, 96.0f) / 2.0f;
	walk->maxFrame = Int2(6, 8);
	walk->SetParentRT(*col);

	roll = new ObImage(L"Roll.png");
	roll->scale = Vector2(96.0f, 96.0f) / 2.0f;
	roll->maxFrame = Int2(6, 8);
	roll->SetParentRT(*col);
	roll->visible = false;

	frameY[Dir_R] = 0;
	frameY[Dir_T] = 1;
	frameY[Dir_L] = 2;
	frameY[Dir_B] = 3;
	frameY[Dir_RB] = 4;
	frameY[Dir_LB] = 5;
	frameY[Dir_LT] = 6;
	frameY[Dir_RT] = 7;

	plState = PlayerState::IDLE;
}

Player::~Player()
{
	SafeDelete(col);
	SafeDelete(walk);
	SafeDelete(roll);
}

void Player::Update(ObCircle* _player, ObRect* _boss)
{
	switch (plState)
	{
	case PlayerState::IDLE:
		Idle(_player);
		break;
	case PlayerState::WALK:
		Walk(_player);
		break;
	case PlayerState::ROLL:
		Roll();
		break;
	case PlayerState::HIT:
		Hit(_boss);
		break;
	default:
		break;
	}

	col->Update();
	walk->Update();
	roll->Update();

	cout << col->GetWorldPos().x << endl;
	cout << col->GetWorldPos().y << endl;
}

void Player::Render()
{
	col->Render();
	walk->Render();
	roll->Render();
}

void Player::Idle(ObCircle* _boss)
{	
	Input();
	LookTarget(INPUT->GetMouseWorldPos(), walk);
	
	//Idle->Walk
	if (moveDir != Vector2(0.0f, 0.0f))
	{
		plState = PlayerState::WALK;
		walk->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
	}
	if (col->Intersect(_boss))
	{
		plState = PlayerState::HIT;
		rollTime = 0.0f;
	}
}

void Player::Walk(ObCircle* _boss)
{
	Input();
	LookTarget(INPUT->GetMouseWorldPos(), walk);

	col->MoveWorldPos(moveDir * 200.0f * DELTA);

	//Walk -> Idle
	if (moveDir == Vector2(0.0f, 0.0f))
	{
		plState = PlayerState::IDLE;
		walk->ChangeAnim(ANIMSTATE::STOP, 0.1f);
		walk->frame.x = 0;
	}

	//Walk -> Roll
	if (INPUT->KeyPress(VK_SPACE))
	{
		plState = PlayerState::ROLL;
		roll->ChangeAnim(ANIMSTATE::ONCE, 0.1f);
		walk->visible = false;
		roll->visible = true;
		rollTime = 0.0f;
	}

	if (col->Intersect(_boss))
	{
		plState = PlayerState::HIT;
		rollTime = 0.0f;
	}
}

void Player::Roll()
{
	rollTime += DELTA;

	Input();
	LookTarget(col->GetWorldPos() + moveDir, roll);

	col->MoveWorldPos(moveDir * 500.0f *
		cosf(rollTime / 0.6f * DIV2PI) * DELTA);
	//			0 ~ 1		   0 ~ 90 <<정규화란 의미

	//Roll -> Walk
	if (rollTime > 0.6f)
	{
		plState = PlayerState::WALK;
		walk->visible = true;
		roll->visible = false;
	}
}

void Player::Hit(ObRect* _boss)
{
	rollTime += DELTA;
	if (rollTime > 0.3f) //moveDir
	{
		walk->color = Color(RANDOM->Float(0.5f, 0.8f), 0.5f, 0.5f, 0.5f);
		col->MoveWorldPos(-moveDir * 500.0f *
			cosf(rollTime / 0.6f * DIV2PI) * DELTA);
		//			0 ~ 1		   0 ~ 90 <<정규화란 의미
		
	}



	//Roll -> Walk
	if (rollTime > 0.6f)
	{

		if (moveDir != Vector2(0.0f, 0.0f))
		{
			walk->color = Color(0.5f, 0.5f, 0.5f, 0.5f);
			plState = PlayerState::WALK;
			walk->ChangeAnim(ANIMSTATE::LOOP, 0.1f);
		}
		else
		{
			walk->color = Color(0.5f, 0.5f, 0.5f, 0.5f);
			plState = PlayerState::IDLE;
			walk->ChangeAnim(ANIMSTATE::STOP, 0.1f);
			walk->frame.x = 0;
		}
	}


}

void Player::Input()
{
	moveDir = Vector2(0.0f, 0.0f);

	if (INPUT->KeyPress('S'))
	{
		moveDir.y = -1.0f;
	}
	else if (INPUT->KeyPress('W'))
	{
		moveDir.y = 1.0f;
	}

	if (INPUT->KeyPress('A'))
	{
		moveDir.x = -1.0f;
	}
	else if (INPUT->KeyPress('D'))
	{
		moveDir.x = 1.0f;
	}
	
	moveDir.Normalize();
}

void Player::LookTarget(Vector2 target, ObImage* img)
{
	Vector2 dir = target - col->GetWorldPos();
	float radian = Utility::DirToRadian(dir);

	//오른쪽
	if (-DIV8PI <= radian && radian <= DIV8PI)
	{
		dirState = Dir_R;
	}
	else if (DIV8PI <= radian && radian <= DIV4PI + DIV8PI)
	{
		dirState = Dir_RT;
	}
	else if (DIV4PI + DIV8PI <= radian && radian <= DIV8PI + DIV4PI * 2.0f)
	{
		dirState = Dir_T;
	}
	else if (DIV8PI + DIV4PI * 2.0f <= radian && radian <= DIV8PI + DIV4PI * 3.0f)
	{
		dirState = Dir_LT;
	}
	else if ( -(DIV8PI + DIV4PI * 2.0f) >= radian && radian >= -(DIV8PI + DIV4PI * 3.0f))
	{
		dirState = Dir_LB;
	}
	else if (-(DIV8PI + DIV4PI) >= radian && radian >= -(DIV8PI + DIV4PI * 2.0f))
	{
		dirState = Dir_B;
	}
	else if (-(DIV8PI) >= radian && radian >= -(DIV8PI + DIV4PI))
	{
		dirState = Dir_RB;
	}
	else
	{
		dirState = Dir_L;
	}

	img->frame.y = frameY[dirState];
}

ObRect* Player::getCol()
{
	return col;
}
