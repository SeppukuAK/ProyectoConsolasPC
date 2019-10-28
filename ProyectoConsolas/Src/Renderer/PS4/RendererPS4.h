class RendererPS4
{
	//TODO:Poner atributos
	//Los métodos son estáticos
	static void Clear();

	/*
		Pone un Pixel en el RenderBuffer. No aparece en pantalla hasta que se hace present
	*/
	static void PutPixel();

	/*
		Muestra en pantalla el RenderBuffer.
	*/
	static void Present();
};