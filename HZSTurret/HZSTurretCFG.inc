void LoadHZSTurretConfig()
{
    KeyValues kv = new KeyValues("HZSTurret");

    char cfgDir[PLATFORM_MAX_PATH];
    BuildPath(Path_SM, cfgDir, sizeof(cfgDir), "configs/HZSTurret");
    if (!FileExists(cfgDir))
    {
        CreateDirectory(cfgDir, 511);
    }

    char path[PLATFORM_MAX_PATH];
    BuildPath(Path_SM, path, sizeof(path), "configs/HZSTurret/HZSTurretConfig.cfg");

    if (!FileExists(path))
    {
        WriteDefaultScenarioConfig(path);
    }

    if (!FileToKeyValues(kv, path))
    {
        PrintToServer("[HZSTurretConfig] 配置读取失败！");
        LoadDefaultConfigValues();
        delete kv;
        return;
    }

    g_HZSTURRETCFG.Enable = KvGetSafeBool(kv, "enable_plugins", true);
    g_HZSTURRETCFG.TurretRange = KvGetSafeFloat(kv, "Turret_Range", 500.0);
    g_HZSTURRETCFG.TurretDamage = KvGetSafeNum(kv, "Turret_Damage", 10);
    g_HZSTURRETCFG.TurretAttackInterval = KvGetSafeFloat(kv, "Turret_Attack_Interval", 0.1);
    KvGetString(kv, "Turret_Cant_Attack", g_HZSTURRETCFG.TurretCantAttack, sizeof(g_HZSTURRETCFG.TurretCantAttack), "");
    KvGetString(kv, "Turret_Models", g_HZSTURRETCFG.TurretModels, sizeof(g_HZSTURRETCFG.TurretModels), "");
    KvGetString(kv, "Turret_Attack_Sound", g_HZSTURRETCFG.TurretAttackSound, sizeof(g_HZSTURRETCFG.TurretAttackSound), "");
    g_HZSTURRETCFG.TurretDropRange = KvGetSafeFloat(kv, "Turret_Drop_Range", 500.0);

    delete kv;

    ValidateConfigParameters();

    Precache();
}

void Precache()
{
    if (strlen(g_HZSTURRETCFG.TurretModels) > 0)
    {
        PrecacheModel(g_HZSTURRETCFG.TurretModels, true);
    }
    if (strlen(g_HZSTURRETCFG.TurretAttackSound) > 0)
    {
	    PrecacheSound(g_HZSTURRETCFG.TurretAttackSound, true);
    }
}

void WriteDefaultScenarioConfig(const char[] path)
{
    Handle file = OpenFile(path, "w");
    if (file == INVALID_HANDLE)
        return;

    WriteFileLine(file, "// HZSTurret 大灾变炮塔配置文件");
    WriteFileLine(file, "// enable_plugins 是否开启此插件 默认:1");
    WriteFileLine(file, "// Turret_Range 炮塔攻击最大范围 默认:500.0");
    WriteFileLine(file, "// Turret_Damage 炮塔每次攻击伤害 默认:10");
    WriteFileLine(file, "// Turret_Attack_Interval 炮塔攻击间隔频率 默认:0.1秒");
    WriteFileLine(file, "// Turret_Cant_Attack 炮塔无法攻击 (填写的丧尸名称) 默认不填写 为攻击所有");
    WriteFileLine(file, "// Turret_Models 设置炮塔的自定义模型");
    WriteFileLine(file, "// Turret_Attack_Sound 设置炮塔攻击时的声音");
    WriteFileLine(file, "// Turret_Drop_Range 设置炮塔放置时与自身距离不超过x码 默认:500.0");
    WriteFileLine(file, "\"HZSTurret\"");
    WriteFileLine(file, "{");
    WriteFileLine(file, "    \"enable_plugins\"    \"1\"");
    WriteFileLine(file, "    \"Turret_Range\"    \"500.0\"");
    WriteFileLine(file, "    \"Turret_Damage\"           \"10\"");
    WriteFileLine(file, "    \"Turret_Attack_Interval\"              \"0.1\"");
    WriteFileLine(file, "    \"Turret_Cant_Attack\"              \"\"");
    WriteFileLine(file, "    \"Turret_Models\"              \"models/weapons/combine_turrets/1floor_turret.mdl\"");
    WriteFileLine(file, "    \"Turret_Attack_Sound\"              \"zr/sentry/attack.wav\"");
    WriteFileLine(file, "    \"Turret_Drop_Range\"              \"500.0\"");
    WriteFileLine(file, "}");

    CloseHandle(file);
}


void LoadDefaultConfigValues()
{
    g_HZSTURRETCFG.Enable = true;
    g_HZSTURRETCFG.TurretRange = 500.0;
    g_HZSTURRETCFG.TurretDamage = 10;
    g_HZSTURRETCFG.TurretAttackInterval = 0.1;

    strcopy(g_HZSTURRETCFG.TurretCantAttack, sizeof(g_HZSTURRETCFG.TurretCantAttack), "");
    strcopy(g_HZSTURRETCFG.TurretModels, sizeof(g_HZSTURRETCFG.TurretModels), "models/weapons/combine_turrets/1floor_turret.mdl");
    strcopy(g_HZSTURRETCFG.TurretAttackSound, sizeof(g_HZSTURRETCFG.TurretAttackSound), "zr/sentry/attack.wav");
    g_HZSTURRETCFG.TurretDropRange = 500.0;
}


void ValidateConfigParameters()
{
    if (g_HZSTURRETCFG.TurretRange < 0.0)
    {
        g_HZSTURRETCFG.TurretRange = 0.0;
    }
    if (g_HZSTURRETCFG.TurretDamage < 0)
    {
        g_HZSTURRETCFG.TurretDamage = 0;
    }
    if (g_HZSTURRETCFG.TurretAttackInterval < 0.0)
    {
        g_HZSTURRETCFG.TurretAttackInterval = 0.0;
    }
    if(g_HZSTURRETCFG.TurretDropRange < 0.0)
    {
        g_HZSTURRETCFG.TurretDropRange = 500.0;
    }
    
}

float KvGetSafeFloat(KeyValues kv, const char[] key, float default_val)
{
    char buf[64];
    KvGetString(kv, key, buf, sizeof(buf), "");

    if (StrEqual(buf, "")) 
    {
        return default_val; 
    }

    return StringToFloat(buf); 
}

int KvGetSafeNum(KeyValues kv, const char[] key, int default_val)
{
    char buf[64];
    KvGetString(kv, key, buf, sizeof(buf), "");

    if (StrEqual(buf, "")) 
    {
        return default_val;
    }

    return StringToInt(buf); 
}

bool KvGetSafeBool(KeyValues kv, const char[] key, bool default_val)
{
    return KvGetSafeNum(kv, key, default_val ? 1 : 0) != 0;
}