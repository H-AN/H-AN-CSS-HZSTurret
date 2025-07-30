stock FindFirstZombieTargetInRange(int entity, float vAngle[3], float vEntPosition[3], float entHeight = 60.0)
{
    float vZombiePosition[3];
    float fDistance[2048];
    int iTargets[2048];
    int numTargets = 0;

    GetEntPropVector(entity, Prop_Send, "m_vecOrigin", vEntPosition);
    vEntPosition[2] += entHeight;

	char name[64];
    
	
    int totalZombies = Han_GetZombieCount();
    for (int i = 0; i < totalZombies; i++)
    {
        int zombie = Han_GetZombieByIndex(i);
        if (!IsValidEntity(zombie) || GetEntProp(zombie, Prop_Data, "m_iHealth") <= 0)
            continue;

		if(Han_GetZombieName(zombie, name, sizeof(name))) 
        {
			if (IsZombieProtected(name))
			{
				continue;
			}
		}


        GetEntPropVector(zombie, Prop_Send, "m_vecOrigin", vZombiePosition);
		
       
		float ZombieSize = GetEntPropFloat(zombie, Prop_Send, "m_flModelScale");


		vZombiePosition[2] += (entHeight * ZombieSize);

        fDistance[numTargets] = GetVectorDistance(vEntPosition, vZombiePosition, false);

        if (fDistance[numTargets] <= g_HZSTURRETCFG.TurretRange) // && fDistance[numTargets] > 50.0
        {
            iTargets[numTargets] = zombie;
            numTargets++;
        }
    }

    if (numTargets == 0)
        return -1;

    int closestZombie = GetZombieEntityMinDistance(iTargets, fDistance, numTargets);
    if (closestZombie == -1 || !IsValidEntity(closestZombie))
        return -1;
    
	float closestZombieSize = GetEntPropFloat(closestZombie, Prop_Send, "m_flModelScale");

    GetEntPropVector(closestZombie, Prop_Send, "m_vecOrigin", vZombiePosition);
    vZombiePosition[2] += entHeight * closestZombieSize;
    MakeVectorFromPoints(vEntPosition, vZombiePosition, vAngle);
	vZombiePosition[2] += entHeight * closestZombieSize;
    GetVectorAngles(vAngle, vAngle);

    Handle trace = TR_TraceRayFilterEx(vEntPosition, vZombiePosition, MASK_SHOT, RayType_EndPoint, TraceFilterEnt, entity);
    if (!TR_DidHit(trace))
    {
        CloseHandle(trace);
        return closestZombie;
    }



    CloseHandle(trace);
    return -1;
}

int GetZombieEntityMinDistance(zombie[], Float:distance[], count)
{
    if (count < 1)
        return -1;

    float minDist = 999999.0;
    int closest = -1;

    for (int i = 0; i < count; i++)
    {
        if (distance[i] < minDist && distance[i] <= g_HZSTURRETCFG.TurretRange) // && distance[i] > 50.0
        {
            minDist = distance[i];
            closest = zombie[i];
        }
    }
    return closest;
}






stock void Projectile(int id_zombie, int attacker, float position[3], int damage)
{
    if (id_zombie > 0 && IsValidEntity(id_zombie) && Han_IsZombie(id_zombie))
    {
        float ZombiePos[3];
        GetEntPropVector(id_zombie, Prop_Send, "m_vecOrigin", ZombiePos);

		float ZombieSize = GetEntPropFloat(id_zombie, Prop_Send, "m_flModelScale");

        ZombiePos[0] += GetRandomFloat(-2.0, 2.0);
        ZombiePos[1] += GetRandomFloat(-2.0, 2.0);
        ZombiePos[2] += ((50.0 * ZombieSize) + GetRandomFloat(-10.0, 10.0));

        CreateBulletTrace(position, ZombiePos, GetRandomFloat(1000.0, 5000.0), GetRandomFloat(1.0, 5.0), GetRandomFloat(1.0, 5.0), "255 255 255");

		if(IsValidClient(attacker) && IsPlayerAlive(attacker))
		{
        	Han_SafeDamageZombie(attacker, id_zombie, damage);
		}

    }
}




stock CreateBulletTrace(const float origin[3], const float dest[3], const float speed = 6000.0, const float startwidth = 0.5, const float endwidth = 0.2, const String:color[] = "200 200 0")
{
	int entity = CreateEntityByName("env_spritetrail");
	if (entity == -1)
	{
		LogError("Couldn't create entity 'bullet_trace'");
		return -1;
	}
	DispatchKeyValue(entity, "classname", "bullet_trace");
	DispatchKeyValue(entity, "spritename", "materials/sprites/laser.vmt");
	DispatchKeyValue(entity, "renderamt", "255");
	DispatchKeyValue(entity, "rendercolor", color);
	DispatchKeyValue(entity, "rendermode", "5");
	DispatchKeyValueFloat(entity, "startwidth", startwidth);
	DispatchKeyValueFloat(entity, "endwidth", endwidth);
	DispatchKeyValueFloat(entity, "lifetime", 240.0 / speed);
	if (!DispatchSpawn(entity))
	{
		AcceptEntityInput(entity, "Kill");
		LogError("Couldn't create entity 'bullet_trace'");
		return -1;
	}
	
	SetEntPropFloat(entity, Prop_Send, "m_flTextureRes", 0.05);
	
	float vecVeloc[3], angRotation[3];
	MakeVectorFromPoints(origin, dest, vecVeloc);
	GetVectorAngles(vecVeloc, angRotation);
	NormalizeVector(vecVeloc, vecVeloc);
	ScaleVector(vecVeloc, speed);
	
	TeleportEntity(entity, origin, angRotation, vecVeloc);
	
	char _tmp[128];
	FormatEx(_tmp, sizeof(_tmp), "OnUser1 !self:kill::%0.4f:-1", GetVectorDistance(origin, dest) / speed);
	SetVariantString(_tmp);
	AcceptEntityInput(entity, "AddOutput");
	AcceptEntityInput(entity, "FireUser1");
	
	return entity;
}


stock SentryActive(entity)
{
	if(IsValidEntity(entity) && IsValidEdict(entity))
	{
		float vEntPosition[3];
		GetEntPropVector(entity, Prop_Send, "m_vecOrigin", vEntPosition);
		RemoveEntity(entity);
		vEntPosition[2] += 15.0;

	}
}


/*
public bool TraceFilterEnt(int entityhit, int mask, any entity)
{
	return (entityhit != entity);
}
*/
public bool TraceFilterEnt(int entity, int contentsMask, any data)
{
    if (entity == data)
        return false;

    if (!IsValidEntity(entity))
        return false;

    char name[64];
    if (Han_IsZombie(entity) && Han_GetZombieName(entity, name, sizeof(name)))
    {
        if (IsZombieProtected(name))
            return false;
    }

    return true;
}

public bool:RayDontHitSelf(entity, contentsMask, any:data) 
{ 
	return (entity != data); 
}


public bool TraceFilter_All(entity,contentsMask)
{
	return false;
}


ShowMuzzleFlash(float pos[3],  float angle[3])
{  
 	int particle = CreateEntityByName("info_particle_system");
	DispatchKeyValue(particle, "effect_name", "muzzle_machinegun"); 
	DispatchSpawn(particle);
	ActivateEntity(particle); 
	TeleportEntity(particle, pos, angle, NULL_VECTOR);
	AcceptEntityInput(particle, "start");	
	CreateTimer(0.01, DeleteParticles, particle, TIMER_FLAG_NO_MAPCHANGE);	
}

public Action DeleteParticles(Handle timer, any particle)
{
	if (IsValidEntity(particle))
	{
		char classname[64];
		GetEdictClassname(particle, classname, sizeof(classname));
		if (StrEqual(classname, "info_particle_system", false))
		{
			AcceptEntityInput(particle, "stop");
			AcceptEntityInput(particle, "kill");
			RemoveEdict(particle);	 
		}
	}
	return Plugin_Continue;
}

bool IsZombieProtected(const char[] zombieName)
{
    if (g_HZSTURRETCFG.TurretCantAttack[0] == '\0')
        return false;

    char buffer[256];
    strcopy(buffer, sizeof(buffer), g_HZSTURRETCFG.TurretCantAttack);

    char parts[128][64]; 
    int count = ExplodeString(buffer, ",", parts, sizeof(parts), sizeof(parts[]));

    for (int i = 0; i < count; i++)
    {
        TrimString(parts[i]); 
        if (StrEqual(parts[i], zombieName, false))
        {
            return true; 
        }
    }

    return false;
}



stock bool IsValidClient(client)
{
    return (client > 0 && client <= MaxClients && IsClientInGame(client));
}
