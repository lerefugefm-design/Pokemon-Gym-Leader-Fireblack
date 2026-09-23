# Fire Black V1.3 import status

Source verified from the user-provided Fire Black V1.3 ROM (BPRF): SHA-256 `df96e6a5a7aba8fd4cf9ea8e434312c6a7e1884ef22147dee76975f9b25d89a3`.

## Verified extraction

The existing extraction contains 411 scanned species slots, 388 active named species and 203 evolution records. The extracted species table includes base stats, both types, two ability IDs, catch/EXP/EV data, held items, gender ratio, egg cycles, friendship, growth rate, egg groups, body colour and flip flag.

Important Fire Black engine detail: its raw type IDs are not identical to stock pokefirered. In particular Fire Black uses Fairy at raw ID 17 and Dark at raw ID 23. Import code must translate by semantic type name rather than blindly copying the raw byte.

## Import order

1. Keep the current branch compiling at every checkpoint.
2. Import species IDs/names and base stats/types/abilities by internal ID.
3. Import the 203 extracted evolution records, preserving Fire Black's branching evolution methods.
4. Port learnsets/TM compatibility only after base species identity is stable.
5. Port sprites/palettes/icons after species IDs are locked.
6. Do not import the experimental special Charmander line; the target is unmodified Fire Black V1.3 roster behaviour.

## Current build

The `gym-leader-fireblack` branch now compiles successfully with the modern ARM toolchain and GitHub Actions produces a test `.gba` artifact. The next integration work should therefore be data-driven and incremental, with CI used as a regression gate after each chunk.

## Safety

Never commit the source commercial ROM or a full ROM dump to this public repository. Only derived structured data and original source changes belong here.
