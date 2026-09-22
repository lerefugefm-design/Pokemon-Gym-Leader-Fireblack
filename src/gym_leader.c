#include "global.h"
#include "event_data.h"
#include "pokemon.h"
#include "random.h"
#include "gym_leader.h"
#include "constants/gym_leader.h"
#include "constants/pokemon.h"
#include "constants/species.h"

struct GymRankRules
{
    u8 recommendedLevel;
    u8 maxPartySize;
    u16 promotionReputation;
};

static const struct GymRankRules sGymRankRules[] =
{
    {12, 2,   0},
    {18, 2,   8},
    {24, 3,  20},
    {30, 3,  38},
    {36, 4,  60},
    {42, 5,  88},
    {50, 6, 120},
    {58, 6, 160},
};

static const u16 sGymUpgradeCosts[] =
{
    1200, 1800, 2200, 1500, 3500, 2800, 2600, 5000,
};

static const u16 sDefaultStarterBySpecialty[GYM_SPECIALTY_COUNT] =
{
    [GYM_SPECIALTY_NORMAL]   = SPECIES_EEVEE,
    [GYM_SPECIALTY_FIGHTING] = SPECIES_MACHOP,
    [GYM_SPECIALTY_FLYING]   = SPECIES_SWABLU,
    [GYM_SPECIALTY_POISON]   = SPECIES_NIDORAN_F,
    [GYM_SPECIALTY_GROUND]   = SPECIES_TRAPINCH,
    [GYM_SPECIALTY_ROCK]     = SPECIES_ARON,
    [GYM_SPECIALTY_BUG]      = SPECIES_SCYTHER,
    [GYM_SPECIALTY_GHOST]    = SPECIES_GASTLY,
    [GYM_SPECIALTY_STEEL]    = SPECIES_ARON,
    [GYM_SPECIALTY_FIRE]     = SPECIES_CHARMANDER,
    [GYM_SPECIALTY_WATER]    = SPECIES_SQUIRTLE,
    [GYM_SPECIALTY_GRASS]    = SPECIES_BULBASAUR,
    [GYM_SPECIALTY_ELECTRIC] = SPECIES_MAREEP,
    [GYM_SPECIALTY_PSYCHIC]  = SPECIES_RALTS,
    [GYM_SPECIALTY_ICE]      = SPECIES_SNORUNT,
    [GYM_SPECIALTY_DRAGON]   = SPECIES_DRATINI,
    [GYM_SPECIALTY_DARK]     = SPECIES_ABSOL,
    [GYM_SPECIALTY_FAIRY]    = SPECIES_CLEFAIRY,
};

static u8 GetRankFromReputation(u16 reputation)
{
    s32 i;
    for (i = ARRAY_COUNT(sGymRankRules) - 1; i >= 0; i--)
    {
        if (reputation >= sGymRankRules[i].promotionReputation)
            return i;
    }
    return 0;
}

u8 GymLeader_SpecialtyToEngineType(u8 specialty)
{
    static const u8 sEngineTypes[GYM_SPECIALTY_COUNT] =
    {
        [GYM_SPECIALTY_NORMAL]   = TYPE_NORMAL,
        [GYM_SPECIALTY_FIGHTING] = TYPE_FIGHTING,
        [GYM_SPECIALTY_FLYING]   = TYPE_FLYING,
        [GYM_SPECIALTY_POISON]   = TYPE_POISON,
        [GYM_SPECIALTY_GROUND]   = TYPE_GROUND,
        [GYM_SPECIALTY_ROCK]     = TYPE_ROCK,
        [GYM_SPECIALTY_BUG]      = TYPE_BUG,
        [GYM_SPECIALTY_GHOST]    = TYPE_GHOST,
        [GYM_SPECIALTY_STEEL]    = TYPE_STEEL,
        [GYM_SPECIALTY_FIRE]     = TYPE_FIRE,
        [GYM_SPECIALTY_WATER]    = TYPE_WATER,
        [GYM_SPECIALTY_GRASS]    = TYPE_GRASS,
        [GYM_SPECIALTY_ELECTRIC] = TYPE_ELECTRIC,
        [GYM_SPECIALTY_PSYCHIC]  = TYPE_PSYCHIC,
        [GYM_SPECIALTY_ICE]      = TYPE_ICE,
        [GYM_SPECIALTY_DRAGON]   = TYPE_DRAGON,
        [GYM_SPECIALTY_DARK]     = TYPE_DARK,
        [GYM_SPECIALTY_FAIRY]    = TYPE_FAIRY,
    };

    if (specialty >= GYM_SPECIALTY_COUNT)
        return TYPE_NORMAL;
    return sEngineTypes[specialty];
}

void GymLeader_InitNewCareer(void)
{
    VarSet(VAR_GYM_SPECIALTY_TYPE, GYM_SPECIALTY_NORMAL);
    VarSet(VAR_GYM_RANK, 0);
    VarSet(VAR_GYM_REPUTATION, 0);
    VarSet(VAR_GYM_BUDGET, 1000);
    VarSet(VAR_GYM_DEFENSE_COUNT, 0);
    VarSet(VAR_GYM_WIN_STREAK, 0);
    VarSet(VAR_GYM_BADGES_ISSUED, 0);
    VarSet(VAR_GYM_WEEK, 1);
    VarSet(VAR_GYM_CHALLENGER_SEED, Random());
    VarSet(VAR_GYM_UPGRADES, 0);
    VarSet(VAR_GYM_LAST_RESULT, GYM_RESULT_NONE);
    VarSet(VAR_GYM_INSPECTION_PROGRESS, 0);
    VarSet(VAR_GYM_SIGNATURE_SPECIES, SPECIES_NONE);
    VarSet(VAR_GYM_RULESET, GYM_RULESET_STRICT_MONOTYPE);
}

void GymLeader_RecordDefenseResult(bool8 won)
{
    u16 rank = VarGet(VAR_GYM_RANK);
    u16 reputation = VarGet(VAR_GYM_REPUTATION);
    u16 budget = VarGet(VAR_GYM_BUDGET);

    VarSet(VAR_GYM_DEFENSE_COUNT, VarGet(VAR_GYM_DEFENSE_COUNT) + 1);

    if (won)
    {
        reputation += 3;
        budget += 500 + rank * 250;
        VarSet(VAR_GYM_WIN_STREAK, VarGet(VAR_GYM_WIN_STREAK) + 1);
        VarSet(VAR_GYM_LAST_RESULT, GYM_RESULT_WIN);
    }
    else
    {
        reputation = (reputation > 2) ? reputation - 2 : 0;
        budget += 200;
        VarSet(VAR_GYM_WIN_STREAK, 0);
        VarSet(VAR_GYM_BADGES_ISSUED, VarGet(VAR_GYM_BADGES_ISSUED) + 1);
        VarSet(VAR_GYM_LAST_RESULT, GYM_RESULT_LOSS);
    }

    if (budget > 60000)
        budget = 60000;

    VarSet(VAR_GYM_REPUTATION, reputation);
    VarSet(VAR_GYM_BUDGET, budget);
    VarSet(VAR_GYM_RANK, GetRankFromReputation(reputation));
    VarSet(VAR_GYM_RULESET, VarGet(VAR_GYM_RANK) >= 4 ? GYM_RULESET_ONE_WILDCARD : GYM_RULESET_STRICT_MONOTYPE);
}

u8 GymLeader_GetRecommendedLevel(void)
{
    u16 rank = VarGet(VAR_GYM_RANK);
    u16 defenses = VarGet(VAR_GYM_DEFENSE_COUNT);
    u8 level;

    if (rank >= ARRAY_COUNT(sGymRankRules))
        rank = ARRAY_COUNT(sGymRankRules) - 1;

    level = sGymRankRules[rank].recommendedLevel;
    level += min(defenses % 4, 3);
    return level;
}

u8 GymLeader_GetMaxPartySize(void)
{
    u16 rank = VarGet(VAR_GYM_RANK);
    if (rank >= ARRAY_COUNT(sGymRankRules))
        rank = ARRAY_COUNT(sGymRankRules) - 1;
    return sGymRankRules[rank].maxPartySize;
}

u16 GymLeader_GetStarterSpecies(u8 specialty)
{
    if (specialty >= GYM_SPECIALTY_COUNT)
        specialty = GYM_SPECIALTY_NORMAL;
    return sDefaultStarterBySpecialty[specialty];
}

bool8 GymLeader_SpeciesMatchesSpecialty(u16 species, u8 specialty)
{
    u8 type;
    if (species == SPECIES_NONE || species >= NUM_SPECIES)
        return FALSE;

    type = GymLeader_SpecialtyToEngineType(specialty);
    return gSpeciesInfo[species].types[0] == type || gSpeciesInfo[species].types[1] == type;
}

u8 GymLeader_CountPartySpecialists(void)
{
    u8 i;
    u8 count = 0;
    u8 specialty = VarGet(VAR_GYM_SPECIALTY_TYPE);

    for (i = 0; i < PARTY_SIZE; i++)
    {
        u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES);
        if (species != SPECIES_NONE && GymLeader_SpeciesMatchesSpecialty(species, specialty))
            count++;
    }
    return count;
}

u16 GymLeader_GetUpgradeCost(u8 upgradeIndex)
{
    if (upgradeIndex >= ARRAY_COUNT(sGymUpgradeCosts))
        return 0;
    return sGymUpgradeCosts[upgradeIndex];
}

bool8 GymLeader_PurchaseUpgrade(u8 upgradeIndex)
{
    u16 budget = VarGet(VAR_GYM_BUDGET);
    u16 flags = VarGet(VAR_GYM_UPGRADES);
    u16 cost = GymLeader_GetUpgradeCost(upgradeIndex);
    u16 mask;

    if (upgradeIndex >= 8 || cost == 0)
        return FALSE;

    mask = 1 << upgradeIndex;
    if (flags & mask)
        return FALSE;

    if (upgradeIndex == 4 && !(flags & GYM_UPGRADE_PUZZLE_BASIC))
        return FALSE;

    if (budget < cost)
        return FALSE;

    VarSet(VAR_GYM_BUDGET, budget - cost);
    VarSet(VAR_GYM_UPGRADES, flags | mask);
    return TRUE;
}

bool8 GymLeader_IsCurrentPartyLegal(void)
{
    u8 i;
    u8 total = 0;
    u8 specialists = 0;
    u8 specialty = VarGet(VAR_GYM_SPECIALTY_TYPE);
    u8 maxParty = GymLeader_GetMaxPartySize();
    u8 wildcardAllowance = VarGet(VAR_GYM_RULESET) == GYM_RULESET_ONE_WILDCARD ? 1 : 0;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        u16 species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES);
        if (species == SPECIES_NONE)
            continue;

        total++;
        if (GymLeader_SpeciesMatchesSpecialty(species, specialty))
            specialists++;
    }

    if (total == 0 || total > maxParty)
        return FALSE;
    return (total - specialists) <= wildcardAllowance;
}

void Special_GymLeader_InitCareer(void)
{
    GymLeader_InitNewCareer();
}

void Special_GymLeader_SetSpecialty(void)
{
    u16 specialty = gSpecialVar_0x8004;
    if (specialty >= GYM_SPECIALTY_COUNT)
        specialty = GYM_SPECIALTY_NORMAL;

    VarSet(VAR_GYM_SPECIALTY_TYPE, specialty);
    VarSet(VAR_GYM_SIGNATURE_SPECIES, GymLeader_GetStarterSpecies(specialty));
}

void Special_GymLeader_GetStarter(void)
{
    gSpecialVar_Result = GymLeader_GetStarterSpecies(VarGet(VAR_GYM_SPECIALTY_TYPE));
}

void Special_GymLeader_GiveStarter(void)
{
    struct Pokemon mon;
    u16 species = GymLeader_GetStarterSpecies(VarGet(VAR_GYM_SPECIALTY_TYPE));

    CreateMon(&mon, species, 5, USE_RANDOM_IVS, FALSE, 0, OT_ID_PLAYER_ID, 0);
    gSpecialVar_Result = GiveMonToPlayer(&mon);
    VarSet(VAR_GYM_SIGNATURE_SPECIES, species);
}

void Special_GymLeader_RecordWin(void)
{
    GymLeader_RecordDefenseResult(TRUE);
}

void Special_GymLeader_RecordLoss(void)
{
    GymLeader_RecordDefenseResult(FALSE);
}

void Special_GymLeader_GetRecommendedLevel(void)
{
    gSpecialVar_Result = GymLeader_GetRecommendedLevel();
}

void Special_GymLeader_CheckPartyRules(void)
{
    gSpecialVar_Result = GymLeader_IsCurrentPartyLegal();
}

void Special_GymLeader_AdvanceWeek(void)
{
    VarSet(VAR_GYM_WEEK, VarGet(VAR_GYM_WEEK) + 1);
    VarSet(VAR_GYM_CHALLENGER_SEED, Random());
}

void Special_GymLeader_GetUpgradeCost(void)
{
    gSpecialVar_Result = GymLeader_GetUpgradeCost(gSpecialVar_0x8004);
}

void Special_GymLeader_PurchaseUpgrade(void)
{
    gSpecialVar_Result = GymLeader_PurchaseUpgrade(gSpecialVar_0x8004);
}
