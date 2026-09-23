# Dynamic challenger and roguelite progression

## Core loop

The Gym is a repeatable progression loop rather than a sequence of automatic badges:

1. Fight challengers.
2. Earn money through the existing battle economy.
3. Recruit and improve Gym defenders.
4. Optionally entrust one of the player's Pokemon to a defender for passive training.
5. Meet the upgrade requirements and pay for the next Gym level.
6. Unlock the next training area and a higher difficulty ceiling.
7. Reset the per-level challenger battle counter and repeat.

## Challenger scaling

Challenger level is calculated from three inputs:

- purchased Gym level;
- battles completed at the current Gym level;
- the lowest-level usable Pokemon in the player's current party.

The weakest party member is the anchor so one over-levelled ace cannot by itself force the entire challenger roster upward. Repeated battles add gradual pressure. The calculated result is always clamped to a level cap defined for the current Gym level.

`challenger_level = min(gym_level_cap, weakest_party_level + battle_progress_bonus)`

The exact cap table and battle-progress curve are balance data and must remain easy to tune.

`VAR_GYM_LEVEL_BATTLES` counts challenger battles only for the current purchased Gym level and resets when the player buys the next Gym upgrade. `VAR_GYM_CHALLENGER_LEVEL` stores the calculated level used when preparing a challenger encounter.

## Defender attrition

Before the leader battle, hired defenders reduce the challenger's available party according to `GYM_CHALLENGER_ATTRITION.md`. This is deterministic and does not require simulating NPC battles.

The challenger is generated first, then defender attrition is applied, then the player fights the remaining party.

## Entrusted Pokemon

A player may entrust a Pokemon to a hired Gym defender. The Pokemon is unavailable to the active party while entrusted. Challenger activity grants limited training progress to that Pokemon.

Saved prototype state:
- `VAR_GYM_TRAINEE_SLOT`: entrusted-Pokemon slot/handle;
- `VAR_GYM_TRAINEE_EXP`: accumulated training progress.

The final implementation should reuse existing Pokemon storage/day-care primitives where practical rather than duplicating full Pokemon serialization in vars.

Passive training is capped by Gym level. It must not allow a Pokemon to exceed the current Gym's training cap merely by waiting or farming weak challengers.

## Roguelite direction

The repeatable challenger loop is intentionally compatible with a light roguelite structure. Difficulty can rise within each Gym tier while permanent purchases improve the player's defensive setup. The prototype should keep runs short and readable; random modifiers, streak rewards and special challengers can be layered on later without changing the saved progression model.

## Required implementation order

1. Challenger level calculator and per-Gym battle counter.
2. Level-cap balance table.
3. Defender attrition applied to generated challenger parties.
4. Upgrade purchase/recruitment scripts.
5. Entrusted-Pokemon storage and EXP award.
6. Optional roguelite streak/modifier layer only after the base loop is playable.
