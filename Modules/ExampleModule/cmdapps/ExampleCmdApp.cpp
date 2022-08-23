/*============================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center (DKFZ)
All rights reserved.

Use of this source code is governed by a 3-clause BSD license that can be
found in the LICENSE file.

============================================================================*/

#include <qapplication.h>
#include <QWidget.h>
#include "QmitkRegisterClasses.h"

#include <algorithm>
#include <string>
#include "MainViewer.h"
#include "ImageProcessing.h"



/*void imageProcessingTest()
{
	std::vector<std::string> fileList;
	fileList.push_back("D:\\imageTest\\TestImage.bmp");
	fileList.push_back("D:\\imageTest\\TestImage.tif");
	fileList.push_back("D:\\imageTest\\TestImage.png");
	//fileList.push_back("D:\\dcmTest\\slice_512x512x512\\");
	//fileList.push_back("D:\\imageTest\\TestImage.raw");

	std::vector<std::string> sharpenFileList;
	sharpenFileList.push_back("D:\\imageTestResult\\sharpen.bmp");
	sharpenFileList.push_back("D:\\imageTestResult\\sharpen.tif");
	sharpenFileList.push_back("D:\\imageTestResult\\sharpen.png");

	std::vector<std::string> medianFileList;
	medianFileList.push_back("D:\\imageTestResult\\median.bmp");
	medianFileList.push_back("D:\\imageTestResult\\median.tif");
	medianFileList.push_back("D:\\imageTestResult\\median.png");

	ImageProcessing processing;

	try
	{
		std::vector<mitk::BaseData::Pointer> data = mitk::IOUtil::Load(fileList);

		for (int i = 0; i < data.size(); i++)
		{
			mitk::BaseData::Pointer dataPtr = data.at(i);
			mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(dataPtr.GetPointer());
			if (image.IsNotNull())
			{
				mitk::Image::Pointer output = processing.runSharpenFilter(image);
				mitk::IOUtil::Save(output, sharpenFileList[i]);

				mitk::Image::Pointer medianOutput = processing.runMedianFilter(image, 3);
				mitk::IOUtil::Save(medianOutput, medianFileList[i]);
			}
		}

	}
	catch (mitk::Exception& e)
	{
		e.Print(cout);
	}
}*/



int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	QmitkRegisterClasses();


	QWidget mainWidget;

	QLayout* mainLayout = new QVBoxLayout(&mainWidget);
	mainWidget.setLayout(mainLayout);

	MainViewer* mainViewer = new MainViewer(&mainWidget);
	mainLayout->addWidget(mainViewer);

	try
	{
		mainViewer->initialize();

		mainWidget.show();
		mainWidget.move(300, 100);
		mainWidget.resize(1600, 1000);

		std::vector<std::string> fileList;
		fileList.push_back("D:\\imageTest\\TestImage.bmp");
		fileList.push_back("D:\\imageTest\\TestImage.tif");
		fileList.push_back("D:\\imageTest\\TestImage.png");
		fileList.push_back("D:\\dcmTest\\slice_512x512x512\\");

		std::vector<mitk::BaseData::Pointer> loadDataList = mitk::IOUtil::Load(fileList);
		if (loadDataList.size() > 0)
		{
			mitk::BaseData::Pointer data = loadDataList.at(1);
			mitk::Image::Pointer image = dynamic_cast<mitk::Image*>(data.GetPointer());
			if (image != nullptr)
			{
				mainViewer->setImage(image);
			}
		}
	}
	catch (itk::ExceptionObject& e)
	{
		e.Print(cout);
	}
	catch (std::exception& e)
	{
		cout << e.what();
	}


	return app.exec();
}

