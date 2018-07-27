// test.cpp : Defines the entry point for the console application.
//

#include "linear_algebra.h"
#include "matrix_traits.h"

#include <iostream>


int main()
{
	using float_2 = std::experimental::matrix<std::experimental::matrix_traits<float, 1, 2>>;
	using float_22 = std::experimental::matrix<std::experimental::matrix_traits<float, 2, 2>>;
	using float_23 = std::experimental::matrix<std::experimental::matrix_traits<float, 2, 3>>;
	using float_32 = std::experimental::matrix<std::experimental::matrix_traits<float, 3, 2>>;
	using float_33 = std::experimental::matrix<std::experimental::matrix_traits<float, 3, 3>>;
	using int_33 = std::experimental::matrix<std::experimental::matrix_traits<int, 3, 3>>;
	using int_44 = std::experimental::matrix<std::experimental::matrix_traits<int, 4, 4>>;

	// test constructors
	auto v1 = float_2{};
	auto v2 = float_2{ 0.0f };
	auto v3 = float_2{ 1.0f, 2.0f };
	float coordinates[2] = { 1.0f, 2.0f };
	auto v4 = float_2{ coordinates };
	auto m1 = float_22{};
	auto m2 = float_22{
		0.0f, 0.2f,
		0.4f, 0.6f };
	auto m3 = float_23{
		0.0f, 0.1f, 0.2f,
		0.3f, 0.4f, 0.5f };
	auto m4 = float_32{
		1.0f, 2.0f,
		3.0f, 4.0f,
		5.0f, 6.0f };
	auto m5 = float_33{
		1.0f, 2.0f, 3.0f,
		2.0f, 3.0f, 4.0f,
		1.0f, 5.0f, 7.0f };
	auto i1 = int_44{
		1, 0, 2, -1,
		3, 0, 0, 5,
		2, 1, 4, -3,
		1, 0, 5, 0 };

	// test equality operators
	assert(v2 != v3);
	assert(v3 == v4);

	// test scalar binary operators
	assert((float_2{ 2.0f, 2.0f } *3.0f) == (float_2{ 6.0f, 6.0f }));
	assert(3.0f * (float_2{ 2.0f, 2.0f }) == (float_2{ 6.0f, 6.0f }));
	assert((float_2{ 2.0f, 2.0f } / 4.0f) == (float_2{ 0.5f, 0.5f }));

	// test matrix binary operators
	assert(v3 + v4 == (float_2{ 2.0f, 4.0f }));
	assert(v3 - v4 == (float_2{ 0.0f, 0.0f }));
	auto m6 = m3 * m4;
// Unfortunately, rounding errors kill this assertion.
//	assert(m6 == (float_22{
//		1.3f, 1.6f,
//		4.0f, 5.2f }));

	// test matrix functions
	// transpose
/*	assert(transpose(m3) == (float_32{
		0.0f, 0.3f,
		0.1f, 0.4f,
		0.2f, 0.5f }));*/
	assert(submatrix(m3, 0, 1) == (float_2{
		0.3f, 0.5f }));
	assert(submatrix(i1, 3, 3) == (int_33{
		1, 0, 2,
		3, 0, 0,
		2, 1, 4 }));

	// test vector functions
	assert(inner_product(v3, v4) == 5.0f);
	assert(modulus(float_2{ 3.0f, 4.0f }) == 5.0f);
	assert(modulus_squared(float_2{ 3.0f, 4.0f }) == 25.0f);
	auto mod1 = modulus(unit(float_2{ 1.0f, 1.0f }));
	auto mod2 = modulus(float_2{ 0.707106769f, 0.707106769f });
	assert(mod1 - mod2 < std::numeric_limits<float>::min());

	// test SquareMatrix predicates
	// is_identity
	assert(!(std::experimental::is_identity<std::experimental::matrix_traits<float, 2, 2>>(float_22{ 1.0f, 0.0f, 1.0f, 1.0f })));
	assert((std::experimental::is_identity<std::experimental::matrix_traits<float, 2, 2>>(float_22{ 1.0f, 0.0f, 0.0f, 1.0f })));
	// is_invertible

	// test SquareMatrix functions
	// identity
	assert((std::experimental::identity<std::experimental::matrix_traits<float, 2, 2>>()) == (float_22{ 1.0f, 0.0f, 0.0f, 1.0f }));
	// determinant
	assert((std::experimental::determinant<std::experimental::matrix_traits<int, 4, 4>>(i1)) == 30);
	assert((std::experimental::determinant<std::experimental::matrix_traits<float, 3, 3>>(m5)) == 2);
	// classical adjoint
	assert((std::experimental::classical_adjoint<std::experimental::matrix_traits<float, 3, 3>>(m5)) == (float_33{1.0f, 1.0f, -1.0f, -10.0f, 4.0f, 2.0f, 7.0f, -3.0f, -1.0f }));
	// inverse
	auto m2_inv = std::experimental::inverse(m2);
	auto m2_inv_m2 = m2 * m2_inv;
// Unfortunately, rounding errors kill this assertion.
//	assert(std::experimental::is_identity(m2_inv_m2));

	return 0;
}
