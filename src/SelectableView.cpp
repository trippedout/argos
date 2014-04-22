#include "SelectableView.h"

//sets the current selected state; after calling this, handle drawing that state;
void SelectableView::selectView(bool select)
{
	this->selected = select;
}