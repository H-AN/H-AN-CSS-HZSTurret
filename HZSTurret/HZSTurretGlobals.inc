int iTurret[MAXPLAYERS+1];
int offsCollision;
#define MAX_TARGETS 512

enum struct HZSTURRETCFG
{
    bool Enable;
    float TurretRange;
    int TurretDamage;
    float TurretAttackInterval;
    char TurretCantAttack[128];
    char TurretModels[PLATFORM_MAX_PATH];
    char TurretAttackSound[PLATFORM_MAX_PATH];
    float TurretDropRange;
}

HZSTURRETCFG g_HZSTURRETCFG;