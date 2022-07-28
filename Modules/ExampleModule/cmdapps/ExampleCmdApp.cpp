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

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	QmitkRegisterClasses();

	MainViewer mainWidget;
	try
	{
		mainWidget.initialize();
		mainWidget.show();
		mainWidget.move(300, 100);
		mainWidget.resize(1600, 1000);
	}
	catch (itk::ExceptionObject& e)
	{
		e.Print(cout);
	}


	return app.exec();
}

