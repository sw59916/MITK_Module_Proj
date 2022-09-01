#pragma once
#include <mitkImage.h>
#include <omp.h>

#include "ImageProcessingKernel.h"


#define IMPL_PROCESSING_RUN_MACRO(implClassPointer, inputImage, outputImage)		\
switch (inputImage->GetPixelType().GetComponentType())								\
{																					\
case mitk::PixelType::ItkIOComponentType::UCHAR:									\
{ implClassPointer->run<unsigned char>(inputImage, outputImage); break; }			\
case mitk::PixelType::ItkIOComponentType::CHAR:										\
{ implClassPointer->run<char>(inputImage, outputImage); break; }					\
case mitk::PixelType::ItkIOComponentType::USHORT:									\
{ implClassPointer->run<unsigned short>(inputImage, outputImage); break; }			\
case mitk::PixelType::ItkIOComponentType::SHORT:									\
{ implClassPointer->run<short>(inputImage, outputImage); break; }					\
case mitk::PixelType::ItkIOComponentType::UINT:										\
{ implClassPointer->run<unsigned int>(inputImage, outputImage); break; }			\
case mitk::PixelType::ItkIOComponentType::INT:										\
{ implClassPointer->run<int>(inputImage, outputImage); break; }						\
case mitk::PixelType::ItkIOComponentType::ULONG:									\
{ implClassPointer->run<unsigned long>(inputImage, outputImage); break; }			\
case mitk::PixelType::ItkIOComponentType::LONG:										\
{ implClassPointer->run<long>(inputImage, outputImage); break; }					\
case mitk::PixelType::ItkIOComponentType::LONGLONG:									\
{ implClassPointer->run<long long>(inputImage, outputImage); break; }				\
case mitk::PixelType::ItkIOComponentType::ULONGLONG:								\
{ implClassPointer->run<unsigned long long>(inputImage, outputImage); break; }		\
case mitk::PixelType::ItkIOComponentType::FLOAT:									\
{ implClassPointer->run<float>(inputImage, outputImage); break; }					\
case mitk::PixelType::ItkIOComponentType::DOUBLE:									\
{ implClassPointer->run<double>(inputImage, outputImage); break; }					\
case mitk::PixelType::ItkIOComponentType::LDOUBLE:									\
{ implClassPointer->run<long double>(inputImage, outputImage); break; }				\
}																					\

class SharpenFilterImpl
{
public:
	SharpenFilterImpl() {}
	virtual ~SharpenFilterImpl() {}

public:
	void initialize() {}

	template<typename TPixelType>
	void run(mitk::Image::Pointer inputImage, mitk::Image::Pointer outputImage);
};

class MedianBlurImpl
{
public:
	MedianBlurImpl() { m_radius = 1; }
	virtual ~MedianBlurImpl() {}

public:
	void initialize(int radius) { m_radius = radius; }

	template<typename TPixelType>
	void run(mitk::Image::Pointer inputImage, mitk::Image::Pointer outputImage);

private:
	int m_radius;
};

class GaussianBlurImpl
{
public:
	GaussianBlurImpl() { m_sigma = 1.f; m_kernelSize = 0; }
	virtual ~GaussianBlurImpl() {}

public:
	void initialize(float sigma, int kernelSize) { m_sigma = sigma; m_kernelSize = kernelSize; }

	template<typename TPixelType>
	void run(mitk::Image::Pointer inputImage, mitk::Image::Pointer outputImage);

private:
	float m_sigma;
	int m_kernelSize;
};

class BilateralFilterImpl
{
public:
	BilateralFilterImpl() { m_sigmaSpace = 1.f; m_sigmaColor = 1.f; m_kernelSize = 0; }
	virtual ~BilateralFilterImpl() {}

public:
	void initialize(float sigmaSpace, float sigmaColor, int kernelSize) { m_sigmaSpace = sigmaSpace; m_sigmaColor = sigmaColor; m_kernelSize = kernelSize; }

	template<typename TPixelType>
	void run(mitk::Image::Pointer inputImage, mitk::Image::Pointer outputImage);

private:
	float m_sigmaSpace;
	float m_sigmaColor;
	int m_kernelSize;
};









template<typename TPixelType>
void SharpenFilterImpl::run(mitk::Image::Pointer inputImage, mitk::Image::Pointer outputImage)
{
	mitk::ImageReadAccessor inputAcc(inputImage);
	mitk::ImageWriteAccessor outputAcc(outputImage);
	TPixelType* inputBuffer = (TPixelType*)inputAcc.GetData();
	TPixelType* outputBuffer = (TPixelType*)outputAcc.GetData();

	if (inputImage->GetDimension() == 2)
	{
		ProcessingKernel::Pointer kernel = ProcessingKernel::createSharpenKernel2D();
		
#pragma omp parallel for
		for (int y = 0; y < inputImage->GetDimension(1); y++)
		{
			for (int x = 0; x < inputImage->GetDimension(0); x++)
			{
				itk::Index<2> targetIdx;
				targetIdx[0] = x;
				targetIdx[1] = y;

				ProcessingKernel::kernelProcessing<TPixelType, 2>(inputImage, outputImage, inputBuffer, outputBuffer, kernel.get(), targetIdx);
			}
		}
	}
	else if (inputImage->GetDimension() == 3)
	{
		ProcessingKernel::Pointer kernel = ProcessingKernel::createSharpenKernel3D();

#pragma omp parallel for
		for (int z = 0; z < inputImage->GetDimension(2); z++)
		{
			for (int y = 0; y < inputImage->GetDimension(1); y++)
			{
				for (int x = 0; x < inputImage->GetDimension(0); x++)
				{
					itk::Index<3> targetIdx;
					targetIdx[0] = x;
					targetIdx[1] = y;
					targetIdx[2] = z;

					ProcessingKernel::kernelProcessing<TPixelType, 3>(inputImage, outputImage, inputBuffer, outputBuffer, kernel.get(), targetIdx);
				}
			}
		}
	}
}

template<typename TPixelType>
void MedianBlurImpl::run(mitk::Image::Pointer inputImage, mitk::Image::Pointer outputImage)
{
	if (inputImage->GetDimension() == 2)
	{
		mitk::ImageReadAccessor inputAcc(inputImage);
		mitk::ImageWriteAccessor outputAcc(outputImage);
		TPixelType* inputBuffer = (TPixelType*)inputAcc.GetData();
		TPixelType* outputBuffer = (TPixelType*)outputAcc.GetData();

		int numOfComponent = inputImage->GetPixelType().GetNumberOfComponents();

		itk::Size<2> imageSize;
		imageSize[0] = inputImage->GetDimension(0);
		imageSize[1] = inputImage->GetDimension(1);
		

#pragma omp parallel for
		for (int y = 0; y < inputImage->GetDimension(1); y++)
		{
			for (int x = 0; x < inputImage->GetDimension(0); x++)
			{
				itk::Index<2> targetIdx;
				targetIdx[0] = x;
				targetIdx[1] = y;

				for (int comp = 0; comp < numOfComponent; comp++)
				{
					int outputBufferOffset = Util::Image::coordinateToBufferOffset(targetIdx, imageSize) * numOfComponent + comp;

					if (x < m_radius || x >= inputImage->GetDimension(0) - m_radius ||
						y < m_radius || y >= inputImage->GetDimension(1) - m_radius)
					{
						outputBuffer[outputBufferOffset] = inputBuffer[outputBufferOffset];
						continue;
					}

					itk::Index<2> kernelIdx;
					std::vector<TPixelType> pixelVector;
					for (int ky = -m_radius; ky <= m_radius; ky++)
					{
						for (int kx = -m_radius; kx <= m_radius; kx++)
						{
							kernelIdx[0] = targetIdx[0] + kx;
							kernelIdx[1] = targetIdx[1] + ky;

							int inputBufferOffset = Util::Image::coordinateToBufferOffset(kernelIdx, imageSize) * numOfComponent + comp;
							pixelVector.push_back(inputBuffer[inputBufferOffset]);
						}
					}

					std::sort(pixelVector.begin(), pixelVector.end());
					outputBuffer[outputBufferOffset] = pixelVector[pixelVector.size() / 2];
				}
			}
		}
	}
	else if (inputImage->GetDimension() == 3)
	{
		itk::Index<3> targetIdx;
		for (int z = 0; z < inputImage->GetDimension(2); z++)
		{
			for (int y = 0; y < inputImage->GetDimension(1); y++)
			{
				for (int x = 0; x < inputImage->GetDimension(0); x++)
				{
					targetIdx[0] = x;
					targetIdx[1] = y;
					targetIdx[2] = z;

				}
			}
		}
	}
}

template<typename TPixelType>
void GaussianBlurImpl::run(mitk::Image::Pointer inputImage, mitk::Image::Pointer outputImage)
{
	mitk::ImageReadAccessor inputAcc(inputImage);
	mitk::ImageWriteAccessor outputAcc(outputImage);
	TPixelType* inputBuffer = (TPixelType*)inputAcc.GetData();
	TPixelType* outputBuffer = (TPixelType*)outputAcc.GetData();

	if (inputImage->GetDimension() == 2)
	{
		ProcessingKernel::Pointer kernel = ProcessingKernel::createGaussianKernel2D(m_sigma, m_kernelSize);

#pragma omp parallel for
		for (int y = 0; y < inputImage->GetDimension(1); y++)
		{
			for (int x = 0; x < inputImage->GetDimension(0); x++)
			{
				itk::Index<2> targetIdx;
				targetIdx[0] = x;
				targetIdx[1] = y;

				ProcessingKernel::kernelProcessing<TPixelType, 2>(inputImage, outputImage, inputBuffer, outputBuffer, kernel.get(), targetIdx);
			}
		}
	}
	else if (inputImage->GetDimension() == 3)
	{
	}
}

template<typename TPixelType>
void BilateralFilterImpl::run(mitk::Image::Pointer inputImage, mitk::Image::Pointer outputImage)
{
	mitk::ImageReadAccessor inputAcc(inputImage);
	mitk::ImageWriteAccessor outputAcc(outputImage);
	TPixelType* inputBuffer = (TPixelType*)inputAcc.GetData();
	TPixelType* outputBuffer = (TPixelType*)outputAcc.GetData();

	// change size by sigma value
	if (m_kernelSize == 0)
		m_kernelSize = m_sigmaSpace * 6 + 1;
	//
	// change size to odd
	if (m_kernelSize % 2 == 0)
		m_kernelSize++;
	//

	if (inputImage->GetDimension() == 2)
	{
#pragma omp parallel for
		for (int y = 0; y < inputImage->GetDimension(1); y++)
		{
			for (int x = 0; x < inputImage->GetDimension(0); x++)
			{
				itk::Index<2> targetIdx;
				targetIdx[0] = x;
				targetIdx[1] = y;

				ProcessingKernel::Pointer bilateralKernel = ProcessingKernel::createBilatralKernel2D<TPixelType>(inputImage, inputBuffer, targetIdx, m_sigmaSpace, m_sigmaColor, m_kernelSize);

				ProcessingKernel::kernelProcessing<TPixelType, 2>(inputImage, outputImage, inputBuffer, outputBuffer, bilateralKernel.get(), targetIdx);
			}
		}

		// kernel value test
		/*{
			cout << std::fixed;
			cout.precision(6);


			targetIdx[0] = 662;
			targetIdx[1] = 560;
			ProcessingKernel::Pointer bilateralKernel = ProcessingKernel::createBilatralKernel2D<TPixelType>(inputImage, targetIdx, m_sigmaSpace, m_sigmaColor, m_kernelSize);
			
			for (int y = 0; y < bilateralKernel->getKernelSize()[1]; y++)
			{
				for (int x = 0; x < bilateralKernel->getKernelSize()[0]; x++)
				{
					cout << bilateralKernel->getElement(x, y, 0) << " ";
				}
				cout << endl;
			}
			cout << endl;

			targetIdx[0] = 662;
			targetIdx[1] = 555;
			bilateralKernel = ProcessingKernel::createBilatralKernel2D<TPixelType>(inputImage, targetIdx, m_sigmaSpace, m_sigmaColor, m_kernelSize);

			for (int y = 0; y < bilateralKernel->getKernelSize()[1]; y++)
			{
				for (int x = 0; x < bilateralKernel->getKernelSize()[0]; x++)
				{
					cout << bilateralKernel->getElement(x, y, 0) << " ";
				}
				cout << endl;
			}
			cout << endl;

			targetIdx[0] = 679;
			targetIdx[1] = 559;
			bilateralKernel = ProcessingKernel::createBilatralKernel2D<TPixelType>(inputImage, targetIdx, m_sigmaSpace, m_sigmaColor, m_kernelSize);

			for (int y = 0; y < bilateralKernel->getKernelSize()[1]; y++)
			{
				for (int x = 0; x < bilateralKernel->getKernelSize()[0]; x++)
				{
					cout << bilateralKernel->getElement(x, y, 0) << " ";
				}
				cout << endl;
			}
			cout << endl;

			return;
		}*/
		//
	}
	else if (inputImage->GetDimension() == 3)
	{
	}
}
