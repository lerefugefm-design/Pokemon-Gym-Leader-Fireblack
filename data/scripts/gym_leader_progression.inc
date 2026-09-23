# Gym progression script primitives
#
# These routines are intentionally map-independent so the village NPC/menu can call them
# while the visual maps are iterated separately.
#
# VAR_RESULT:
#   TryUpgrade routines: TRUE on success, FALSE when requirements are not met.
#   Attrition routine: number of challenger Pokemon removed.

GymLeader_EventScript_ResetTierProgress::
	setvar VAR_GYM_LEVEL_BATTLES, 0
	setvar VAR_GYM_CHALLENGER_LEVEL, 0
	return

GymLeader_EventScript_RecordChallengerBattle::
	addvar VAR_GYM_LEVEL_BATTLES, 1
	return

# Upgrade gates. Prices remain balance data; these first values make the economy testable.
# Level 1 -> 2 requires two defenders and full (1-KO) defender power.
GymLeader_EventScript_TryUpgradeToLevel2::
	setvar VAR_RESULT, FALSE
	goto_if_ne VAR_GYM_LEADER_LEVEL, 0, GymLeader_EventScript_UpgradeReturn
	goto_if_lt VAR_GYM_DEFENDER_COUNT, 2, GymLeader_EventScript_UpgradeReturn
	goto_if_lt VAR_GYM_DEFENDER_POWER, 2, GymLeader_EventScript_UpgradeReturn
	checkmoney 3000
	goto_if_eq VAR_RESULT, FALSE, GymLeader_EventScript_UpgradeReturn
	removemoney 3000
	setvar VAR_GYM_LEADER_LEVEL, 1
	call GymLeader_EventScript_ResetTierProgress
	setvar VAR_RESULT, TRUE
GymLeader_EventScript_UpgradeReturn::
	return

# Level 2 -> 3. This unlocks Zone C and the Move Tutor.
GymLeader_EventScript_TryUpgradeToLevel3::
	setvar VAR_RESULT, FALSE
	goto_if_ne VAR_GYM_LEADER_LEVEL, 1, GymLeader_EventScript_UpgradeReturn
	goto_if_lt VAR_GYM_DEFENDER_COUNT, 2, GymLeader_EventScript_UpgradeReturn
	checkmoney 7500
	goto_if_eq VAR_RESULT, FALSE, GymLeader_EventScript_UpgradeReturn
	removemoney 7500
	setvar VAR_GYM_LEADER_LEVEL, 2
	call GymLeader_EventScript_ResetTierProgress
	setvar VAR_RESULT, TRUE
	return

# Level 3 -> 4. This unlocks Zone D.
GymLeader_EventScript_TryUpgradeToLevel4::
	setvar VAR_RESULT, FALSE
	goto_if_ne VAR_GYM_LEADER_LEVEL, 2, GymLeader_EventScript_UpgradeReturn
	goto_if_lt VAR_GYM_DEFENDER_COUNT, 2, GymLeader_EventScript_UpgradeReturn
	goto_if_lt VAR_GYM_DEFENDER_POWER, 2, GymLeader_EventScript_UpgradeReturn
	checkmoney 15000
	goto_if_eq VAR_RESULT, FALSE, GymLeader_EventScript_UpgradeReturn
	removemoney 15000
	setvar VAR_GYM_LEADER_LEVEL, 3
	call GymLeader_EventScript_ResetTierProgress
	setvar VAR_RESULT, TRUE
	return

# Zone gates: call before a village exit warp.
GymLeader_EventScript_CheckZoneB::
	setvar VAR_RESULT, FALSE
	goto_if_lt VAR_GYM_LEADER_LEVEL, 1, GymLeader_EventScript_UpgradeReturn
	setvar VAR_RESULT, TRUE
	return

GymLeader_EventScript_CheckZoneC::
	setvar VAR_RESULT, FALSE
	goto_if_lt VAR_GYM_LEADER_LEVEL, 2, GymLeader_EventScript_UpgradeReturn
	setvar VAR_RESULT, TRUE
	return

GymLeader_EventScript_CheckZoneD::
	setvar VAR_RESULT, FALSE
	goto_if_lt VAR_GYM_LEADER_LEVEL, 3, GymLeader_EventScript_UpgradeReturn
	setvar VAR_RESULT, TRUE
	return

# Defender attrition in half-KO units. The caller uses VAR_RESULT to trim the
# challenger party after it has been generated.
GymLeader_EventScript_CalculateAttrition::
	setvar VAR_RESULT, 0
	goto_if_eq VAR_GYM_DEFENDER_COUNT, 0, GymLeader_EventScript_AttritionReturn
	goto_if_eq VAR_GYM_DEFENDER_POWER, 0, GymLeader_EventScript_AttritionReturn
	copyvar VAR_0x8004, VAR_GYM_DEFENDER_COUNT
	copyvar VAR_0x8005, VAR_GYM_DEFENDER_POWER
GymLeader_EventScript_AttritionLoop::
	subvar VAR_0x8005, 1
	goto_if_eq VAR_0x8005, 0, GymLeader_EventScript_AttritionPowerDone
	addvar VAR_0x8004, VAR_GYM_DEFENDER_COUNT
	goto GymLeader_EventScript_AttritionLoop
GymLeader_EventScript_AttritionPowerDone::
# Convert accumulated half-KOs into whole KOs by repeated subtraction.
	goto_if_lt VAR_0x8004, 2, GymLeader_EventScript_AttritionReturn
	subvar VAR_0x8004, 2
	addvar VAR_RESULT, 1
	goto GymLeader_EventScript_AttritionPowerDone
GymLeader_EventScript_AttritionReturn::
	return
