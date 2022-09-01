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

#include <mitkIOConstants.h>

#include <algorithm>
#include <string>
#include "Viewer/MainViewer.h"
#include "Viewer/ImageProcessingWidget.h"
#include "ImageProcessing/ImageProcessing.h"
#include "Utility/UtilImage.h"
#include "Utility/UtilStopwatch.h"

#include <time.h>

void imageProcessingTest()
{
	Util::Image::RawImageInfo info;
	info.dimension = 2;
	info.pixelType = mitk::PixelType::ItkIOComponentType::UCHAR;
	info.sizeX = 1280;
	info.sizeY = 1280;
	mitk::Image::Pointer image = Util::Image::Load("D:\\imageTest\\TestImage.raw", info);
	ImageProcessing processing;

	Util::Stopwatch stopwatch;
	stopwatch.start();
	mitk::Image::Pointer sharpenResult = processing.runSharpenFilter(image);
	stopwatch.stop();
	cout << "Processing Time : " << stopwatch.getTime() << endl;

	stopwatch.start();
	mitk::Image::Pointer medianResult = processing.runMedianFilter(image, 2);
	stopwatch.stop();
	cout << "Processing Time : " << stopwatch.getTime() << endl;

	stopwatch.start();
	mitk::Image::Pointer gaussianResult = processing.runGaussianBlur(image, 2.f);
	stopwatch.stop();
	cout << "Processing Time : " << stopwatch.getTime() << endl;

	stopwatch.start();
	mitk::Image::Pointer bilateralResult = processing.runBilateralFilter(image, 2.f, 10.f);
	stopwatch.stop();
	cout << "Processing Time : " << stopwatch.getTime() << endl;


	Util::Image::Save(image, "D:\\imageTestResult\\Original.tif");
	Util::Image::Save(sharpenResult, "D:\\imageTestResult\\Sharpen.tif");
	Util::Image::Save(medianResult, "D:\\imageTestResult\\Median.tif");
	Util::Image::Save(gaussianResult, "D:\\imageTestResult\\Gaussian.tif");
	Util::Image::Save(bilateralResult, "D:\\imageTestResult\\Bilateral_30.tif");
}


int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	QmitkRegisterClasses();


	QWidget mainWidget;
	QLayout* mainLayout = new QVBoxLayout(&mainWidget);
	mainWidget.setLayout(mainLayout);

	MainViewer* mainViewer = new MainViewer(&mainWidget);
	mainLayout->addWidget(mainViewer);


	QWidget secondWidget;
	QLayout* layout = new QVBoxLayout(&secondWidget);
	secondWidget.setLayout(layout);

	ImageProcessingWidget* processingWidget = new ImageProcessingWidget(&secondWidget);
	layout->addWidget(processingWidget);

	try
	{
		mitk::Image::Pointer image = Util::Image::Load("D:\\dcmTest\\sliceDcm");



		mainViewer->initialize();
		mainViewer->setVolumeRendering(true);
		processingWidget->initialize(mainViewer->getImageNode());

		


		mainWidget.show();
		mainWidget.move(700, 100);
		mainWidget.resize(1600, 1000);

		secondWidget.show();
		secondWidget.move(300, 300);
		secondWidget.resize(400, 200);

		if (image != nullptr)
		{
			mainViewer->setImage(image);

			processingWidget->updateParamByImage();
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

