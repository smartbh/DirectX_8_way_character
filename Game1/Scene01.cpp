#include "stdafx.h"

void Scene01::Init()
{
	pl = new Player();
}

void Scene01::Release()
{
}

void Scene01::Update()
{
	pl->Update();
}

void Scene01::LateUpdate()
{
	
}

void Scene01::Render()
{
	pl->Render();
}

void Scene01::ResizeScreen()
{

}
