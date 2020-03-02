#pragma once

// Solid dependency for client
class SolidDependency
{
public:
    SolidDependency();
// this method client want to use
    int setField(int field);

    static int setStaticField(int field);

private:
    int someField;
    static int staticField;
};

