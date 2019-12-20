#pragma once
#include <atomic> 
#include "../Utilities/ConcurrentQueue.h"
#include "Color.h"
#include "../Utilities/Rect.h"

class Image;
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

struct RenderCommandDrawSpriteParams
{
	Image* Image;
	Rect SourceRect;	//No es puntero porque al procesar el comando, el rectángulo puede haberse destruido
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
	RenderCommandDrawSpriteParams DrawSpriteParams;
};

/*
	Diferentes tipos de comandos
*/
enum RendererCommandType
{
	CLEAR,				//Borra la pantalla
	PUT_PIXEL,			//Sirve para hacer pruebas (x,y,c)
	END_FRAME,			//Hace el present
	DRAW_SPRITE			//Pinta una seccion de una imagen en la posición especificada
};

/*
	Tipo de los comandos de Render
*/
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
	static const Color SCREEN_CLEAR_COLOR;		//Color con el que se limpia la pantalla

	static std::thread* t;						//Hilo
	static Queue<RenderCommand> commandQueue; 	//Cola concurrente (compartida entre hebras) de comandos

	//Escritura y lectura seguras
	static std::atomic <bool> quitRequested;	//Indica si se quiere parar la aplicación
	static std::atomic <int> pendingFrames;		//Número de comandos de endframe encolados actualmente

	//Comandos utiles durante toda la ejecución
	static RenderCommand clearCommand;
	static RenderCommand presentCommand;

public:
	/*
		Inicia la hebra.
		Crea los comandos básicos.
		Limpia los buffers
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
	static void EnqueueCommand(const RenderCommand renderCommand);

	/*
		Encola un comando de Clear
	*/
	static void EnqueueClearCommand();

	/*
		Encola un comando de present
	*/
	static void EnqueuePresentCommand();

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
};

