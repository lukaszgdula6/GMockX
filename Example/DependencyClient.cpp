#include "DependencyClient.hpp"
#include "SolidDependency.hpp"

int DependencyClient::useDependency(int field)
{
// usage of solid dependency
// Of course this is only simplified example
    return SolidDependency().setField(field);
}

int DependencyClient::useStaticDependency(int field)
{
    // usage of solid dependency, static method
    // Of course this is only simplified example
    return SolidDependency::setStaticField(field);
}
