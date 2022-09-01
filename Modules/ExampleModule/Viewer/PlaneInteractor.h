
#include "MitkExampleModuleExports.h"

#include <mitkInteractionEvent.h>
#include <mitkInteractionPositionEvent.h>
#include <mitkDataInteractor.h>
#include <mitkBaseRenderer.h>

class MITKEXAMPLEMODULE_EXPORT PlaneInteractor : public mitk::DataInteractor
{
public:
	mitkClassMacro(PlaneInteractor, DataInteractor)
	itkFactorylessNewMacro(Self)

private:
	PlaneInteractor();
	~PlaneInteractor();

	void ConnectActionsAndFunctions() override;
	void DataNodeChanged() override;

private:
	void moveStarted(mitk::StateMachineAction*, mitk::InteractionEvent* event);
	void moving(mitk::StateMachineAction*, mitk::InteractionEvent* event);

private:
	void move2D(mitk::BaseRenderer* sender, mitk::InteractionPositionEvent* positionEvent);
	void moveStart3D(mitk::BaseRenderer* sender, mitk::InteractionPositionEvent* positionEvent);
	void move3D(mitk::BaseRenderer* sender, mitk::InteractionPositionEvent* positionEvent);

private:
	mitk::DataNode::Pointer m_selectedPlaneNode;

	itk::Index<3> m_LastPlaneIdx;
	mitk::Point2D m_LastDisplayCoordinate;
	int m_LastPlanePos;
};