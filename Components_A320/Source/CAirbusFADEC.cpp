
// qt-plus
#include "CLogger.h"

// Quick3D
#include "CConsoleBoard.h"
#include "C3DScene.h"
#include "CWing.h"
#include "CElevator.h"
#include "CJetEngine.h"

// Application
#include "CAirbusFADEC.h"

using namespace Math;

//-------------------------------------------------------------------------------------------------

CComponent* CAirbusFADEC::instanciator(C3DScene* pScene)
{
    return new CAirbusFADEC(pScene);
}

//-------------------------------------------------------------------------------------------------

CAirbusFADEC::CAirbusFADEC(C3DScene* pScene)
    : CAirbusFlightComputer(pScene)
{
    LOG_DEBUG("CAirbusFADEC::CAirbusFADEC()");
}

//-------------------------------------------------------------------------------------------------

CAirbusFADEC::~CAirbusFADEC()
{
    LOG_DEBUG("CAirbusFADEC::~CAirbusFADEC()");
}

//-------------------------------------------------------------------------------------------------

void CAirbusFADEC::loadParameters(const QString& sBaseFile, CXMLNode xComponent)
{
    CAirbusFlightComputer::loadParameters(sBaseFile, xComponent);

    CXMLNode xNode = xComponent.getNodeByTagName(ParamName_Engines);

    m_rEngine1Target.setName(xNode.attributes()[ParamName_Engine1Target]);
    m_rEngine2Target.setName(xNode.attributes()[ParamName_Engine2Target]);
}

//-------------------------------------------------------------------------------------------------

void CAirbusFADEC::solveLinks(C3DScene* pScene)
{
    CAirbusFlightComputer::solveLinks(pScene);

    m_rEngine1Target.solve(pScene, QSP<CComponent>(this));
    m_rEngine2Target.solve(pScene, QSP<CComponent>(this));
}

//-------------------------------------------------------------------------------------------------

void CAirbusFADEC::clearLinks(C3DScene* pScene)
{
    CAirbusFlightComputer::clearLinks(pScene);

    m_rEngine1Target.clear();
    m_rEngine2Target.clear();
}

//-------------------------------------------------------------------------------------------------

void CAirbusFADEC::update(double dDeltaTime)
{
    CAirbusFlightComputer::update(dDeltaTime);
}

//-------------------------------------------------------------------------------------------------

void CAirbusFADEC::work(double dDeltaTime)
{
    CAirbusFlightComputer::work(dDeltaTime);

    QSP<CJetEngine> pEngine1 = QSP_CAST(CJetEngine, m_rEngine1Target.component());
    QSP<CJetEngine> pEngine2 = QSP_CAST(CJetEngine, m_rEngine2Target.component());

    if (pEngine1 != nullptr && pEngine2 != nullptr )
    {
        CAirbusData* pFG_CommandedThrust_norm = data(adFG_CommandedThrust_norm);
        CAirbusData* pFCU_AutoThrust_Engaged = data(adFCU_AutoThrust_Engaged);
        CAirbusData* pThrottle_1_norm = data(adThrottle_1_norm);
        CAirbusData* pThrottle_2_norm = data(adThrottle_2_norm);

        double dFG_CommandedThrust_norm = 0.0;
        bool bFCU_AutoThrust_Engaged = false;
        double dThrottle_1_norm = 0.0;
        double dThrottle_2_norm = 0.0;

        if (pFG_CommandedThrust_norm != nullptr)
            dFG_CommandedThrust_norm = pFG_CommandedThrust_norm->data().toDouble();

        if (pFCU_AutoThrust_Engaged != nullptr)
            bFCU_AutoThrust_Engaged = pFCU_AutoThrust_Engaged->data().toBool();

        if (pThrottle_1_norm != nullptr)
            dThrottle_1_norm = pThrottle_1_norm->data().toDouble();

        if (pThrottle_2_norm != nullptr)
            dThrottle_2_norm = pThrottle_2_norm->data().toDouble();

        if (bFCU_AutoThrust_Engaged == true)
        {
            pEngine1->setCurrentFuelFlow_norm(dFG_CommandedThrust_norm);
            pEngine2->setCurrentFuelFlow_norm(dFG_CommandedThrust_norm);
        }
        else
        {
            pEngine1->setCurrentFuelFlow_norm(dThrottle_1_norm);
            pEngine2->setCurrentFuelFlow_norm(dThrottle_2_norm);
        }

        pushData(CAirbusData(m_sName, adFADEC_Engine1_N1_norm, pEngine1->n1_norm()));
        pushData(CAirbusData(m_sName, adFADEC_Engine2_N1_norm, pEngine2->n1_norm()));
    }
}
