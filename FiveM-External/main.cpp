#include "Framework\Overlay\Overlay.h"
#include "Framework\Memory\Memory.h"
#include "Cheat\Cheat.h"
#include <thread>

Cheat* fivem = new Cheat;
Overlay* overlay = new Overlay;

// Render thread
void Overlay::OverlayUserFunction()
{
	fivem->Misc();
	fivem->RenderInfo();

	if (g.ShowMenu)
		fivem->RenderMenu();

	if (g.ESP)
		fivem->RenderESP();
}

// Debug時のみコンソールあり
#if _DEBUG
int main()
#else 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
	// Fix DPI Scale
	SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);

	// Memory
	if (!m.AttachProcess("grcWindow", WINDOW_CLASS))
		return 1;

	// Overlay
	if (!overlay->InitOverlay("grcWindow", WINDOW_CLASS))
		return 2;

	// Offset
	if (!Game->InitOffset())
		return 3;

	// Create some threads
	g.process_active = true;
	std::thread([&]() { fivem->UpdateList(); }).detach();
	
	overlay->OverlayLoop();
	overlay->DestroyOverlay();
	m.DetachProcess();
	delete fivem, overlay, Game;

	return 0;
}