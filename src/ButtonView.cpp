#include "ButtonView.h"

ButtonView::ButtonView(Assets* assets) : View(assets)
{
}

ButtonView::~ButtonView(void)
{
}

void ButtonView::selectView(bool select)
{
	SelectableView::selectView(select);
}

void ButtonView::activate()
{

}

void ButtonView::deactivate()
{

}