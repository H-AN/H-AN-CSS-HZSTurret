<h1>HZSTurret - ZombieScenario 自动炮塔插件 / Auto Turret Plugin</h1>

<p><strong>插件介绍 (Description):</strong><br>
HZSTurret 是 <code>ZombieScenario</code>（大灾变插件）的衍生模块，用于创建可以自动攻击丧尸的哨戒炮塔。支持范围、伤害、频率等多项参数的自由配置，也支持排除部分特定丧尸不被锁定攻击。</p>

<p><strong>HZSTurret</strong> is an extension plugin for <code>ZombieScenario</code>, allowing players to deploy auto turrets that automatically attack zombies. Supports customizable range, damage, attack frequency, and zombie type filtering.</p>

<hr>

<h2>📦 插件功能 / Features</h2>
<ul>
  <li>🔫 自动炮塔攻击大灾变 NPC 丧尸 / Auto-targeting ZombieScenario NPC zombies</li>
  <li>🎯 设置攻击范围与频率 / Configurable attack range and interval</li>
  <li>💥 自定义每次攻击伤害 / Customizable damage per shot</li>
  <li>⛔ 排除指定丧尸类型不攻击 / Exclude specific zombie types by name</li>
  <li>🎵 支持自定义炮塔模型和音效 / Supports custom models and attack sounds</li>
  <li>📏 炮塔放置有距离限制 / Turret deployment has distance limit</li>
</ul>

<hr>

<h2>⚙️ 配置说明 / Configuration</h2>

<pre><code>// HZSTurret 大灾变炮塔配置文件
// HZSTurret config file for zombie turrets

// enable_plugins 是否启用插件（1=开启，0=关闭）
// enable_plugins Whether to enable the plugin (1 = enabled, 0 = disabled)

// Turret_Range 炮塔攻击最大范围
// Turret_Range Maximum attack range

// Turret_Damage 炮塔每次攻击伤害
// Turret_Damage Damage per attack

// Turret_Attack_Interval 炮塔攻击间隔（秒）
// Turret_Attack_Interval Attack interval (in seconds)

// Turret_Cant_Attack 不攻击的丧尸名称（多个用英文逗号分隔）
// Turret_Cant_Attack Zombie names the turret should NOT attack (comma-separated)

// Turret_Models 炮塔自定义模型路径
// Turret_Models Model path for custom turret appearance

// Turret_Attack_Sound 炮塔攻击时的声音路径
// Turret_Attack_Sound Sound path for turret's attack sound

// Turret_Drop_Range 放置炮塔最大距离（超过该距离会提示失败）
// Turret_Drop_Range Max distance to deploy turret (too far will fail)
</code></pre>

<h3>📋 示例配置 / Example Config</h3>

<pre><code>"HZSTurret"
{
    "enable_plugins"        "1"
    "Turret_Range"          "500.0"
    "Turret_Damage"         "10"
    "Turret_Attack_Interval" "0.1"
    "Turret_Cant_Attack"    "普通丧尸 一阶段,暗影芭比"
    "Turret_Models"         "models/weapons/combine_turrets/1floor_turret.mdl"
    "Turret_Attack_Sound"   "zr/sentry/attack.wav"
    "Turret_Drop_Range"     "500.0"
}
</code></pre>

<hr>

<h2>🧠 使用方法 / How to Use</h2>
<ul>
  <li><strong>放置炮塔：</strong> 玩家朝向的目标点必须在 <code>Turret_Drop_Range</code> 范围内，否则提示放置失败。
  <br><strong>Deploy turret:</strong> The target point you're aiming at must be within <code>Turret_Drop_Range</code>, or deployment fails.</li>

  <li><strong>攻击逻辑：</strong> 炮塔每隔 <code>Turret_Attack_Interval</code> 秒攻击一次，锁定范围内最近的合法丧尸目标。
  <br><strong>Attack logic:</strong> The turret attacks the nearest valid zombie every <code>Turret_Attack_Interval</code> seconds.</li>

  <li><strong>排除丧尸：</strong> 配置文件中可设置多个丧尸名称不被炮塔锁定攻击（例如 Boss 或剧情单位）。
  <br><strong>Zombie exclusion:</strong> You can exclude certain zombie types from being targeted, such as bosses or scripted enemies.</li>
</ul>

<hr>

<h2>🧩 插件依赖 / Dependency</h2>
<p><strong>必须依赖：</strong> ZombieScenario 主插件（大灾变）提供的 NPC 丧尸系统。</p>
<p><strong>Required:</strong> Depends on <code>ZombieScenario</code> core plugin for NPC zombie support.</p>

<h2>✅ 建议搭配插件 / Recommended Companion Plugins</h2>
<ul>
  <li><code>ZombieScenario</code> - 核心丧尸系统插件 / Core zombie NPC system (required)</li>
</ul>
