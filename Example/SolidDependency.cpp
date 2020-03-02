#include "SolidDependency.hpp"

int SolidDependency::staticField;

SolidDependency::SolidDependency() : someField(0)
{}

int SolidDependency::setField(int field)
{
    return this->someField = field;
}

int SolidDependency::setStaticField(int field)
{
    return staticField = field;
}
