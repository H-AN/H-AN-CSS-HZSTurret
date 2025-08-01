#include <sourcemod>
#include <sdktools>
#include <sdkhooks>
#include <cstrike>
#include <morecolors>
#include <HanZombieScenarioAPI>

#include "HZSTurret/HZSTurretGlobals"
#include "HZSTurret/HZSTurretCFG"
#include "HZSTurret/HZSTurretEvent"
#include "HZSTurret/HZSTurretHelper"



public OnPluginStart()
{
    RegConsoleCmd("sm_turret", CreateTurret,"");
    offsCollision = FindSendPropInfo("CBaseEntity", "m_CollisionGroup");
    HookEvents();
}

public OnMapStart()
{
    LoadHZSTurretConfig();
    offsCollision = FindSendPropInfo("CBaseEntity", "m_CollisionGroup");
}

public Action CreateTurret(int client, int args)
{
    if (!IsValidClient(client) || !IsPlayerAlive(client))	
        return Plugin_Handled;

    if(!g_HZSTURRETCFG.Enable)
        return Plugin_Handled;

    if(client && iTurret[client])
    {
        PrintToChat(client,"你已经放置了自律机枪,同时只能存在一个.");
        return Plugin_Handled;
    }


    float start[3], angle[3], end[3], normal[3];
    GetClientEyePosition(client, start); 
    GetClientEyeAngles(client, angle);
    TR_TraceRayFilter(start, angle, MASK_SOLID, RayType_Infinite, RayDontHitSelf, client); 

    if (TR_DidHit(INVALID_HANDLE)) 
    {
        TR_GetEndPosition(end, INVALID_HANDLE); 
        TR_GetPlaneNormal(INVALID_HANDLE, normal); 
        GetVectorAngles(normal, normal); 
        normal[0] += 90.0; 

       
        new gun = CreateEntityByName("prop_physics_override");  //prop_dynamic_override prop_physics_override
        DispatchKeyValue( gun, "Model", g_HZSTURRETCFG.TurretModels);                
        DispatchKeyValue(gun, "Solid", "6");                                                     
        SetEntPropString(gun, Prop_Data, "m_iName", "guns");                 
        DispatchSpawn(gun);                                                     
        SetEntPropEnt(gun, Prop_Send, "m_hOwnerEntity",client);            
        SetEntProp(gun, Prop_Data, "m_iHealth", 9999999);                              
        SetEntProp(gun, Prop_Data, "m_takedamage", 2);       
        TeleportEntity(gun, end, normal, NULL_VECTOR);    

        SetEntData(gun, offsCollision, 2, 1, true);

        SetEntityRenderMode(gun, RENDER_TRANSALPHA);                              
        SetEntityRenderColor(gun,255, 255, 255, 255);                            
        if(client)
        {
            decl Float:vClientOrigin[3],Float:gunOrigin[3];
            GetEntPropVector(gun, Prop_Send, "m_vecOrigin", gunOrigin);
            GetClientAbsOrigin(client, vClientOrigin);
            if(GetVectorDistance(vClientOrigin, gunOrigin) > g_HZSTURRETCFG.TurretDropRange)
            {
                AcceptEntityInput(gun, "kill");
                RemoveEdict(gun);

                PrintToChat(client, "放置距离超过自身距离,必须小于自身%.f码范围!!", g_HZSTURRETCFG.TurretDropRange);
                return Plugin_Handled;
            }
        }
        
        CreateTimer(0.1, Timer_ActivateSentry, EntIndexToEntRef(gun));
        iTurret[client] = gun;
    }


    return Plugin_Handled;
}

public Action Timer_ActivateSentry(Handle hTimer, any entityRef)
{
    int gun = EntRefToEntIndex(entityRef);

	CreateTimer(g_HZSTURRETCFG.TurretAttackInterval, Timer_SentryThink, EntRefToEntIndex(gun), TIMER_REPEAT | TIMER_FLAG_NO_MAPCHANGE );

    return Plugin_Continue;
}


public Action Timer_SentryThink(Handle hTimer, any entityRef)
{
    int iEntity = EntRefToEntIndex(entityRef);

    if (iEntity == INVALID_ENT_REFERENCE || !IsValidEntity(iEntity) || GetEntProp(iEntity, Prop_Data, "m_iHealth") <= 0)
        return Plugin_Stop;

    int attacker = GetEntPropEnt(iEntity, Prop_Send, "m_hOwnerEntity");

    float vAngle[3], anglevector[3], vEntPosition[3];
    int id_zombie = FindFirstZombieTargetInRange(iEntity, vAngle, vEntPosition)
    if (id_zombie != -1)
    { 
        int damage = g_HZSTURRETCFG.TurretDamage;
        Projectile(id_zombie, attacker, vEntPosition, damage);
        EmitSoundToAll(g_HZSTURRETCFG.TurretAttackSound, SOUND_FROM_WORLD, SNDCHAN_AUTO, SNDLEVEL_NORMAL, SND_NOFLAGS, SNDVOL_NORMAL, SNDPITCH_NORMAL, -1, vEntPosition);
        TeleportEntity(iEntity, NULL_VECTOR, vAngle, NULL_VECTOR);

        float TurretPos[3], EnemyPos[3], TuretAngle[3], vecDir[3];
        GetEntPropVector(iEntity, Prop_Send, "m_angRotation", TuretAngle);
        GetEntPropVector(iEntity, Prop_Send, "m_vecOrigin", TurretPos);
        GetEntPropVector(id_zombie, Prop_Send, "m_vecOrigin", EnemyPos);

        float ZombieSize = GetEntPropFloat(id_zombie, Prop_Send, "m_flModelScale");

        EnemyPos[2] += (50.0 * ZombieSize);

        MakeVectorFromPoints(EnemyPos, TurretPos, vecDir);
        GetVectorAngles(vecDir, vecDir);
        vecDir[2] = 0.0;

        TuretAngle[1] += 360.0;
        float m_iDegreesY = (((vecDir[0] - TuretAngle[0]) + 60.0) / 180.0);
        float m_iDegreesX = (((vecDir[1] - TuretAngle[1]) + 60.0) / 180.0);

        SetEntPropFloat(iEntity, Prop_Send, "m_flPoseParameter", ClampFloat(m_iDegreesX, 0.0, 1.0), 1);
        SetEntPropFloat(iEntity, Prop_Send, "m_flPoseParameter", ClampFloat(m_iDegreesY, 0.0, 1.0), 1);

        GetAngleVectors(vAngle, anglevector, NULL_VECTOR, NULL_VECTOR);
        NormalizeVector(anglevector, anglevector);
        ScaleVector(anglevector, 20.0);
        AddVectors(vEntPosition, anglevector, vEntPosition);

        vEntPosition[0] += 20.0;
        vEntPosition[2] -= 5.0;
        TE_SetupGlowSprite(vEntPosition, PrecacheModel("materials/sprites/muzzleflash4.vmt"), 0.1, 0.25, 255);
        TE_SendToAll();

        TE_SetupBeamPoints(vEntPosition, EnemyPos, PrecacheModel("materials/sprites/laser.vmt"), 0, 0, 0, 0.25, 1.0, 1.0, 0, 0.0, {255, 0, 0, 255}, 0);
        TE_SendToAll();

        ShowMuzzleFlash(vEntPosition, vAngle);
    }

    return Plugin_Continue;
}

float ClampFloat(float value, float min, float max)
{
    if (value < min)
        return min;
    else if (value > max)
        return max;
    return value;
}

public void OnClientDisconnect(int client)
{
    iTurret[client] = 0;
}















