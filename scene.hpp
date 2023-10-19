#pragma once

#include "grid.hpp"
#include "sun.hpp"
#include "earth.hpp"
#include "tracer.hpp"
#include "stars.hpp"

#include <memory>

#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>

/// <summary>
/// Class that stores all content of the 3D scene.
/// </summary>
class Scene
{
public:
	/// <summary>
	/// Constructor. Allocates the scene content.
	/// </summary>
	Scene() :
		mGrid(std::make_unique<Grid>()),
		mSun(std::make_unique<Sun>()),
		mEarth(std::make_unique<Earth>()),
		mTracer(std::make_unique<Tracer>()),
		mStars(std::make_unique<Stars>())
	{
	}

	/// <summary>
	/// Initialization function that lets all scene elements add their props to the renderer.
	/// </summary>
	/// <param name="renderer">Renderer to add the props to.</param>
	void InitRenderer(vtkSmartPointer<vtkRenderer> renderer)
	{
		// add actors of all scene elements to the renderer
		mGrid->InitRenderer(renderer);
		mSun->InitRenderer(renderer);
		mEarth->InitRenderer(renderer);
		mTracer->InitRenderer(renderer);
		mStars->InitRenderer(renderer);
	}

	/// <summary>
	/// Initialization function that allows scene elements to add UI elements to the interactor.
	/// </summary>
	/// <param name="renderWindowInteractor">Interactor to add UI elements to.</param>
	void InitUI(vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor)
	{
	}

	/// <summary>
	/// Updates the content of the scene.
	/// </summary>
	/// <param name="dt">Time passed since the last Update in milliseconds.</param>
	/// <param name="t">Total time passed since start of the application in milliseconds.</param>
	void Update(double dt, double t)
	{
		mEarth->Update(dt, t * 0.001);
		mTracer->Update(dt, t * 0.001);
		mSun->Update(dt, t * 0.001);
	}

	/// <summary>
	/// Event handler that is called when the user picked the world coordinate pnt.
	/// </summary>
	/// <param name="pnt">3D world coordinate that was picked.</param>
	void Pick(const Vector3d& pnt)
	{
		mTracer->Pick(pnt);
	}

private:
	Scene(const Scene&) = delete;						// Delete the copy-constructor.
	void operator=(const Scene&) = delete;				// Delete the assignment operator.

	std::unique_ptr<Grid> mGrid;						// a reference grid to provide spatial context
	std::unique_ptr<Sun> mSun;							// First massive body: Sun
	std::unique_ptr<Earth> mEarth;						// Second massive body: Earth
	std::unique_ptr<Tracer> mTracer;					// Tracer for the third body with marginal mass.
	std::unique_ptr<Stars> mStars;						// Stars panorama background
};