#pragma once

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QmitkSliceWidget.h>

#include <mitkImage.h>
#include <mitkResliceMethodProperty.h>


class CustomSliceWidget : public QWidget
{
	Q_OBJECT
public:
	CustomSliceWidget(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);
	virtual ~CustomSliceWidget();

public:
	void initializeWidget(mitk::StandaloneDataStorage::Pointer dataStorage, mitk::DataNode::Pointer imageNode, mitk::SliceNavigationController::ViewDirection viewDir);

	mitk::DataNode* getPlaneNode() { return m_planeNode; }

protected:
	virtual void initializeBottomWidget(QWidget* baseWidget);

private slots:
	void OnResliceMethodChanged(const QString& resliceMethodName);
	void OnResliceNumChanged(int resliceNum);

private:
	mitk::StandaloneDataStorage* m_dataStorage;
	mitk::DataNode* m_planeNode;

	QmitkSliceWidget* m_sliceWidget;
	QComboBox* m_comboBoxResliceMethod;

	QLabel* m_labelResliceNum;
};

