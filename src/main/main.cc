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

#include <chrono>
#include <iostream>

#include "state.h"

using namespace std;
using namespace std::chrono;
using namespace pidollarsofpi;

constexpr duration<int64_t> stateSaveInterval = seconds(60);
constexpr duration<int64_t> resultSaveInterval = seconds(600);

int main(int argc, char **argv) {
  try {
    State state = State::load();

    steady_clock::time_point stateSaveStart = steady_clock::now();
    unsigned long lastStateQ = 0;

    steady_clock::time_point resultSaveStart = steady_clock::now();
    unsigned long lastResultQ = 0;

    while (true) {
      if (steady_clock::now() - stateSaveStart >= stateSaveInterval &&
          lastStateQ != state.q) {
        cout << "Saving state at q = " << state.q.get_ui() << endl;
        state.saveState();

        stateSaveStart = steady_clock::now();
        lastStateQ = state.q.get_ui();
      }

      if (steady_clock::now() - resultSaveStart >= resultSaveInterval &&
          lastResultQ != state.q) {
        cout << "Saving result at q = " << state.q.get_ui() << endl;
        state.saveResult();

        resultSaveStart = steady_clock::now();
        lastResultQ = state.q.get_ui();
      }

      state.calculate();
    }
  } catch (runtime_error const &e) {
    cerr << e.what() << endl;
    return 1;
  }
}
