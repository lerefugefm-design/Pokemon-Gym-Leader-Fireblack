# Gym Leader hub progression specification

This branch deliberately uses the existing FireRed map, warp, trainer, shop, healing and event systems instead of introducing a second overworld engine.

## First playable slice

The game is centred on one village hub. The hub provides:
- one Pokemon Center;
- one Poke Mart;
- one evolving Gym;
- four small surrounding training areas: A, B, C and D;
- one Move Tutor, enabled after Gym level 3.

## Progression state

Use one progression variable for the Gym level. The intended states are:

| Gym level | Accessible training areas | Hub unlock |
| --- | --- | --- |
| 0 | A | Center, Mart, Gym |
| 1 | A, B | — |
| 2 | A, B, C | — |
| 3 | A, B, C, D | Move Tutor |

The same Gym map is reused at every level. Its trainers, leader team and dialogue are selected from the progression state. A Gym victory increments the progression state exactly once.

## Gate behaviour

Each exit from the village checks the Gym progression variable before allowing travel:
- A: always open;
- B: requires Gym level 1;
- C: requires Gym level 2;
- D: requires Gym level 3.

A locked exit must display a short message explaining which Gym level is required. No badge dependency is required for this prototype.

## Economy and services

Trainer battles keep the stock prize-money mechanism. The Mart uses the stock shop system and the Pokemon Center uses the stock healing/PC systems. This keeps the prototype compatible with the existing FireRed engine and avoids duplicating proven systems.

## Map implementation references

For implementation, reuse the existing structures already present in this repository:
- PalletTown for a small outdoor hub, connections and warps;
- standard Pokemon Center maps for healing and PC services;
- standard Poke Mart maps for shopping;
- ViridianCity_Gym for trainer events, Gym battle scripting and progression flags.

The visual layout can be replaced independently after the progression loop is functional.

## Implementation order

1. Create/re-purpose the village hub and four exits.
2. Gate B/C/D from the progression variable.
3. Connect Center, Mart and one Gym interior.
4. Implement Gym level 1 and unlock B.
5. Extend the same Gym script through levels 2 and 3.
6. Enable the Move Tutor at level 3.
7. Add area D and then expand later Gym levels only after this loop is playable.
