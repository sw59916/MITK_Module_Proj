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
	try
	{
		loadData();
	}
	catch (mitk::Exception& e)
	{
		e.Print(cout);
	}

	setupWidget();

	initializeMouseInteraction();

	mitk::RenderingManager::GetInstance()->RequestUpdateAll();
}

void MainViewer::loadData()
{
	// load image
	//mitk::IOUtil::Load("D:\\dcmTest\\DCM\\Slice0000.dcm", *ds);
	mitk::IOUtil::Load("D:\\dcmTest\\slice_512x512x512" , *ds); // \\slice0000.dcm", *ds);


	// image setting
	mitk::StandaloneDataStorage::SetOfObjects::ConstPointer dataNodes = ds->GetAll();
	mitk::DataNode::Pointer imageNode = dataNodes->at(0);
	mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(imageNode->GetData());

	if (image.IsNotNull())
	{
		/*
		imageNode->SetProperty("volumerendering", mitk::BoolProperty::New(false));
		std::vector<std::string> keys = imageNode->GetPropertyKeys();

		mitk::TransferFunction::Pointer tf = mitk::TransferFunction::New();
		tf->InitializeByMitkImage(image);

		//tf->GetColorTransferFunction()->AddRGBPoint(5000, 0.0, 0.0, 0.0);
		//tf->GetColorTransferFunction()->AddRGBPoint(tf->GetColorTransferFunction()->GetRange()[1], 0.0, 0.0, 1.0);

		tf->GetScalarOpacityFunction()->AddPoint(5000, 0);
		tf->GetScalarOpacityFunction()->AddPoint(tf->GetColorTransferFunction()->GetRange()[1], 1);

		imageNode->SetProperty("TransferFunction", mitk::TransferFunctionProperty::New(tf.GetPointer()));
		*/
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

	auto geo = ds->ComputeBoundingGeometry3D(ds->GetAll());
	mitk::RenderingManager::GetInstance()->InitializeViews(geo);


	mitk::StandaloneDataStorage::SetOfObjects::ConstPointer dataNode = ds->GetAll();
	mitk::DataNode::Pointer imageNode = dataNode->at(0);

	axialView = new CustomSliceWidget(topWidget);
	topLayout->addWidget(axialView);
	axialView->initializeWidget(ds, imageNode, mitk::SliceNavigationController::Axial);

	sagittalView = new CustomSliceWidget(topWidget);
	topLayout->addWidget(sagittalView);
	sagittalView->initializeWidget(ds, imageNode, mitk::SliceNavigationController::Sagittal);

	frontalView = new CustomSliceWidget(bottomWidget);
	bottomLayout->addWidget(frontalView);
	frontalView->initializeWidget(ds, imageNode, mitk::SliceNavigationController::Frontal);

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
