#include "MainViewer.h"
#include "PlaneInteractor.h"

MainViewer::MainViewer(QWidget* parent) : QWidget(parent)
{
	ds = mitk::StandaloneDataStorage::New();
}

MainViewer::~MainViewer()
{
}

void MainViewer::initialize()
{
	setupWidget();

	initializeMouseInteraction();

	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void MainViewer::setImage(mitk::Image::Pointer image)
{
	mitk::DataNode::Pointer imageNode = mitk::DataNode::New();
	imageNode->SetData(image->Clone());
	ds->Add(imageNode);

	// image setting
	if (image.IsNotNull())
	{
		// Set Volume Render Option
		/*imageNode->SetProperty("volumerendering", mitk::BoolProperty::New(true));

		/*mitk::TransferFunction::Pointer tf = mitk::TransferFunction::New();
		tf->InitializeByMitkImage(image);*/

		// Add Color
		//tf->GetColorTransferFunction()->AddRGBPoint(5000, 0.0, 0.0, 0.0);
		//tf->GetColorTransferFunction()->AddRGBPoint(tf->GetColorTransferFunction()->GetRange()[1], 0.0, 0.0, 1.0);

		// Add Opacity
		/*tf->GetScalarOpacityFunction()->AddPoint(7000, 0);
		tf->GetScalarOpacityFunction()->AddPoint(tf->GetColorTransferFunction()->GetRange()[1], 1);

		imageNode->SetProperty("TransferFunction", mitk::TransferFunctionProperty::New(tf.GetPointer()));*/
	}

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

	auto geo = ds->ComputeBoundingGeometry3D(ds->GetAll());
	mitk::RenderingManager::GetInstance()->InitializeViews(geo);

	axialView->setImage(imageNode);
	sagittalView->setImage(imageNode);
	frontalView->setImage(imageNode);
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

	//
}

void MainViewer::initializeMouseInteraction()
{
	mitk::StandaloneDataStorage::SetOfObjects::ConstPointer dataNode = ds->GetAll();
	mitk::DataNode::Pointer imageNode = dataNode->at(0);

	auto exampleModule = us::ModuleRegistry::GetModule("MitkExampleModule");

	if (exampleModule == nullptr)
		return;

	auto interactor = PlaneInteractor::New();
	interactor->LoadStateMachine("PlaneMove.xml", exampleModule);
	interactor->SetEventConfig("PlaneMoveConfig.xml", exampleModule);
	interactor->SetDataNode(imageNode);
}
