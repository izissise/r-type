#include "ADrawable.hpp"

ADrawable::ADrawable(bool hide)
: _hide(hide)
{
  
}

void  ADrawable::setHide(bool hide)
{
  _hide = hide;
}

bool  ADrawable::isHidden() const
{
  return _hide;
}