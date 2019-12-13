#pragma once
#include <thread>
#include <atomic> 
#include "../Utilities/ConcurrentQueue.h"
#include "Color.h"

class Sprite;

//Parámetros de los diferentes comandos
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
	int* HeightDiff;		//Matriz de enteros que se utiliza para el pintado de la lluvia
	Color* Background;		//Matriz de colores con la imagen de fondo
	bool ForcePaint;		//Indica si hay que forzar el pintado de toda la pantalla
};

struct RenderCommandDrawSpriteParams
{
	Sprite* Sprite;
	int PosX;
	int PosY; 
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
	RenderCommandDrawSpriteParams DrawSpriteParams;
};

enum RendererCommandType
{
	CLEAR,				//Borra la pantalla
	PUT_PIXEL,			//Sirve para hacer pruebas (x,y,c)
	END_FRAME,			//Hace el present
	RENDER_RAIN_EFFECT,	//Pinta el resultado de la simulación de la lluvia
	DRAW_SPRITE			//Pinta una seccion de una imagen en la posición especificada
};

struct RenderCommand
{
	RendererCommandType Type;
	RenderCommandParam Param;		//Parametros adicionales
};

/*
	Cuando se crea el objeto no se lanza automaticamente la hebra
	Hebra de Render
*/
class RendererThread
{
private:
	static std::thread *t;						//Hilo
	static Queue<RenderCommand> commandQueue; 	//Cola concurrente (compartida entre hebras) de comandos

	//Escritura y lectura seguras
	static std::atomic <bool> quitRequested;	//Indica si se quiere parar la aplicación
	static std::atomic <int> pendingFrames;	//Número de comandos de endframe encolados actualmente

public:
	/*
		Inicia la hebra
	*/
	static void Start();

	/*
		Para la hebra
	*/
	static void Stop();

	/*
		Encola un comando para que lo procese la hebra de render.
		Es llamado desde la hebra de logica.
	*/
	static void EnqueueCommand(RenderCommand renderCommand);

	/*
		Devuelve cuantos comandos de endframe hay encolados actualmente
	*/
	static int GetPendingFrames() { return pendingFrames; };

private:

	/*
		Bucle de la hebra de renderizado.
		Procesa los eventos en la cola.
	*/
	static void RenderLoop();

	/*
		Dibuja el estado actual de la lluvia
	*/
	static void DrawRain(Color* background, int* heightDiffs, bool forcePaint);

	/*
		Restringe un valor al rango
	*/
	static int Clamp(int min, int max, int value);
};

