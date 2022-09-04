#pragma once

#include <QWidget>
#include <QHBoxLayout>

#include <QmitkRenderWindow.h>
#include <QmitkSliceWidget.h>

#include <mitkIOUtil.h>
#include <mitkStandaloneDataStorage.h>
#include <mitkImage.h>
#include <mitkTransferFunction.h>
#include <mitkTransferFunctionProperty.h>
#include <mitkImageToSurfaceFilter.h>
#include <mitkNodePredicateDataType.h>
#include <mitkResliceMethodProperty.h>
#include <mitkInteractionEvent.h>
#include <mitkInteractionKeyEvent.h>
#include <mitkVolumeMapperVtkSmart3D.h>


#include <mitkPlaneGeometryDataMapper2D.h>
#include <mitkCameraController.h>

#include "MitkExampleModuleExports.h"

#include "CustomSliceWidget.h"
#include "PlaneInteractor.h"


class MITKEXAMPLEMODULE_EXPORT CustomVolumeMapper : public mitk::VolumeMapperVtkSmart3D
{
public:
	mitkClassMacro(CustomVolumeMapper, VolumeMapperVtkSmart3D);

	itkFactorylessNewMacro(Self);

	itkCloneMacro(Self);

	double getRequiredFPS();
	void setRequiredFPS(double fps);

protected:
	CustomVolumeMapper();
	~CustomVolumeMapper() override;

	void GenerateDataForRenderer(mitk::BaseRenderer* renderer) override;

private:
	double m_requiredFPS;
};

class MITKEXAMPLEMODULE_EXPORT MainViewer : public QWidget
{
public:
	MainViewer(QWidget* parent = nullptr);
	virtual ~MainViewer();

public:
	virtual void initialize();
	virtual void setImage(mitk::Image::Pointer image);

public:
	void setVolumeRendering(bool volumRender);

	mitk::DataNode::Pointer getImageNode() { return m_imageNode; }
	
protected:
	virtual void setupImage();
	virtual void setupWidget();
	virtual void initializeMouseInteraction();


private:
	mitk::StandaloneDataStorage::Pointer ds;
	mitk::DataNode::Pointer m_imageNode;

	CustomVolumeMapper::Pointer m_volumeMapper;

	QmitkRenderWindow* renderWindow;
	CustomSliceWidget* axialView;
	CustomSliceWidget* sagittalView;
	CustomSliceWidget* frontalView;

	PlaneInteractor::Pointer m_interactor;

	bool m_volumeRendering;
};

