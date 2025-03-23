#pragma once
#include <manifold/manifold.h>

#include <string>
#include <vector>

namespace mesh_analysis {

manifold::Manifold loadObjFile(const std::string& filename);

void analyzeMesh(const manifold::Manifold& mesh, const std::string& label);

}  // namespace mesh_analysis