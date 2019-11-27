#pragma once
#include <thread>
#include <atomic> 
#include "../Utilities/ConcurrentQueue.h"
#include "../Renderer/Utilities/Color.h"

struct RenderCommandClearParams
{
	Color Color;
};

struct RenderCommandPutPixelParams
{
	int X;
	int Y;
	Color Color;
};

struct RenderCommandRainParams
{
	int* HeightDiff;
	Color* Background;
	bool ForcePaint;
};

/*
	Todos los campos están en la misma región de memoria
	No se pueden usar tipos con constructoras (clases)
*/
union RenderCommandParam
{
	RenderCommandClearParams ClearParams;
	RenderCommandPutPixelParams PutPixelParams;
	RenderCommandRainParams RainParams;
};

enum RendererCommandType {
	CLEAR,				//Vacia el frameBuffer correspondiente. //TODO: No se usa en esta practica (SI en pruebas)
	END_FRAME,			//Hace el present
	PUT_PIXEL,			//TODO: Solo para hacer pruebas (x,y,c)
	RENDER_RAIN_EFFECT	//Escribeme este estado de lluvia
};

struct RenderCommand {
	RendererCommandType Type;
	RenderCommandParam Param;		//Parametros adicionales
};

/*
	Cuando se crea el objeto no se lanza automaticamente la hebra
*/
class RendererThread
{
private:
	std::thread *t;
	std::atomic <bool> quitRequested;	//Escritura y lectura seguras
	std::atomic <int> pendingFrames;	//Número de comandos de endframe encolados actualmente
	Queue<RenderCommand> commandQueue; 	//Cola concurrente (compartida entre hebras) de comandos

public:
	/*
		Inicia la hebra
	*/
	void Start();

	/*
		Para la hebra
	*/
	void Stop();

	/*
		Es llamado desde la hebra de logica
	*/
	void EnqueueCommand(RenderCommand renderCommand);

	/*
		Devuelve cuantos comandos de endframe hay encolados actualmente
	*/
	int GetPendingFrames() { return pendingFrames; };

private:
	void RenderLoop();

	void DrawRain(Color* background, int* heightDiffs, bool forcePaint);

	/*
		Restringe un valor al rango
	*/
	int Clamp(int min, int max, int value);
};

