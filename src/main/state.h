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

#ifndef PIDOLLARSOFPI_STATE_H_
#define PIDOLLARSOFPI_STATE_H_

#include <gmpxx.h>

#include <istream>
#include <ostream>
#include <string>

namespace pidollarsofpi {
/**
 * Current state of the Chudnovsky algorithm
 * See https://en.wikipedia.org/wiki/Chudnovsky_algorithm
 */
class State {
 public:
  mpq_class sum;
  mpz_class q;
  mpq_class m;
  mpz_class k;
  mpz_class l;
  mpz_class x;

  State(State const &) noexcept = default;
  State(State &&) noexcept = default;

  ~State() noexcept = default;

  State &operator=(State const &) noexcept = default;
  State &operator=(State &&) noexcept = default;

  void calculate() noexcept;

  void saveState() const;
  void saveResult() const;

  static State load();

 private:
  State() noexcept = default;
  friend std::istream &operator>>(std::istream &, State &);
  friend std::ostream &operator<<(std::ostream &, State const &);
};
std::istream &operator>>(std::istream &, State &);
std::ostream &operator<<(std::ostream &, State const &);

}  // namespace pidollarsofpi

#endif  // PIDOLLARSOFPI_STATE_H_
