#include <gtest/gtest.h>

#include <fstream>
#include <sstream>

#include "obj_analysis.h"

using namespace mesh_analysis;
using namespace manifold;

TEST(ObjAnalysisTest, LoadValidObjFile) {
  std::ofstream testFile("test/models/cube.obj");
  testFile << "v -0.5 -0.5 -0.5\n";
  testFile << "v -0.5 -0.5  0.5\n";
  testFile << "v -0.5  0.5 -0.5\n";
  testFile << "v -0.5  0.5  0.5\n";
  testFile << "v  0.5 -0.5 -0.5\n";
  testFile << "v  0.5 -0.5  0.5\n";
  testFile << "v  0.5  0.5 -0.5\n";
  testFile << "v  0.5  0.5  0.5\n";
  testFile << "f 1 3 4\n";
  testFile << "f 1 4 2\n";
  testFile << "f 5 7 8\n";
  testFile << "f 5 8 6\n";
  testFile << "f 1 2 6\n";
  testFile << "f 1 6 5\n";
  testFile << "f 3 7 8\n";
  testFile << "f 3 8 4\n";
  testFile << "f 1 5 7\n";
  testFile << "f 1 7 3\n";
  testFile << "f 2 4 8\n";
  testFile << "f 2 8 6\n";
  testFile.close();

  Manifold mesh = loadObjFile("test/models/cube.obj");

  EXPECT_EQ(mesh.NumVert(), 8);
  EXPECT_EQ(mesh.NumTri(), 12);
  EXPECT_EQ(mesh.Status(), Manifold::Error::NoError);
}

TEST(ObjAnalysisTest, LoadInvalidObjFile) {
  Manifold mesh = loadObjFile("non_existent_file.obj");

  EXPECT_EQ(mesh.NumVert(), 8);
  EXPECT_EQ(mesh.NumTri(), 12);
  EXPECT_EQ(mesh.Status(), Manifold::Error::NoError);
}

TEST(ObjAnalysisTest, SuccessfulMerge) {
  Manifold cube1 = Manifold::Cube(vec3(1.0), true);
  Manifold cube2 = Manifold::Cube(vec3(1.0), true).Translate(vec3(1.5, 0, 0));

  Manifold unionMesh = cube1 + cube2;

  EXPECT_GT(unionMesh.NumVert(), 0);
  EXPECT_GT(unionMesh.NumTri(), 0);
  EXPECT_EQ(unionMesh.Status(), Manifold::Error::NoError);

  EXPECT_GT(unionMesh.NumVert(), cube1.NumVert());
}

TEST(ObjAnalysisTest, ProblemMergeOperations) {
  Manifold invalidMesh = Manifold::Cube(vec3(1.0, 1.0, 0.0), true);
  Manifold validCube = Manifold::Cube(vec3(1.0), true);

  Manifold resultMesh = validCube + invalidMesh;

  analyzeMesh(resultMesh, "Result of problematic merge");

  EXPECT_GE(resultMesh.NumVert(), validCube.NumVert());
  EXPECT_GE(resultMesh.NumTri(), validCube.NumTri());

  Manifold cube1 = Manifold::Cube(vec3(1.0), true);
  Manifold cube2 =
      Manifold::Cube(vec3(1.0), true).Translate(vec3(0.5, 0.5, 0.5));

  Manifold unionResult = cube1 + cube2;

  EXPECT_EQ(unionResult.Status(), Manifold::Error::NoError);
  EXPECT_GT(unionResult.Volume(), cube1.Volume());
}

TEST(ObjAnalysisTest, MeshMergeWithTolerance) {
  MeshGL mesh1, mesh2;

  std::vector<float> verts1 = {-1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f,
                               -1.0f, 1.0f,  -1.0f, -1.0f, 1.0f,  1.0f};

  std::vector<float> verts2 = {-0.99f, 0.99f, 0.99f, 1.0f, -1.0f,
                               -1.0f,  1.0f,  -1.0f, 1.0f, 1.0f,
                               1.0f,   -1.0f, 1.0f,  1.0f, 1.0f};

  std::vector<unsigned int> tris1 = {0, 1, 3, 0, 3, 2};

  std::vector<unsigned int> tris2 = {0, 3, 4, 0, 1, 3, 1, 4, 3, 1, 2, 4};

  mesh1.vertProperties = verts1;
  mesh1.triVerts = tris1;
  mesh1.numProp = 3;

  mesh2.vertProperties = verts2;
  mesh2.triVerts = tris2;
  mesh2.numProp = 3;

  try {
    mesh1.Merge(0.1);
    EXPECT_TRUE(false) << "Merge should fail on non-manifold mesh";
  } catch (const std::exception& e) {
    std::string error = e.what();
    EXPECT_TRUE(error.find("Manifold") != std::string::npos);
  }

  try {
    mesh2.Merge(0.1);
    std::cout << "Mesh2 merge with tolerance 0.1 successful" << std::endl;
  } catch (const std::exception& e) {
    FAIL() << "Mesh2 merge should succeed: " << e.what();
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}