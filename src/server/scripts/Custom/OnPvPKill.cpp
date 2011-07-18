#include "ScriptPCH.h"

struct SystemInfo
{
    uint32 KillStreak;
    uint32 LastKillStreak;
    uint32 LastGUIDKill;
    char ColoredName[30];
};
        
static std::map<uint32, SystemInfo> KillingStreak;

class System_OnPvPKill : public PlayerScript
{
    public:
        System_OnPvPKill() : PlayerScript("System_OnPvPKill") {}

        void ColorClass(char *output, Player *pPlayer)
        {
            switch(pPlayer->getClass())
            {
                case CLASS_WARRIOR:
                    sprintf(output, "|cff%s%s|r", "C79C6E", pPlayer->GetName());
                    break;
                case CLASS_PALADIN:
                    sprintf(output, "|cff%s%s|r", "F58CBA", pPlayer->GetName());
                    break;
                case CLASS_HUNTER:
                    sprintf(output, "|cff%s%s|r", "ABD473", pPlayer->GetName());
                    break;
                case CLASS_ROGUE:
                    sprintf(output, "|cff%s%s|r", "FFF569", pPlayer->GetName());
                    break;
                case CLASS_PRIEST:
                    sprintf(output, "|cff%s%s|r", "FFFFFF", pPlayer->GetName());
                    break;
                case CLASS_DEATH_KNIGHT:
                    sprintf(output, "|cff%s%s|r", "C41F3B", pPlayer->GetName());
                    break;
                case CLASS_SHAMAN:
                    sprintf(output, "|cff%s%s|r", "0070DE", pPlayer->GetName());
                    break;
                case CLASS_MAGE:
                    sprintf(output, "|cff%s%s|r", "69CCF0", pPlayer->GetName());
                    break;
                case CLASS_WARLOCK:
                    sprintf(output, "|cff%s%s|r", "9482C9", pPlayer->GetName());
                    break;
                case CLASS_DRUID:
                    sprintf(output, "|cff%s%s|r", "FF7D0A", pPlayer->GetName());
                    break;
                default:
                    sLog->outError("OnPVPKill :: ColorClass :: Unknown class %d.", pPlayer->getClass());
                    sprintf(output, "|cff%s%s|r", "999999", pPlayer->GetName());
                    break;
            }
            return;
        }

        void OnPVPKill(Player *pKiller, Player *pVictim)
        {
            char msg[500];
            char msg2[500];
            uint32 kGUID;
            uint32 vGUID;
            kGUID = pKiller->GetGUID();
            vGUID = pVictim->GetGUID();
            int32 honor = 20;

            sLog->outError("OnPVPKill :: %s killed %s.", pKiller->GetName(), pVictim->GetName());

            if (kGUID == vGUID)
                return;
            if (KillingStreak[kGUID].LastGUIDKill == vGUID)
                return;

            sLog->outError("OnPVPKill :: Passed checks.");
            
            KillingStreak[kGUID].KillStreak++;
            KillingStreak[vGUID].LastKillStreak = KillingStreak[vGUID].KillStreak;
            KillingStreak[vGUID].KillStreak = 0;
            KillingStreak[kGUID].LastGUIDKill = vGUID;
            KillingStreak[vGUID].LastGUIDKill = 0;
            
            ColorClass(KillingStreak[kGUID].ColoredName, pKiller);
            ColorClass(KillingStreak[vGUID].ColoredName, pVictim);

            sLog->outError("OnPVPKill :: %s is on a killstreak of %d.", pKiller->GetName(), KillingStreak[kGUID].KillStreak);

            if (KillingStreak[vGUID].LastKillStreak >= 5)
            {
                pKiller->ModifyCurrency(CURRENCY_TYPE_HONOR_POINTS, int32(honor) * (100/2.4) * KillingStreak[vGUID].LastKillStreak);
                if (KillingStreak[vGUID].LastKillStreak >= 10)
                {
                    if (KillingStreak[vGUID].LastKillStreak >= 15)
                    {
                        if (KillingStreak[vGUID].LastKillStreak >= 20) // 20+
                        {
                            sprintf(msg2, "%s's |cffff6600GODLIKE|r (%d) streak was ended by %s!", KillingStreak[vGUID].ColoredName, KillingStreak[vGUID].LastKillStreak, KillingStreak[kGUID].ColoredName);
                            sWorld->SendWorldText(LANG_PVP_SYSTEMMESSAGE, msg2);
                        }
                        else // 15-20
                        {
                            sprintf(msg2, "%s's |cffff6600LEGENDARY|r (%d) streak was ended by %s!", KillingStreak[vGUID].ColoredName, KillingStreak[vGUID].LastKillStreak, KillingStreak[kGUID].ColoredName);
                            sWorld->SendWorldText(LANG_PVP_SYSTEMMESSAGE, msg2);
                        }
                    }
                    else // 10-15
                    {
                        sprintf(msg2, "%s's |cffff6600DOMINATING|r (%d) streak was ended by %s!", KillingStreak[vGUID].ColoredName, KillingStreak[vGUID].LastKillStreak, KillingStreak[kGUID].ColoredName);
                        sWorld->SendWorldText(LANG_PVP_SYSTEMMESSAGE, msg2);
                    }
                }
                else // 5-10
                {
                    sprintf(msg2, "%s's |cffff6600SERIAL KILLER|r (%d) streak was ended by %s!", KillingStreak[vGUID].ColoredName, KillingStreak[vGUID].LastKillStreak, KillingStreak[kGUID].ColoredName);
                    sWorld->SendWorldText(LANG_PVP_SYSTEMMESSAGE, msg2);
                }
            }

            switch(KillingStreak[kGUID].KillStreak)
            {
                case 5:
                    sprintf(msg, "%s is a |cffff6600SERIAL KILLER|r! (%d kill streak).", KillingStreak[kGUID].ColoredName, KillingStreak[kGUID].KillStreak);
                    sWorld->SendWorldText(LANG_PVP_SYSTEMMESSAGE, msg);
                    pKiller->ModifyCurrency(CURRENCY_TYPE_HONOR_POINTS, int32(honor) * (100/2.4) * KillingStreak[kGUID].KillStreak);
                    break;
                case 10:
                    sprintf(msg, "%s is |cffff6600DOMINATING|r! (%d kill streak).", KillingStreak[kGUID].ColoredName, KillingStreak[kGUID].KillStreak);
                    sWorld->SendWorldText(LANG_PVP_SYSTEMMESSAGE, msg);
                    pKiller->ModifyCurrency(CURRENCY_TYPE_HONOR_POINTS, int32(honor) * (100/2.4) * KillingStreak[kGUID].KillStreak);
                    break;
                case 15:
                    sprintf(msg, "%s is |cffff6600LEGENDARY|r! (%d kill streak).", KillingStreak[kGUID].ColoredName, KillingStreak[kGUID].KillStreak);
                    sWorld->SendWorldText(LANG_PVP_SYSTEMMESSAGE, msg);
                    pKiller->ModifyCurrency(CURRENCY_TYPE_HONOR_POINTS, int32(honor) * (100/2.4) * KillingStreak[kGUID].KillStreak);
                    break;
                case 20:
                    sprintf(msg, "%s is |cffff0000GODLIKE|r! Someone kill this player! (%d kill streak).", KillingStreak[kGUID].ColoredName, KillingStreak[kGUID].KillStreak);
                    sWorld->SendWorldText(LANG_PVP_SYSTEMMESSAGE, msg);
                    pKiller->ModifyCurrency(CURRENCY_TYPE_HONOR_POINTS, int32(honor) * (100/2.4) * KillingStreak[kGUID].KillStreak);
                    break;
                default:
                    if (KillingStreak[kGUID].KillStreak > 20)
                    {
                        sprintf(msg, "%s is |cffff0000GODLIKE|r! Someone kill this player! (%d kill streak).", KillingStreak[kGUID].ColoredName, KillingStreak[kGUID].KillStreak);
                        sWorld->SendWorldText(LANG_PVP_SYSTEMMESSAGE, msg);
                        pKiller->ModifyCurrency(CURRENCY_TYPE_HONOR_POINTS, int32(honor) * (100/2.4));
                    }
                    break;
            }
        }
};

void AddSC_PVP_System()
{
    new System_OnPvPKill;
}
