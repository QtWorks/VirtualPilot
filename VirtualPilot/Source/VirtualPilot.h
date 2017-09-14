
#pragma once

// QT
#include <QtGlobal>

#if QT_VERSION >= 0x050000
#include <QMainWindow>
#else
#include <QtGui/QMainWindow>
#endif

// Application
#include "ui_VirtualPilot.h"
#include "CGLWidgetScene.h"
#include "CComponent.h"
#include "CView.h"
#include "CWorldTerrain.h"
#include "CWorldTerrainMap.h"

class VirtualPilot : public QMainWindow
{
    Q_OBJECT

public:

    //-------------------------------------------------------------------------------------------------
    // Constructors and destructor
    //-------------------------------------------------------------------------------------------------

    //! Constructeur
#if QT_VERSION >= 0x050000
    VirtualPilot(QString sSceneFileName, QWidget *parent = 0);
#else
    VirtualPilot(QString sSceneFileName, QWidget *parent = 0, Qt::WFlags flags = 0);
#endif

    //! Destructor
    virtual ~VirtualPilot();

    //-------------------------------------------------------------------------------------------------
    // Control methods
    //-------------------------------------------------------------------------------------------------

    //!
    void readPreferences();

    //!
    void loadScene(QString sFileName);

    //!
    void loadVehicle(QString sFileName);

    //!
    void showMap();

    //-------------------------------------------------------------------------------------------------
    // Protected methods
    //-------------------------------------------------------------------------------------------------

protected:

    //!
    virtual void resizeEvent(QResizeEvent *event);

    //!
    int randInt(int low, int high);

    //-------------------------------------------------------------------------------------------------
    // Slots
    //-------------------------------------------------------------------------------------------------

private slots:

    void onTimer();
    void onLoadSceneClicked();
    void onLoadVehicleClicked();
    void onPreferencesClicked();
    void onQuitClicked();
    void onDumpSceneClicked();
    void onResize();

    void onTimeChanged(int iValue);
    void onFogLevelChanged(int iValue);
    void onWindLevelChanged(int iValue);
    void onShaderQualityChanged(int iValue);
    void onOverlookFOVChanged(int iValue);

    void onBoundsOnlyClicked();
    void onNormalsOnlyClicked();
    void onOverlookClicked();

    //-------------------------------------------------------------------------------------------------
    // Properties
    //-------------------------------------------------------------------------------------------------

protected:

    Ui::VirtualPilotClass           ui;
    CGLWidgetScene*                 m_pScene;
    CView*                          m_pView;
    CWorldTerrainMap*               m_pMap;
    QTimer                          m_tTimer;
    CAverager<double>               m_FPS;
    QDateTime                       m_tPreviousTime;
    QString                         m_sPathVehicles;
    bool                            m_bProcessEvents;
    bool                            m_bRun;
    bool                            m_bRealTime;
};
