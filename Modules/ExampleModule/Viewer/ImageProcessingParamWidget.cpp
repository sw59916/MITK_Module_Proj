#include "ImageProcessingParamWidget.h"

#include <QLayout>
#include <QLabel>
#include <QLineEdit>
#include <QValidator>

MedianBlurParamWidget::MedianBlurParamWidget(QWidget* parent)
	: QMenu(parent)
{
	initialize();
}

MedianBlurParamWidget::~MedianBlurParamWidget()
{
}

void MedianBlurParamWidget::initialize()
{
	QGridLayout* parentLayout = new QGridLayout(this);
	this->setLayout(parentLayout);

	Qt::Alignment labelAlign;
	labelAlign.setFlag(Qt::AlignmentFlag::AlignHCenter, true);
	labelAlign.setFlag(Qt::AlignmentFlag::AlignVCenter, true);

	QLabel* labelRadius = new QLabel(this);
	labelRadius->setAlignment(labelAlign);
	labelRadius->setText("Radius");

	QLineEdit* editRadius = new QLineEdit(this);

	QIntValidator* intValidator = new QIntValidator;
	intValidator->setRange(1, 100);
	editRadius->setValidator(intValidator);

	editRadius->setText(QString::number(m_param.m_radius));

	parentLayout->addWidget(labelRadius, 0, 0);
	parentLayout->addWidget(editRadius, 0, 1);

	connect(editRadius, &QLineEdit::textChanged, this, &MedianBlurParamWidget::OnRadiusChanged);
}

void MedianBlurParamWidget::OnRadiusChanged(const QString& value)
{
	m_param.m_radius = value.toInt();

	emit paramChanged(m_param);
}

GaussianBlurParamWidget::GaussianBlurParamWidget(QWidget* parent)
	: QMenu(parent)
{
	initialize();
}

GaussianBlurParamWidget::~GaussianBlurParamWidget()
{

}

void GaussianBlurParamWidget::initialize()
{
	QGridLayout* parentLayout = new QGridLayout(this);
	this->setLayout(parentLayout);

	Qt::Alignment labelAlign;
	labelAlign.setFlag(Qt::AlignmentFlag::AlignHCenter, true);
	labelAlign.setFlag(Qt::AlignmentFlag::AlignVCenter, true);

	QLabel* labelSigma = new QLabel(this);
	labelSigma->setAlignment(labelAlign);
	labelSigma->setText("Sigma");

	QLineEdit* editSigma = new QLineEdit(this);
	QDoubleValidator* doubleValidator = new QDoubleValidator;
	doubleValidator->setRange(0.1, 50.f, 2);
	editSigma->setValidator(doubleValidator);
	editSigma->setText(QString::number(m_param.m_sigma));

	QLabel* labelSize = new QLabel(this);
	labelSize->setAlignment(labelAlign);
	labelSize->setText("Size");

	QLineEdit* editSize = new QLineEdit(this);
	QIntValidator* intValidator = new QIntValidator;
	intValidator->setRange(1, 50);
	editSize->setValidator(intValidator);
	editSize->setText(QString::number(m_param.m_size));

	parentLayout->addWidget(labelSigma, 0, 0);
	parentLayout->addWidget(editSigma, 0, 1);
	parentLayout->addWidget(labelSize, 1, 0);
	parentLayout->addWidget(editSize, 1, 1);

	connect(editSigma, &QLineEdit::textChanged, this, &GaussianBlurParamWidget::OnSigmaChanged);
	connect(editSize, &QLineEdit::textChanged, this, &GaussianBlurParamWidget::OnSizeChanged);
}

void GaussianBlurParamWidget::OnSigmaChanged(const QString& value)
{
	m_param.m_sigma = value.toFloat();
	emit paramChanged(m_param);
}
void GaussianBlurParamWidget::OnSizeChanged(const QString& value)
{
	m_param.m_size = value.toInt();
	emit paramChanged(m_param);
}



BilateralFilterParamWidget::BilateralFilterParamWidget(QWidget* parent)
	: QMenu(parent)
{
	initialize();
}

BilateralFilterParamWidget::~BilateralFilterParamWidget()
{
}
void BilateralFilterParamWidget::initialize()
{
	QGridLayout* parentLayout = new QGridLayout(this);
	this->setLayout(parentLayout);

	Qt::Alignment labelAlign;
	labelAlign.setFlag(Qt::AlignmentFlag::AlignHCenter, true);
	labelAlign.setFlag(Qt::AlignmentFlag::AlignVCenter, true);

	QLabel* labelSigmaSpace = new QLabel(this);
	labelSigmaSpace->setAlignment(labelAlign);
	labelSigmaSpace->setText("Sigma Space");

	QLineEdit* editSigmaSpace = new QLineEdit(this);
	QDoubleValidator* doubleValidator = new QDoubleValidator;
	doubleValidator->setRange(0.1, 50.f, 2);
	editSigmaSpace->setValidator(doubleValidator);
	editSigmaSpace->setText(QString::number(m_param.m_sigmaSpace));

	QLabel* labelSigmaColor = new QLabel(this);
	labelSigmaColor->setAlignment(labelAlign);
	labelSigmaColor->setText("Sigma Color");

	QLineEdit* editSigmaColor = new QLineEdit(this);
	doubleValidator = new QDoubleValidator;
	doubleValidator->setRange(0.1, 50.f, 2);
	editSigmaColor->setValidator(doubleValidator);
	editSigmaColor->setText(QString::number(m_param.m_sigmaColor));

	QLabel* labelSize = new QLabel(this);
	labelSize->setAlignment(labelAlign);
	labelSize->setText("Size");

	QLineEdit* editSize = new QLineEdit(this);
	QIntValidator* intValidator = new QIntValidator;
	intValidator->setRange(1, 50);
	editSize->setValidator(intValidator);
	editSize->setText(QString::number(m_param.m_size));

	parentLayout->addWidget(labelSigmaSpace, 0, 0);
	parentLayout->addWidget(editSigmaSpace, 0, 1);
	parentLayout->addWidget(labelSigmaColor, 1, 0);
	parentLayout->addWidget(editSigmaColor, 1, 1);
	parentLayout->addWidget(labelSize, 2, 0);
	parentLayout->addWidget(editSize, 2, 1);

	connect(editSigmaSpace, &QLineEdit::textChanged, this, &BilateralFilterParamWidget::OnSigmaSpaceChanged);
	connect(editSigmaColor, &QLineEdit::textChanged, this, &BilateralFilterParamWidget::OnSigmaColorChanged);
	connect(editSize, &QLineEdit::textChanged, this, &BilateralFilterParamWidget::OnSizeChanged);
}

void BilateralFilterParamWidget::OnSigmaSpaceChanged(const QString& value)
{
	m_param.m_sigmaSpace = value.toFloat();
	emit paramChanged(m_param);
}

void BilateralFilterParamWidget::OnSigmaColorChanged(const QString& value)
{
	m_param.m_sigmaColor = value.toFloat();
	emit paramChanged(m_param);
}

void BilateralFilterParamWidget::OnSizeChanged(const QString& value)
{
	m_param.m_size = value.toInt();
	emit paramChanged(m_param);
}
