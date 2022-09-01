#pragma once
#include "MitkExampleModuleExports.h"

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>
#include <QCheckBox>
#include <ctkDoubleRangeSlider.h>

#include <mitkImage.h>
#include <mitkDataNode.h>

#include "ImageProcessing/ImageProcessing.h"
#include "ImageProcessingParamWidget.h"


class MITKEXAMPLEMODULE_EXPORT ImageProcessingWidget : public QWidget
{
	Q_OBJECT
public:
	ImageProcessingWidget(QWidget* parent = nullptr);
	~ImageProcessingWidget();

public:
	void initialize(mitk::DataNode::Pointer imageNode);
	void initializeWidget();
	void updateParamByImage();

private:
	QWidget* createLevelWindowWidget(QWidget* parent = nullptr);

	void updateUI();

private slots:
	void OnBrightnessChanged(int value);
	void OnContrastChanged(int value);
	void OnLevelWindowValueChanged(double minValue, double maxValue);
	void OnCheckSharpenFilter(int state);
	void OnCheckMedianBlur(int state);
	void OnCheckGaussianBlur(int state);
	void OnCheckBilateralFilter(int state);
	void OnMedianParamChanged(ImageProcessing::MedianBlurParam param);
	void OnGaussainParamChanged(ImageProcessing::GaussianBlurParam parma);
	void OnBilateralParamChanged(ImageProcessing::BilateralFilterParam param);

private:
	mitk::DataNode::Pointer m_imageNode;

	mitk::LevelWindow m_levelWindow;

	mitk::Image::Pointer m_filterInput;
	CustomImageMapper2D::Pointer m_mapper;

	bool m_uiUpdating;

	QSlider* m_sliderBrightness;
	QSlider* m_sliderContrast;
	QLabel* m_labelBrightnessValue;
	QLabel* m_labelContrastValue;
	QLineEdit* m_editLevelMin;
	QLineEdit* m_editLevelMax;
	ctkDoubleRangeSlider* m_levelWindowSlider;

	QCheckBox* m_checkBoxSharpen;
	QCheckBox* m_checkBoxMedian;
	QCheckBox* m_checkBoxGaussian;
	QCheckBox* m_checkBoxBilateralFilter;
	
	MedianBlurParamWidget* m_medianParamWidget;
	GaussianBlurParamWidget* m_gaussianParamWidget;
	BilateralFilterParamWidget* m_bilateralParamWidget;
	 
};