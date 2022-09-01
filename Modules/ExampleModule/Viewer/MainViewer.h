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


#include <mitkPlaneGeometryDataMapper2D.h>
#include <mitkCameraController.h>

#include "MitkExampleModuleExports.h"

#include "CustomSliceWidget.h"
#include "PlaneInteractor.h"


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

	mitk::DataNode* m_axialPlaneNode;
	mitk::DataNode* m_sagittalPlaneNode;
	mitk::DataNode* m_frontalPlaneNode;

	QmitkRenderWindow* renderWindow;
	CustomSliceWidget* axialView;
	CustomSliceWidget* sagittalView;
	CustomSliceWidget* frontalView;

	PlaneInteractor::Pointer m_interactor;

	bool m_volumeRendering;
};
