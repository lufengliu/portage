/*
This file is part of the Ristra portage project.
Please see the license file at the root of this repository, or at:
    https://github.com/laristra/portage/blob/master/LICENSE
*/
#ifndef PORTAGE_SUPPORT_BASIS_H_
#define PORTAGE_SUPPORT_BASIS_H_

#include <cassert>
#include <cmath>
#include <array>
#include <vector>

// portage includes
#include "portage/support/portage.h"
#include "wonton/support/Point.h"
namespace Portage {
namespace Meshfree {
namespace Basis {

using std::array;
using std::vector;

enum Type {Unitary, Linear, Quadratic, LastBasis};

 constexpr array<size_t,4> quadratic_sizes={0,3,6,10};

////////////////////////////////////////////////////////////////////////////////
// Traits
////////////////////////////////////////////////////////////////////////////////

template<Type type, size_t dim>
class Traits{};

template<size_t dim>
class Traits<Unitary, dim>
{
  public:
  static constexpr size_t function_size=1;
  static constexpr array<size_t,2> jet_size={1,1};
  using array_t = array<double, function_size>;
  using matrix_t = array<array<double, function_size>, function_size>;
};

template<size_t dim>
class Traits<Linear, dim>
{
  public:
  static constexpr size_t function_size=dim+1;
  static constexpr array<size_t,2> jet_size={dim+1,dim+1};
  using array_t = array<double, function_size>;
  using matrix_t = array<array<double, function_size>, function_size>;
};

template<size_t dim>
class Traits<Quadratic, dim>
{
  public:
  static constexpr size_t function_size=quadratic_sizes[dim];
  // Intel 18.0.1 compiler cannot handle this declaration so we use the alternative
  //  static constexpr array<size_t,2>
  //    jet_size={quadratic_sizes[dim], quadratic_sizes[dim]};
  static constexpr array<size_t,2>
    jet_size={function_size, function_size};
  using array_t = array<double, function_size>;
  using matrix_t = array<array<double, function_size>, function_size>;
};

template<size_t dim>
size_t function_size(Type btype){
  size_t result;
  switch (btype) {
    case Unitary:
      result = Traits<Unitary, dim>::function_size;
      break;
    case Linear:
      result = Traits<Linear, dim>::function_size;
      break;
    case Quadratic:
      result = Traits<Quadratic, dim>::function_size;
      break;
    default:
      assert(false);
  }
  return result;
}

template<size_t dim>
array<size_t,2> jet_size(Type btype) {
  array<size_t,2> result;
  switch(btype) {
    case Unitary:
      result = Traits<Unitary, dim>::jet_size;
      break;
    case Linear:
      result = Traits<Linear, dim>::jet_size;
      break;
    case Quadratic:
      result = Traits<Quadratic, dim>::jet_size;
      break;
    default:
      assert(false);
  }
  return result;
}

////////////////////////////////////////////////////////////////////////////////
// Templates
////////////////////////////////////////////////////////////////////////////////

template<Type type, size_t dim>
array<double, Traits<type, dim>::function_size>
function(Wonton::Point<dim> x){assert(false);}

template<Type type, size_t dim>
array<
array<double, Traits<type, dim>::function_size>,
Traits<type, dim>::function_size
>
jet(Wonton::Point<dim> x){assert(false);}

template<Type type, size_t dim>
array<
array<double, Traits<type, dim>::function_size>,
Traits<type, dim>::function_size
>
inverse_jet(Wonton::Point<dim> x){assert(false);}

template<Type type, size_t dim>
array<double, Traits<type, dim>::function_size>
shift(Wonton::Point<dim> x, Wonton::Point<dim> y){
  auto ijet = inverse_jet<type,dim>(x);
  auto by = function<type,dim>(y);
  array<double, Traits<type, dim>::function_size> r{0.0};

  size_t n = Traits<type, dim>::function_size;
  for (size_t i=0; i<n; i++) r[i] = 0.;
  for (size_t i=0; i<n; i++) for (size_t j=0; j<n; j++) {
    r[i] += ijet[i][j]*by[j];
  }

  return r;
}

template<Type type, size_t dim>
typename Traits<type,dim>::matrix_t transfactor(const Wonton::Point<dim> &c) {
  typename Traits<type,dim>::matrix_t result;
  assert(false);
  return result;
}

template<size_t dim>
vector<double> function(Type type, Wonton::Point<dim> x){
  size_t nbasis = function_size<dim>(type);
  vector<double> result(nbasis);
  switch (type) {
    case Unitary: {
      auto resulta = function<Unitary, dim>(x);
      for (size_t i=0; i<nbasis; i++) result[i] = resulta[i];
      break;
    }
    case Linear: {
      auto resulta = function<Linear, dim>(x);
      for (size_t i=0; i<nbasis; i++) result[i] = resulta[i];
      break;
    }
    case Quadratic: {
      auto resulta = function<Quadratic, dim>(x);
      for (size_t i=0; i<nbasis; i++) result[i] = resulta[i];
      break;
    }
    default:
      assert(false);
  }
  return result;
}

template<size_t dim>
vector<double> shift(Type type, Wonton::Point<dim> x, Wonton::Point<dim> y){
  size_t nbasis = function_size<dim>(type);
  vector<double> result(nbasis);
  switch (type) {
    case Unitary: {
      auto resulta = shift<Unitary, dim>(x,y);
      for (size_t i=0; i<nbasis; i++) result[i] = resulta[i];
      break;
    }
    case Linear: {
      auto resulta = shift<Linear, dim>(x,y);
      for (size_t i=0; i<nbasis; i++) result[i] = resulta[i];
      break;
    }
    case Quadratic: {
      auto resulta = shift<Quadratic, dim>(x,y);
      for (size_t i=0; i<nbasis; i++) result[i] = resulta[i];
      break;
    }
    default:
      assert(false);
  }
  return result;
}

template<size_t dim>
vector<vector<double>> jet(Type type, Wonton::Point<dim> x){
  auto njet = jet_size<dim>(type);
  vector<vector<double>> result(njet[0],vector<double>(njet[1],0.));
  switch (type) {
    case Unitary: {
      auto resulta = jet<Unitary, dim>(x);
      for (size_t i=0; i<njet[0]; i++) for (size_t j=0; j<njet[1]; j++)
        result[i][j] = resulta[i][j];
      break;
    }
    case Linear: {
      auto resulta = jet<Linear, dim>(x);
      for (size_t i=0; i<njet[0]; i++) for (size_t j=0; j<njet[1]; j++)
        result[i][j] = resulta[i][j];
      break;
    }
    case Quadratic: {
      auto resulta = jet<Quadratic, dim>(x);
      for (size_t i=0; i<njet[0]; i++) for (size_t j=0; j<njet[1]; j++)
        result[i][j] = resulta[i][j];
      break;
    }
    default:
      assert(false);
  }
  return result;
}

template<size_t dim>
vector<vector<double>> inverse_jet(Type type, Wonton::Point<dim> x){
  auto njet = jet_size<dim>(type);
  vector<vector<double>> result(njet[0],vector<double>(njet[1],0.));
  switch (type) {
    case Unitary: {
      auto resulta = inverse_jet<Unitary, dim>(x);
      for (size_t i=0; i<njet[0]; i++) for (size_t j=0; j<njet[1]; j++)
        result[i][j] = resulta[i][j];
      break;
    }
    case Linear: {
      auto resulta = inverse_jet<Linear, dim>(x);
      for (size_t i=0; i<njet[0]; i++) for (size_t j=0; j<njet[1]; j++)
        result[i][j] = resulta[i][j];
      break;
    }
    case Quadratic: {
      auto resulta = inverse_jet<Quadratic, dim>(x);
      for (size_t i=0; i<njet[0]; i++) for (size_t j=0; j<njet[1]; j++)
        result[i][j] = resulta[i][j];
      break;
    }
    default:
      assert(false);
  }
  return result;
}

template<size_t dim>
  vector<vector<double>> transfactor(const Type type, const Wonton::Point<dim> &c) {
  size_t nbasis = function_size<dim>(type);
  vector<vector<double>> result(nbasis, vector<double>(nbasis,0.));
  switch(type) {
  case Unitary: {
    auto tf = transfactor<Unitary,dim>(c);
    for (size_t i=0; i<nbasis; i++) for (size_t j=0; j<nbasis; j++) result[i][j] = tf[i][j];
    break;
  }
  case Linear: {
    auto tf = transfactor<Linear,dim>(c);
    for (size_t i=0; i<nbasis; i++) for (size_t j=0; j<nbasis; j++) result[i][j] = tf[i][j];
    break;
  }
  case Quadratic: {
    auto tf = transfactor<Quadratic,dim>(c);
    for (size_t i=0; i<nbasis; i++) for (size_t j=0; j<nbasis; j++) result[i][j] = tf[i][j];
    break;
  }
  default:
    assert(false);
  }
  return result;
}

////////////////////////////////////////////////////////////////////////////////
// Unitary
////////////////////////////////////////////////////////////////////////////////

template<>
inline
array<double, Traits<Unitary, 1>::function_size>
function<Unitary, 1>(Wonton::Point<1> x) {
  array<double, Traits<Unitary, 1>::function_size> r{0.0};
  r[0] = 1.;
  return r;
}

template<>
inline
array<double, Traits<Unitary, 2>::function_size>
function<Unitary, 2>(Wonton::Point<2> x) {
  array<double, Traits<Unitary, 2>::function_size> r{0.0};
  r[0] = 1.;
  return r;
}

template<>
inline
array<double, Traits<Unitary, 3>::function_size>
function<Unitary, 3>(Wonton::Point<3> x) {
  array<double, Traits<Unitary, 3>::function_size> r{0.0};
  r[0] = 1.;
  return r;
}

//---------------------------------------------------------------

template<>
inline
array<
array<double, Traits<Unitary, 1>::function_size>,
Traits<Unitary, 1>::function_size
>
jet<Unitary,1>(Wonton::Point<1> x){
  array<
  array<double, Traits<Unitary, 1>::function_size>,
  Traits<Unitary, 1>::function_size
    > r{0.0};
  r[0][0] = 1.;
  return r;
}

template<>
inline
array<
array<double, Traits<Unitary, 2>::function_size>,
Traits<Unitary, 2>::function_size
>
jet<Unitary,2>(Wonton::Point<2> x){
  array<
  array<double, Traits<Unitary, 2>::function_size>,
  Traits<Unitary, 2>::function_size
    > r{0.0};
  r[0][0] = 1.;
  return r;
}

template<>
inline
array<
array<double, Traits<Unitary, 3>::function_size>,
Traits<Unitary, 3>::function_size
>
jet<Unitary,3>(Wonton::Point<3> x){
  array<
  array<double, Traits<Unitary, 3>::function_size>,
  Traits<Unitary, 3>::function_size
    > r{0.0};
  r[0][0] = 1.;
  return r;
}

//---------------------------------------------------------------

template<>
inline
array<
array<double, Traits<Unitary, 1>::function_size>,
Traits<Unitary, 1>::function_size
>
inverse_jet<Unitary,1>(Wonton::Point<1> x){
  array<
  array<double, Traits<Unitary, 1>::function_size>,
  Traits<Unitary, 1>::function_size
    > r{0.0};
  r[0][0] = 1.;
  return r;
}

template<>
inline
array<
array<double, Traits<Unitary, 2>::function_size>,
Traits<Unitary, 2>::function_size
>
inverse_jet<Unitary,2>(Wonton::Point<2> x){
  array<
  array<double, Traits<Unitary, 2>::function_size>,
  Traits<Unitary, 2>::function_size
    > r{0.0};
  r[0][0] = 1.;
  return r;
}

template<>
inline
array<
array<double, Traits<Unitary, 3>::function_size>,
Traits<Unitary, 3>::function_size
>
inverse_jet<Unitary,3>(Wonton::Point<3> x){
  array<
  array<double, Traits<Unitary, 3>::function_size>,
  Traits<Unitary, 3>::function_size
    > r{0.0};
  r[0][0] = 1.;
  return r;
}

//---------------------------------------------------------------

template<>
inline
array<double, Traits<Unitary, 1>::function_size>
shift<Unitary, 1>(Wonton::Point<1> x, Wonton::Point<1> y) {
  array<double, Traits<Unitary, 1>::function_size> r{0.0};
  r[0] = 1.;
  return r;
}

template<>
inline
array<double, Traits<Unitary, 2>::function_size>
shift<Unitary, 2>(Wonton::Point<2> x, Wonton::Point<2> y) {
  array<double, Traits<Unitary, 2>::function_size> r{0.0};
  r[0] = 1.;
  return r;
}

template<>
inline
array<double, Traits<Unitary, 3>::function_size>
shift<Unitary, 3>(Wonton::Point<3> x, Wonton::Point<3> y) {
  array<double, Traits<Unitary, 3>::function_size> r{0.0};
  r[0] = 1.;
  return r;
}

//---------------------------------------------------------------

template<>
inline
typename Traits<Unitary,1>::matrix_t transfactor<Unitary,1>(const Wonton::Point<1> &c) {
  typename Traits<Unitary,1>::matrix_t tf;
  tf[0][0] = 1.;
  return tf;
}

template<>
inline
typename Traits<Unitary,2>::matrix_t transfactor<Unitary,2>(const Wonton::Point<2> &c) {
  typename Traits<Unitary,2>::matrix_t tf;
  tf[0][0] = 1.;
  return tf;
}

template<>
inline
typename Traits<Unitary,3>::matrix_t transfactor<Unitary,3>(const Wonton::Point<3> &c) {
  typename Traits<Unitary,3>::matrix_t tf;
  tf[0][0] = 1.;
  return tf;
}

////////////////////////////////////////////////////////////////////////////////
// Linear
////////////////////////////////////////////////////////////////////////////////

template<>
inline
array<double, Traits<Linear, 1>::function_size>
function<Linear, 1>(Wonton::Point<1> x) {
  array<double, Traits<Linear, 1>::function_size> r{0.0};
  r[0] = 1.;
  r[1] = x[0];
  return r;
}

template<>
inline
array<double, Traits<Linear, 2>::function_size>
function<Linear, 2>(Wonton::Point<2> x) {
  array<double, Traits<Linear, 2>::function_size> r{0.0};
  r[0] = 1.;
  r[1] = x[0];
  r[2] = x[1];
  return r;
}

template<>
inline
array<double, Traits<Linear, 3>::function_size>
function<Linear, 3>(Wonton::Point<3> x) {
  array<double, Traits<Linear, 3>::function_size> r{0.0};
  r[0] = 1.;
  r[1] = x[0];
  r[2] = x[1];
  r[3] = x[2];
  return r;
}

//---------------------------------------------------------------

template<>
inline
array<
array<double, Traits<Linear, 1>::function_size>,
Traits<Linear, 1>::function_size
>
jet<Linear,1>(Wonton::Point<1> x){
  array<
  array<double, Traits<Linear, 1>::function_size>,
  Traits<Linear, 1>::function_size
    > r{0.0};
  r[0][0] = 1.;
  r[1][0] = x[0];
  r[0][1] = 0.;
  r[1][1] = 1.;
  return r;
}

template<>
inline
array<
array<double, Traits<Linear, 2>::function_size>,
Traits<Linear, 2>::function_size
>
jet<Linear,2>(Wonton::Point<2> x){
  array<
  array<double, Traits<Linear, 2>::function_size>,
  Traits<Linear, 2>::function_size
    > r{0.0};
  r[0][0] = 1.;
  r[1][0] = x[0];
  r[2][0] = x[1];
  r[0][1] = 0.;
  r[1][1] = 1.;
  r[2][1] = 0.;
  r[0][2] = 0.;
  r[1][2] = 0.;
  r[2][2] = 1.;
  return r;
}

template<>
inline
array<
array<double, Traits<Linear, 3>::function_size>,
Traits<Linear, 3>::function_size
>
jet<Linear,3>(Wonton::Point<3> x){
  array<
  array<double, Traits<Linear, 3>::function_size>,
  Traits<Linear, 3>::function_size
    > r{0.0};
  r[0][0] = 1.;
  r[1][0] = x[0];
  r[2][0] = x[1];
  r[3][0] = x[2];
  r[0][1] = 0.;
  r[1][1] = 1.;
  r[2][1] = 0.;
  r[3][1] = 0.;
  r[0][2] = 0.;
  r[1][2] = 0.;
  r[2][2] = 1.;
  r[3][2] = 0.;
  r[0][3] = 0.;
  r[1][3] = 0.;
  r[2][3] = 0.;
  r[3][3] = 1.;
  return r;
}

//---------------------------------------------------------------

template<>
inline
array<
array<double, Traits<Linear, 1>::function_size>,
Traits<Linear, 1>::function_size
>
inverse_jet<Linear,1>(Wonton::Point<1> x){
  array<
  array<double, Traits<Linear, 1>::function_size>,
  Traits<Linear, 1>::function_size
    > r{0.0};
  r[0][0] = 1.;
  r[1][0] = -x[0];
  r[0][1] = 0.;
  r[1][1] = 1.;
  return r;
}

template<>
inline
array<
array<double, Traits<Linear, 2>::function_size>,
Traits<Linear, 2>::function_size
>
inverse_jet<Linear,2>(Wonton::Point<2> x){
  array<
  array<double, Traits<Linear, 2>::function_size>,
  Traits<Linear, 2>::function_size
    > r{0.0};
  r[0][0] = 1.;
  r[1][0] = -x[0];
  r[2][0] = -x[1];
  r[0][1] = 0.;
  r[1][1] = 1.;
  r[2][1] = 0.;
  r[0][2] = 0.;
  r[1][2] = 0.;
  r[2][2] = 1.;
  return r;
}

template<>
inline
array<
array<double, Traits<Linear, 3>::function_size>,
Traits<Linear, 3>::function_size
>
inverse_jet<Linear,3>(Wonton::Point<3> x){
  array<
  array<double, Traits<Linear, 3>::function_size>,
  Traits<Linear, 3>::function_size
    > r{0.0};
  r[0][0] = 1.;
  r[1][0] = -x[0];
  r[2][0] = -x[1];
  r[3][0] = -x[2];
  r[0][1] = 0.;
  r[1][1] = 1.;
  r[2][1] = 0.;
  r[3][1] = 0.;
  r[0][2] = 0.;
  r[1][2] = 0.;
  r[2][2] = 1.;
  r[3][2] = 0.;
  r[0][3] = 0.;
  r[1][3] = 0.;
  r[2][3] = 0.;
  r[3][3] = 1.;
  return r;
}

//---------------------------------------------------------------

template<>
inline
array<double, Traits<Linear, 1>::function_size>
shift<Linear, 1>(Wonton::Point<1> x, Wonton::Point<1> y) {
  array<double, Traits<Linear, 1>::function_size> r{0.0};
  r[0] = 1.;
  r[1] = y[0]-x[0];
  return r;
}

template<>
inline
array<double, Traits<Linear, 2>::function_size>
shift<Linear, 2>(Wonton::Point<2> x, Wonton::Point<2> y) {
  array<double, Traits<Linear, 2>::function_size> r{0.0};
  r[0] = 1.;
  r[1] = y[0]-x[0];
  r[2] = y[1]-x[1];
  return r;
}

template<>
inline
array<double, Traits<Linear, 3>::function_size>
shift<Linear, 3>(Wonton::Point<3> x, Wonton::Point<3> y) {
  array<double, Traits<Linear, 3>::function_size> r{0.0};
  r[0] = 1.;
  r[1] = y[0]-x[0];
  r[2] = y[1]-x[1];
  r[3] = y[2]-x[2];
  return r;
}

//---------------------------------------------------------------

template<>
inline
typename Traits<Linear,1>::matrix_t transfactor<Linear,1>(const Wonton::Point<1> &c) {
  typename Traits<Linear,1>::matrix_t tf;
  tf[0][0]=1;
  tf[0][1]=0;
  tf[1][0]=c[0];
  tf[1][1]=1;
  return tf;
}

template<>
inline
typename Traits<Linear,2>::matrix_t transfactor<Linear,2>(const Wonton::Point<2> &c) {
  typename Traits<Linear,2>::matrix_t tf;
  tf[0][0]=1;
  tf[0][1]=0;
  tf[0][2]=0;
  tf[1][0]=c[0];
  tf[1][1]=1;
  tf[1][2]=0;
  tf[2][0]=c[1];
  tf[2][1]=0;
  tf[2][2]=1;
  return tf;
}

template<>
inline
typename Traits<Linear,3>::matrix_t transfactor<Linear,3>(const Wonton::Point<3> &c) {
  typename Traits<Linear,3>::matrix_t tf;
  tf[0][0]=1;
  tf[0][1]=0;
  tf[0][2]=0;
  tf[0][3]=0;
  tf[1][0]=c[0];
  tf[1][1]=1;
  tf[1][2]=0;
  tf[1][3]=0;
  tf[2][0]=c[1];
  tf[2][1]=0;
  tf[2][2]=1;
  tf[2][3]=0;
  tf[3][0]=c[2];
  tf[3][1]=0;
  tf[3][2]=0;
  tf[3][3]=1;
 return tf;
}

////////////////////////////////////////////////////////////////////////////////
// Quadratic
////////////////////////////////////////////////////////////////////////////////

template<>
inline
array<double, Traits<Quadratic, 1>::function_size>
function<Quadratic, 1>(Wonton::Point<1> x) {
  array<double, Traits<Quadratic, 1>::function_size> r{0.0};
  r[0] = 1.;
  r[1] = x[0];
  r[2] = .5 * x[0] * x[0];
  return r;
}

template<>
inline
array<double, Traits<Quadratic, 2>::function_size>
function<Quadratic, 2>(Wonton::Point<2> x) {
  array<double, Traits<Quadratic, 2>::function_size> r{0.0};
  r[0] = 1.;
  r[1] = x[0];
  r[2] = x[1];
  r[3] = .5 * x[0] * x[0];
  r[4] = x[0] * x[1];
  r[5] = .5 * x[1] * x[1];
  return r;
}

template<>
inline
array<double, Traits<Quadratic, 3>::function_size>
function<Quadratic, 3>(Wonton::Point<3> x) {
  array<double, Traits<Quadratic, 3>::function_size> r{0.0};
  r[0] = 1.;
  r[1] = x[0];
  r[2] = x[1];
  r[3] = x[2];
  r[4] = .5 * x[0] * x[0];
  r[5] = x[0] * x[1];
  r[6] = x[0] * x[2];
  r[7] = .5 * x[1] * x[1];
  r[8] = x[1] * x[2];
  r[9] = .5 * x[2] * x[2];
  return r;
}

//---------------------------------------------------------------

template<>
inline
array<
array<double, Traits<Quadratic, 1>::function_size>,
Traits<Quadratic, 1>::function_size
>
jet<Quadratic,1>(Wonton::Point<1> x){
  array<
  array<double, Traits<Quadratic, 1>::function_size>,
  Traits<Quadratic, 1>::function_size
    > r{0.0};
  r[0][0] = 1.;

  r[1][0] = x[0];
  r[1][1] = 1.;

  r[2][0] = x[0] * x[0] * .5;
  r[2][1] = x[0];
  r[2][2] = 1.;
  return r;
}

template<>
inline
array<
array<double, Traits<Quadratic, 2>::function_size>,
Traits<Quadratic, 2>::function_size
>
jet<Quadratic,2>(Wonton::Point<2> x){
  array<
  array<double, Traits<Quadratic, 2>::function_size>,
  Traits<Quadratic, 2>::function_size
    > r{0.0};
  r[0][0] = 1.;
  r[1][0] = x[0];
  r[2][0] = x[1];
  r[3][0] = .5 * x[0] * x[0];
  r[4][0] = x[0] * x[1];
  r[5][0] = .5 * x[1] * x[1];

  r[1][1] = 1.;
  r[3][1] = x[0];
  r[4][1] = x[1];

  r[2][2] = 1.;
  r[4][2] = x[0];
  r[5][2] = x[1];

  for (size_t i = 3; i < 6; i++)
    r[i][i] = 1.;
  return r;
}

template<>
inline
array<
array<double, Traits<Quadratic, 3>::function_size>,
Traits<Quadratic, 3>::function_size
>
jet<Quadratic,3>(Wonton::Point<3> x){
  array<
  array<double, Traits<Quadratic, 3>::function_size>,
  Traits<Quadratic, 3>::function_size
    > r{0.0};
  r[0][0] = 1.;
  r[1][0] = x[0];
  r[2][0] = x[1];
  r[3][0] = x[2];
  r[4][0] = .5 * x[0] * x[0];
  r[5][0] = x[0] * x[1];
  r[6][0] = x[0] * x[2];
  r[7][0] = .5 * x[1] * x[1];
  r[8][0] = x[1] * x[2];
  r[9][0] = .5 * x[2] * x[2];

  r[1][1] = 1.;
  r[4][1] = x[0];
  r[5][1] = x[1];
  r[6][1] = x[2];

  r[2][2] = 1.;
  r[5][2] = x[0];
  r[7][2] = x[1];
  r[8][2] = x[2];

  r[3][3] = 1.;
  r[6][3] = x[0];
  r[8][3] = x[1];
  r[9][3] = x[2];

  for (size_t i = 4; i < 10; i++)
    r[i][i] = 1.;
  return r;
}

//---------------------------------------------------------------

template<>
inline
array<
array<double, Traits<Quadratic, 1>::function_size>,
Traits<Quadratic, 1>::function_size
>
inverse_jet<Quadratic,1>(Wonton::Point<1> x){
  Wonton::Point<1> mx;
  for (size_t i=0; i<1; i++) mx[i] = -x[i];
  return jet<Quadratic,1>(mx);
}

template<>
inline
array<
array<double, Traits<Quadratic, 2>::function_size>,
Traits<Quadratic, 2>::function_size
>
inverse_jet<Quadratic,2>(Wonton::Point<2> x){
  Wonton::Point<2> mx;
  for (size_t i=0; i<2; i++) mx[i] = -x[i];
  return jet<Quadratic,2>(mx);
}

template<>
inline
array<
array<double, Traits<Quadratic, 3>::function_size>,
Traits<Quadratic, 3>::function_size
>
inverse_jet<Quadratic,3>(Wonton::Point<3> x){
  Wonton::Point<3> mx;
  for (size_t i=0; i<3; i++) mx[i] = -x[i];
  return jet<Quadratic,3>(mx);
}

//---------------------------------------------------------------

template<>
inline
array<double, Traits<Quadratic, 1>::function_size>
shift<Quadratic, 1>(Wonton::Point<1> x, Wonton::Point<1> y) {
  Wonton::Point<1> d;
  for (size_t i=0; i<1; i++) d[i] = y[i]-x[i];
  return function<Quadratic,1>(d);
}

template<>
inline
array<double, Traits<Quadratic, 2>::function_size>
shift<Quadratic, 2>(Wonton::Point<2> x, Wonton::Point<2> y) {
  Wonton::Point<2> d;
  for (size_t i=0; i<2; i++) d[i] = y[i]-x[i];
  return function<Quadratic,2>(d);
}

template<>
inline
array<double, Traits<Quadratic, 3>::function_size>
shift<Quadratic, 3>(Wonton::Point<3> x, Wonton::Point<3> y) {
  Wonton::Point<3> d;
  for (size_t i=0; i<3; i++) d[i] = y[i]-x[i];
  return function<Quadratic,3>(d);
}

//---------------------------------------------------------------

template<>
inline
typename Traits<Quadratic,1>::matrix_t transfactor<Quadratic,1>(const Wonton::Point<1> &c) {
  typename Traits<Quadratic,1>::matrix_t tf;
  tf[0][0]=1;
  tf[0][1]=0;
  tf[0][2]=0;
  tf[1][0]=c[0];
  tf[1][1]=1;
  tf[1][2]=0;
  tf[2][0]=pow(c[0], 2)/2;
  tf[2][1]=c[0];
  tf[2][2]=1;
  return tf;
}

template<>
inline
typename Traits<Quadratic,2>::matrix_t transfactor<Quadratic,2>(const Wonton::Point<2> &c) {
  typename Traits<Quadratic,2>::matrix_t tf;
  tf[0][0]=1;
  tf[0][1]=0;
  tf[0][2]=0;
  tf[0][3]=0;
  tf[0][4]=0;
  tf[0][5]=0;
  tf[1][0]=c[0];
  tf[1][1]=1;
  tf[1][2]=0;
  tf[1][3]=0;
  tf[1][4]=0;
  tf[1][5]=0;
  tf[2][0]=c[1];
  tf[2][1]=0;
  tf[2][2]=1;
  tf[2][3]=0;
  tf[2][4]=0;
  tf[2][5]=0;
  tf[3][0]=pow(c[0], 2)/2;
  tf[3][1]=c[0];
  tf[3][2]=0;
  tf[3][3]=1;
  tf[3][4]=0;
  tf[3][5]=0;
  tf[4][0]=c[0]*c[1];
  tf[4][1]=c[1];
  tf[4][2]=c[0];
  tf[4][3]=0;
  tf[4][4]=1;
  tf[4][5]=0;
  tf[5][0]=pow(c[1], 2)/2;
  tf[5][1]=0;
  tf[5][2]=c[1];
  tf[5][3]=0;
  tf[5][4]=0;
  tf[5][5]=1;
  return tf;
}

template<>
inline
typename Traits<Quadratic,3>::matrix_t transfactor<Quadratic,3>(const Wonton::Point<3> &c) {
  typename Traits<Quadratic,3>::matrix_t tf;
  tf[0][0]=1;
  tf[0][1]=0;
  tf[0][2]=0;
  tf[0][3]=0;
  tf[0][4]=0;
  tf[0][5]=0;
  tf[0][6]=0;
  tf[0][7]=0;
  tf[0][8]=0;
  tf[0][9]=0;
  tf[1][0]=c[0];
  tf[1][1]=1;
  tf[1][2]=0;
  tf[1][3]=0;
  tf[1][4]=0;
  tf[1][5]=0;
  tf[1][6]=0;
  tf[1][7]=0;
  tf[1][8]=0;
  tf[1][9]=0;
  tf[2][0]=c[1];
  tf[2][1]=0;
  tf[2][2]=1;
  tf[2][3]=0;
  tf[2][4]=0;
  tf[2][5]=0;
  tf[2][6]=0;
  tf[2][7]=0;
  tf[2][8]=0;
  tf[2][9]=0;
  tf[3][0]=c[2];
  tf[3][1]=0;
  tf[3][2]=0;
  tf[3][3]=1;
  tf[3][4]=0;
  tf[3][5]=0;
  tf[3][6]=0;
  tf[3][7]=0;
  tf[3][8]=0;
  tf[3][9]=0;
  tf[4][0]=pow(c[0], 2)/2;
  tf[4][1]=c[0];
  tf[4][2]=0;
  tf[4][3]=0;
  tf[4][4]=1;
  tf[4][5]=0;
  tf[4][6]=0;
  tf[4][7]=0;
  tf[4][8]=0;
  tf[4][9]=0;
  tf[5][0]=c[0]*c[1];
  tf[5][1]=c[1];
  tf[5][2]=c[0];
  tf[5][3]=0;
  tf[5][4]=0;
  tf[5][5]=1;
  tf[5][6]=0;
  tf[5][7]=0;
  tf[5][8]=0;
  tf[5][9]=0;
  tf[6][0]=c[0]*c[2];
  tf[6][1]=c[2];
  tf[6][2]=0;
  tf[6][3]=c[0];
  tf[6][4]=0;
  tf[6][5]=0;
  tf[6][6]=1;
  tf[6][7]=0;
  tf[6][8]=0;
  tf[6][9]=0;
  tf[7][0]=pow(c[1], 2)/2;
  tf[7][1]=0;
  tf[7][2]=c[1];
  tf[7][3]=0;
  tf[7][4]=0;
  tf[7][5]=0;
  tf[7][6]=0;
  tf[7][7]=1;
  tf[7][8]=0;
  tf[7][9]=0;
  tf[8][0]=c[1]*c[2];
  tf[8][1]=0;
  tf[8][2]=c[2];
  tf[8][3]=c[1];
  tf[8][4]=0;
  tf[8][5]=0;
  tf[8][6]=0;
  tf[8][7]=0;
  tf[8][8]=1;
  tf[8][9]=0;
  tf[9][0]=pow(c[2], 2)/2;
  tf[9][1]=0;
  tf[9][2]=0;
  tf[9][3]=c[2];
  tf[9][4]=0;
  tf[9][5]=0;
  tf[9][6]=0;
  tf[9][7]=0;
  tf[9][8]=0;
  tf[9][9]=1;
  return tf;
}

}  // namespace Basis
}  // namespace Meshfree
}  // namespace Portage

#endif  // PORTAGE_SUPPORT_BASIS_H_
