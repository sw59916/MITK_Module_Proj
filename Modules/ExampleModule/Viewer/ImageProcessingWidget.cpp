#include "ImageProcessingWidget.h"

#include <QLayout>
#include <QToolButton>
#include <QWidgetAction>

#include <mitkLevelWindowProperty.h>
#include <mitkRenderingManager.h>


ImageProcessingWidget::ImageProcessingWidget(QWidget* parent)
	: QWidget(parent)
{
	m_imageNode = nullptr;
	m_uiUpdating = false;

	m_mapper = CustomImageMapper2D::New();
}

ImageProcessingWidget::~ImageProcessingWidget()
{
}

void ImageProcessingWidget::initialize(mitk::DataNode::Pointer imageNode)
{
	m_imageNode = imageNode;

	initializeWidget();

	updateParamByImage();
}

void ImageProcessingWidget::initializeWidget()
{
	QGridLayout* parentLayout = new QGridLayout(this);
	this->setLayout(parentLayout);
	
	Qt::Alignment labelAlign;
	labelAlign.setFlag(Qt::AlignmentFlag::AlignHCenter, true);
	labelAlign.setFlag(Qt::AlignmentFlag::AlignVCenter, true);

	QLabel* labelBrightness = new QLabel(this);
	labelBrightness->setText("Brightness");
	labelBrightness->setAlignment(labelAlign);
	QLabel* labelContrast = new QLabel(this);
	labelContrast->setText("Contrast");
	labelContrast->setAlignment(labelAlign);

	m_sliderBrightness = new QSlider(this);
	m_sliderBrightness->setOrientation(Qt::Orientation::Horizontal);
	m_sliderBrightness->setRange(0, 100);
	m_sliderContrast = new QSlider(this);
	m_sliderContrast->setOrientation(Qt::Orientation::Horizontal);
	m_sliderContrast->setRange(0, 100);

	m_labelBrightnessValue = new QLabel(this);
	m_labelBrightnessValue->setAlignment(labelAlign);
	m_labelContrastValue = new QLabel(this);
	m_labelContrastValue->setAlignment(labelAlign);

	QLabel* labelLevelWindow = new QLabel(this);
	labelLevelWindow->setText("LevelWindow");
	labelLevelWindow->setAlignment(labelAlign);

	QWidget* levelWindowWidget = createLevelWindowWidget(this);

	m_checkBoxSharpen = new QCheckBox(this);
	m_checkBoxSharpen->setText("Use SharpenFilter");

	m_checkBoxMedian = new QCheckBox(this);
	m_checkBoxMedian->setText("Use MeidanBlur");
	QToolButton* buttonMedianSetting = new QToolButton(this);
	buttonMedianSetting->setText("Setting");

	m_medianParamWidget = new MedianBlurParamWidget(this);
	buttonMedianSetting->setPopupMode(QToolButton::ToolButtonPopupMode::InstantPopup);
	buttonMedianSetting->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	buttonMedianSetting->setMenu(m_medianParamWidget);

	m_checkBoxGaussian = new QCheckBox(this);
	m_checkBoxGaussian->setText("Use GaussianBlur");
	QToolButton* buttonGaussianSetting = new QToolButton(this);
	buttonGaussianSetting->setText("Setting");

	m_gaussianParamWidget = new GaussianBlurParamWidget(this);
	buttonGaussianSetting->setPopupMode(QToolButton::ToolButtonPopupMode::InstantPopup);
	buttonGaussianSetting->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	buttonGaussianSetting->setMenu(m_gaussianParamWidget);
	
	m_checkBoxBilateralFilter = new QCheckBox(this);
	m_checkBoxBilateralFilter->setText("Use BilateralFilter");
	QToolButton* buttonBilateralSetting = new QToolButton(this);
	buttonBilateralSetting->setText("Setting");

	m_bilateralParamWidget = new BilateralFilterParamWidget(this);
	buttonBilateralSetting->setPopupMode(QToolButton::ToolButtonPopupMode::InstantPopup);
	buttonBilateralSetting->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	buttonBilateralSetting->setMenu(m_bilateralParamWidget);


	parentLayout->addWidget(labelBrightness, 0, 0);
	parentLayout->addWidget(m_sliderBrightness, 0, 1);
	parentLayout->addWidget(m_labelBrightnessValue, 0, 2);
	parentLayout->addWidget(labelContrast, 1, 0);
	parentLayout->addWidget(m_sliderContrast, 1, 1);
	parentLayout->addWidget(m_labelContrastValue, 1, 2);
	parentLayout->addWidget(labelLevelWindow, 2, 0);
	parentLayout->addWidget(levelWindowWidget, 2, 1, 1, 2);

	parentLayout->addWidget(m_checkBoxSharpen, 3, 0, 1, 2);
	/*parentLayout->addWidget(buttonSharpenSetting, 3, 2);*/
	parentLayout->addWidget(m_checkBoxMedian, 4, 0, 1, 2);
	parentLayout->addWidget(buttonMedianSetting, 4, 2);
	parentLayout->addWidget(m_checkBoxGaussian, 5, 0, 1, 2);
	parentLayout->addWidget(buttonGaussianSetting, 5, 2);
	parentLayout->addWidget(m_checkBoxBilateralFilter, 6, 0, 1, 2);
	parentLayout->addWidget(buttonBilateralSetting, 6, 2);

	mitk::LevelWindowProperty::Pointer levelWindowProp = dynamic_cast<mitk::LevelWindowProperty*>(m_imageNode->GetProperty("levelwindow"));
	if (levelWindowProp.IsNotNull())
		m_levelWindow = levelWindowProp->GetLevelWindow();

	updateUI();

	connect(m_sliderBrightness, &QSlider::valueChanged, this, &ImageProcessingWidget::OnBrightnessChanged);
	connect(m_sliderContrast, &QSlider::valueChanged, this, &ImageProcessingWidget::OnContrastChanged);
	connect(m_levelWindowSlider, &ctkDoubleRangeSlider::valuesChanged, this, &ImageProcessingWidget::OnLevelWindowValueChanged);

	connect(m_checkBoxSharpen, &QCheckBox::stateChanged, this, &ImageProcessingWidget::OnCheckSharpenFilter);
	connect(m_checkBoxMedian, &QCheckBox::stateChanged, this, &ImageProcessingWidget::OnCheckMedianBlur);
	connect(m_checkBoxGaussian, &QCheckBox::stateChanged, this, &ImageProcessingWidget::OnCheckGaussianBlur);
	connect(m_checkBoxBilateralFilter, &QCheckBox::stateChanged, this, &ImageProcessingWidget::OnCheckBilateralFilter);

	connect(m_medianParamWidget, &MedianBlurParamWidget::paramChanged, this, &ImageProcessingWidget::OnMedianParamChanged);
	connect(m_gaussianParamWidget, &GaussianBlurParamWidget::paramChanged, this, &ImageProcessingWidget::OnGaussainParamChanged);
	connect(m_bilateralParamWidget, &BilateralFilterParamWidget::paramChanged, this, &ImageProcessingWidget::OnBilateralParamChanged);
}

void ImageProcessingWidget::updateParamByImage()
{
	if (m_imageNode == nullptr)
		return;

	mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(m_imageNode->GetData());
	if (image == nullptr)
		return;
	
	// Link OutputImage
	m_imageNode->SetMapper(mitk::BaseRenderer::Standard2D, m_mapper);

	mitk::LevelWindowProperty::Pointer levelWindowProp = dynamic_cast<mitk::LevelWindowProperty*>(m_imageNode->GetProperty("levelwindow"));
	if (levelWindowProp.IsNull())
		return;

	m_levelWindow = levelWindowProp->GetLevelWindow();

	updateUI();
}


QWidget* ImageProcessingWidget::createLevelWindowWidget(QWidget* parent)
{
	QWidget* widget = new QWidget(parent);
	
	QGridLayout* layout = new QGridLayout(widget);
	widget->setLayout(layout);

	layout->setHorizontalSpacing(2);
	layout->setMargin(0);

	QIntValidator* aIntValidator = new QIntValidator;
	aIntValidator->setRange(0, 100000000);

	m_editLevelMin = new QLineEdit(widget);
	m_editLevelMin->setFixedSize(60, 22);
	m_editLevelMin->setValidator(aIntValidator);
	m_editLevelMax = new QLineEdit(widget);
	m_editLevelMax->setFixedSize(60, 22);
	m_editLevelMax->setValidator(aIntValidator);
	m_levelWindowSlider = new ctkDoubleRangeSlider(widget);

	m_levelWindowSlider->setOrientation(Qt::Orientation::Horizontal);
	m_levelWindowSlider->setMinimum(4000);
	m_levelWindowSlider->setMaximum(16383);
	m_levelWindowSlider->setMinimumValue(6000);
	m_levelWindowSlider->setMaximumValue(10000);

	layout->addWidget(m_editLevelMin, 0, 0);
	layout->addWidget(m_levelWindowSlider, 0, 1);
	layout->addWidget(m_editLevelMax, 0, 2);

	return widget;
}

void ImageProcessingWidget::updateUI()
{
	m_uiUpdating = true;

	// Setting Level -> Brightness
	double brightness = (m_levelWindow.GetLevel() - m_levelWindow.GetRangeMin()) / m_levelWindow.GetRange();
	int brightnessPercent = brightness * 100;
	if (m_sliderBrightness->sliderPosition() != brightnessPercent)
		m_sliderBrightness->setSliderPosition(brightnessPercent);

	QString brightnessPercentString = QString("%1%").arg(brightnessPercent);
	if (m_labelBrightnessValue->text() != brightnessPercentString)
		m_labelBrightnessValue->setText(brightnessPercentString);
	//

	// Setting Window -> Contrast
	double contrast = (m_levelWindow.GetWindow() / (m_levelWindow.GetRange() * 2));
	int contrastPercent = contrast * 100;
	if (m_sliderContrast->sliderPosition() != contrastPercent)
		m_sliderContrast->setSliderPosition(contrastPercent);

	QString contrastPercentString = QString("%1%").arg(contrastPercent);
	if (m_labelContrastValue->text() != contrastPercentString)
		m_labelContrastValue->setText(contrastPercentString);
	//

	// Setting LevelWindow Slider
	double lowerMin = m_levelWindow.GetRangeMin();
	double upperMax = m_levelWindow.GetRangeMax();
	double lower = m_levelWindow.GetLowerWindowBound();
	double upper = m_levelWindow.GetUpperWindowBound();

	if (lowerMin != m_levelWindowSlider->minimum() &&
		upperMax != m_levelWindowSlider->maximum())
	{
		m_levelWindowSlider->setRange(lowerMin, upperMax);
	}
	else if (lowerMin != m_levelWindowSlider->minimum())
	{
		m_levelWindowSlider->setMinimum(lowerMin);
	}
	else if (upperMax != m_levelWindowSlider->maximum())
	{
		m_levelWindowSlider->setMaximum(upperMax);
	}

	if (lower != m_levelWindowSlider->minimumValue() &&
		upper != m_levelWindowSlider->maximumValue())
	{
		m_levelWindowSlider->setValues(lower, upper);
	}
	else if (lower != m_levelWindowSlider->minimumValue())
	{
		m_levelWindowSlider->setMinimumValue(lower);
	}
	else if (upper != m_levelWindowSlider->maximumValue())
	{
		m_levelWindowSlider->setMaximumValue(upper);
	}

	if (m_editLevelMin->text() != QString::number(lower))
		m_editLevelMin->setText(QString::number(lower));

	if (m_editLevelMax->text() != QString::number(upper))
		m_editLevelMax->setText(QString::number(upper));

	//
	mitk::LevelWindowProperty::Pointer levelWindowProp = dynamic_cast<mitk::LevelWindowProperty*>(m_imageNode->GetProperty("levelwindow"));
	if (levelWindowProp.IsNotNull())
	{
		levelWindowProp->SetLevelWindow(m_levelWindow);
		
		updateImage();
	}

	m_uiUpdating = false;
}

void ImageProcessingWidget::updateImage()
{
	mitk::RenderingManager::RenderWindowVector windows = mitk::RenderingManager::GetInstance()->GetAllRegisteredRenderWindows();
	for (auto& window : windows)
	{
		if (mitk::BaseRenderer::GetInstance(window)->GetMapperID() == mitk::BaseRenderer::Standard3D)
			continue;

		mitk::BaseRenderer::GetInstance(window)->SendUpdateSlice();
	}

	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}



void ImageProcessingWidget::OnBrightnessChanged(int value)
{
	if (m_uiUpdating)
		return;

	double brightness = ((value/100.0) * m_levelWindow.GetRange() + m_levelWindow.GetRangeMin());
	m_levelWindow.SetLevelWindow(brightness, m_levelWindow.GetWindow(), false);
	updateUI();
}
void ImageProcessingWidget::OnContrastChanged(int value)
{
	if (m_uiUpdating)
		return;

	double contrast = (value / 100.0) * (m_levelWindow.GetRange() * 2);
	m_levelWindow.SetLevelWindow(m_levelWindow.GetLevel(), contrast, false);
	updateUI();
}
void ImageProcessingWidget::OnLevelWindowValueChanged(double minValue, double maxValue)
{
	if (m_uiUpdating)
		return;

	m_levelWindow.SetWindowBounds(minValue, maxValue, false);
	updateUI();
}

void ImageProcessingWidget::OnCheckSharpenFilter(int state)
{
	if (m_checkBoxSharpen->checkState() == Qt::CheckState::Checked)
	{
		if (m_checkBoxMedian->checkState() == Qt::Checked)
			m_checkBoxMedian->setCheckState(Qt::Unchecked);
		if (m_checkBoxGaussian->checkState() == Qt::Checked)
			m_checkBoxGaussian->setCheckState(Qt::Unchecked);
		if (m_checkBoxBilateralFilter->checkState() == Qt::Checked)
			m_checkBoxBilateralFilter->setCheckState(Qt::Unchecked);

		m_mapper->getProcessingObject().SetProcessingType(ImageProcessing::ImageProcessingFilterType::SharpenFilter);
	}
	else
		m_mapper->getProcessingObject().SetProcessingType(ImageProcessing::ImageProcessingFilterType::None);

	updateImage();
}

void ImageProcessingWidget::OnCheckMedianBlur(int state)
{
	if (m_checkBoxMedian->checkState() == Qt::CheckState::Checked)
	{
		if (m_checkBoxSharpen->checkState() == Qt::Checked)
			m_checkBoxSharpen->setCheckState(Qt::Unchecked);
		if (m_checkBoxGaussian->checkState() == Qt::Checked)
			m_checkBoxGaussian->setCheckState(Qt::Unchecked);
		if (m_checkBoxBilateralFilter->checkState() == Qt::Checked)
			m_checkBoxBilateralFilter->setCheckState(Qt::Unchecked);

		m_mapper->getProcessingObject().SetProcessingType(ImageProcessing::ImageProcessingFilterType::MedianBlur);
	}
	else
		m_mapper->getProcessingObject().SetProcessingType(ImageProcessing::ImageProcessingFilterType::None);

	updateImage();
}

void ImageProcessingWidget::OnCheckGaussianBlur(int state)
{
	if (m_checkBoxGaussian->checkState() == Qt::CheckState::Checked)
	{
		if (m_checkBoxSharpen->checkState() == Qt::Checked)
			m_checkBoxSharpen->setCheckState(Qt::Unchecked);
		if (m_checkBoxMedian->checkState() == Qt::Checked)
			m_checkBoxMedian->setCheckState(Qt::Unchecked);
		if (m_checkBoxBilateralFilter->checkState() == Qt::Checked)
			m_checkBoxBilateralFilter->setCheckState(Qt::Unchecked);

		m_mapper->getProcessingObject().SetProcessingType(ImageProcessing::ImageProcessingFilterType::GaussianBlur);
	}
	else
		m_mapper->getProcessingObject().SetProcessingType(ImageProcessing::ImageProcessingFilterType::None);

	updateImage();
}

void ImageProcessingWidget::OnCheckBilateralFilter(int state)
{
	if (m_checkBoxBilateralFilter->checkState() == Qt::CheckState::Checked)
	{
		if (m_checkBoxSharpen->checkState() == Qt::Checked)
			m_checkBoxSharpen->setCheckState(Qt::Unchecked); 
		if (m_checkBoxMedian->checkState() == Qt::Checked)
			m_checkBoxMedian->setCheckState(Qt::Unchecked);
		if (m_checkBoxGaussian->checkState() == Qt::Checked)
			m_checkBoxGaussian->setCheckState(Qt::Unchecked);

		m_mapper->getProcessingObject().SetProcessingType(ImageProcessing::ImageProcessingFilterType::BilateralFilter);
	}
	else
		m_mapper->getProcessingObject().SetProcessingType(ImageProcessing::ImageProcessingFilterType::None);

	updateImage();
}

void ImageProcessingWidget::OnMedianParamChanged(ImageProcessing::MedianBlurParam param)
{
	m_mapper->getProcessingObject().SetMedianBlurParam(param);

	updateImage();
}

void ImageProcessingWidget::OnGaussainParamChanged(ImageProcessing::GaussianBlurParam param)
{
	m_mapper->getProcessingObject().SetGaussianBlurParam(param);

	updateImage();
}

void ImageProcessingWidget::OnBilateralParamChanged(ImageProcessing::BilateralFilterParam param)
{
	m_mapper->getProcessingObject().SetBilateralFilterParam(param);

	updateImage();
}
