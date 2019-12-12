#pragma once

#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPolyDataReader.h>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkDataSetMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>



class Viewer
{
public:
  Viewer() {}
  ~Viewer(void) {}

public:
  const void view_mesh(std::string file){
    vtkSmartPointer<vtkXMLUnstructuredGridReader> reader = vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
    reader->SetFileName(file.c_str());
    reader->Update();

    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    mapper->SetInputConnection(reader->GetOutputPort());
    vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper(mapper);

    vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);
    actor->GetProperty()->EdgeVisibilityOn();
    renderer->AddActor(actor);
    renderer->SetBackground(.3, .6, .3);

    renderWindow->Render();
    renderWindowInteractor->Start();
}
};
