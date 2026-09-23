# Gym challenger attrition rules

The player's hired Gym defenders weaken challengers before the leader battle. These preliminary battles are abstracted: the game does not simulate full NPC-vs-NPC battles.

## Saved state

- `VAR_GYM_LEADER_LEVEL`: purchased Gym level.
- `VAR_GYM_DEFENDER_COUNT`: number of active hired defenders.
- `VAR_GYM_DEFENDER_POWER`: strength of each defender, stored in half-KO units.
  - 1 = half a Pokemon KO contribution.
  - 2 = one Pokemon KO contribution.

Two half-KO contributions combine into one removed challenger Pokemon. Fractions never remove a Pokemon by themselves.

## Current target balance

| Gym level | Challenger starts with | Defender effect | Example result |
| --- | ---: | ---: | --- |
| 1 | 4 | 1 Pokemon | 1 defender -> 3; 2 defenders -> 2 |
| 2 | 4 | 1/2 Pokemon | 1 defender -> 4; 2 defenders -> 3 |
| 3 | 6 | 1 Pokemon | 1 defender -> 5; 2 defenders -> 4 |
| 4 | 6 | 1/2 Pokemon | 1 defender -> 6; 2 defenders -> 5 |

The final challenger party size is:

`starting_party_size - floor(defender_count * defender_power / 2)`

The result must never fall below the minimum challenger party size selected for balancing.

## Upgrade loop

Money from trainer/challenger victories is retained through the stock money system. At a given Gym level the player can buy defender recruitment and permitted defender-strength upgrades. Reaching the current level's defender cap makes the next Gym-level purchase available; progression is never automatic after a victory.

Buying the next Gym level changes the surrounding-area access and the upgrade caps. Exact prices and caps remain balance data so they can be tuned without rewriting the progression scripts.
