@ Gym progression script primitives
@
@ Map-independent routines used by the village NPC/menu.

GymLeader_EventScript_ResetTierProgress::
	setvar VAR_GYM_LEVEL_BATTLES, 0
	setvar VAR_GYM_CHALLENGER_LEVEL, 0
	return

GymLeader_EventScript_RecordChallengerBattle::
	@ Defense results already increment this counter in GymLeader_RecordDefenseResult.
	@ Keep this helper for map scripts that need to record a non-standard battle.
	addvar VAR_GYM_LEVEL_BATTLES, 1
	return

@ Prepare a normal challenger encounter. Scripts can read:
@   VAR_0x8004 = scaled challenger level
@   VAR_0x8005 = effective party size after defender attrition.
GymLeader_EventScript_PrepareChallenger::
	special Special_GymLeader_GetChallengerLevel
	copyvar VAR_0x8004, VAR_RESULT
	special Special_GymLeader_GetChallengerPartySize
	copyvar VAR_0x8005, VAR_RESULT
	return

@ Upgrade gates. Prices are initial balancing values.
GymLeader_EventScript_TryUpgradeToLevel2::
	setvar VAR_RESULT, FALSE
	compare VAR_GYM_LEADER_LEVEL, 0
	goto_if_ne GymLeader_EventScript_UpgradeReturn
	compare VAR_GYM_DEFENDER_COUNT, 2
	goto_if_lt GymLeader_EventScript_UpgradeReturn
	compare VAR_GYM_DEFENDER_POWER, 2
	goto_if_lt GymLeader_EventScript_UpgradeReturn
	checkmoney 3000
	compare VAR_RESULT, FALSE
	goto_if_eq GymLeader_EventScript_UpgradeReturn
	removemoney 3000
	setvar VAR_GYM_LEADER_LEVEL, 1
	call GymLeader_EventScript_ResetTierProgress
	setvar VAR_RESULT, TRUE
GymLeader_EventScript_UpgradeReturn::
	return

GymLeader_EventScript_TryUpgradeToLevel3::
	setvar VAR_RESULT, FALSE
	compare VAR_GYM_LEADER_LEVEL, 1
	goto_if_ne GymLeader_EventScript_UpgradeReturn
	compare VAR_GYM_DEFENDER_COUNT, 2
	goto_if_lt GymLeader_EventScript_UpgradeReturn
	checkmoney 7500
	compare VAR_RESULT, FALSE
	goto_if_eq GymLeader_EventScript_UpgradeReturn
	removemoney 7500
	setvar VAR_GYM_LEADER_LEVEL, 2
	call GymLeader_EventScript_ResetTierProgress
	setvar VAR_RESULT, TRUE
	return

GymLeader_EventScript_TryUpgradeToLevel4::
	setvar VAR_RESULT, FALSE
	compare VAR_GYM_LEADER_LEVEL, 2
	goto_if_ne GymLeader_EventScript_UpgradeReturn
	compare VAR_GYM_DEFENDER_COUNT, 2
	goto_if_lt GymLeader_EventScript_UpgradeReturn
	compare VAR_GYM_DEFENDER_POWER, 2
	goto_if_lt GymLeader_EventScript_UpgradeReturn
	checkmoney 15000
	compare VAR_RESULT, FALSE
	goto_if_eq GymLeader_EventScript_UpgradeReturn
	removemoney 15000
	setvar VAR_GYM_LEADER_LEVEL, 3
	call GymLeader_EventScript_ResetTierProgress
	setvar VAR_RESULT, TRUE
	return

GymLeader_EventScript_CheckZoneB::
	setvar VAR_RESULT, FALSE
	compare VAR_GYM_LEADER_LEVEL, 1
	goto_if_lt GymLeader_EventScript_UpgradeReturn
	setvar VAR_RESULT, TRUE
	return

GymLeader_EventScript_CheckZoneC::
	setvar VAR_RESULT, FALSE
	compare VAR_GYM_LEADER_LEVEL, 2
	goto_if_lt GymLeader_EventScript_UpgradeReturn
	setvar VAR_RESULT, TRUE
	return

GymLeader_EventScript_CheckZoneD::
	setvar VAR_RESULT, FALSE
	compare VAR_GYM_LEADER_LEVEL, 3
	goto_if_lt GymLeader_EventScript_UpgradeReturn
	setvar VAR_RESULT, TRUE
	return

@ Defender attrition is stored in half-KO units. VAR_RESULT returns whole KOs.
GymLeader_EventScript_CalculateAttrition::
	setvar VAR_RESULT, 0
	compare VAR_GYM_DEFENDER_COUNT, 0
	goto_if_eq GymLeader_EventScript_AttritionReturn
	compare VAR_GYM_DEFENDER_POWER, 0
	goto_if_eq GymLeader_EventScript_AttritionReturn
	copyvar VAR_0x8004, VAR_GYM_DEFENDER_COUNT
	copyvar VAR_0x8005, VAR_GYM_DEFENDER_POWER
GymLeader_EventScript_AttritionLoop::
	subvar VAR_0x8005, 1
	compare VAR_0x8005, 0
	goto_if_eq GymLeader_EventScript_AttritionPowerDone
	addvar VAR_0x8004, VAR_GYM_DEFENDER_COUNT
	goto GymLeader_EventScript_AttritionLoop
GymLeader_EventScript_AttritionPowerDone::
	compare VAR_0x8004, 2
	goto_if_lt GymLeader_EventScript_AttritionReturn
	subvar VAR_0x8004, 2
	addvar VAR_RESULT, 1
	goto GymLeader_EventScript_AttritionPowerDone
GymLeader_EventScript_AttritionReturn::
	return
