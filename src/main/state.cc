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
mpz_class pow(mpz_class const &base, unsigned long exponent) {
  mpz_class result;
  mpz_pow_ui(result.get_mpz_t(), base.get_mpz_t(), exponent);
  return result;
}
mpq_class canonicalize(mpq_class q) {
  q.canonicalize();
  return q;
}

void State::calculate() noexcept {
  // assume this never happens - we must have run for 2^64 iterations if it did!
  // if (!q.fits_ulong_p()) {
  //   throw runtime_error("q is too large");
  // }

  static mpz_class const c1 = 1_mpz;
  static mpz_class const c12 = 12_mpz;
  static mpz_class const c16 = 16_mpz;

  static mpz_class const lIncrement = 545140134_mpz;
  static mpz_class const xFactor = -262537412640768000_mpz;

  // calculate this term and add onto sum
  sum += m * canonicalize(mpq_class(l, x));

  // prepare for next term
  k += c12;
  m *= canonicalize(mpq_class(pow(k, 3) - c16 * k, pow(q + c1, 3)));
  l += lIncrement;
  x *= xFactor;
  q++;
}

constexpr double digits_per_iteration = 14.1816474627254776555;
string State::finalize() const noexcept {
  mpf_set_default_prec(digits_per_iteration * (q.get_ui() + 1) * log2(10) + 1);
  mpf_class result = 426880_mpf * sqrt(10005_mpf) /
                     mpf_class(sum + m * canonicalize(mpq_class(l, x)));

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
  is >> state.sum >> state.q >> state.m >> state.k >> state.l >> state.x;
  return is;
}
ostream &operator<<(std::ostream &os, State const &state) {
  os << state.sum << " " << state.q << " " << state.m << " " << state.k << " "
     << state.l << " " << state.x;
  return os;
}
}  // namespace pidollarsofpi
