#ifndef GUARD_GYM_LEADER_H
#define GUARD_GYM_LEADER_H

#include "global.h"

void GymLeader_InitNewCareer(void);
void GymLeader_RecordDefenseResult(bool8 won);
u8 GymLeader_GetRecommendedLevel(void);
u8 GymLeader_GetChallengerLevel(void);
u8 GymLeader_GetChallengerPartySize(void);
u8 GymLeader_GetMaxPartySize(void);
u16 GymLeader_GetStarterSpecies(u8 specialty);
u8 GymLeader_SpecialtyToEngineType(u8 specialty);
bool8 GymLeader_SpeciesMatchesSpecialty(u16 species, u8 specialty);
bool8 GymLeader_IsCurrentPartyLegal(void);
u8 GymLeader_CountPartySpecialists(void);
u16 GymLeader_GetUpgradeCost(u8 upgradeIndex);
bool8 GymLeader_PurchaseUpgrade(u8 upgradeIndex);

void Special_GymLeader_InitCareer(void);
void Special_GymLeader_SetSpecialty(void);
void Special_GymLeader_GetStarter(void);
void Special_GymLeader_GiveStarter(void);
void Special_GymLeader_RecordWin(void);
void Special_GymLeader_RecordLoss(void);
void Special_GymLeader_GetRecommendedLevel(void);
void Special_GymLeader_GetChallengerLevel(void);
void Special_GymLeader_GetChallengerPartySize(void);
void Special_GymLeader_CheckPartyRules(void);
void Special_GymLeader_AdvanceWeek(void);
void Special_GymLeader_GetUpgradeCost(void);
void Special_GymLeader_PurchaseUpgrade(void);

#endif // GUARD_GYM_LEADER_H
