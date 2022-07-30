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
#include <thread>
#include <vector>

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

  vector<thread> threads;

  threads.emplace_back([this]() { sum += m * canonicalize(mpq_class(l, x)); });
  threads.emplace_back([this]() { k += c12; });
  for_each(threads.begin(), threads.end(), [](thread &t) { t.join(); });
  threads.clear();

  threads.emplace_back([this]() {
    m *= canonicalize(mpq_class(pow(k, 3) - c16 * k, pow(q + c1, 3)));
  });
  threads.emplace_back([this]() { l += lIncrement; });
  threads.emplace_back([this]() { x *= xFactor; });
  for_each(threads.begin(), threads.end(), [](thread &t) { t.join(); });

  q++;
}

void State::saveState() const {
  ofstream fout("state");
  if (!fout) throw runtime_error("could not open state file");

  fout << *this;
}

constexpr double digits_per_iteration = 14.1816474627254776555;
void State::saveResult() const {
  ofstream fout("result");
  if (!fout) throw runtime_error("could not open result file");

  mpf_set_default_prec(digits_per_iteration * q.get_ui() * log2(10) + 1);
  mpf_class result = 426880_mpf * sqrt(10005_mpf) / mpf_class(sum);

  mp_exp_t exponent;
  string raw = result.get_str(exponent);

  fout << raw.substr(0, exponent) << "." << raw.substr(exponent);
}

State State::load() {
  ifstream fin("state");
  if (!fin) throw runtime_error("could not open state file");

  State state;
  fin >> state;

  if (!fin.eof()) throw runtime_error("state file has bad format");

  return state;
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
