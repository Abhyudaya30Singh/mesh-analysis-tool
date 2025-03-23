# Mesh Analysis Tool

A tool for loading, analyzing and manipulating 3D meshes using the Manifold library.

## Build

```
mkdir build
cd build
cmake ..
make
```

## Usage

Load and analyze a mesh:

```cpp
auto mesh = mesh_analysis::loadObjFile("model.obj");
mesh_analysis::analyzeMesh(mesh, "My Model");
```

Merge operations:

```cpp
auto mesh1 = mesh_analysis::loadObjFile("model1.obj");
auto mesh2 = mesh_analysis::loadObjFile("model2.obj");

auto unionMesh = mesh1 + mesh2;
```

## Tests

The test cases demonstrate both successful and problematic merge operations.

Run the tests:

```
cd build
./test/test_obj_analysis
```

## Example

Try the example program:

```
cd build
./examples/example_analysis my_model.obj
```
