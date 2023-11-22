/*!
 * \file    iplfeatures.h
 * \author  IDS Imaging Development Systems GmbH
 * \date    2022-06-01
 * \since   2.1.0
 *
 * \brief   The IPL features class provides controls to access IPL features
 *
 * \version 1.0.0
 *
 * Copyright (C) 2022 - 2023, IDS Imaging Development Systems GmbH.
 *
 * The information in this document is subject to change without notice
 * and should not be construed as a commitment by IDS Imaging Development Systems GmbH.
 * IDS Imaging Development Systems GmbH does not assume any responsibility for any errors
 * that may appear in this document.
 *
 * This document, or source code, is provided solely as an example of how to utilize
 * IDS Imaging Development Systems GmbH software libraries in a sample application.
 * IDS Imaging Development Systems GmbH does not assume any responsibility
 * for the use or reliability of any portion of this document.
 *
 * General permission to copy or modify is hereby granted.
 */

#ifndef IPLFEATURES_H
#define IPLFEATURES_H

#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QObject>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QTabWidget>
#include <QTimer>
#include <QWidget>

class IplFeaturesWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit IplFeaturesWidget(QWidget* parent);
    ~IplFeaturesWidget() override = default;

public slots:
    void CheckForChangedValues();

signals:
    void error(QString message);
    void exposureAutoModeChanged(bool active);
    void updateExposureTime();

private:
    void CreateControls();

    // Update control functions
    void UpdateBrightnessAutoModes();
    void UpdateBrightnessRoi();
    void UpdateBrightnessOptions();

    void UpdateWhiteBalanceAutoMode();
    void UpdateWhiteBalanceRoi();

    void UpdateGain();
    void UpdateGamma();

    void UpdateColorCorrection();

    void UpdateTransformations();

    void UpdateHotpixel();

    void UpdateEdgeEnhancement();

    // Handler functions when GUI controls are manipulated
    void OnComboExposureMode(const QString& text);
    void OnComboGainMode(const QString& text);

    void OnComboBrightnessRoiMode(const QString& text);
    void OnSliderBrightnessRoi(int value);
    void OnEditBrightnessRoi();

    void OnSliderTarget(int value);
    void OnEditTarget();

    void OnSliderTargetPercentile(int value);
    void OnEditTargetPercentile();

    void OnSliderTargetTolerance(int value);
    void OnEditTargetTolerance();

    void OnComboWhiteBalanceMode(const QString& text);

    void OnComboWhiteBalanceRoiMode(const QString& text);
    void OnSliderWhiteBalanceRoi(int value);
    void OnEditWhiteBalanceRoi();

    void OnSliderGainMaster(int value);
    void OnEditGainMaster();

    void OnSliderGainRed(int value);
    void OnEditGainRed();

    void OnSliderGainGreen(int value);
    void OnEditGainGreen();

    void OnSliderGainBlue(int value);
    void OnEditGainBlue();

    void OnSliderGamma(int value);
    void OnEditGamma();

    void OnColorCorrectionEnable(int state);
    void OnEditCcm();

    void OnButtonCcmDefault();
    void OnButtonCcmIdentity();

    void OnCheckBoxMirrorLeftRight(int state);
    void OnCheckBoxMirrorUpDown(int state);

    void OnHotpixelCorrectionEnable(int state);
    void OnSpinBoxHotpixelSensitivity(int value);
    void OnButtonHotpixelResetList();

    void OnEdgeEnhancementEnable(bool enabled);
    void OnEdgeEnhancementFactorChanged(int value);

    // GUI elements for auto brightness
    QLabel* m_labelExposureMode{};
    QComboBox* m_comboExposureMode{};

    QLabel* m_labelGainMode{};
    QComboBox* m_comboGainMode{};

    QLabel* m_labelBrightnessROIMode{};
    QComboBox* m_comboBrightnessRoiMode{};

    QLabel* m_labelBrightnessRoiOffsetX{};
    QSlider* m_sliderBrightnessRoiOffsetX{};
    QLineEdit* m_editBrightnessRoiOffsetX{};
    QLabel* m_labelBrightnessRoiOffsetY{};
    QSlider* m_sliderBrightnessRoiOffsetY{};
    QLineEdit* m_editBrightnessRoiOffsetY{};
    QLabel* m_labelBrightnessRoiWidth{};
    QSlider* m_sliderBrightnessRoiWidth{};
    QLineEdit* m_editBrightnessRoiWidth{};
    QLabel* m_labelBrightnessRoiHeight{};
    QSlider* m_sliderBrightnessRoiHeight{};
    QLineEdit* m_editBrightnessRoiHeight{};

    QLabel* m_labelTarget{};
    QSlider* m_sliderTarget{};
    QLineEdit* m_editTarget{};

    QLabel* m_labelTargetPercentile{};
    QSlider* m_sliderTargetPercentile{};
    QLineEdit* m_editTargetPercentile{};

    QLabel* m_labelTargetTolerance{};
    QSlider* m_sliderTargetTolerance{};
    QLineEdit* m_editTargetTolerance{};

    // GUI elements for auto white balance
    QLabel* m_labelWhiteBalanceMode{};
    QComboBox* m_comboWhiteBalanceMode{};

    QLabel* m_labelWhiteBalanceRoiMode{};
    QComboBox* m_comboWhiteBalanceRoiMode{};

    QLabel* m_labelWhiteBalanceRoiOffsetX{};
    QSlider* m_sliderWhiteBalanceRoiOffsetX{};
    QLineEdit* m_editWhiteBalanceRoiOffsetX{};
    QLabel* m_labelWhiteBalanceRoiOffsetY{};
    QSlider* m_sliderWhiteBalanceRoiOffsetY{};
    QLineEdit* m_editWhiteBalanceRoiOffsetY{};
    QLabel* m_labelWhiteBalanceRoiWidth{};
    QSlider* m_sliderWhiteBalanceRoiWidth{};
    QLineEdit* m_editWhiteBalanceRoiWidth{};
    QLabel* m_labelWhiteBalanceRoiHeight{};
    QSlider* m_sliderWhiteBalanceRoiHeight{};
    QLineEdit* m_editWhiteBalanceRoiHeight{};

    // GUI elements for gain and gamma
    QLabel* m_labelGainMaster{};
    QSlider* m_sliderGainMaster{};
    QLineEdit* m_editGainMaster{};
    QLabel* m_labelGainRed{};
    QSlider* m_sliderGainRed{};
    QLineEdit* m_editGainRed{};
    QLabel* m_labelGainGreen{};
    QSlider* m_sliderGainGreen{};
    QLineEdit* m_editGainGreen{};
    QLabel* m_labelGainBlue{};
    QSlider* m_sliderGainBlue{};
    QLineEdit* m_editGainBlue{};

    QLabel* m_labelGamma{};
    QSlider* m_sliderGamma{};
    QLineEdit* m_editGamma{};

    // GUI elements for color correction
    QCheckBox* m_checkBoxColorCorrectionEnable{};

    QLabel* m_labelCcm{};
    QLineEdit* m_editCcm00{};
    QLineEdit* m_editCcm01{};
    QLineEdit* m_editCcm02{};
    QLineEdit* m_editCcm10{};
    QLineEdit* m_editCcm11{};
    QLineEdit* m_editCcm12{};
    QLineEdit* m_editCcm20{};
    QLineEdit* m_editCcm21{};
    QLineEdit* m_editCcm22{};

    QPushButton* m_buttonCcmDefault{};
    QPushButton* m_buttonCcmIdentity{};

    // GUI elements for image transformations
    QCheckBox* m_checkBoxMirrorLeftRight{};
    QCheckBox* m_checkBoxMirrorUpDown{};

    // GUI elements for hotpixel
    QCheckBox* m_checkBoxHotpixelEnable{};
    QLabel* m_labelHotpixelSensitivity{};
    QSpinBox* m_spinBoxHotpixelSensitivity{};
    QPushButton* m_buttonHotpixelResetList{};

    QLabel* m_labelHotpixelCount{};
    QLabel* m_labelHotpixelCountValue{};

    // GUI elements for tab page composition
    QWidget* m_brightnessWidget{};
    QGridLayout* m_brightnessLayout{};

    QWidget* m_whiteBalanceWidget{};
    QGridLayout* m_whiteBalanceLayout{};

    QWidget* m_gainWidget{};
    QGridLayout* m_gainLayout{};

    QWidget* m_colorCorrectionWidget{};
    QGridLayout* m_colorCorrectionLayout{};

    QWidget* m_transformWidget{};
    QGridLayout* m_transformLayout{};

    QWidget* m_hotpixelWidget{};
    QGridLayout* m_hotpixelLayout{};

    // edge enhancement
    QWidget* m_edgeEnhancementWidget{};
    QCheckBox* m_checkBoxEdgeEnhancementEnable{};
    QSlider* m_sliderEdgeEnhancementFactor{};
    QGridLayout* m_edgeEnhancementLayout{};
    QLabel* m_labelEdgeEnhancementFactor{};
    QSpinBox* m_spinBoxEdgeEnhancementFactor{};

    // Timer to update controls
    QTimer* m_timer{};

    // Variables to calculate slider positions for double values
    double m_targetPercentileMin{};
    double m_targetPercentileMax{};
    double m_targetPercentileInc{};

    double m_gainMasterMin{};
    double m_gainMasterMax{};
    double m_gainMasterInc{};
    double m_gainRedMin{};
    double m_gainRedMax{};
    double m_gainRedInc{};
    double m_gainGreenMin{};
    double m_gainGreenMax{};
    double m_gainGreenInc{};
    double m_gainBlueMin{};
    double m_gainBlueMax{};
    double m_gainBlueInc{};

    double m_gammaMin{};
    double m_gammaMax{};
    double m_gammaInc{};

    bool m_isMonoCamera{};
};


#endif // IPLFEATURES_H
