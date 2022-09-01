#include "CustomSliceWidget.h"

CustomSliceWidget::CustomSliceWidget(QWidget* parent, Qt::WindowFlags f)
	: QWidget(parent, f)
{

}

CustomSliceWidget::~CustomSliceWidget()
{

}

void CustomSliceWidget::initializeWidget(mitk::StandaloneDataStorage::Pointer dataStorage, mitk::SliceNavigationController::ViewDirection viewDir)
{
	m_dataStorage = dataStorage;
	m_viewDir = viewDir;

	QVBoxLayout* parentLayout = new QVBoxLayout(this);
	
	m_sliceWidget = new QmitkSliceWidget(this);
	parentLayout->addWidget(m_sliceWidget);

	QWidget* bottomWidget = new QWidget(this);
	parentLayout->addWidget(bottomWidget);

	// Initialize SliceWidget
	m_sliceWidget->SetDataStorage(dataStorage);
	/*m_sliceWidget->GetRenderWindow()->ActivateMenuWidget(true);*/

	m_planeNode = m_sliceWidget->GetRenderer()->GetCurrentWorldPlaneGeometryNode();
	m_planeNode->SetProperty("reslice.thickslices.showarea", mitk::BoolProperty::New(true));
	m_dataStorage->Add(m_planeNode);

	// Initialize BottomWidget
	initializeBottomWidget(bottomWidget);
}

void CustomSliceWidget::setColor(mitk::Color color)
{
	m_planeNode->SetColor(color);
}

void CustomSliceWidget::setImageNode(mitk::DataNode::Pointer imageNode)
{
	m_imageNode = imageNode;
}

void CustomSliceWidget::initializeImage()
{
	mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(m_imageNode->GetData());
	if (image.IsNotNull())
	{
		m_sliceWidget->SetData(m_imageNode, m_viewDir);

		unsigned int steps = 0;
		unsigned int halfPos = 0;

		steps = m_sliceWidget->GetSliceNavigationController()->GetSlice()->GetSteps();
		halfPos = steps / 2;
		m_sliceWidget->GetSliceNavigationController()->GetSlice()->SetPos(halfPos);
	}
}

void CustomSliceWidget::initializeBottomWidget(QWidget* baseWidget)
{
	baseWidget->setMaximumHeight(75);

	QGridLayout* gridLayout = new QGridLayout(baseWidget);
	gridLayout->setMargin(5);
	gridLayout->setSpacing(10);
	baseWidget->setLayout(gridLayout);

	
	Qt::Alignment labelAlign;
	labelAlign.setFlag(Qt::AlignmentFlag::AlignHCenter, true);
	labelAlign.setFlag(Qt::AlignmentFlag::AlignVCenter, true);

	// Load Reslice Data
	mitk::ResliceMethodProperty::Pointer curResliceMethodProp = dynamic_cast<mitk::ResliceMethodProperty*>(m_planeNode->GetProperty("reslice.thickslices"));
	mitk::IntProperty::Pointer curResliceNumProp = dynamic_cast<mitk::IntProperty*>(m_planeNode->GetProperty("reslice.thickslices"));
	int curResliceMethodID = 0;
	int curResliceNum = 0;
	if (curResliceMethodProp.IsNotNull())
		curResliceMethodID = curResliceMethodProp->GetValueAsId();
	if (curResliceNumProp.IsNotNull())
		curResliceNum = curResliceNumProp->GetValue();
	//

	// Initialize Reslice Mode
	QLabel* labelResliceMethod = new QLabel(baseWidget);
	labelResliceMethod->setText("Reslice Method");
	labelResliceMethod->setAlignment(labelAlign);
	gridLayout->addWidget(labelResliceMethod, 0, 0);

	m_comboBoxResliceMethod = new QComboBox(baseWidget);
	m_comboBoxResliceMethod->setMaximumWidth(180);
	gridLayout->addWidget(m_comboBoxResliceMethod, 0, 1);

	mitk::ResliceMethodProperty::Pointer resliceMethod = mitk::ResliceMethodProperty::New();
	mitk::EnumerationProperty::EnumIdsContainerType& methodList = resliceMethod->GetEnumIds();
	for (auto& iter : methodList)
		m_comboBoxResliceMethod->insertItem(iter.first, QString::fromStdString(iter.second));

	m_comboBoxResliceMethod->setCurrentIndex(curResliceMethodID);
	OnResliceMethodChanged(m_comboBoxResliceMethod->currentText());

	//

	// Initialize Reslice Slider
	QLabel* labelResliceSlider = new QLabel(baseWidget);
	labelResliceSlider->setAlignment(labelAlign);
	labelResliceSlider->setText("Reslice Thickness");
	gridLayout->addWidget(labelResliceSlider, 1, 0);

	QSlider* sliderResliceNum = new QSlider(baseWidget);
	sliderResliceNum->setMinimum(0);
	sliderResliceNum->setMaximum(50);
	sliderResliceNum->setOrientation(Qt::Orientation::Horizontal);
	sliderResliceNum->setValue(curResliceNum);
	gridLayout->addWidget(sliderResliceNum, 1, 1);

	m_labelResliceNum = new QLabel(baseWidget);
	m_labelResliceNum->setAlignment(labelAlign);
	m_labelResliceNum->setText(QString::number(curResliceNum * 2 + 1));
	gridLayout->addWidget(m_labelResliceNum, 1, 2);
	//

	OnResliceNumChanged(curResliceNum);

	// Initialize Event
	connect(m_comboBoxResliceMethod, &QComboBox::currentTextChanged, this, &CustomSliceWidget::OnResliceMethodChanged);
	connect(sliderResliceNum, &QSlider::valueChanged, this, &CustomSliceWidget::OnResliceNumChanged);
}


void CustomSliceWidget::OnResliceMethodChanged(const QString& resliceMethodName)
{
	mitk::ResliceMethodProperty::Pointer resliceMethod = mitk::ResliceMethodProperty::New(resliceMethodName.toStdString());
	m_planeNode->SetProperty("reslice.thickslices", resliceMethod);

	m_sliceWidget->GetRenderer()->SendUpdateSlice();
	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void CustomSliceWidget::OnResliceNumChanged(int resliceNum)
{
	mitk::IntProperty::Pointer resliceNumProp = mitk::IntProperty::New(resliceNum);
	m_planeNode->SetProperty("reslice.thickslices.num", resliceNumProp);

	if (resliceNum == 0)
	{
		m_planeNode->SetProperty("reslice.thickslices.showarea", mitk::BoolProperty::New(false));

		// change disable
		m_planeNode->SetProperty("reslice.thickslices", mitk::ResliceMethodProperty::New(0));
	}
	else
	{
		m_planeNode->SetProperty("reslice.thickslices.showarea", mitk::BoolProperty::New(true));

		QString resliceMethodName = m_comboBoxResliceMethod->currentText();
		m_planeNode->SetProperty("reslice.thickslices", mitk::ResliceMethodProperty::New(resliceMethodName.toStdString()));
	}


	m_labelResliceNum->setText(QString::number(resliceNum * 2 + 1));

	m_sliceWidget->GetRenderer()->SendUpdateSlice();
	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}


