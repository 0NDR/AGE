#include "CombinedController.h"
void CombinedController::makeCurrent()
{
    this->Window::makeCurrent();
    this->AudioController::makeCurrent();
}
