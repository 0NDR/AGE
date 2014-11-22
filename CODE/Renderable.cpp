#include "Renderable.h"
void Renderable::setWindow(Window* newWindow)
{
    renderWindow = newWindow;
}

void Renderable::setShader(Shader* newShader)
{
    renderShader = newShader;
}
