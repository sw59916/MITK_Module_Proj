#pragma once
#include <vector>
#include <mitkImage.h>
#include <mitkImageReadAccessor.h>
#include <mitkImageWriteAccessor.h>

struct ProcessingKernel
{
public:
	typedef std::shared_ptr<ProcessingKernel> Pointer;

	ProcessingKernel();
	ProcessingKernel(int sizeX, int sizeY, int sizeZ);
	~ProcessingKernel();

public:
	void createArray(int sizeX, int sizeY, int sizeZ);

	double getElement(int x, int y, int z);
	double* getElementPtr();
	void setElement(int x, int y, int z, double val);
	void setElement(const std::vector<double>& arr);

	itk::Index<3> getCenter();
	itk::Size<3> getKernelSize();
	int getElementCount();

public:
	template<typename TPixelType, int Dimension>
	static inline void kernelProcessing(mitk::Image::Pointer input, mitk::Image::Pointer output, TPixelType* inputBuffer, TPixelType* outputBuffer, ProcessingKernel * kernel, itk::Index<Dimension>& targetIndex);

	static ProcessingKernel::Pointer createSharpenKernel2D();
	static ProcessingKernel::Pointer createSharpenKernel3D();

	static ProcessingKernel::Pointer createGaussianKernel2D(float sigma, int size = 0);

	template<typename TPixelType>
	static ProcessingKernel::Pointer createBilatralKernel2D(mitk::Image::Pointer inputImage, TPixelType* inputImageBuffer, itk::Index<2>& targetIdx, float sigmaSpace, float sigmaColor, int kernelSize = 0);

protected:
	std::vector<double> m_kernel;
	itk::Size<3> m_size;
};


template<typename TPixelType, int Dimension>
void ProcessingKernel::kernelProcessing(mitk::Image::Pointer input, mitk::Image::Pointer output, TPixelType* inputBuffer, TPixelType* outputBuffer, ProcessingKernel * kernel, itk::Index<Dimension>& targetIndex)
{
	itk::Size<Dimension> imageSize;
	for (int i = 0; i < Dimension; i++)
		imageSize[i] = input->GetDimension(i);

	itk::Index<3> kernelCenter = kernel->getCenter();

	size_t numOfComponent = input->GetPixelType().GetNumberOfComponents();

	bool bOutOfRange = false;

	for (int dim = 0; dim < Dimension; dim++)
	{
		if (targetIndex[dim] < 0 || targetIndex[dim] >= imageSize[dim])
		{
			cout << "Kernel Processing : PixelIndex out of range" << endl;
			return;
		}
		if (targetIndex[dim] < kernelCenter[dim] || targetIndex[dim] >= imageSize[dim] - kernelCenter[dim])
			bOutOfRange = true;
	}

	if (bOutOfRange == true)
	{
		int bufferOffset = Util::Image::coordinateToBufferOffset(targetIndex, imageSize) * numOfComponent;
		for(int comp = 0; comp < numOfComponent; comp++)
			outputBuffer[bufferOffset + comp] = inputBuffer[bufferOffset + comp];
		return;
	}


	itk::Index<Dimension> inputIdx;
	itk::Index<3> kernelIdx;

	int outputBufferOffset = Util::Image::coordinateToBufferOffset(targetIndex, imageSize) * numOfComponent;

	for (int comp = 0; comp < numOfComponent; comp++)
	{
		double sumValue = 0.0;
		for (int kIdx = 0; kIdx < kernel->getElementCount(); kIdx++)
		{
			kernelIdx = Util::Image::bufferOffsetToCoordinate(kIdx, kernel->getKernelSize());

			for (int dim = 0; dim < Dimension; dim++)
				inputIdx[dim] = targetIndex[dim] + (kernelIdx[dim] - kernelCenter[dim]);

			int inputBufferOffset = Util::Image::coordinateToBufferOffset(inputIdx, imageSize) * numOfComponent + comp;
			sumValue += (double)inputBuffer[inputBufferOffset] * kernel->getElementPtr()[kIdx];
		}

		outputBuffer[outputBufferOffset + comp] = (TPixelType)std::max(0.0, std::min(sumValue, (double)std::numeric_limits<TPixelType>::max()));
	}
}

template<typename TPixelType>
ProcessingKernel::Pointer ProcessingKernel::createBilatralKernel2D(mitk::Image::Pointer inputImage, TPixelType* inputImageBuffer, itk::Index<2>& targetIdx, float sigmaSpace, float sigmaColor, int kernelSize)
{
	// change size by sigma value
	if (kernelSize == 0)
		kernelSize = sigmaSpace * 6 + 1;
	//
	// change size to odd
	if (kernelSize % 2 == 0)
		kernelSize++;
	//

	int radius = (kernelSize - 1.f) / 2.f;
	itk::Size<2> imageSize;
	for (int i = 0; i < 2; i++)
		imageSize[i] = inputImage->GetDimension(i);
	size_t numOfComponent = inputImage->GetPixelType().GetNumberOfComponents();

	// Check Out of boundary
	if (targetIdx[0] < radius || targetIdx[0] >= imageSize[0] - radius ||
		targetIdx[1] < radius || targetIdx[1] >= imageSize[1] - radius)
	{
		ProcessingKernel::Pointer tempKernel(new ProcessingKernel(1, 1, 1));
		tempKernel->setElement(0, 0, 0, 1);
		return tempKernel;
	}
	//

	ProcessingKernel::Pointer kernel(new ProcessingKernel(kernelSize, kernelSize, 1));
	std::vector<double> kernelElement(kernelSize * kernelSize);

	float sum = 0;
	int kIdx = 0;
	for (int y = -radius; y <= radius; y++)
	{
		for (int x = -radius; x <= radius; x++)
		{
			// Get Color Diff
			itk::Index<2> kernelIdx;
			kernelIdx[0] = targetIdx[0] + x;
			kernelIdx[1] = targetIdx[1] + y;
			int targetIdxBufferOffset = Util::Image::coordinateToBufferOffset(targetIdx, imageSize) * numOfComponent;
			int kernelIdxBufferOffset = Util::Image::coordinateToBufferOffset(kernelIdx, imageSize) * numOfComponent;

			double colorDiff = abs((double)(inputImageBuffer[targetIdxBufferOffset] - inputImageBuffer[kernelIdxBufferOffset]));
			//

			double spaceDist = exp(-((x * x) + (y * y)) / (2.f * sigmaSpace * sigmaSpace));
			double colorDist = exp(-(colorDiff * colorDiff) / (2.f * sigmaColor * sigmaColor));
			double kernelValue = spaceDist * colorDist;

			kernelElement[kIdx++] = kernelValue;
			sum += kernelValue;
		}
	}

	for (auto& iter : kernelElement)
		iter /= sum;

	kernel->setElement(kernelElement);

	return kernel;
}