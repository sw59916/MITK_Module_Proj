#include "ImageProcessing.h"
#include "Utility/UtilImage.h"



ImageProcessing::ImageProcessing()
{
}

ImageProcessing::~ImageProcessing()
{

}

void ImageProcessing::runSharpenFilter(mitk::Image::Pointer inputImage, mitk::Image::Pointer outputImage)
{
	Util::Image::checkSameImageFormat(inputImage, outputImage);

	std::shared_ptr<SharpenFilterImpl> impl(new SharpenFilterImpl);
	
	impl->initialize();
	IMPL_PROCESSING_RUN_MACRO(impl, inputImage, outputImage);
}

mitk::Image::Pointer ImageProcessing::runSharpenFilter(mitk::Image::Pointer inputImage)
{
	mitk::Image::Pointer outputImage = mitk::Image::New();
	outputImage->Initialize(inputImage);
	runSharpenFilter(inputImage, outputImage);

	return outputImage;
}

void ImageProcessing::runMedianFilter(mitk::Image::Pointer inputImage, mitk::Image::Pointer outputImage, int radius)
{
	Util::Image::checkSameImageFormat(inputImage, outputImage);

	std::shared_ptr<MedianBlurImpl> impl(new MedianBlurImpl);

	impl->initialize(radius);
	IMPL_PROCESSING_RUN_MACRO(impl, inputImage, outputImage);
}

mitk::Image::Pointer ImageProcessing::runMedianFilter(mitk::Image::Pointer inputImage, int radius)
{
	mitk::Image::Pointer outputImage = mitk::Image::New();
	outputImage->Initialize(inputImage);
	runMedianFilter(inputImage, outputImage, radius);

	return outputImage;
}

void ImageProcessing::runGaussianBlur(mitk::Image::Pointer inputImage, mitk::Image::Pointer outputImage, float sigma, int size)
{
	Util::Image::checkSameImageFormat(inputImage, outputImage);

	std::shared_ptr<GaussianBlurImpl> impl(new GaussianBlurImpl);

	impl->initialize(sigma, size);
	IMPL_PROCESSING_RUN_MACRO(impl, inputImage, outputImage);
}

mitk::Image::Pointer ImageProcessing::runGaussianBlur(mitk::Image::Pointer inputImage, float sigma, int size)
{
	mitk::Image::Pointer outputImage = mitk::Image::New();
	outputImage->Initialize(inputImage);
	runGaussianBlur(inputImage, outputImage, sigma, size);

	return outputImage;
}

void ImageProcessing::runBilateralFilter(mitk::Image::Pointer inputImage, mitk::Image::Pointer outputImage, float sigmaSpace, float sigmaColor, int size)
{
	Util::Image::checkSameImageFormat(inputImage, outputImage);

	std::shared_ptr<BilateralFilterImpl> impl(new BilateralFilterImpl);

	impl->initialize(sigmaSpace, sigmaColor, size);
	IMPL_PROCESSING_RUN_MACRO(impl, inputImage, outputImage);
}

mitk::Image::Pointer ImageProcessing::runBilateralFilter(mitk::Image::Pointer inputImage, float sigmaSpace, float sigmaColor, int size)
{
	mitk::Image::Pointer outputImage = mitk::Image::New();
	outputImage->Initialize(inputImage);
	runBilateralFilter(inputImage, outputImage, sigmaSpace, sigmaColor, size);

	return outputImage;
}


ImageProcessingPipelineObject::ImageProcessingPipelineObject()
	: m_ProcessingType(ImageProcessing::ImageProcessingFilterType::None)
{
	this->SetNumberOfRequiredInputs(1);
	this->SetNumberOfRequiredOutputs(1);
}

ImageProcessingPipelineObject::~ImageProcessingPipelineObject()
{
}





void ImageProcessingPipelineObject::GenerateData()
{
	if (GetInput() == nullptr)
	{
		cout << "Input Image is not exist" << endl;
		return;
	}

	GetOutput()->Initialize(GetInput());
	switch (m_ProcessingType)
	{
	case ImageProcessing::ImageProcessingFilterType::SharpenFilter:
		m_processor.runSharpenFilter(GetInput(), GetOutput());
		break;
	case ImageProcessing::ImageProcessingFilterType::MedianBlur:
		m_processor.runMedianFilter(GetInput(), GetOutput(), m_MedianBlurParam.m_radius);
		break;
	case ImageProcessing::ImageProcessingFilterType::GaussianBlur:
		m_processor.runGaussianBlur(GetInput(), GetOutput(), m_GaussianBlurParam.m_sigma, m_GaussianBlurParam.m_size);
		break;
	case ImageProcessing::ImageProcessingFilterType::BilateralFilter:
		m_processor.runBilateralFilter(GetInput(), GetOutput(), m_BilateralFilterParam.m_sigmaSpace, m_BilateralFilterParam.m_sigmaColor, m_BilateralFilterParam.m_size);
		break;
	default:
	{
		mitk::ImageReadAccessor readAccess(this->GetInput());
		GetOutput()->SetImportVolume(readAccess.GetData());
		break;
	}
	}
}





CustomImageMapper2D::CustomImageMapper2D()
	: mitk::ImageVtkMapper2D(), m_filterInputImage(mitk::Image::New())
{
	m_processingObject.SetInput(m_filterInputImage);
}

CustomImageMapper2D::~CustomImageMapper2D()
{
}

void CustomImageMapper2D::GenerateDataForRenderer(mitk::BaseRenderer* renderer)
{
	__super::GenerateDataForRenderer(renderer);

	LocalStorage* localStorage = m_LSH.GetLocalStorage(renderer);
	vtkSmartPointer<vtkImageData> originalImage = localStorage->m_ReslicedImage;

	m_filterInputImage->Initialize(originalImage);
	m_filterInputImage->SetImportVolume(originalImage->GetScalarPointer(), 0, 0, mitk::Image::DontManageMemory);

	originalImage->DeepCopy(m_processingObject.GetVtkImageData());
}



