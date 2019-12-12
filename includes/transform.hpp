#pragma once
#include "cpptoml.h"
#include "viewer.hpp"
#include "options.hpp"
#include <memory>
#include "cpptoml.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <tuple>

#include <vtkXMLWriter.h>
#include <vtkSmartPointer.h>
#include <vtkAppendFilter.h>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>
#include <vtkCleanPolyData.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkAppendPoints.h>
#include <vtkTransform.h>
#include <vtkTransformFilter.h>
#include <vtkDataSetMapper.h>


enum TransformType
{ TRANSLATION, MERGER };

struct Params
{
  virtual ~Params(void) = default;
  TransformType transform_type;

};

struct MergeParams : public Params
{
  std::vector<std::string> inputs;
  std::string output;
};

struct TranslationParams : public Params
{
  std::string input, output;
  std::vector<double> xyz;

};

class Transform
{
public:
  Transform(void) {}
  ~Transform(void) {}

  void transform_merge(MergeParams *params)
  {

    auto input1 = vtkSmartPointer<vtkUnstructuredGrid>::New();
    auto input2 = vtkSmartPointer<vtkUnstructuredGrid>::New();
    std::string inputFilename1 = params->inputs[0];
    std::string inputFilename2 = params->inputs[1];
    std::string output = params->output;
    auto reader1 =vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
    reader1->SetFileName(inputFilename1.c_str());
    reader1->Update();
    input1->ShallowCopy(reader1->GetOutput());
    auto reader2 = vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
    reader2->SetFileName(inputFilename2.c_str());
    reader2->Update();
    input2->ShallowCopy(reader2->GetOutput());
    auto appendFilter = vtkSmartPointer<vtkAppendPoints>::New();
    appendFilter->AddInputData(input1);
    appendFilter->AddInputData(input2);
    auto cleanFilter =  vtkSmartPointer<vtkCleanPolyData>::New();
    cleanFilter->SetInputConnection(appendFilter->GetOutputPort());
    cleanFilter->Update();
    auto reader = vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
    reader->SetFileName(output.c_str());
    reader->Update();
    auto viewer = new Viewer();
    viewer->view_mesh(output);
    auto t= vtkSmartPointer<vtkUnstructuredGrid>::New();
    t->ShallowCopy(cleanFilter->GetOutput());
    auto writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    writer->SetFileName(output.c_str());
    writer->SetInputData(t);
    writer->Write();
  }

  void transform_translate(TranslationParams *params){
    std::string input = params->input;
    std::string output = params->output;
    auto reader1 =vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
    reader1->SetFileName(input.c_str());
    reader1->Update();
    auto originalMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    originalMapper->SetInputConnection(reader1->GetOutputPort());
    auto originalActor = vtkSmartPointer<vtkActor>::New();
    originalActor->SetMapper(originalMapper);
    auto translation = vtkSmartPointer<vtkTransform>::New();
    translation->Translate(params->xyz[0],params->xyz[1],params->xyz[2]);
    auto reader = vtkSmartPointer<vtkXMLUnstructuredGridReader>::New();
    reader->SetFileName(output.c_str());
    reader->Update();
    auto transformFilter = vtkSmartPointer<vtkTransformFilter>::New();
    transformFilter->SetInputConnection(reader->GetOutputPort());
    transformFilter->SetTransform(translation);
    transformFilter->Update();
    auto transformedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
    transformedMapper->SetInputConnection(transformFilter->GetOutputPort());
    auto transformedActor = vtkSmartPointer<vtkActor>::New();
    transformedActor->SetMapper(transformedMapper);
    auto renderer =  vtkSmartPointer<vtkRenderer>::New();
    renderer->AddActor(originalActor);
    renderer->AddActor(transformedActor);
    renderer->SetBackground(.2, .6, .2);
    auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
    auto renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    renderWindowInteractor->SetRenderWindow(renderWindow);
    renderWindow->Render();
    renderWindowInteractor->Start();
    auto t= vtkSmartPointer<vtkUnstructuredGrid>::New();
    t->ShallowCopy(transformFilter->GetOutput());
    auto writer = vtkSmartPointer<vtkXMLUnstructuredGridWriter>::New();
    writer->SetFileName(output.c_str());
    writer->SetInputData(t);
    writer->Write();
  }
};
