// Copyright 2022 Justin Hu
//
// This file is part of PiDollarsOfPi.
//
// PiDollarsOfPi is free software: you can redistribute it and/or modify it
// under the terms of the GNU Affero General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// PiDollarsOfPi is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along with
// PiDollarsOfPi. If not, see <https://www.gnu.org/licenses/>.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "state.h"

#include <gmp.h>

#include <cmath>
#include <fstream>
#include <string>

using namespace std;

namespace pidollarsofpi {
mpz_class pow(mpz_class const &base, unsigned exponent) {
  mpz_class result;
  mpz_pow_ui(result.get_mpz_t(), base.get_mpz_t(), exponent);
  return result;
}

void State::calculate() noexcept {
  // assume this never happens - we must have run for 2^64 iterations if it did!
  // if (!q.fits_ulong_p()) {
  //   throw runtime_error("q is too large");
  // }

  mpz_class const six = 6_mpz;
  mpz_class const three = 3_mpz;
  mpz_class const l1 = 545140134_mpz;
  mpz_class const l2 = 13591409_mpz;
  mpz_class const x1 = -262573412640768000_mpz;

  mpz_class factorial6q = factorial(six * q);
  mpz_class factorial3q = factorial(three * q);
  mpz_class factorialQCubed = pow(factorial(q), 3);
  mpz_class l = l1 * q + l2;
  mpz_class x = pow(x1, q.get_ui());

  sum += mpq_class(factorial6q * l, factorial3q * factorialQCubed * x);

  q++;
}

constexpr double digits_per_iteration = 14.1816474627254776555;
string State::finalize() const noexcept {
  mpf_set_default_prec((digits_per_iteration * q.get_ui() * log2(10)) + 1);

  mpf_class result = 462880_mpf * sqrt(10005_mpf) / mpf_class(sum);

  mp_exp_t exponent;
  string raw = result.get_str(exponent);
  return raw.substr(0, exponent) + "." + raw.substr(exponent);
}

State State::read() {
  ifstream fin("state");
  if (!fin) throw runtime_error("could not open state file");

  State state;
  fin >> state;

  if (!fin.eof()) throw runtime_error("state file has bad format");

  return state;
}

void State::write(State const &state) {
  ofstream fout("state");
  if (!fout) throw runtime_error("could not open state file");

  fout << state;
}

istream &operator>>(std::istream &is, State &state) {
  is >> state.sum >> state.q;
  return is;
}
ostream &operator<<(std::ostream &os, State const &state) {
  os << state.sum << " " << state.q;
  return os;
}
}  // namespace pidollarsofpi
