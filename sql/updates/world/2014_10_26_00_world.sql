UPDATE `trinity_string` SET `content_default`='Sender: %s(%llu), Receiver: %s(%llu)' WHERE `entry`=1153;
UPDATE `trinity_string` SET `content_default`='%llu - sender: %s (guid: %llu account: %u) receiver: %s (guid: %llu account: %u) %s' WHERE `entry`=509;
UPDATE `trinity_string` SET `content_default`='Game Object |cffffffff|Hgameobject:%llu|h[%s]|h|r (%s) turned' WHERE `entry`=276;
UPDATE `trinity_string` SET `content_default`='Game Object |cffffffff|Hgameobject:%llu|h[%s]|h|r (%s) moved' WHERE `entry`=277;
UPDATE `trinity_string` SET `content_default`='%d (Entry: %d) - |cffffffff|Hgameobject:%llu|h[%s X:%f Y:%f Z:%f MapId:%d]|h|r ' WHERE `entry`=517;
UPDATE `trinity_string` SET `content_default`='Selected object:\n|cffffffff|Hgameobject:%llu|h[%s]|h|r GUID: %u ID: %u\nX: %f Y: %f Z: %f MapId: %u\nOrientation: %f\nPhasemask %u' WHERE `entry`=524;
UPDATE `trinity_string` SET `content_default`='%llu - |cffffffff|Hcreature:%llu|h[%s X:%f Y:%f Z:%f MapId:%d]|h|r' WHERE `entry`=515;
UPDATE `trinity_string` SET `content_default`='%llu - %s X:%f Y:%f Z:%f MapId:%d' WHERE `entry`=1110;
UPDATE `trinity_string` SET `content_default`='%llu - %s X:%f Y:%f Z:%f MapId:%d' WHERE `entry`=1111;
UPDATE `trinity_string` SET `content_default`='Character guid %llu in use!' WHERE `entry`=1117;
UPDATE `trinity_string` SET `content_default`='Game Object (GUID: %llu) not found' WHERE `entry`=273;
UPDATE `trinity_string` SET `content_default`='>> Add Game Object ''%i'' (%s) (GUID: %llu) added at ''%f %f %f''.' WHERE `entry`=525;
UPDATE `trinity_string` SET `content_default`='Creature (GUID: %llu) not found' WHERE `entry`=287;

ALTER TABLE `creature` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_addon` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_formations` CHANGE `leaderGUID` `leaderGUID` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_formations` CHANGE `memberGUID` `memberGUID` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event_creature` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event_gameobject` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event_model_equip` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event_npc_vendor` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event_npcflag` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `gameobject` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `linked_respawn` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `linked_respawn` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `pool_creature` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `pool_gameobject` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `smart_scripts` CHANGE `entryorguid` `entryorguid` BIGINT(20) NOT NULL DEFAULT '0';
ALTER TABLE `transports` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `vehicle_accessory` CHANGE `guid` `guid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `waypoint_data` CHANGE `wpguid` `wpguid` BIGINT(20) UNSIGNED NOT NULL DEFAULT '0';
