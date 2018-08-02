#if !defined MATRIX_TRAITS_18_07_29_15_03_45
#define MATRIX_TRAITS_18_07_29_15_03_45

#include <numeric>
#include <algorithm>
#include <functional>
#include <cassert>

namespace std {
	namespace experimental {
		template <class Scalar>
		struct matrix_impl
		{
			template <class InputIterator>
			static constexpr bool equal(InputIterator lhs, InputIterator end, InputIterator rhs) noexcept;

			template <class InputIterator>
			static constexpr bool not_equal(InputIterator lhs, InputIterator end, InputIterator rhs) noexcept;

			template <class InputIterator, class OutputIterator>
			static constexpr OutputIterator matrix_multiply_scalar(InputIterator lhs, InputIterator end, OutputIterator res, Scalar rhs) noexcept;

			template <class InputIterator, class OutputIterator>
			static constexpr InputIterator matrix_multiply_matrix(InputIterator lhs, InputIterator rhs, OutputIterator res, size_t m, size_t n, size_t q) noexcept;

			template <class InputIterator, class OutputIterator>
			static constexpr OutputIterator divide(InputIterator lhs, InputIterator end, OutputIterator res, Scalar rhs);

			template <class InputIterator, class OutputIterator>
			static constexpr OutputIterator add(InputIterator lhs, InputIterator end, InputIterator rhs, OutputIterator res) noexcept;

			template <class InputIterator, class OutputIterator>
			static constexpr OutputIterator subtract(InputIterator lhs, InputIterator end, InputIterator rhs, OutputIterator res) noexcept;

			template <class InputIterator>
			static constexpr bool is_identity(InputIterator lhs, size_t m) noexcept;

			template <class OutputIterator>
			static constexpr OutputIterator identity(OutputIterator res, size_t m) noexcept;

			template <class InputIterator>
			static constexpr Scalar determinant(InputIterator lhs, size_t m) noexcept;

			template <class InputIterator, class OutputIterator>
			static constexpr InputIterator inverse(InputIterator lhs, OutputIterator res, size_t m, size_t n);	// TO DO

			template <class InputIterator>
			static constexpr Scalar inner_product(InputIterator lhs, InputIterator end, InputIterator rhs) noexcept;

			template <class InputIterator>
			static constexpr Scalar modulus(InputIterator lhs, InputIterator end) noexcept;

			template <class InputIterator>
			static constexpr Scalar modulus_squared(InputIterator lhs, InputIterator end) noexcept;

			template <class InputIterator, class OutputIterator>
			static constexpr OutputIterator unit(InputIterator lhs, InputIterator end, OutputIterator res);

			template <class InputIterator, class OutputIterator>
			static constexpr OutputIterator submatrix(InputIterator lhs, OutputIterator res, size_t m, size_t n, size_t i, size_t j) noexcept;
		};

		////////////////////////////////////////////////////////
		// matrix_traits
		////////////////////////////////////////////////////////
		template<class Scalar, size_t RowCount, size_t ColCount> struct matrix_traits;

		template<class Scalar, size_t RowCount, size_t ColCount>
		struct matrix_traits_base
		{
			static const size_t row = RowCount;
			static const size_t col = ColCount;
			struct matrix_type {
				constexpr matrix_type() noexcept = default;
				matrix_type(std::initializer_list<Scalar>) noexcept;			// Pass by value or rref?
				matrix_type(Scalar const(&)[RowCount * ColCount]) noexcept;		// Really, this should be a span or a range
				Scalar _Data[RowCount * ColCount];
			};

			template<size_t OtherRow, size_t OtherCol>
			using other = matrix_traits<Scalar, OtherRow, OtherCol>;
			using matrix_t = matrix_type;
			using scalar_t = Scalar;

			static constexpr bool equal(matrix_t const& lhs, matrix_t const& rhs) noexcept;
			static constexpr bool not_equal(matrix_t const& lhs, matrix_t const& rhs) noexcept;
			static constexpr matrix_t matrix_multiply_scalar(matrix_t const& lhs, scalar_t const& rhs) noexcept;
			template <size_t ColCount2> static constexpr typename matrix_traits<Scalar, RowCount, ColCount2>::matrix_t matrix_multiply_matrix(matrix_t const& lhs, typename matrix_traits<Scalar, ColCount, ColCount2>::matrix_t const& rhs) noexcept;
			static constexpr matrix_t divide(matrix_t const& lhs, scalar_t const& rhs) noexcept;
			static constexpr matrix_t add(matrix_t const& lhs, matrix_t const& rhs) noexcept;
			static constexpr matrix_t subtract(matrix_t const& lhs, matrix_t const& rhs) noexcept;
		};

		template<class Scalar>
		struct unity_matrix_traits : public matrix_traits_base<Scalar, 1, 1>
		{
			template<typename... Args> unity_matrix_traits(Args&&... args) noexcept;

			using matrix_t = typename matrix_traits_base<Scalar, 1, 1>::matrix_t;
			using scalar_t = Scalar;

			static const size_t row = 1;
			static const size_t col = 1;

			static constexpr bool is_identity(matrix_t const& mat) noexcept;
			static constexpr matrix_t identity() noexcept;
			static constexpr scalar_t determinant(matrix_t const& mat) noexcept;
			static constexpr matrix_t inverse(matrix_t const& mat);
		};

		template<class Scalar, size_t RowCount, size_t ColCount>
		struct vector_traits : public matrix_traits_base<Scalar, RowCount, ColCount>
		{
			template<typename... Args> vector_traits(Args&&... args) noexcept;

			using matrix_t = typename matrix_traits_base<Scalar, RowCount, ColCount>::matrix_t;
			using scalar_t = Scalar;

			static const size_t row = RowCount;
			static const size_t col = ColCount;

			static constexpr scalar_t inner_product(matrix_t const& lhs, matrix_t const& rhs) noexcept;
			static constexpr scalar_t modulus(matrix_t const& mat) noexcept;
			static constexpr scalar_t modulus_squared(matrix_t const& mat) noexcept;
			static constexpr matrix_t unit(matrix_t const& mat) noexcept;
		};

		template<class Scalar, size_t RowCount, size_t ColCount>
		struct non_vector_traits : public matrix_traits_base<Scalar, RowCount, ColCount>
		{
			template<typename... Args> non_vector_traits(Args&&... args) noexcept;

			using matrix_t = typename matrix_traits_base<Scalar, RowCount, ColCount>::matrix_t;
			using scalar_t = Scalar;

			static const size_t row = RowCount;
			static const size_t col = ColCount;

			static constexpr typename matrix_traits<Scalar, RowCount - 1, ColCount - 1>::matrix_t submatrix(matrix_t const& mat, size_t m, size_t n) noexcept;
		};

		template<class Scalar, size_t RowCount>
		struct square_matrix_traits : public non_vector_traits<Scalar, RowCount, RowCount>
		{
			template<typename... Args> square_matrix_traits(Args&&... args) noexcept;

			using matrix_t = typename non_vector_traits<Scalar, RowCount, RowCount>::matrix_t;
			using scalar_t = Scalar;

			static const size_t row = RowCount;
			static const size_t col = RowCount;

			static constexpr bool is_identity(matrix_t const& mat) noexcept;
			static constexpr matrix_t identity() noexcept;
			static constexpr scalar_t determinant(matrix_t const& mat) noexcept;
			static constexpr typename square_matrix_traits<Scalar, RowCount>::matrix_t classical_adjoint(matrix_t const& mat) noexcept;
			static constexpr matrix_t inverse(matrix_t const& mat);
		};

		template<class Scalar, size_t RowCount, size_t ColCount>
		struct matrix_traits : public std::conditional_t < RowCount == 1 && ColCount == 1, unity_matrix_traits<Scalar>,
			std::conditional_t<RowCount == 1 || ColCount == 1, vector_traits<Scalar, RowCount, ColCount>,
			std::conditional_t<RowCount == ColCount, square_matrix_traits<Scalar, RowCount>,
			non_vector_traits<Scalar, RowCount, ColCount>>>>
		{
			template<typename... Args> matrix_traits(Args&&... args) noexcept;

			using parent = std::conditional_t < RowCount == 1 && ColCount == 1, unity_matrix_traits<Scalar>,
				std::conditional_t<RowCount == 1 || ColCount == 1, vector_traits<Scalar, RowCount, ColCount>,
				std::conditional_t<RowCount == ColCount, square_matrix_traits<Scalar, RowCount>,
				non_vector_traits<Scalar, RowCount, ColCount>>>>;
			using matrix_t = typename parent::matrix_t;
			using scalar_t = Scalar;

			static const size_t row = RowCount;
			static const size_t col = ColCount;
		};

		template<class Scalar, size_t RowCount, size_t ColCount>
		static constexpr typename matrix_traits<Scalar, ColCount, RowCount>::matrix_t transpose(typename matrix_traits<Scalar, RowCount, ColCount>::matrix_t const& mat) noexcept;

		////////////////////////////////////////////////////////
		// dynamic_matrix_traits
		////////////////////////////////////////////////////////
		template<class Scalar, class Allocator = std::allocator<Scalar>>
		struct dynamic_matrix_traits
		{
			struct matrix_type {
				constexpr matrix_type() noexcept = default;
				matrix_type(size_t, size_t, std::initializer_list<Scalar>) noexcept;	// Pass by value or rref?
				matrix_type(std::unique_ptr<Scalar>) noexcept;							// Really, this should be a span or a range
				size_t _Row;
				size_t _Col;
				std::unique_ptr<Scalar> _Data;
			};

			using other = dynamic_matrix_traits<Scalar, Allocator>;
			using matrix_t = matrix_type;
			using scalar_t = Scalar;

			static constexpr bool equal(matrix_t const& lhs, matrix_t const& rhs) noexcept;
			static constexpr bool not_equal(matrix_t const& lhs, matrix_t const& rhs) noexcept;
			static constexpr matrix_t matrix_multiply_scalar(matrix_t const& lhs, scalar_t const& rhs) noexcept;
			static constexpr matrix_t matrix_multiply_matrix(matrix_t const& lhs, matrix_t const& rhs);
			static constexpr matrix_t divide(matrix_t const& lhs, scalar_t const& rhs);
			static constexpr matrix_t add(matrix_t const& lhs, matrix_t const& rhs);
			static constexpr matrix_t subtract(matrix_t const& lhs, matrix_t const& rhs);
			static constexpr bool is_identity(matrix_t const& mat) noexcept;
			static constexpr matrix_t identity(size_t) noexcept;
			static constexpr scalar_t determinant(matrix_t const& mat);
			static constexpr matrix_t inverse(matrix_t const& mat);
			static constexpr scalar_t inner_product(matrix_t const& lhs, matrix_t const& rhs);
			static constexpr scalar_t modulus(matrix_t const& mat);
			static constexpr scalar_t modulus_squared(matrix_t const& mat);
			static constexpr matrix_t unit(matrix_t const& mat);
			static constexpr matrix_t submatrix(matrix_t const& mat, size_t m, size_t n);
		};

		template<class Scalar, class Allocator = std::allocator<Scalar>>
		static constexpr typename dynamic_matrix_traits<Scalar, Allocator>::matrix_t transpose(typename dynamic_matrix_traits<Scalar, Allocator>::matrix_t const& mat) noexcept;
	}
}

////////////////////////////////////////////////////////
// matrix_impl implementation
////////////////////////////////////////////////////////
template <class Scalar>
template <class InputIterator>
constexpr bool std::experimental::matrix_impl<Scalar>::equal(InputIterator lhs, InputIterator end, InputIterator rhs) noexcept
{
	return std::equal(lhs, end, rhs);
}

template <class Scalar>
template <class InputIterator>
constexpr bool std::experimental::matrix_impl<Scalar>::not_equal(InputIterator lhs, InputIterator end, InputIterator rhs) noexcept
{
	return !equal(lhs, end, rhs);
}

template<class Scalar>
template <class InputIterator, class OutputIterator>
inline constexpr OutputIterator std::experimental::matrix_impl<Scalar>::matrix_multiply_scalar(InputIterator lhs, InputIterator end, OutputIterator res, Scalar rhs) noexcept
{
	return std::transform(lhs, end, res, [&](const auto& el) {return el * rhs; });
}

template<class Scalar>
template <class InputIterator, class OutputIterator>
inline constexpr InputIterator std::experimental::matrix_impl<Scalar>::matrix_multiply_matrix(InputIterator lhs, InputIterator rhs, OutputIterator res, size_t m, size_t n, size_t q) noexcept
{
	for (auto i = 0U; i < m; ++i)
	{
		for (auto j = 0U; j < q; ++j)
		{
			auto dp = Scalar(0);
			for (auto k = 0U; k < n; ++k)
			{
				dp += *lhs++ * *rhs;
				rhs += q;
			}
			*res++ = dp;
			lhs -= n;
			rhs -= q * n;
			++rhs;
		}
		lhs += n;
		rhs -= q;
	}
	return res;
}

template<class Scalar>
template <class InputIterator, class OutputIterator>
inline constexpr OutputIterator std::experimental::matrix_impl<Scalar>::divide(InputIterator lhs, InputIterator end, OutputIterator res, Scalar rhs)
{
	return std::transform(lhs, end, res, [&](const auto& el) {return el / rhs; });
}

template<class Scalar>
template <class InputIterator, class OutputIterator>
inline constexpr OutputIterator std::experimental::matrix_impl<Scalar>::add(InputIterator lhs, InputIterator end, InputIterator rhs, OutputIterator res) noexcept
{
	return std::transform(lhs, end, rhs, res, [&](const auto& lel, const auto& rel) {return lel + rel; });
}

template<class Scalar>
template <class InputIterator, class OutputIterator>
inline constexpr OutputIterator std::experimental::matrix_impl<Scalar>::subtract(InputIterator lhs, InputIterator end, InputIterator rhs, OutputIterator res) noexcept
{
	return std::transform(lhs, end, rhs, res, [&](const auto& lel, const auto& rel) {return lel - rel; });
}

template<class Scalar>
template <class InputIterator>
inline constexpr bool std::experimental::matrix_impl<Scalar>::is_identity(InputIterator lhs, size_t m) noexcept
{
	auto x = m + 1;
	for (auto y = 0; y != m * m; ++y, ++lhs)
	{
		if (x == (m + 1) && *lhs != Scalar(1))
		{
			return false;
		}
		else if (x != m + 1)
		{
			if (*lhs != Scalar(0)) return false;
		}
		if (--x == 0) x = m + 1;
	}
	return true;
}

template<class Scalar>
template <class OutputIterator>
inline constexpr OutputIterator std::experimental::matrix_impl<Scalar>::identity(OutputIterator res, size_t m) noexcept
{
	auto r = res;
	auto x = m + 1;
	for (auto y = 0; y != m * m; ++y, ++res)
	{
		if (x == (m + 1))
		{
			*res = Scalar(1);
		}
		else if (x != m + 1)
		{
			*res = Scalar(0);
		}
		if (--x == 0) x = m + 1;
	}
	return r;
}

template<class Scalar>
template <class InputIterator>
inline constexpr Scalar std::experimental::matrix_impl<Scalar>::determinant(InputIterator lhs, size_t m) noexcept
{
	if (m == 1U) return *lhs;
	else if (m == 2U) return (*lhs * *(lhs + 3U)) - (*(lhs + 1U) * *(lhs + 2U));
	else if (m > 2U)
	{
		auto det = Scalar(0);
		auto sign = Scalar(1);
		for (auto f = 0U; f < m; ++f)
		{
			std::vector<Scalar> mat;
			mat.reserve((m - 1) * (m - 1));
			auto sub = matrix_impl<Scalar>::submatrix(lhs, std::back_inserter(mat), m, m, 0U, f);
			auto cofactor = sign * *(lhs + f) * matrix_impl<Scalar>::determinant(sub, m - 1U);
			det += cofactor;
			sign = -sign;
		}
		return det;
	}
}

template<class Scalar>
template <class InputIterator>
inline constexpr Scalar std::experimental::matrix_impl<Scalar>::inner_product(InputIterator lhs, InputIterator end, InputIterator rhs) noexcept
{
	return std::inner_product(lhs, end, rhs, Scalar(0));
}

template<class Scalar>
template <class InputIterator>
inline constexpr Scalar std::experimental::matrix_impl<Scalar>::modulus(InputIterator lhs, InputIterator end) noexcept
{
	return Scalar(std::sqrt(modulus_squared(lhs, end)));
}

template<class Scalar>
template <class InputIterator>
inline constexpr Scalar std::experimental::matrix_impl<Scalar>::modulus_squared(InputIterator lhs, InputIterator end) noexcept
{
	return std::accumulate(lhs, end, Scalar(0), [&](Scalar tot, const auto& el) {return tot + (el * el); });
}

template<class Scalar>
template <class InputIterator, class OutputIterator>
inline constexpr OutputIterator std::experimental::matrix_impl<Scalar>::unit(InputIterator lhs, InputIterator end, OutputIterator res)
{
	auto mod = modulus(lhs, end);
	return std::transform(lhs, end, res, [&](const auto& el) { return el / mod; });
}

template<class Scalar>
template <class InputIterator, class OutputIterator>
inline constexpr OutputIterator std::experimental::matrix_impl<Scalar>::submatrix(InputIterator lhs, OutputIterator res, size_t m, size_t n, size_t i, size_t j) noexcept
{
	for (auto r = 0U; r < m; ++r)
	{
		for (auto c = 0U; c < n; ++c)
		{
			if (r != i && c != j)
			{
				*res++ = *lhs;
			}
			++lhs;
		}
	}
	return res;
}

////////////////////////////////////////////////////////
// matrix_traits implementation
////////////////////////////////////////////////////////
// matrix_traits_base
template<class Scalar, size_t RowCount, size_t ColCount>
inline std::experimental::matrix_traits_base<Scalar, RowCount, ColCount>::matrix_type::matrix_type(std::initializer_list<Scalar> il) noexcept
{
	assert(il.size() <= row * col);
	std::copy(std::begin(il), std::end(il), _Data);
}

template<class Scalar, size_t RowCount, size_t ColCount>
inline std::experimental::matrix_traits_base<Scalar, RowCount, ColCount>::matrix_type::matrix_type(Scalar const(&src)[RowCount * ColCount]) noexcept
{
	std::copy(src, src + (row * col), _Data);
}

template<class Scalar, size_t RowCount, size_t ColCount>
inline constexpr bool std::experimental::matrix_traits_base<Scalar, RowCount, ColCount>::equal(matrix_t const& lhs, matrix_t const& rhs) noexcept
{
	return matrix_impl<Scalar>::equal(lhs._Data, lhs._Data + (row * col), rhs._Data);
}

template<class Scalar, size_t RowCount, size_t ColCount>
inline constexpr bool std::experimental::matrix_traits_base<Scalar, RowCount, ColCount>::not_equal(matrix_t const& lhs, matrix_t const& rhs) noexcept
{
	return matrix_impl<Scalar>::not_equal(lhs._Data, lhs._Data + (row * col), rhs._Data);
}

template<class Scalar, size_t RowCount, size_t ColCount>
inline constexpr typename std::experimental::matrix_traits_base<Scalar, RowCount, ColCount>::matrix_t std::experimental::matrix_traits_base<Scalar, RowCount, ColCount>::matrix_multiply_scalar(typename matrix_traits_base<Scalar, RowCount, ColCount>::matrix_t const& lhs, typename matrix_traits_base<Scalar, RowCount, ColCount>::scalar_t const& rhs) noexcept
{
	auto res(lhs);
	matrix_impl<Scalar>::matrix_multiply_scalar(lhs._Data, lhs._Data + (row * col), res._Data, rhs);
	return res;
}

template<class Scalar, size_t RowCount, size_t ColCount1>
template<size_t ColCount2>
inline constexpr typename std::experimental::matrix_traits<Scalar, RowCount, ColCount2>::matrix_t std::experimental::matrix_traits_base<Scalar, RowCount, ColCount1>::matrix_multiply_matrix(typename matrix_traits_base<Scalar, RowCount, ColCount1>::matrix_t const& lhs, typename matrix_traits<Scalar, ColCount1, ColCount2>::matrix_t const& rhs) noexcept
{
	auto res = typename matrix_traits<Scalar, RowCount, ColCount2>::matrix_t{};
	matrix_impl<Scalar>::matrix_multiply_matrix(lhs._Data, rhs._Data, res._Data, row, col, ColCount2);
	return res;
}

template<class Scalar, size_t RowCount, size_t ColCount>
inline constexpr typename std::experimental::matrix_traits_base<Scalar, RowCount, ColCount>::matrix_t std::experimental::matrix_traits_base<Scalar, RowCount, ColCount>::divide(typename matrix_traits_base<Scalar, RowCount, ColCount>::matrix_t const& lhs, typename matrix_traits_base<Scalar, RowCount, ColCount>::scalar_t const& rhs) noexcept
{
	auto res(lhs);
	matrix_impl<Scalar>::divide(lhs._Data, lhs._Data + (row * col), res._Data, rhs);
	return res;
}

template<class Scalar, size_t RowCount, size_t ColCount>
inline constexpr typename std::experimental::matrix_traits_base<Scalar, RowCount, ColCount>::matrix_t std::experimental::matrix_traits_base<Scalar, RowCount, ColCount>::add(typename matrix_traits_base<Scalar, RowCount, ColCount>::matrix_t const& lhs, typename matrix_traits_base<Scalar, RowCount, ColCount>::matrix_t const& rhs) noexcept
{
	auto res(lhs);
	matrix_impl<Scalar>::add(lhs._Data, lhs._Data + (row * col), rhs._Data, res._Data);
	return res;
}

template<class Scalar, size_t RowCount, size_t ColCount>
inline constexpr typename std::experimental::matrix_traits_base<Scalar, RowCount, ColCount>::matrix_t std::experimental::matrix_traits_base<Scalar, RowCount, ColCount>::subtract(typename matrix_traits_base<Scalar, RowCount, ColCount>::matrix_t const& lhs, typename matrix_traits_base<Scalar, RowCount, ColCount>::matrix_t const& rhs) noexcept
{
	auto res(lhs);
	matrix_impl<Scalar>::subtract(lhs._Data, lhs._Data + (row * col), rhs._Data, res._Data);
	return res;
}

template<class Scalar, size_t RowCount, size_t ColCount>
inline constexpr typename std::experimental::matrix_traits<Scalar, RowCount - 1, ColCount - 1>::matrix_t std::experimental::non_vector_traits<Scalar, RowCount, ColCount>::submatrix(typename matrix_traits_base<Scalar, RowCount, ColCount>::matrix_t const& mat, size_t i, size_t j) noexcept
{
	auto res = typename matrix_traits<Scalar, RowCount - 1, ColCount - 1>::matrix_t{};
	matrix_impl<Scalar>::submatrix(mat._Data, res._Data, row, col, i, j);
	return res;
}

// unity_matrix_traits

template<class Scalar>
template<typename... Args>
inline std::experimental::unity_matrix_traits<Scalar>::unity_matrix_traits(Args&&... args) noexcept
	: matrix_traits_base<Scalar, 1, 1>(std::forward(args...))
{}

template<class Scalar>
inline constexpr Scalar std::experimental::unity_matrix_traits<Scalar>::determinant(typename unity_matrix_traits<Scalar>::matrix_t const& mat) noexcept
{
	return mat._Data[0];
}

// vector_traits

template<class Scalar, size_t RowCount, size_t ColCount>
template<typename... Args>
inline std::experimental::vector_traits<Scalar, RowCount, ColCount>::vector_traits(Args&&... args) noexcept
	: matrix_traits_base<Scalar, RowCount, ColCount>(std::forward(args...))
{}

template<class Scalar, size_t RowCount, size_t ColCount>
inline constexpr Scalar std::experimental::vector_traits<Scalar, RowCount, ColCount>::inner_product(typename vector_traits<Scalar, RowCount, ColCount>::matrix_t const& lhs, typename vector_traits<Scalar, RowCount, ColCount>::matrix_t const& rhs) noexcept
{
	return matrix_impl<Scalar>::inner_product(lhs._Data, lhs._Data + (row * col), rhs._Data);
}

template<class Scalar, size_t RowCount, size_t ColCount>
inline constexpr Scalar std::experimental::vector_traits<Scalar, RowCount, ColCount>::modulus(typename vector_traits<Scalar, RowCount, ColCount>::matrix_t const& mat) noexcept
{
	return matrix_impl<Scalar>::modulus(mat._Data, mat._Data + (row * col));
}

template<class Scalar, size_t RowCount, size_t ColCount>
inline constexpr Scalar std::experimental::vector_traits<Scalar, RowCount, ColCount>::modulus_squared(typename vector_traits<Scalar, RowCount, ColCount>::matrix_t const& mat) noexcept
{
	return matrix_impl<Scalar>::modulus_squared(mat._Data, mat._Data + (row * col));
}

template<class Scalar, size_t RowCount, size_t ColCount>
inline constexpr typename std::experimental::vector_traits<Scalar, RowCount, ColCount>::matrix_t std::experimental::vector_traits<Scalar, RowCount, ColCount>::unit(typename vector_traits<Scalar, RowCount, ColCount>::matrix_t const& mat) noexcept
{
	auto res(mat);
	matrix_impl<Scalar>::unit(mat._Data, mat._Data + (row * col), res._Data);
	return res;
}

// square_matrix_traits
template<class Scalar, size_t RowCount>
template<typename... Args>
inline std::experimental::square_matrix_traits<Scalar, RowCount>::square_matrix_traits(Args&&... args) noexcept
	: non_vector_traits<Scalar, RowCount, RowCount>(std::forward(args...))
{}

template<class Scalar, size_t RowCount>
inline constexpr bool std::experimental::square_matrix_traits<Scalar, RowCount>::is_identity(typename square_matrix_traits<Scalar, RowCount>::matrix_t const& mat) noexcept
{
	return matrix_impl<Scalar>::is_identity(mat._Data, row);
}

template<class Scalar, size_t RowCount>
inline constexpr typename std::experimental::square_matrix_traits<Scalar, RowCount>::matrix_t std::experimental::square_matrix_traits<Scalar, RowCount>::identity() noexcept
{
	auto res = matrix_t{};
	matrix_impl<Scalar>::identity(res._Data, row);
	return res;
}

template<class Scalar, size_t RowCount>
inline constexpr Scalar std::experimental::square_matrix_traits<Scalar, RowCount>::determinant(typename square_matrix_traits<Scalar, RowCount>::matrix_t const& mat) noexcept
{
	if constexpr (RowCount == 1) return mat._Data[0];
	else if constexpr (RowCount == 2) return (mat._Data[0] * mat._Data[3]) - (mat._Data[1] * mat._Data[2]);
	else if constexpr (RowCount > 2)
	{
		auto det = scalar_t(0);
		auto sign = scalar_t(1);
		for (auto f = 0; f < RowCount; ++f)
		{
			auto sub = matrix_traits<Scalar, RowCount, RowCount>::submatrix(mat, 0, f);
			auto cofactor = sign * mat._Data[f] * matrix_traits<Scalar, RowCount - 1, RowCount - 1>::determinant(sub);
			det += cofactor;
			sign = -sign;
		}
		return det;
	}
}

template<class Scalar, size_t RowCount>
inline constexpr typename std::experimental::square_matrix_traits<Scalar, RowCount>::matrix_t std::experimental::square_matrix_traits<Scalar, RowCount>::classical_adjoint(typename square_matrix_traits<Scalar, RowCount>::matrix_t const& mat) noexcept
{
	auto res = matrix_t{};
	for (auto i = 0; i < RowCount; ++i)
	{
		auto sign = i % 2 == 0 ? scalar_t(1) : scalar_t(-1);
		for (auto j = 0; j < RowCount; ++j)
		{
			auto sub = matrix_traits<Scalar, RowCount, RowCount>::submatrix(mat, i, j);
			auto det = matrix_traits<Scalar, RowCount - 1, RowCount - 1>::determinant(sub);
			res._Data[i * RowCount + j] = sign * det;
			sign = -sign;
		}
	}
	return transpose<Scalar, RowCount, RowCount>(res);
}

template<class Scalar, size_t RowCount>
inline constexpr typename std::experimental::square_matrix_traits<Scalar, RowCount>::matrix_t std::experimental::square_matrix_traits<Scalar, RowCount>::inverse(typename square_matrix_traits<Scalar, RowCount>::matrix_t const& mat)
{
	auto adj = classical_adjoint(mat);
	auto det = determinant(mat);
	std::transform(adj._Data, adj._Data + (RowCount * RowCount), adj._Data, [&](const auto& el) { return el / det; });
	return adj;
}

// Free functions
template<class Scalar, size_t RowCount, size_t ColCount>
inline constexpr typename std::experimental::matrix_traits<Scalar, ColCount, RowCount>::matrix_t std::experimental::transpose(typename matrix_traits<Scalar, RowCount, ColCount>::matrix_t const& mat) noexcept
{
	auto res = typename matrix_traits<Scalar, RowCount, ColCount>::template other<ColCount, RowCount>::matrix_t{};
	for (auto i = 0; i < RowCount; ++i)
	{
		for (auto j = 0; j < ColCount; ++j)
		{
			res._Data[i + j * RowCount] = mat._Data[i * ColCount + j];
		}
	}
	return res;
}

#endif

/*
From Simon Brand:

template<class rep, size_t dim, class scalar_type>
inline constexpr std::experimental::matrix<rep> std::experimental::operator*(std::experimental::matrix<rep> const& lhs, scalar_type const& rhs) noexcept
{
auto filler = [&]<size_t... Idx>(std::index_sequence<Idx...>){
auto res(lhs);
(res.i[Idx] *= rhs, ...);
return res; };
return filler (std::make_index_sequence<dim>{});
}

*/
