#include "RendererThread.h"

using namespace std;

void RendererThread::Start() {

	//Recibe la funcion a ejecutar
	//t = new thread(RenderLoop);//TODO: Pasar argumentos?
	//if (t != nullptr)
	//	Stop();

	quitRequested = false;
}

void RendererThread::Stop() {
	if (t != nullptr) {
		quitRequested = true;

		//Esperamos a que acabe.
		t->join();

		delete t;
		t = nullptr;
	}
}

void RendererThread::RenderLoop() {
	while (!quitRequested)
	{
		//Pinta siguiente frame

	}
}
