
@page math Using Vectors, Matrices, Transforms, and the Math Library

Kale comes with a custom built linear algebra and math library. Since it is custom and does not rely on popular implementations, expect it to be slower and less powerful. If you need to squeeze every bit of performance out for whatever reason, you can look towards glm, BLAS, etc (that being said, this engine likely isn't the right one for you if you need this much performance).

@section math_s1 Vectors

## Basic Usage

Kale has templated Vectors with 2, 3, and 4 dimensions. Each vector class instance is named "Vector" + the number of dimensions + the initials of the underlying type. For example a vector of 2 floats is @ref Kale::Vector2f "Vector2f". Or a vector with 3 size_t is @ref Kale::Vector3st "Vector3st". The component member names of each vector class is listed below:

- @ref Kale::Vector2 "Vector2<T>" - x, y
- @ref Kale::Vector3 "Vector3<T>" - x, y, z
- @ref Kale::Vector4 "Vector4<T>" - x, y, z, w

## Arithmetic, Boolean Operations, Floating Points, & Convenience Methods

Each vector class has overloads for all basic arithmetic and boolean operations. They can also be printed via any @ref logging methods. You also don't have to worry about floating point errors as template specialization is used on floating types to account for epsilon. Some basic algebra/linear algebra methods exist as well such as @ref Kale::Vector2#dot "dot products", @ref Kale::Vector2#cross "cross products", etc. You can also cast a vector to another type using @ref Kale::Vector2#cast "VectorN::cast<T>()". Convenience methods exist for glsl style swizzling and clamping. Static methods also exist to get vectors with predefined values such as the max or min of the types, or values initialized to point at a certain directionl.

@section math_s2 Matrices

## Basic Usage

Kale also has templated matrices for any sizes. Not all sizes are well supported however, many methods are blocked via template specialization to only work on certain smaller sizes. The template class signature is @ref Kale::Matrix "Matrix<width, height, type>". All the matrix data is stored in the @ref Kale::Matrix#data "data" member, don't be afraid to manually tinker with it if necessary. Printing of matrices is also supported via @ref logging methods.

## Accessing Elements

You can access elements from the matrix like an array via the @ref Kale::Matrix#operator[] "operator[size_t index]" overload. You can also access it by column and row via the @ref Kale::Matrix#operator()() "operator(size_t col, size_t row)" overload. The operators use debug checks and throws if you access a value out of the bounds in debug mode. @ref Kale::Matrix#get "get" method variants do not perform these checks in debug mode.

## Algebra & Operations

Matrices also support basic algebra operations such as @ref Kale::Matrix#operator+ "addition", @ref Kale::Matrix#operator- "subtraction", or @ref Kale::Matrix#operator* "multiplication". Multiplication is template specialized based on size to be fast where possible. @ref Kale::Matrix#transpose "Transposing" and some basic guassian elimination operations are also supported. You can also calculate the deteriment of the matrix via @ref Kale::Matrix#det ".det()" which is also specialized based on the size of the matrix.

## Transforms & Cameras

Transformation matrices inherit from the @ref Kale::Matrix3f "Matrix3f" class. The @ref Kale::Camera "Camera" class is just an alias for the @ref Kale::Transform "Transform" class. Transformation matrices have all the matrix functionality along with convenience methods for translations, rotations, and scaling. Instead of using multiplication to transform vectors, you can use the @ref Kale::Transform#transform "transform" method to directly transform @ref Kale::Vector2f "Vector2f"s. You can also use this method to transform some basic math geometries.

@section math_s4 Utilities & Constants

## Radians & Degrees Conversions & Methodology

There are also available functions to convert from radians to degrees (@ref Kale::radToDeg "radToDeg(float rad)") and vice versa (@ref Kale::degToRad "degToRad(float deg)"). Normally when a method requires an angle as an argument, it will also take an @ref Kale::AngleUnit "AngleUnit" enum value to determine what unit you are supplying with.

## Floating Point Value Checking & Constants

There are also functions defined to check if floating types are equal accounting for floating type error with epsilon. A few predefined constants exist for mathematical usage:

- @ref Kale::isFloating0 "isFloating0"
- @ref Kale::isFloatingEqual "isFloatingEqual"
- @ref Kale::PI "PI"
- @ref Kale::E "E"
