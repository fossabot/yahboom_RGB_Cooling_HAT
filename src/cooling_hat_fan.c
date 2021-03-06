/*
BSD 3-Clause License

Copyright (c) 2020, Michal Duda
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <unistd.h>
#include "cooling_hat_fan.h"
#include "cooling_hat_i2c.h"
#include "cooling_hat_utils.h"

static enum fan_speed last_fan_speed = -1;

void set_fan_speed(enum fan_speed value) {
    if (last_fan_speed == value)
        return;

    if (last_fan_speed == fan_speed_0_percent && value == fan_speed_20_percent) {
        // There wa an observation that fan is not able to start circulating from the OFF state to the low 20% speed.
        // Therefore lets help it with the initial 40% speed.
        DEBUG_PRINT("[I2C] Speeding fan to a higher speed from the OFF state");
        i2c_write(i2c_fan_register, fan_speed_40_percent);
        sleep(2);
    }

    last_fan_speed = value;

    DEBUG_PRINT("[I2C] Changing fan speed to %d", value);
    i2c_write(i2c_fan_register, value);
}
