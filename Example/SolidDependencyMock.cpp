#include "SolidDependencyMock.hpp"

using namespace ::testing;

int SolidDependency::staticField;

// we have to define c-tor as it is defined in SolidDependency.hpp
// though we do not have to set someField - it's not going to be used anyway
SolidDependency::SolidDependency()
{}

// let's define setField method
int SolidDependency::setField(int field)
{
// and forward call to mock instance registered for SolidDependency type
    return MockList<SolidDependencyMock, SolidDependency>::instance().findMockFor(this)->setField(field);
}

// let's define setStaticField static method
int SolidDependency::setStaticField(int field)
{
// and forward call to mock instance registered for static part of SolidDependency type
// we cannot reuse instance registered for non-static as static call has no this!
    return MockList<SolidDependencyMock, SolidDependency>::instance().getStaticMock()->setStaticField(field);
}
