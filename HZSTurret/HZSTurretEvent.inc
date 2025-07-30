HookEvents()
{
    HookEvent("round_start", Event_RoundStart);
    HookEvent("player_death", Event_Death);
    HookEvent("round_end", Event_end);
}

public Event_RoundStart(Handle:event,const String:name[],bool:dontBroadcast)
{
    for(int i = 1; i <= MaxClients; ++i)
	{
        iTurret[i] = 0;
    }
}

public Action Event_Death(Event event, const char[] name, bool dontBroadcast) 
{
		
	int client = GetClientOfUserId(GetEventInt(event, "userid"));
    if(iTurret[client])
	{
        SentryActive(iTurret[client]);
        iTurret[client] = 0;
        PrintToChat(client, "你已经死亡,自律机枪已经自毁!!");
    }
    return Plugin_Continue;
}

public Event_end(Handle:event, const String:name[], bool:dontBroadcast)
{
	for(new i = 1; i <= MaxClients; ++i)
	{ 
		if(iTurret[i])
		{
			SentryActive(iTurret[i]);
			iTurret[i] = 0;
		}	
	}
}
