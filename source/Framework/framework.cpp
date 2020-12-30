#include "Framework/framework.h"
#include "nds.h"

using namespace fw;

Framework* Framework::instance = nullptr;

void Framework::VBlank()
{
	instance->frame++;
}

void Framework::Init()
{
	if (instance == nullptr)
		instance = this;
	else exit(1); // only one instance allowed

	shouldRun = true;
	irqSet(IRQ_VBLANK, Framework::VBlank);
}

void Framework::Update()
{

}

void Framework::LateUpdate()
{

}

void Framework::Render()
{

}

int Framework::Run()
{
	Init();

	while(shouldRun)
	{
		Update();
		LateUpdate();
		Render();
		swiWaitForVBlank();
	}

	return 0;
}

void Framework::Stop()
{
	shouldRun = false;
}

int Framework::GetFrame()
{
	return frame;
}