# Combat stats
## Types of stats
When talking about stats, we differentiate between combat values and level values. All of them are defined in `TPCombatValue.h`. Combat values determine a character's combat powers. Specifically, combat values are:
- Health,
- Mana,
- Stamina,
- Attack,
- Magic Attack,
- Defense,
- Magic Defense and
- Poise.

> NOTE: Of all the stats, magic attack is the only one that currently isn't used anywhere. That's because it is meant to be used in ability blueprints, modifying the damage dealt by spells!

Furthermore, there are two kinds of combat values: base values and bonus values. The base values can be edited per character, and they determine how level values affect the final combat value. Bonus values are values added by equipment/status effects/etc.

Level values are values that can be increased by leveling up. They determine the combat values. Level values are:
- Vitality,
- Endurance,
- Strength, 
- Dexterity,
- Intelligence and
- Resistance.

## Stat getters
- `float GetBaseValue(ECombatValue SearchedValue)` - returns the base value;
- `float GetCurrentValue(ECombatValue SearchedValue)` - returns the current value (after calculations) without added bonuses;
- `float GetBonusValue(ECombatValue SearchedValue)` - returns the added bonus value;
- float `GetFullValue(ECombatValue SearchedValue)` - returns the current value + the bonus value. This value is used for all combat calculations;
- int32 `GetLevelValue(ELevelValue SearchedValue)` - returns the level value;

## Elements
Elements are essentially different kinds of magic. The system functions similar to how types in Pokemon games function. Elements are defined in `TPAttackValues.h`. Check the section "Calculations" to see how these affect damage calculations. Currently, the elements are:
- None,
- Fire,
- Lightning,
- Life,
- Wind,
- Water,
- Ice,
- Earth,
- Light and
- Darkness.

> NOTE: This enum class can be modified at will without breaking anything. If the element system is not necessary for the project this plugin is used in, simply set every weapon and ability to have the element "None".

## Attack types
Currently, there are four attack types: Slash, Stab, Blunt and Special (meant to be used for spells or anything that isn't directly related to weapons). Check the section "Calculations" to see how these affect damage calculations. 

## Weaknesses, resistances and immunities
There are two static arrays in the combat component that determine weaknesses, resistances and immunities. Those are `float AttackDefenses[EAttackType::MAX]` and `float MagicDefenses[EMagicElementType::MAX]`. 
> NOTE: The names of the arrays might be misleading - these have nothing to do with the defense and magic defense combat values. They have the word "defenses" in their names because that's what weaknesses, resistances and immunities are, essentially. 

The elements of these arrays are simple multipliers to the damage dealt to a character - the lower the value, the less damage will be dealt by that particular attack type/element. 

## Bone defenses
The editable array `DamagedBones` allows the user to define different damage weakness and resistances per bone. The array elemt is a struct called `FDamagedBone`, which consists of nothing more than a bone name and a multiplier. Once damage is dealt, if any bone from the array is hit, the damage will be multiplied by the defined multiplier. A good example of using this array would be setting up a character to take more damage when hit in the head, and less damage when hit in the legs.

## Calculations
### Combat value calculations
The final size of the combat values is calculated by the given formula:
`FinalValue = BaseValueFormula + BonusValue`. 
The base value formula depends on the stat itself. For instance, this is the formula for health:
`MaxHP = BaseHealth * Vitality * 2.8 + BaseHealth * Endurance * 0.67 + BaseHealth * Strength * 0.4 + BaseHealth * Dexterity * 0.4 + BaseHealth * Intelligence * 0.2 + BaseHealth * Resistance * 0.5`
As you can see, every level stat will affect the final health value by some amount. This is not the case for all combat values. However, the total multiplier across all level values will be around 4.5 for all combat values.

All formulas can be found in `UTPCombatComponent::SetCurrentValues()`. 

### Damage calculations
The damage calculation is done in the function `void UTPCombatComponent::TakeHit(float InDamage, float InForce, FVector InForceDirection, EAttackType InAttackType, EMagicElementType InElementType, const FHitResult& Hit)`. 
This function is blueprint callable. It is called when a character is hit by a weapon. In that case, the damage is the attack stat of the damage dealing character and the force, attack type and magic element type are determined by the weapon. 
When using a non-weapon ability to attack (i.e. casting a fireball), the ability should just spawn and shoot the fireball, and this function should be called from the fireball itself, on impact. If you want the character stats to modify the damage done by the fireball, any modifiers should be set upon spawning the fireball. 

When the function is called, the passed damage will go through the following changes:
- Multiplied by the appropriate values from the `AttackDefenses` and `MagicDefenses` arrays;
- Multiplied by the appropriate values from the `DamagedBones` array, if any bone defined in the array is hit;
- Decreased by a percentage of itself, based on the character's magic defense, provided the attack type is "special". The percentage is the square root of the magic defense full value, and caps at 80%;
- Decreased by a percentage of itself, based on the character's defense, provided the attack type isn't "special". The percentage is the square root of the defense full value, and caps at 80%;
- Increased/decreased by a very small, random percentage of itself (random value between -5% and 5%). This is done so the exact same attack under the exact same condition does not produce the exact same result every single time;

> NOTE: The last step should be controllable by different variables, as a lot of designers will probably want to skip it and have exact values!

### Knockback calculations
> IMPORTANT: This entire system will likely be changed, to enable knockback animations as well as ragdoll, depending on the amount of accumulated knockback!

Every attack has a force behind it. In the case of spells, a custom force should be used, when calling the `TakeHit` function. In the case of weapons, that force is determined by a variable called `WeaponBaseForce`. The weapon base force can further be modified in attack animations, by adding an animation curve. The name of the curve should match the name set in the project's developer settings (`TPCombatSettings -> WeaponForceCurveName`). If this curve doesn't exist, then only `WeaponBaseForce` will be used. 

Every time a character is hit with a force greater than zero, the force accumulates into a float variable called `AccumulatedKnockback`. This variable is also reduced in the Tick function, by an amount specified with the variable `KnockbackForceReductionRate`. If the accumulated knockback gets bigger than the full poise value * `Knockbacktreshold` (editable), the function `StartRagdoll` is called on the character. 
Depending on the force and the poise, the duration of the knockback is calculated (it will never go above `KnockbackMaxDuration`). If the character is launched in the air, the duration doesn't start ticking until the character lands.

### Parry/Break calculation
If a weapon is in blocking mode (set by calling `ATPWeaponBase::SetWeaponBlocking(bool bNewBlocking)`), and is directly hit by an attack, the function `WeaponTakeHit` is called to check if the block is successful. The function is overridable in BPs. Its base form reduces the incoming force by the appropriate attack and magic block multipliers, and subtracts the final value from the weapon's current poise. The current poise is restored over time only if the weapon isn't in blocking mode.

If an attacking weapon collides with a blocking weapon, the same calculation is performed. If the block is successful, the attacker gets parried, and a parried animation is played (the parry animation depends on the character AND the attack ability). 