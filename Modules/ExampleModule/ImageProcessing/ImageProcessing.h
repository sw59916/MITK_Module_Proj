#pragma once

#include <MitkExampleModuleExports.h>

#include <vector>
#include <mitkImage.h>
#include <mitkImageToImageFilter.h>
#include <mitkImageVtkMapper2D.h>
#include "ImageProcessingImpl.h"





class MITKEXAMPLEMODULE_EXPORT ImageProcessing
{
public:
	ImageProcessing();
	~ImageProcessing();

	enum class ImageProcessingFilterType
	{
		None, SharpenFilter, MedianBlur, GaussianBlur, BilateralFilter
	};

	struct MedianBlurParam
	{
		MedianBlurParam() { m_radius = 1; }
		MedianBlurParam(int radius) { m_radius = radius; }
		~MedianBlurParam() {}

		bool operator!=(const MedianBlurParam& rhs) const
		{
			return this->m_radius != rhs.m_radius;
		}

		int m_radius;
	};

	struct GaussianBlurParam
	{
		GaussianBlurParam() { m_sigma = 1; m_size = 0; }
		GaussianBlurParam(float sigma, int size = 0) { m_sigma = sigma, m_size = size; }
		~GaussianBlurParam() {}

		bool operator!=(const GaussianBlurParam& rhs) const
		{
			return this->m_sigma != rhs.m_sigma || this->m_size != rhs.m_size;
		}

		float m_sigma;
		int m_size;
	};

	struct BilateralFilterParam
	{
		BilateralFilterParam() { m_sigmaSpace = 1.f; m_sigmaColor = 1.f, m_size = 0; }
		BilateralFilterParam(float sigmaSpace, float sigmaColor, int size) { m_sigmaSpace = sigmaSpace, m_sigmaColor = sigmaColor; m_size = size; }
		~BilateralFilterParam() {}

		bool operator!=(const BilateralFilterParam& rhs) const
		{
			return this->m_sigmaSpace != rhs.m_sigmaSpace || this->m_sigmaColor != rhs.m_sigmaColor || this->m_size != rhs.m_size;
		}

		float m_sigmaSpace;
		float m_sigmaColor;
		int m_size;
	};

public:
	void runSharpenFilter(mitk::Image::Pointer inputImage, mitk::Image::Pointer outputImage);
	mitk::Image::Pointer runSharpenFilter(mitk::Image::Pointer inputImage);

	void runMedianFilter(mitk::Image::Pointer inputImage, mitk::Image::Pointer outputImage, int radius);
	mitk::Image::Pointer runMedianFilter(mitk::Image::Pointer inputImage, int radius);

	void runGaussianBlur(mitk::Image::Pointer inputImage, mitk::Image::Pointer outputImage, float sigma, int size = 0);
	mitk::Image::Pointer runGaussianBlur(mitk::Image::Pointer inputImage, float sigma, int size = 0);

	void runBilateralFilter(mitk::Image::Pointer inputImage, mitk::Image::Pointer outputImage, float sigmaSpace, float sigmaColor, int size = 0);
	mitk::Image::Pointer runBilateralFilter(mitk::Image::Pointer inputImage, float sigmaSpace, float sigmaColor, int size = 0);
};


class MITKEXAMPLEMODULE_EXPORT ImageProcessingPipelineObject : public mitk::ImageToImageFilter
{
public:
	ImageProcessingPipelineObject();
	~ImageProcessingPipelineObject();

	itkGetMacro(ProcessingType, ImageProcessing::ImageProcessingFilterType);
	itkSetMacro(ProcessingType, ImageProcessing::ImageProcessingFilterType);

	itkGetMacro(MedianBlurParam, ImageProcessing::MedianBlurParam);
	itkSetMacro(MedianBlurParam, ImageProcessing::MedianBlurParam);
	itkGetMacro(GaussianBlurParam, ImageProcessing::GaussianBlurParam);
	itkSetMacro(GaussianBlurParam, ImageProcessing::GaussianBlurParam);
	itkGetMacro(BilateralFilterParam, ImageProcessing::BilateralFilterParam);
	itkSetMacro(BilateralFilterParam, ImageProcessing::BilateralFilterParam);

public:
	virtual void GenerateData() override;

private:
	ImageProcessing m_processor;

	ImageProcessing::ImageProcessingFilterType m_ProcessingType;
	ImageProcessing::MedianBlurParam m_MedianBlurParam;
	ImageProcessing::GaussianBlurParam m_GaussianBlurParam;
	ImageProcessing::BilateralFilterParam m_BilateralFilterParam;
};


class MITKEXAMPLEMODULE_EXPORT CustomImageMapper2D : public mitk::ImageVtkMapper2D
{
public:
	/** Standard class typedefs. */
	mitkClassMacro(CustomImageMapper2D, ImageVtkMapper2D);

	/** Method for creation through the object factory. */
	itkFactorylessNewMacro(Self);
	itkCloneMacro(Self);

public:
	CustomImageMapper2D();
	virtual ~CustomImageMapper2D();

public:
	virtual void GenerateDataForRenderer(mitk::BaseRenderer* renderer) override;

public:
	ImageProcessingPipelineObject& getProcessingObject() { return m_processingObject; }

private:
	mitk::Image::Pointer m_filterInputImage;
	ImageProcessingPipelineObject m_processingObject;
};