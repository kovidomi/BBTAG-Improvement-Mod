#include "CustomHudWindow.h"

void CustomHudWindow::SetScale(const ImVec2 & scale)
{
	m_healthWindowLeft.SetScale(scale);
	m_healthWindowRight.SetScale(scale);
}

void CustomHudWindow::BeforeDraw()
{
	m_healthWindowLeft.Update();
	m_healthWindowRight.Update();
}
