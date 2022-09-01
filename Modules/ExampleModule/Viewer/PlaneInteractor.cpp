#include "PlaneInteractor.h"
#include <mitkPlane.h>
#include <mitkImage.h>
#include <mitkNodePredicateDataType.h>
#include <mitkSmartPointerProperty.h>
#include <mitkPlaneGeometryDataMapper2D.h>
#include <mitkPlaneGeometryDataVtkMapper3D.h>

#include <mitkWeakPointerProperty.h>
#include <mitkPlaneGeometryDataToSurfaceFilter.h>
#include <math.h>

PlaneInteractor::PlaneInteractor()
{

}

PlaneInteractor::~PlaneInteractor()
{
}


void PlaneInteractor::ConnectActionsAndFunctions() 
{
	CONNECT_FUNCTION("planeMoveStarted", moveStarted)
	CONNECT_FUNCTION("planeMoved", moving)
}
void PlaneInteractor::DataNodeChanged()
{
	this->ResetToStartState();
}

void PlaneInteractor::moveStarted(mitk::StateMachineAction*, mitk::InteractionEvent* event)
{
	mitk::BaseRenderer* renderer = event->GetSender();
	mitk::InteractionPositionEvent* positionEvent = static_cast<mitk::InteractionPositionEvent*>(event);

	if (renderer->GetMapperID() == mitk::BaseRenderer::Standard2D)
	{
		move2D(renderer, positionEvent);
	}
	else if (renderer->GetMapperID() == mitk::BaseRenderer::Standard3D)
	{
		moveStart3D(renderer, positionEvent);
	}
}

void PlaneInteractor::moving(mitk::StateMachineAction*, mitk::InteractionEvent* event)
{
	mitk::BaseRenderer* renderer = event->GetSender();
	mitk::InteractionPositionEvent* positionEvent = static_cast<mitk::InteractionPositionEvent*>(event);

	if (renderer->GetMapperID() == mitk::BaseRenderer::Standard2D)
	{
		move2D(renderer, positionEvent);
	}
	else if (renderer->GetMapperID() == mitk::BaseRenderer::Standard3D)
	{
		move3D(renderer, positionEvent);
	}
}

void PlaneInteractor::move2D(mitk::BaseRenderer* sender, mitk::InteractionPositionEvent* positionEvent)
{
	std::vector<vtkRenderWindow*> renderWindowList = mitk::RenderingManager::GetInstance()->GetAllRegisteredRenderWindows();
	for (vtkRenderWindow* renderWinodw : renderWindowList)
	{
		if (mitk::BaseRenderer::GetInstance(renderWinodw)->GetMapperID() == mitk::BaseRenderer::Standard2D && renderWinodw != sender->GetRenderWindow())
		{
			mitk::BaseRenderer::GetInstance(renderWinodw)->GetSliceNavigationController()->SelectSliceByPoint(positionEvent->GetPositionInWorld());
		}
	}
}

void PlaneInteractor::moveStart3D(mitk::BaseRenderer* sender, mitk::InteractionPositionEvent* positionEvent)
{
	m_selectedPlaneNode = nullptr;

	mitk::DataStorage::SetOfObjects::ConstPointer planeList = sender->GetDataStorage()->GetSubset(mitk::TNodePredicateDataType<mitk::PlaneGeometryData>::New());
	for (int i = 0; i < planeList->size(); i++)
	{
		mitk::DataNode::Pointer planeNode = planeList->at(i);

		mitk::PlaneGeometryData::Pointer planeGeoData = dynamic_cast<mitk::PlaneGeometryData*>(planeNode->GetData());
		if (planeGeoData.IsNull())
		{
			cout << "PlaneNode[" << i << "] : ";
			cout << "Data is not PlaneGeometryData" << endl;
			continue;
		}

		itk::Index<3> idxPt;
		planeGeoData->GetGeometry()->WorldToIndex(positionEvent->GetPositionInWorld(), idxPt);

		if (idxPt[2] >= -1 && idxPt[2] <= 1)
		{
			mitk::WeakPointerProperty::Pointer planeRendererProp = dynamic_cast<mitk::WeakPointerProperty*>(planeNode->GetProperty("renderer"));
			mitk::BaseRenderer::Pointer planeRenderer = dynamic_cast<mitk::BaseRenderer*>(planeRendererProp->GetValue().GetPointer());
			if (planeRenderer.IsNotNull())
			{
				m_selectedPlaneNode = planeNode;
				m_LastPlaneIdx = idxPt;
				m_LastDisplayCoordinate = positionEvent->GetPointerPositionOnScreen();
				m_LastPlanePos = planeRenderer->GetSliceNavigationController()->GetSlice()->GetPos();
			}
		}
		// �������� �ڵ�........
		// PlaneGeometryData�� ����� Surface�޾ƿ����� �ڵ�
		/*mitk::Mapper* mapper = planeNode->GetMapper(renderer->GetMapperID());

		mitk::SmartPointerProperty::Pointer surfacecreatorprop;
		surfacecreatorprop = dynamic_cast<mitk::SmartPointerProperty*>(mapper->GetDataNode()->GetProperty("surfacegeometry", renderer));

		if(surfacecreatorprop.IsNull())
		{
			cout << "PlaneNode[" << i << "] : ";
			cout << "SurfaceCreator Property is not exist !!!" << endl;
			continue;
		}
		if (surfacecreatorprop->GetSmartPointer().IsNull())
		{
			cout << "PlaneNode[" << i << "] : ";
			cout << "SurfaceCreator Object is not exist !!!" << endl;
			continue;
		}

		mitk::PlaneGeometryDataToSurfaceFilter::Pointer surfaceCreator;
		surfaceCreator = dynamic_cast<mitk::PlaneGeometryDataToSurfaceFilter*>(surfacecreatorprop->GetSmartPointer().GetPointer());

		if (surfaceCreator.IsNull())
		{
			cout << "PlaneNode[" << i << "] : ";
			cout << "DataObject is not SurfaceCreator !!!" << endl;
			continue;
		}

		mitk::Surface::Pointer planeObj = surfaceCreator->GetOutput();

		if (planeObj.IsNull())
		{
			cout << "PlaneNode[" << i << "] : ";
			cout << "Output is Null !!!" << endl;
			continue;
		}*/
		//
	}
}

void PlaneInteractor::move3D(mitk::BaseRenderer* sender, mitk::InteractionPositionEvent* positionEvent)
{
	if (m_selectedPlaneNode.IsNull())
		return;

	mitk::PlaneGeometryData::Pointer planeGeometryData = dynamic_cast<mitk::PlaneGeometryData*>(m_selectedPlaneNode->GetData());
	if (planeGeometryData.IsNull())
		return;

	mitk::WeakPointerProperty::Pointer planeRendererProp = dynamic_cast<mitk::WeakPointerProperty*>(m_selectedPlaneNode->GetProperty("renderer"));
	mitk::BaseRenderer::Pointer planeRenderer = dynamic_cast<mitk::BaseRenderer*>(planeRendererProp->GetValue().GetPointer());
	if (planeRenderer.IsNull())
		return;

	mitk::Vector2D moveVector = (positionEvent->GetPointerPositionOnScreen() - m_LastDisplayCoordinate);
	moveVector *= sender->GetScaleFactorMMPerDisplayUnit();

	// Calculate 
	// PlaneObject�� Z�������� 1Pixel �������� �� World ��ǥ��, Display ��ǥ�� ��ȯ.
	// PlaneObject�� Z�������� 1Pixel ������ ��, Display ��ǥ�迡���� ��ȭ���� ��� ����.
	mitk::Vector2D zDirVector2D;
	{
		itk::Index<3> orgIdx;
		mitk::Point3D orgWorldPos;
		mitk::Point2D orgDisplayPos;

		itk::Index<3> zMoveIdx;
		mitk::Point3D zMoveWorldPos;
		mitk::Point2D zMoveDisplayPos;

		orgIdx = m_LastPlaneIdx;
		orgIdx[2] += (int)planeRenderer->GetSliceNavigationController()->GetSlice()->GetPos() - m_LastPlanePos;

		zMoveIdx = orgIdx;
		zMoveIdx[2] += 1;

		planeGeometryData->GetGeometry()->IndexToWorld(orgIdx, orgWorldPos);
		planeGeometryData->GetGeometry()->IndexToWorld(zMoveIdx, zMoveWorldPos);
		sender->WorldToDisplay(orgWorldPos, orgDisplayPos);
		sender->WorldToDisplay(zMoveWorldPos, zMoveDisplayPos);

		zDirVector2D[0] = zMoveDisplayPos[0] - orgDisplayPos[0];
		zDirVector2D[1] = zMoveDisplayPos[1] - orgDisplayPos[1];
		zDirVector2D *= sender->GetScaleFactorMMPerDisplayUnit();
	}
	

	// Mouse �̵�������, ������ ���� ����vector�� 0�� �������� X ��ȭ���� ������.
	// X, Y�� �����̷� �� ��, �������̰� �ϱ�����.
	double mouseMovingDistance = 0;
	{
		double planeZAxisAngle = atan2l(zDirVector2D[1], zDirVector2D[0]);

		mitk::Point2D rotateMousePos;
		rotateMousePos[0] = cosl(-planeZAxisAngle) * moveVector[0] - sinl(-planeZAxisAngle) * moveVector[1];
		rotateMousePos[1] = sinl(-planeZAxisAngle) * moveVector[0] + cosl(-planeZAxisAngle) * moveVector[1];

		mouseMovingDistance = rotateMousePos[0];
	}

	// Z�� 1Pixel ������ ��, �̵��� ���.
	double zAxisDistance = sqrtl((zDirVector2D[0] * zDirVector2D[0]) + (zDirVector2D[1] * zDirVector2D[1]));

	// PlaneObject ���� �̵��� ���.
	double targetDistance = mouseMovingDistance / zAxisDistance;
	int targetPos = m_LastPlanePos + (int)targetDistance;
	targetPos = std::max(0, std::min(targetPos, (int)planeRenderer->GetSliceNavigationController()->GetSlice()->GetSteps() - 1));

	// Moving
	planeRenderer->GetSliceNavigationController()->GetSlice()->SetPos(targetPos);
}