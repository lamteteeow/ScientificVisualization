#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkImageReader2Factory.h>
#include <vtkSkybox.h>
#include <vtkImageReader2.h>
#include <vtkTexture.h>
#include <vtkPNGReader.h>

// Create class Stars
class Stars
{
public:
	Stars()
	{
		// Assign path to a variable
		std::string path = "C:/Users/Admin/Code Projects/Scientific Visualization framework/scivis-ss23-main/images/panorama_image.png";

		// Load panorama image using vtkImageReader2Factory
		vtkSmartPointer<vtkImageReader2Factory> readerFactory = vtkSmartPointer<vtkImageReader2Factory>::New();
		vtkSmartPointer<vtkImageReader2> reader = readerFactory->CreateImageReader2(path.c_str());
		reader->SetFileName(path.c_str());
		//reader->Update();

		// Create an instance of vtkTexture and assign the image
		mTexture = vtkSmartPointer<vtkTexture>::New();
		mTexture->SetInputConnection(reader->GetOutputPort());

		// Create an instance of vtkSkybox, assign the texture, and set the projection to sphere
		mSkybox = vtkSmartPointer<vtkSkybox>::New();
		mSkybox->SetTexture(mTexture);
		mSkybox->SetProjectionToSphere();

		//reader->Delete();
	}

	void InitRenderer(vtkSmartPointer<vtkRenderer> renderer)
	{
		renderer->AddActor(mSkybox);
	}

private:
	vtkSmartPointer<vtkTexture> mTexture;
	vtkSmartPointer<vtkSkybox> mSkybox;
};