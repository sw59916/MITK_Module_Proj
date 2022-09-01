#include "MainViewer.h"

MainViewer::MainViewer(QWidget* parent) : QWidget(parent)
{
	ds = mitk::StandaloneDataStorage::New();
	m_imageNode = mitk::DataNode::New();
	ds->Add(m_imageNode);

	m_volumeRendering = false;
}

MainViewer::~MainViewer()
{
}

void MainViewer::initialize()
{
	setupWidget();

	initializeMouseInteraction();

	axialView->setImageNode(m_imageNode);
	sagittalView->setImageNode(m_imageNode);
	frontalView->setImageNode(m_imageNode);

	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void MainViewer::setImage(mitk::Image::Pointer image)
{
	m_imageNode->SetData(image->Clone());

	mitk::Mapper* mapper = m_imageNode->GetMapper(mitk::BaseRenderer::Standard3D);
	cout << mapper->GetNameOfClass() << endl;


	auto geo = ds->ComputeBoundingGeometry3D(ds->GetAll());
	mitk::RenderingManager::GetInstance()->InitializeViews(geo);

	setupImage();
}

void MainViewer::setVolumeRendering(bool volumeRender)
{
	m_volumeRendering = volumeRender;

	setupImage();
}

void MainViewer::setupImage()
{
	mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(m_imageNode->GetData());

	if (image.IsNull())
		return;

	// Initialize Slice Viewer
	axialView->initializeImage();
	sagittalView->initializeImage();
	frontalView->initializeImage();
	//

	// Set Volume Render Option
	m_imageNode->SetProperty("volumerendering", mitk::BoolProperty::New(m_volumeRendering));

	mitk::TransferFunction::Pointer tf = mitk::TransferFunction::New();
	tf->InitializeByMitkImage(image);

	// Add Color
	tf->GetColorTransferFunction()->AddRGBPoint(tf->GetColorTransferFunction()->GetRange()[0], 1.0, 1.0, 1.0);
	tf->GetColorTransferFunction()->AddRGBPoint(tf->GetColorTransferFunction()->GetRange()[1], 1.0, 1.0, 1.0);

	// Add Opacity
	tf->GetScalarOpacityFunction()->AddPoint(5500, 0);
	tf->GetScalarOpacityFunction()->AddPoint(7000, 1);

	/*tf->GetScalarOpacityFunction()->AddPoint(7000, 0);
	tf->GetScalarOpacityFunction()->AddPoint(10000, 1);*/

	m_imageNode->SetProperty("TransferFunction", mitk::TransferFunctionProperty::New(tf.GetPointer()));

	// Create Surface by InputImage
	//if (image.IsNotNull())
	//{
	//	mitk::ImageToSurfaceFilter::Pointer surfaceFilter = mitk::ImageToSurfaceFilter::New();
	//	surfaceFilter->SetInput(image);

	//	surfaceFilter->SetThreshold(9000.f);

	//	surfaceFilter->GetOutput()->Update();

	//	mitk::Surface::Pointer surface = surfaceFilter->GetOutput()->Clone();

	//	mitk::DataNode::Pointer surfaceNode = mitk::DataNode::New();
	//	surfaceNode->SetData(surface);

	//	ds->Add(surfaceNode);
	//}
}

void MainViewer::setupWidget()
{
	// Create MPR Layout
	QVBoxLayout* parentLayout = new QVBoxLayout(this);
	parentLayout->setSpacing(2);
	parentLayout->setMargin(0);

	QWidget* topWidget = new QWidget(this);
	parentLayout->addWidget(topWidget);

	QWidget* bottomWidget = new QWidget(this);
	parentLayout->addWidget(bottomWidget);

	QHBoxLayout* topLayout = new QHBoxLayout(topWidget);
	topLayout->setSpacing(2);
	topLayout->setMargin(0);

	QHBoxLayout* bottomLayout = new QHBoxLayout(bottomWidget);
	bottomLayout->setSpacing(2);
	bottomLayout->setMargin(0);
	//

	// Create & Initialize MPR Windows
	renderWindow = new QmitkRenderWindow(bottomWidget);
	renderWindow->GetRenderer()->SetDataStorage(ds);
	renderWindow->GetRenderer()->SetMapperID(mitk::BaseRenderer::Standard3D);



	axialView = new CustomSliceWidget(topWidget);
	topLayout->addWidget(axialView);
	axialView->initializeWidget(ds, mitk::SliceNavigationController::Axial);

	sagittalView = new CustomSliceWidget(topWidget);
	topLayout->addWidget(sagittalView);
	sagittalView->initializeWidget(ds, mitk::SliceNavigationController::Sagittal);

	frontalView = new CustomSliceWidget(bottomWidget);
	bottomLayout->addWidget(frontalView);
	frontalView->initializeWidget(ds, mitk::SliceNavigationController::Coronal);

	bottomLayout->addWidget(renderWindow);

	mitk::Color color;
	color[0] = 0.753f;
	color[1] = 0.f;
	color[2] = 0.f;
	axialView->setColor(color);

	color[0] = 0.f;
	color[1] = 0.69f;
	color[2] = 0.f;
	sagittalView->setColor(color);

	color[0] = 0.f;
	color[1] = 0.502f;
	color[2] = 1.f;
	frontalView->setColor(color);


	//
}

void MainViewer::initializeMouseInteraction()
{
	auto exampleModule = us::ModuleRegistry::GetModule("MitkExampleModule");

	if (exampleModule == nullptr)
		return;

	m_interactor = PlaneInteractor::New();
	m_interactor->LoadStateMachine("PlaneMove.xml", exampleModule);
	m_interactor->SetEventConfig("PlaneMoveConfig.xml", exampleModule);
	m_interactor->SetDataNode(m_imageNode);
}
