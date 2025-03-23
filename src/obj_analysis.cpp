#include "obj_analysis.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace mesh_analysis {

using namespace manifold;

Manifold loadObjFile(const std::string& filename) {
  if (filename.empty()) {
    return Manifold::Cube(vec3(1.0), true);
  }

  std::cout << "Loading OBJ file: " << filename << std::endl;

  std::vector<float> vertProperties;
  std::vector<unsigned int> triVerts;

  std::ifstream file(filename);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return Manifold::Cube(vec3(1.0), true);
  }

  std::cout << "File opened successfully" << std::endl;

  std::vector<vec3> positions;
  std::vector<std::array<unsigned int, 3>> triangles;

  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    std::string token;
    iss >> token;

    if (token == "v") {
      double x, y, z;
      iss >> x >> y >> z;
      positions.push_back(vec3(x, y, z));
    } else if (token == "f") {
      std::string v1, v2, v3;
      iss >> v1 >> v2 >> v3;

      auto getIndex = [](const std::string& str) {
        return static_cast<unsigned int>(
            std::stoi(str.substr(0, str.find('/'))) - 1);
      };

      triangles.push_back({getIndex(v1), getIndex(v2), getIndex(v3)});
    }
  }

  std::cout << "Loaded " << positions.size() << " vertices and "
            << triangles.size() << " triangles" << std::endl;

  if (positions.empty() || triangles.empty()) {
    std::cerr << "No valid geometry found in OBJ file" << std::endl;
    return Manifold::Cube(vec3(1.0), true);
  }

  for (const auto& pos : positions) {
    vertProperties.push_back(static_cast<float>(pos[0]));
    vertProperties.push_back(static_cast<float>(pos[1]));
    vertProperties.push_back(static_cast<float>(pos[2]));
  }

  for (const auto& tri : triangles) {
    triVerts.push_back(tri[0]);
    triVerts.push_back(tri[1]);
    triVerts.push_back(tri[2]);
  }

  MeshGL meshGL;
  meshGL.vertProperties = vertProperties;
  meshGL.triVerts = triVerts;
  meshGL.numProp = 3;

  try {
    Manifold result(meshGL);
    std::cout << "Manifold created successfully. Status: "
              << static_cast<int>(result.Status()) << std::endl;

    if (result.Status() != Manifold::Error::NoError) {
      std::cout << "OBJ loading failed to create a valid manifold. Using "
                   "fallback cube."
                << std::endl;
      return Manifold::Cube(vec3(1.0), true);
    }

    return result;
  } catch (const std::exception& e) {
    std::cerr << "Exception creating Manifold: " << e.what() << std::endl;
    return Manifold::Cube(vec3(1.0), true);
  }
}

void analyzeMesh(const Manifold& mesh, const std::string& label) {
  std::cout << "=== " << label << " Analysis ===" << std::endl;
  std::cout << "Vertices: " << mesh.NumVert() << std::endl;
  std::cout << "Triangles: " << mesh.NumTri() << std::endl;
  std::cout << "Edges: " << mesh.NumEdge() << std::endl;
  std::cout << "Volume: " << mesh.Volume() << std::endl;
  std::cout << "Surface Area: " << mesh.SurfaceArea() << std::endl;

  Box boundingBox = mesh.BoundingBox();
  vec3 size = boundingBox.max - boundingBox.min;
  std::cout << "Bounding Box: " << std::endl;
  std::cout << "  Min: (" << boundingBox.min[0] << ", " << boundingBox.min[1]
            << ", " << boundingBox.min[2] << ")" << std::endl;
  std::cout << "  Max: (" << boundingBox.max[0] << ", " << boundingBox.max[1]
            << ", " << boundingBox.max[2] << ")" << std::endl;
  std::cout << "  Size: (" << size[0] << ", " << size[1] << ", " << size[2]
            << ")" << std::endl;

  std::cout << "Genus: " << mesh.Genus() << " (number of handles in the mesh)"
            << std::endl;
  std::cout << "Status: " << static_cast<int>(mesh.Status());

  switch (mesh.Status()) {
    case Manifold::Error::NoError:
      std::cout << " (No Error)";
      break;
    case Manifold::Error::NonFiniteVertex:
      std::cout << " (Non-Finite Vertex)";
      break;
    case Manifold::Error::NotManifold:
      std::cout << " (Not Manifold)";
      break;
    default:
      std::cout << " (Other Error)";
      break;
  }
  std::cout << std::endl;

  bool isValid = (mesh.Status() == Manifold::Error::NoError);
  std::cout << "Is Valid Manifold: " << (isValid ? "Yes" : "No") << std::endl;
  std::cout << std::endl;
}

}  // namespace mesh_analysis