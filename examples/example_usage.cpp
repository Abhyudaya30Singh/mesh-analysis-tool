#include <iostream>

#include "obj_analysis.h"

int main(int argc, char** argv) {
  std::string filename = argc > 1 ? argv[1] : "";

  auto mesh = mesh_analysis::loadObjFile(filename);
  mesh_analysis::analyzeMesh(mesh, "Original Mesh");

  auto cube = manifold::Manifold::Cube(manifold::vec3(1.0), true)
                  .Translate(manifold::vec3(2.0, 0, 0));

  std::cout << "\n=== Successful Merge Operations ===" << std::endl;

  auto unionMesh = mesh + cube;
  auto diffMesh = mesh - cube;

  mesh_analysis::analyzeMesh(unionMesh, "Union Result");
  mesh_analysis::analyzeMesh(diffMesh, "Difference Result");

  std::cout << "\n=== Problematic Merge Operations ===" << std::endl;

  auto zeroVolumeMesh =
      manifold::Manifold::Cube(manifold::vec3(1.0, 1.0, 0.0), true);
  mesh_analysis::analyzeMesh(zeroVolumeMesh, "Zero-volume Mesh");

  auto problemMerge = mesh + zeroVolumeMesh;
  mesh_analysis::analyzeMesh(problemMerge, "Merge with Zero-volume Mesh");

  auto partialOverlap = manifold::Manifold::Cube(manifold::vec3(1.0), true)
                            .Translate(manifold::vec3(0.5, 0.5, 0.5));
  auto complexMerge = mesh + partialOverlap;
  mesh_analysis::analyzeMesh(complexMerge, "Complex Partial Overlap Merge");

  manifold::MeshGL meshGL;
  std::vector<float> verts = {
      -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,  -1.0f, 1.0f,
      -1.0f, -1.0f, 1.0f,  1.0f,  1.0f,  -1.0f, -1.0f, 1.0f,
      -1.0f, 1.0f,  1.0f,  1.0f,  -1.0f, 1.0f,  1.0f,  1.0f,
  };

  std::vector<unsigned int> tris = {0, 1, 3, 0, 3, 2, 4, 6, 7, 4, 7, 5,
                                    0, 4, 5, 0, 5, 1, 2, 3, 7, 2, 7, 6,
                                    0, 2, 6, 0, 6, 4, 1, 5, 7, 1, 7, 3};

  meshGL.vertProperties = verts;
  meshGL.triVerts = tris;
  meshGL.numProp = 3;

  try {
    manifold::Manifold customMesh(meshGL);
    std::cout << "Successfully created custom mesh from raw data" << std::endl;
    mesh_analysis::analyzeMesh(customMesh, "Custom Mesh");

    auto merged = customMesh + cube;
    mesh_analysis::analyzeMesh(merged, "Custom Mesh + Cube");
  } catch (const std::exception& e) {
    std::cerr << "Error creating custom mesh: " << e.what() << std::endl;
  }

  std::cout << "Analysis complete!" << std::endl;

  return 0;
}