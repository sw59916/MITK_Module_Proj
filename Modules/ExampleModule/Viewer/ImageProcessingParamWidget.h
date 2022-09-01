#pragma once

#include "MitkExampleModuleExports.h"

#include <QWidget>
#include <QMenu>
#include "ImageProcessing/ImageProcessing.h"

class MedianBlurParamWidget : public QMenu
{
	Q_OBJECT
public:
	MedianBlurParamWidget(QWidget* parent = nullptr);
	~MedianBlurParamWidget();

public:
	void initialize();

signals:
	void paramChanged(ImageProcessing::MedianBlurParam param);

private slots:
	void OnRadiusChanged(const QString& value);

private:
	ImageProcessing::MedianBlurParam m_param;
};


class GaussianBlurParamWidget : public QMenu
{
	Q_OBJECT
public:
	GaussianBlurParamWidget(QWidget* parent = nullptr);
	~GaussianBlurParamWidget();

public:
	void initialize();

signals:
	void paramChanged(ImageProcessing::GaussianBlurParam param);

private slots:
	void OnSigmaChanged(const QString& value);
	void OnSizeChanged(const QString& value);

private:
	ImageProcessing::GaussianBlurParam m_param;
};

class BilateralFilterParamWidget : public QMenu
{
	Q_OBJECT
public:
	BilateralFilterParamWidget(QWidget* parent = nullptr);
	~BilateralFilterParamWidget();

public:
	void initialize();

signals:
	void paramChanged(ImageProcessing::BilateralFilterParam param);

private slots:
	void OnSigmaSpaceChanged(const QString& value);
	void OnSigmaColorChanged(const QString& value);
	void OnSizeChanged(const QString& value);

private:
	ImageProcessing::BilateralFilterParam m_param;
};