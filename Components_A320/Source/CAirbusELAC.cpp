
// qt-plus
#include "CLogger.h"

// Quick3D
#include "CConsoleBoard.h"
#include "C3DScene.h"
#include "CWing.h"
#include "CElevator.h"

// Application
#include "CAirbusELAC.h"

//-------------------------------------------------------------------------------------------------

using namespace Math;

//-------------------------------------------------------------------------------------------------

CComponent* CAirbusELAC::instanciator(C3DScene* pScene)
{
    return new CAirbusELAC(pScene);
}

//-------------------------------------------------------------------------------------------------

CAirbusELAC::CAirbusELAC(C3DScene* pScene)
    : CAirbusFlightComputer(pScene)
    , m_pidYaw(1.0, 0.0, 0.001)
    , m_pidPitch(1.0, 0.0, 0.001)
    , m_pidRoll(1.0, 0.0, 0.001)
{
    LOG_DEBUG("CAirbusELAC::CAirbusELAC()");
}

//-------------------------------------------------------------------------------------------------

CAirbusELAC::~CAirbusELAC()
{
    LOG_DEBUG("CAirbusELAC::~CAirbusELAC()");
}

//-------------------------------------------------------------------------------------------------

void CAirbusELAC::loadParameters(const QString& sBaseFile, CXMLNode xComponent)
{
    CAirbusFlightComputer::loadParameters(sBaseFile, xComponent);

    CXMLNode xNode = xComponent.getNodeByTagName(ParamName_ControlSurfaces);

    m_rLeftWingTarget.setName(xNode.attributes()["LeftWingTarget"]);
    m_rRightWingTarget.setName(xNode.attributes()["RightWingTarget"]);
    m_rElevatorTarget.setName(xNode.attributes()["ElevatorTarget"]);
}

//-------------------------------------------------------------------------------------------------

void CAirbusELAC::solveLinks(C3DScene* pScene)
{
    CAirbusFlightComputer::solveLinks(pScene);

    m_rLeftWingTarget.solve(pScene, QSP<CComponent>(this));
    m_rRightWingTarget.solve(pScene, QSP<CComponent>(this));
    m_rElevatorTarget.solve(pScene, QSP<CComponent>(this));
}

//-------------------------------------------------------------------------------------------------

void CAirbusELAC::clearLinks(C3DScene* pScene)
{
    CAirbusFlightComputer::clearLinks(pScene);

    m_rLeftWingTarget.clear();
    m_rRightWingTarget.clear();
    m_rElevatorTarget.clear();
}

//-------------------------------------------------------------------------------------------------

void CAirbusELAC::update(double dDeltaTime)
{
    CAirbusFlightComputer::update(dDeltaTime);
}

//-------------------------------------------------------------------------------------------------

void CAirbusELAC::work(double dDeltaTime)
{
    CAirbusFlightComputer::work(dDeltaTime);

    if (m_iUnitIndex == 0)
    {
        QSP<CWing> pLeftWing = QSP_CAST(CWing, m_rLeftWingTarget.component());
        QSP<CWing> pRightWing = QSP_CAST(CWing, m_rRightWingTarget.component());
        QSP<CElevator> pElevator = QSP_CAST(CElevator, m_rElevatorTarget.component());

        CAirbusData* pInertial_Roll_deg = data(adInertial_Roll_deg);
        CAirbusData* pInertial_Pitch_deg = data(adInertial_Pitch_deg);
        CAirbusData* pInertial_RollVelocity_ds = data(adInertial_RollVelocity_ds);
        CAirbusData* pInertial_PitchVelocity_ds = data(adInertial_PitchVelocity_ds);
        CAirbusData* pFG_CommandedRollVelocity_ds = data(adFG_CommandedRollVelocity_ds);
        CAirbusData* pFG_CommandedPitchVelocity_ds = data(adFG_CommandedPitchVelocity_ds);
        CAirbusData* pStick_CAPT_x = data(adStick_CAPT_x);
        CAirbusData* pStick_CAPT_y = data(adStick_CAPT_y);
        CAirbusData* pFCU_AutoPilot1_Engaged = data(adFCU_AutoPilot1_Engaged);
        CAirbusData* pFCU_AutoPilot2_Engaged = data(adFCU_AutoPilot2_Engaged);

        double dInertial_Roll_deg = 0.0;
        double dInertial_Pitch_deg = 0.0;
        double dInertial_RollVelocity_ds = 0.0;
        double dInertial_PitchVelocity_ds = 0.0;
        double dFG_CommandedRollVelocity_ds = 0.0;
        double dFG_CommandedPitchVelocity_ds = 0.0;
        bool bFCU_AutoPilot1_Engaged = false;
        bool bFCU_AutoPilot2_Engaged = false;

        if (pInertial_Roll_deg != nullptr)
        {
            dInertial_Roll_deg = pInertial_Roll_deg->data().toDouble();
        }

        if (pInertial_Pitch_deg != nullptr)
        {
            dInertial_Pitch_deg = pInertial_Pitch_deg->data().toDouble();
        }

        if (pInertial_PitchVelocity_ds != nullptr)
        {
            dInertial_PitchVelocity_ds = pInertial_PitchVelocity_ds->data().toDouble();
        }

        if (pInertial_RollVelocity_ds != nullptr)
        {
            dInertial_RollVelocity_ds = pInertial_RollVelocity_ds->data().toDouble();
        }

        if (pInertial_PitchVelocity_ds != nullptr)
        {
            dInertial_PitchVelocity_ds = pInertial_PitchVelocity_ds->data().toDouble();
        }

        if (pFG_CommandedRollVelocity_ds != nullptr)
        {
            dFG_CommandedRollVelocity_ds = pFG_CommandedRollVelocity_ds->data().toDouble();
        }

        if (pFG_CommandedPitchVelocity_ds != nullptr)
        {
            dFG_CommandedPitchVelocity_ds = pFG_CommandedPitchVelocity_ds->data().toDouble();
        }

        if (pStick_CAPT_x != nullptr)
        {
            m_vStick.X = pStick_CAPT_x->data().toDouble();
        }

        if (pStick_CAPT_y != nullptr)
        {
            m_vStick.Y = pStick_CAPT_y->data().toDouble();
        }

        if (pFCU_AutoPilot1_Engaged != nullptr)
        {
            bFCU_AutoPilot1_Engaged = pFCU_AutoPilot1_Engaged->data().toBool();
        }

        if (pFCU_AutoPilot2_Engaged != nullptr)
        {
            bFCU_AutoPilot2_Engaged = pFCU_AutoPilot2_Engaged->data().toBool();
        }

        //-----------------------------------------------------------------------------

        if (m_vStick.X == 0.0 && m_vStickPrevious.X != 0.0)
        {
            m_vCommandedAttitude.Z = dInertial_Roll_deg;
        }

        if (m_vStick.Y == 0.0 && m_vStickPrevious.Y != 0.0)
        {
            m_vCommandedAttitude.X = dInertial_Pitch_deg;
        }

        if (bFCU_AutoPilot1_Engaged == false && bFCU_AutoPilot1_Engaged == false)
        {
            if (m_vStick.X != 0.0)
            {
                dFG_CommandedRollVelocity_ds = m_vStick.X * -10.0;
            }
            else
            {
                dFG_CommandedRollVelocity_ds = m_vCommandedAttitude.Z - dInertial_Roll_deg;
                dFG_CommandedRollVelocity_ds = Math::Angles::clipDouble(dFG_CommandedRollVelocity_ds, -10.0, 10.0);
            }

            if (m_vStick.Y != 0.0)
            {
                dFG_CommandedPitchVelocity_ds = m_vStick.Y * -5.0;
            }
            else
            {
                dFG_CommandedPitchVelocity_ds = m_vCommandedAttitude.X - dInertial_Pitch_deg;
                dFG_CommandedPitchVelocity_ds = Math::Angles::clipDouble(dFG_CommandedPitchVelocity_ds, -5.0, 5.0);
            }
        }

        //-----------------------------------------------------------------------------

        // Calcul PID roulis

        m_pidRoll.setSetPoint(dFG_CommandedRollVelocity_ds);
        m_pidRoll.update(dInertial_RollVelocity_ds, dDeltaTime * 1000.0);

        double dRollOutput = m_pidRoll.output();

        if (pLeftWing && pRightWing)
        {
            pLeftWing->setAileronAngle_norm(-dRollOutput);
            pRightWing->setAileronAngle_norm(dRollOutput);
        }

        // Calcul PID tanguage

        m_pidPitch.setSetPoint(dFG_CommandedPitchVelocity_ds);
        m_pidPitch.update(dInertial_PitchVelocity_ds, dDeltaTime * 1000.0);

        double dPitchOutput = m_pidPitch.output();

        if (pElevator != nullptr)
        {
            pElevator->setAileronAngle_norm(-dPitchOutput);
        }

        LOG_VALUE(QString("%1 ROLL PID").arg(m_sName),
                  QString("%1")
                  .arg(QString::number(dRollOutput, 'f', 4))
                  );

        m_vStickPrevious = m_vStick;
    }
}
