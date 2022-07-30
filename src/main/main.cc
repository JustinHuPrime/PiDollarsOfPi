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

#include <iostream>

#include "state.h"

using namespace std;
using namespace pidollarsofpi;

void printUsage(char **argv) {
  cerr << "Usage: " << argv[0] << " <finalize|calculate>" << endl;
}

mpz_class pow(mpz_class const &base, unsigned long exponent) {
  mpz_class result;
  mpz_pow_ui(result.get_mpz_t(), base.get_mpz_t(), exponent);
  return result;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printUsage(argv);
    return 1;
  }

  try {
    State state = State::read();

    if (strcmp(argv[1], "finalize") == 0) {
#ifndef NDEBUG
      cout << "DEBUG(input):  " << state << endl;
#endif

      cout << state.finalize() << endl;
      return 0;
    } else if (strcmp(argv[1], "calculate") == 0) {
#ifndef NDEBUG
      cout << "DEBUG(before): " << state << endl;
#endif

      for (unsigned long i = 0; i < 1000; ++i) state.calculate();

#ifndef NDEBUG
      cout << "DEBUG(after):  " << state << endl;
#endif

      State::write(state);
      return 0;
    } else {
      printUsage(argv);
      return 1;
    }
  } catch (runtime_error const &e) {
    cerr << e.what() << endl;
    return 1;
  }
}
