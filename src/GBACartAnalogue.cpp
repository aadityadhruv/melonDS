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
            AnalogueInput = 0;
        }

        void CartAnalogue::DoSavestate(Savestate* file)
        {
            CartCommon::DoSavestate(file);
            file->Var16(&AnalogueInput);
        }

        u16 CartAnalogue::ROMRead(u32 addr) const
        {
            // CHECKME: Does this apply to the homebrew cart as well?
            return 0xFCFF;
        }

        u8 CartAnalogue::SRAMRead(u32 addr)
        {
            // CHECKME: SRAM address mask
            addr &= 0xFFFF;

            switch (addr)
            {
                case 0:
                    // Read next byte
                    //TODO Figure out what these cases are
                    auto stick_pos = Platform::Addon_AnalogueQuery(UserData);
                    break;
            }

            //TODO: Somehow return the "correct" x/y axis positions to the patched SM64 rom
            return 0;
        }
    }
}
