# Gym roguelite loop

The Gym campaign should behave as a compact management roguelite while retaining standard Pokemon battles.

## Challenger scaling

A challenger is generated from three inputs:
1. purchased Gym level;
2. challenger battles already completed at that Gym level;
3. the lowest-level usable Pokemon in the player's current party.

The weakest party member is the anchor, not the strongest. Battle count adds gradual pressure. Every Gym level has a hard level cap, so neither a highly trained ace nor repeated battles can push challengers beyond that tier.

`VAR_GYM_LEVEL_BATTLES` stores battles completed in the current tier and resets to zero only when the player purchases the next Gym level.

Exact caps, growth intervals and minimum levels are balance constants and must remain data-driven.

## Defender attrition

Before the leader battle, hired defenders reduce the challenger's party according to `GYM_CHALLENGER_ATTRITION.md`. Attrition is deterministic and happens after the challenger party has been selected.

## Pokemon assigned to a defender

The player may temporarily assign one owned Pokemon to a hired Gym defender for training. It is unavailable to the leader while assigned. Challenger activity awards a modest amount of training EXP, accumulated in `VAR_GYM_TRAINING_EXP`.

Training is capped by the current Gym tier. It must not allow the assigned Pokemon to exceed that tier's training cap. Retrieval returns the Pokemon and applies only legal accumulated EXP.

The implementation should reuse existing Pokemon storage/day-care primitives where practical instead of inventing an incompatible Pokemon serialization format.

## Core run loop

Explore unlocked area -> train/catch Pokemon -> defend Gym against challengers -> earn money -> recruit/upgrade defenders -> optionally train a Pokemon with a defender -> reach the tier's management requirements -> voluntarily buy the next Gym upgrade -> unlock the next area and reset that tier's challenger-battle counter.

Failure must not automatically downgrade the Gym. Roguelite penalties/rewards can be added later after the core loop is playable.
