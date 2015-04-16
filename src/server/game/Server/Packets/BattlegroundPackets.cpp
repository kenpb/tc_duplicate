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

#include "BattlegroundPackets.h"

WorldPacket const* WorldPackets::Battleground::PVPSeason::Write()
{
    _worldPacket << uint32(CurrentSeason);
    _worldPacket << uint32(PreviousSeason);

    return &_worldPacket;
}

void WorldPackets::Battleground::AreaSpiritHealerQuery::Read()
{
    _worldPacket >> HealerGuid;
}

void WorldPackets::Battleground::AreaSpiritHealerQueue::Read()
{
    _worldPacket >> HealerGuid;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Battleground::PVPLogData::RatingData const& ratingData)
{
    data.append(ratingData.Prematch, 2);
    data.append(ratingData.Postmatch, 2);
    data.append(ratingData.PrematchMMR, 2);
    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Battleground::PVPLogData::HonorData const& honorData)
{
    data << uint32(honorData.HonorKills);
    data << uint32(honorData.Deaths);
    data << uint32(honorData.ContributionPoints);
    return data;
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Battleground::PVPLogData::PlayerData const& playerData)
{
    data << playerData.PlayerGUID;
    data << uint32(playerData.Kills);
    data << uint32(playerData.DamageDone);
    data << uint32(playerData.HealingDone);
    data << uint32(playerData.Stats.size());
    data << int32(playerData.PrimaryTalentTree);
    data << uint32(playerData.PrimaryTalentTreeNameIndex);
    if (!playerData.Stats.empty())
        data.append(playerData.Stats.data(), playerData.Stats.size());

    data.WriteBit(playerData.Faction);
    data.WriteBit(playerData.IsInWorld);
    data.WriteBit(playerData.Honor.HasValue);
    data.WriteBit(playerData.PreMatchRating.HasValue);
    data.WriteBit(playerData.RatingChange.HasValue);
    data.WriteBit(playerData.PreMatchMMR.HasValue);
    data.WriteBit(playerData.MmrChange.HasValue);
    data.FlushBits();

    if (playerData.Honor.HasValue)
        data << playerData.Honor.Value;

    if (playerData.PreMatchRating.HasValue)
        data << uint32(playerData.PreMatchRating.Value);

    if (playerData.RatingChange.HasValue)
        data << uint32(playerData.RatingChange.Value);

    if (playerData.PreMatchMMR.HasValue)
        data << uint32(playerData.PreMatchMMR.Value);

    if (playerData.MmrChange.HasValue)
        data << uint32(playerData.MmrChange.Value);

    return data;
}

WorldPacket const* WorldPackets::Battleground::PVPLogData::Write()
{
    _worldPacket.reserve(Players.size() * sizeof(PlayerData) + sizeof(PVPLogData));

    _worldPacket.WriteBit(Ratings.HasValue);
    _worldPacket.WriteBit(Winner.HasValue);
    _worldPacket << uint32(Players.size());
    _worldPacket.append(PlayerCount, 2);

    if (Ratings.HasValue)
        _worldPacket << Ratings.Value;

    if (Winner.HasValue)
        _worldPacket << uint8(Winner.Value);

    for (PlayerData const& player : Players)
        _worldPacket << player;

    return &_worldPacket;
}

void WorldPackets::Battleground::BattlemasterJoin::Read()
{
    _worldPacket >> QueueID;
    _worldPacket >> Roles;
    _worldPacket >> BlacklistMap[0] >> BlacklistMap[1];
    JoinAsGroup = _worldPacket.ReadBit();
}

ByteBuffer& operator<<(ByteBuffer& data, WorldPackets::Battleground::BattlefieldStatusHeader const& header)
{
    data << header.Ticket;
    data << uint64(header.QueueID);
    data << uint8(header.RangeMin);
    data << uint8(header.RangeMax);
    data << uint8(header.TeamSize);
    data << uint32(header.InstanceID);
    data.WriteBit(header.RegisteredMatch);
    data.WriteBit(header.TournamentRules);
    data.FlushBits();
    return data;
}

WorldPacket const* WorldPackets::Battleground::BattlefieldStatusNone::Write()
{
    _worldPacket << Ticket;
    return &_worldPacket;
}

WorldPacket const* WorldPackets::Battleground::BattlefieldStatusNeedConfirmation::Write()
{
    _worldPacket << Hdr;
    _worldPacket << uint32(Mapid);
    _worldPacket << uint32(Timeout);
    _worldPacket << uint8(Role);
    return &_worldPacket;
}

WorldPacket const* WorldPackets::Battleground::BattlefieldStatusActive::Write()
{
    _worldPacket << Hdr;
    _worldPacket << uint32(Mapid);
    _worldPacket << uint32(ShutdownTimer);
    _worldPacket << uint32(StartTimer);
    _worldPacket.WriteBit(ArenaFaction);
    _worldPacket.WriteBit(LeftEarly);
    _worldPacket.FlushBits();
    return &_worldPacket;
}

WorldPacket const* WorldPackets::Battleground::BattlefieldStatusQueued::Write()
{
    _worldPacket << Hdr;
    _worldPacket << uint32(AverageWaitTime);
    _worldPacket << uint32(WaitTime);
    _worldPacket.WriteBit(AsGroup);
    _worldPacket.WriteBit(EligibleForMatchmaking);
    _worldPacket.WriteBit(SuspendedQueue);
    _worldPacket.FlushBits();
    return &_worldPacket;
}

WorldPacket const* WorldPackets::Battleground::BattlefieldStatusFailed::Write()
{
    _worldPacket << Ticket;
    _worldPacket << uint64(QueueID);
    _worldPacket << uint32(Reason);
    _worldPacket << ClientID;
    return &_worldPacket;
}

void WorldPackets::Battleground::BattlefieldPort::Read()
{
    _worldPacket >> Ticket;
    AcceptedInvite = _worldPacket.ReadBit();
}

void WorldPackets::Battleground::BattlefieldListRequest::Read()
{
    _worldPacket >> ListID;
}

WorldPacket const* WorldPackets::Battleground::BattlefieldList::Write()
{
    _worldPacket << BattlemasterGuid;
    _worldPacket << int32(BattlemasterListID);
    _worldPacket << uint8(MinLevel);
    _worldPacket << uint8(MaxLevel);
    _worldPacket << uint32(Battlefields.size());
    if (!Battlefields.empty())
        _worldPacket.append(Battlefields.data(), Battlefields.size());

    _worldPacket.WriteBit(PvpAnywhere);
    _worldPacket.WriteBit(HasHolidayWinToday);
    _worldPacket.WriteBit(HasRandomWinToday);
    _worldPacket.WriteBit(IsRandomBG);
    _worldPacket.FlushBits();
    return &_worldPacket;
}

WorldPacket const* WorldPackets::Battleground::PVPOptionsEnabled::Write()
{
    _worldPacket.WriteBit(RatedBattlegrounds);
    _worldPacket.WriteBit(PugBattlegrounds);
    _worldPacket.WriteBit(WargameBattlegrounds);
    _worldPacket.WriteBit(WargameArenas);
    _worldPacket.WriteBit(RatedArenas);
    _worldPacket.WriteBit(ArenaSkirmish);
    _worldPacket.FlushBits();
    return &_worldPacket;
}
