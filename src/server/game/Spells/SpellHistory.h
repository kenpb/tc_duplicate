/*
 * Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SpellHistory_h__
#define SpellHistory_h__

#include "SharedDefines.h"
#include "QueryResult.h"
#include "Transaction.h"
#include <chrono>
#include <deque>

class Item;
class Player;
class Spell;
class SpellInfo;
class Unit;
struct SpellCategoryEntry;

/// Spell cooldown flags sent in SMSG_SPELL_COOLDOWN
enum SpellCooldownFlags
{
    SPELL_COOLDOWN_FLAG_NONE                    = 0x0,
    SPELL_COOLDOWN_FLAG_INCLUDE_GCD             = 0x1,  ///< Starts GCD in addition to normal cooldown specified in the packet
    SPELL_COOLDOWN_FLAG_INCLUDE_EVENT_COOLDOWNS = 0x2   ///< Starts GCD for spells that should start their cooldown on events, requires SPELL_COOLDOWN_FLAG_INCLUDE_GCD set
};

class SpellHistory
{
public:
    typedef std::chrono::system_clock Clock;

    struct CooldownEntry
    {
        Clock::time_point CooldownEnd;
        uint32 ItemId = 0;
        uint32 CategoryId = 0;
        Clock::time_point CategoryEnd;
        bool OnHold = false;
    };

    struct ChargeEntry
    {
        ChargeEntry() { }
        ChargeEntry(Clock::time_point startTime, std::chrono::milliseconds rechargeTime) : RechargeStart(startTime), RechargeEnd(startTime + rechargeTime) { }
        ChargeEntry(Clock::time_point startTime, Clock::time_point endTime) : RechargeStart(startTime), RechargeEnd(endTime) { }

        Clock::time_point RechargeStart;
        Clock::time_point RechargeEnd;
    };

    typedef std::unordered_map<uint32 /*spellId*/, CooldownEntry> CooldownStorageType;
    typedef std::unordered_map<uint32 /*categoryId*/, CooldownEntry*> CategoryCooldownStorageType;
    typedef std::unordered_map<uint32 /*categoryId*/, std::deque<ChargeEntry>> ChargeStorageType;
    typedef std::unordered_map<uint32 /*categoryId*/, Clock::time_point> GlobalCooldownStorageType;

    explicit SpellHistory(Unit* owner) : _owner(owner), _schoolLockouts() { }

    template<class OwnerType>
    void LoadFromDB(PreparedQueryResult cooldownsResult, PreparedQueryResult chargesResult);

    template<class OwnerType>
    void SaveToDB(SQLTransaction& trans);

    void Update();

    void HandleCooldowns(SpellInfo const* spellInfo, Item const* item, Spell* spell = nullptr);
    void HandleCooldowns(SpellInfo const* spellInfo, uint32 itemID, Spell* spell = nullptr);
    bool IsReady(SpellInfo const* spellInfo, uint32 itemId = 0) const;
    template<class PacketType>
    void WritePacket(PacketType* packet) const;

    // Cooldowns
    static Clock::duration const InfinityCooldownDelay;  // used for set "infinity cooldowns" for spells and check

    void StartCooldown(SpellInfo const* spellInfo, uint32 itemId, Spell* spell = nullptr, bool onHold = false);
    void SendCooldownEvent(SpellInfo const* spellInfo, uint32 itemId = 0, Spell* spell = nullptr, bool startCooldown = true);

    template<class Type, class Period>
    void AddCooldown(uint32 spellId, uint32 itemId, std::chrono::duration<Type, Period> cooldownDuration)
    {
        Clock::time_point now = Clock::now();
        AddCooldown(spellId, itemId, now + std::chrono::duration_cast<Clock::duration>(cooldownDuration), 0, now);
    }

    void AddCooldown(uint32 spellId, uint32 itemId, Clock::time_point cooldownEnd, uint32 categoryId, Clock::time_point categoryEnd, bool onHold = false);
    void ModifyCooldown(uint32 spellId, int32 cooldownModMs);
    void ResetCooldown(uint32 spellId, bool update = false);
    void ResetCooldown(CooldownStorageType::iterator& itr, bool update = false);
    template<typename Predicate>
    void ResetCooldowns(Predicate predicate, bool update = false)
    {
        std::vector<int32> resetCooldowns;
        resetCooldowns.reserve(_spellCooldowns.size());
        for (auto itr = _spellCooldowns.begin(); itr != _spellCooldowns.end();)
        {
            if (predicate(itr))
            {
                resetCooldowns.push_back(int32(itr->first));
                ResetCooldown(itr, false);
            }
            else
                ++itr;
        }

        if (update && !resetCooldowns.empty())
            SendClearCooldowns(resetCooldowns);
    }

    void ResetAllCooldowns();
    bool HasCooldown(SpellInfo const* spellInfo, uint32 itemId = 0) const;
    bool HasCooldown(uint32 spellId, uint32 itemId = 0) const;
    uint32 GetRemainingCooldown(SpellInfo const* spellInfo) const;

    // School lockouts
    void LockSpellSchool(SpellSchoolMask schoolMask, uint32 lockoutTime);
    bool IsSchoolLocked(SpellSchoolMask schoolMask) const;

    // Charges
    bool ConsumeCharge(SpellCategoryEntry const* chargeCategoryEntry);
    void RestoreCharge(SpellCategoryEntry const* chargeCategoryEntry);
    void ResetCharges(SpellCategoryEntry const* chargeCategoryEntry);
    void ResetAllCharges();
    bool HasCharge(SpellCategoryEntry const* chargeCategoryEntry) const;
    int32 GetMaxCharges(SpellCategoryEntry const* chargeCategoryEntry) const;
    int32 GetChargeRecoveryTime(SpellCategoryEntry const* chargeCategoryEntry) const;

    // Global cooldown
    bool HasGlobalCooldown(SpellInfo const* spellInfo) const;
    void AddGlobalCooldown(SpellInfo const* spellInfo, uint32 duration);
    void CancelGlobalCooldown(SpellInfo const* spellInfo);

    uint16 GetArenaCooldownsSize();

private:
    Player* GetPlayerOwner() const;
    void SendClearCooldowns(std::vector<int32> const& cooldowns) const;
    CooldownStorageType::iterator EraseCooldown(CooldownStorageType::iterator itr)
    {
        _categoryCooldowns.erase(itr->second.CategoryId);
        return _spellCooldowns.erase(itr);
    }

    Unit* _owner;
    CooldownStorageType _spellCooldowns;
    CategoryCooldownStorageType _categoryCooldowns;
    Clock::time_point _schoolLockouts[MAX_SPELL_SCHOOL];
    ChargeStorageType _categoryCharges;
    GlobalCooldownStorageType _globalCooldowns;

    template<class T>
    struct PersistenceHelper { };
};

#endif // SpellHistory_h__
