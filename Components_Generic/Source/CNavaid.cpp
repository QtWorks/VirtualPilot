
#include "CNavaid.h"

//-------------------------------------------------------------------------------------------------

using namespace Math;

//-------------------------------------------------------------------------------------------------

CNavaid::CNavaid()
{
}

//-------------------------------------------------------------------------------------------------

CNavaid::~CNavaid()
{
}

//-------------------------------------------------------------------------------------------------

void CNavaid::loadParameters(const QString& sBaseFile, CXMLNode xNavaid)
{
    CNavaidComponent::loadParameters(sBaseFile, xNavaid);
}
