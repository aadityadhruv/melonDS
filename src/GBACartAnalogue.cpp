/*
   Copyright 2016-2024 melonDS team

   This file is part of melonDS.

   melonDS is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation, either version 3 of the License, or (at your option)
   any later version.

   melonDS is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along
   with melonDS. If not, see http://www.gnu.org/licenses/.
   */

#include <assert.h>
#include "NDS.h"
#include "GBACart.h"
#include "Platform.h"
#include <algorithm>
#include "math.h"

/* 2^15 / Pi */
#define ANGLE_TO_SHORT 10430.37835047f
/* (float) 0x1000 */
#define FLOAT_TO_FIXED 4096.0f
/* Open-Bus Value (mask) */
#define OPEN_BUS 0xAFFF

namespace melonDS
{
    using Platform::Log;
    using Platform::LogLevel;

    namespace GBACart
    {

        CartAnalogue::CartAnalogue(void* userdata) : 
            CartCommon(Analogue),
            UserData(userdata)
        {
        }

        CartAnalogue::~CartAnalogue() = default;

        void CartAnalogue::Reset()
        {
        }

        void CartAnalogue::DoSavestate(Savestate* file)
        {
            CartCommon::DoSavestate(file);
        }

        u16 CartAnalogue::ROMRead(u32 addr) const
        {
            // Basing this work off https://github.com/TASEmulators/desmume/compare/master...LRFLEW:AM64DS_DeSmuME:analog
            // This check seems to imply it's a ROM read
            if ((addr & 0xFF000000) != 0x09000000) return OPEN_BUS;
            // CHECKME: SRAM address mask
            auto stick_pos = Platform::Addon_AnalogueQuery(UserData);
            int x = std::get<0>(stick_pos);
            int y = std::get<1>(stick_pos);
            float mag = std::hypot(x, y);
            float ang = std::atan2(x, y);
            if (mag > 1.0f) {
                x /= mag;
                y /= mag;
                mag = 1.0f;
            }

            u16 analog_x = static_cast<u16>(std::lround(x * FLOAT_TO_FIXED));
            u16 analog_y = static_cast<u16>(std::lround(y * FLOAT_TO_FIXED));
            u16 analog_magnitude = static_cast<u16>(std::lround(mag * FLOAT_TO_FIXED));
            u16 analog_angle = static_cast<u16>(std::lround(ang * ANGLE_TO_SHORT));
            switch ((addr >> 8) & 0xFFFF) {
                case 0: // Generic
                    switch (addr & 0xFF) {
                        // 0x00 and 0x01 are reserved to match the output
                        // of any physical device made for this purpose.
                        // 0x02 through 0x07 are reserved for future DeSmuME use
                        case 0x08: 
                            return analog_x;
                        case 0x0A:
                            return analog_y;
                        default:
                            return OPEN_BUS;
                    }

                case 1: // AM64DS
                        // Matches the layout of values used in SM64DS
                    switch (addr & 0xFF) {
                        case 0x00:
                            return analog_magnitude;
                        case 0x02:
                            return analog_x;
                        case 0x04:
                            return analog_y;
                        case 0x06:
                            return analog_angle;
                        default:
                            return OPEN_BUS;
                    }
            }
            return OPEN_BUS;

        }

        u8 CartAnalogue::SRAMRead(u32 addr)
        {
            return OPEN_BUS  & 0xFF;
        }
    }
}
