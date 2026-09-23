@ Pokemon Gym Leader - career scripts
.equ GYM_VAR_SPECIALTY, 0x40D0
.equ GYM_VAR_RANK,      0x40D1
.equ GYM_VAR_REPUTATION,0x40D2
.equ GYM_VAR_BUDGET,    0x40D3
.equ GYM_VAR_WEEK,      0x40D7

GymLeader_EventScript_StartCareer::
    lockall
    special Special_GymLeader_InitCareer
    special Special_GymLeader_SetSpecialty
    special Special_GymLeader_GiveStarter
    msgbox GymLeader_Text_CareerBegins, MSGBOX_DEFAULT
    releaseall
    end

GymLeader_EventScript_RecordDefenseWin::
    special Special_GymLeader_RecordWin
    msgbox GymLeader_Text_DefenseWon, MSGBOX_DEFAULT
    end

GymLeader_EventScript_RecordDefenseLoss::
    special Special_GymLeader_RecordLoss
    msgbox GymLeader_Text_DefenseLost, MSGBOX_DEFAULT
    end

GymLeader_EventScript_CheckParty::
    specialvar VAR_RESULT, Special_GymLeader_CheckPartyRules
    compare VAR_RESULT, TRUE
    goto_if_eq GymLeader_EventScript_PartyLegal
    msgbox GymLeader_Text_PartyIllegal, MSGBOX_DEFAULT
    end

GymLeader_EventScript_PartyLegal::
    msgbox GymLeader_Text_PartyLegal, MSGBOX_DEFAULT
    end

GymLeader_EventScript_NextWeek::
    special Special_GymLeader_AdvanceWeek
    msgbox GymLeader_Text_NewWeek, MSGBOX_DEFAULT
    end

GymLeader_EventScript_PurchaseUpgrade::
    specialvar VAR_RESULT, Special_GymLeader_PurchaseUpgrade
    compare VAR_RESULT, TRUE
    goto_if_eq GymLeader_EventScript_UpgradeBought
    msgbox GymLeader_Text_UpgradeFailed, MSGBOX_DEFAULT
    end

GymLeader_EventScript_UpgradeBought::
    msgbox GymLeader_Text_UpgradeBought, MSGBOX_DEFAULT
    end

GymLeader_Text_CareerBegins:
    .string "La Ligue enregistre ta specialite.\p"
    .string "Ton premier Pokemon devient ton Pokemon signature.\p"
    .string "Ton arene est provisoire: deux Pokemon maximum.$"

GymLeader_Text_DefenseWon:
    .string "Defense reussie!\p"
    .string "La reputation de l'arene augmente, ainsi que son budget.$"

GymLeader_Text_DefenseLost:
    .string "Le challenger merite son Badge.\p"
    .string "La saison continue: une defaite ne met jamais fin a ta carriere.$"

GymLeader_Text_PartyIllegal:
    .string "Cette equipe ne respecte pas encore le reglement de la Ligue.$"

GymLeader_Text_PartyLegal:
    .string "Equipe validee. La defense peut commencer.$"

GymLeader_Text_NewWeek:
    .string "Une nouvelle semaine commence.\p"
    .string "Un nouveau challenger a ete inscrit par la Ligue.$"

GymLeader_Text_UpgradeBought:
    .string "Amenagement installe. L'arene progresse.$"

GymLeader_Text_UpgradeFailed:
    .string "Cet amenagement est deja installe, trop cher, ou pas encore disponible.$"
