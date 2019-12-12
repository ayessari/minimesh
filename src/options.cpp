#include <memory>

#include <vtkSmartPointer.h>
#include <vtkAppendFilter.h>

#include <vtkXMLUnstructuredGridReader.h>
#include <vtkUnstructuredGrid.h>

#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkConeSource.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkCleanPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkNamedColors.h>
#include <vtkVersion.h>
#include <vtkXMLUnstructuredGridWriter.h>
#include <vtkGeometryFilter.h>
#include <vtkAppendPoints.h>

#include "viewer.hpp"
#include "options.hpp"
#include "cpptoml.h"

OptionsParser::OptionsParser(const std::string& file_name)
  : m_file_name(file_name)
{
  m_data = cpptoml::parse_file(m_file_name);
}

Params*  OptionsParser::parse(void)
{
  auto transform = m_data->get_table("transform");
  auto name = *transform->get_as<std::string>("name");

  if (name.compare("merge") == 0) {
    bool merge_nodes = *transform->get_as<bool>("merge_nodes");

    auto io = m_data->get_table("io");
    std::vector<std::string> meshes = *io->get_array_of<std::string>("inputs");
    std::string result_file_name = *io->get_as<std::string>("output");

    auto quality = m_data->get_table("quality");
    bool compute_quality = *quality->get_as<bool>("compute_quality");

    MergeParams *merge_params = new MergeParams();
    merge_params->transform_type = MERGER;
    merge_params->inputs = meshes;
    merge_params->output = result_file_name;

    return merge_params;


  } else if (name.compare("translate") == 0) {
    std::vector<double> coords = *transform->get_array_of<double>("translation");

    auto io = m_data->get_table("io");
    std::string mesh = *io->get_as<std::string>("input");
    std::string result_file_name = *io->get_as<std::string>("output");

    auto quality = m_data->get_table("quality");
    bool compute_quality = *quality->get_as<bool>("compute_quality");

    TranslationParams *translation_params = new TranslationParams();
    translation_params->transform_type = TRANSLATION;
    translation_params->xyz = coords;
    translation_params->input = mesh;
    translation_params->output = result_file_name;

    return translation_params;



  } else {
  }
}
