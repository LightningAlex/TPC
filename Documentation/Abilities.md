# Abilities

## Definition
An ability consists of an animation (montage), various different events and stats typically used in combat. Abilities are the core of the combat system.

## Functionality details
### Code structure
The base class for abilities is `UTPAbility`, which inherits from `UObject` (might be changed to `AActor` later for replication purposes).  The base class contains variables used for any ability. Each ability is supposed to be a blueprint which inherits from this base class.

### Ability animations
Each ability must play an animation. The played animation is determined by the following function: 
`UAnimMontage* GetAbilityMontage(UTPCombatComponent* UsedCC)`. The function is a blueprint implementable event, meaning it won't return anything unless overridden. 
In most cases, the overridden function will consists only of a return node, returning a very specific animation. However, for some abilities, the animation will be chosen depending on additional factors. 
> SIMPLE EXAMPLE: Imagine an ability where the character performs a spell to create and launch a fireball. The animation for an ability like this is usually very simple and unique. Thus, the function would consist of only a return node, in which the animation is literally selected in the drop-down.

>COMPLEX EXAMPLE: Imagine an ability called "right hand attack". As its name suggests, this ability would have the character perform a simple attack with the armament in their right hand. However, the animation side of things isn't that simple - it would depend on the armament. Hence, the function would search for the armament type (via equipment in the passed combat component) and return an animation based on that type.

> NOTE: Animationless abilities are planned for the future!

### Ability events
Every ability can fire off the following events:
- Ability Start;
- Ability Tick;
- Ability End;
- Ability Notify;
- Ability Interrupted;

These events are blueprint implementable events. If not defined in blueprints, these events do nothing.

#### Ability Start
As the same suggests, this event is fired off each time the ability starts.

#### Ability Tick
As the name suggests, this event is fired off each frame. This event won't be called unless `bCanAbilityTick` is set to true.

#### Ability End
This event is called when the ability ends naturally (when the animation montage ends). If the animation is interrupted for any reason, this event won't be called.

#### Ability Notify
This event is called per animation notify - if any are set. An ability can fire off this event multiple times. The animation notify will pass an index to the event (so they can be differentiated if multiple are called).

#### Ability Interrupted
This event is called when the ability animation is interrupted for any reason. This can include getting hit or starting a new ability. 
> NOTE: In the future, this event should have an argument for the interruption reason.

## Variables
All listed variables are editable per individual ability. 

### Ability Name
**Type:** FName;

**Usage:** Used to compare ability objects, and to search for abilities. Should be unique;
> Example: When trying to use an ability, the system iterates through all abilities currently on cooldown and compares their names to the name of the tried ability. If the tried ability is found in the cooldown list, it can't be used.

### Ability Display Name
**Type:** FText;

**Usage:** Isn't used for anything in code. It's blueprint readable and meant to be used as a display name in UI;

### Ability Display Description
**Type:** FText;

**Usage:** Isn't used for anything in code. It's blueprint readable and meant to be used as a description in UI;

### Ability Health Cost
**Type:** Float;

**Usage:** Reduces the health of the character using the ability by the amount stored in this variable. If the character doesn't have enough health, the ability isn't usable;

### Ability Mana Cost
**Type:** Float;

**Usage:** Reduces the mana of the character using this ability by the amount stored in this variable. if the character doesn't have enough mana, the ability isn't usable;

### Ability Stamina Cost
**Type:** Float;

**Usage:** Reduces the stamina of the character using this ability by the amount stored in this variable. If the character doesn't have enough stamina, the ability isn't usable;
>NOTE: This might be changed so the character can use the ability if they don't have enough stamina - using the ability will simply take away all remaining stamina (like in Dark Souls). Best to code in both scenarios and expose the option of which one to use in developer settings.

### Movement Speed Multiplier
**Type:** Float;

**Usage:** Multiplies the current maximum speed of the character by this value;

### Can Be Used In Air
**Type:** Bool;
**Usage:** Determines whether the ability can be used while the character is falling;

### Can Ability Tick
**Type:** Bool;

**Usage:** Determines whether the ability can tick, i.e. whether `AbilityTick(float XDeltaTime)` should be called every frame or not;

### Ability Cooldown Duration
**Type:** Float;

**Usage:** Determines how long the cooldown of this ability is, i.e. how much time needs to pass for this ability to be usable again;

### Ability Combo Tag
**Type:** FGameplayTag;

**Usage:** Used to check for potential combos. All abilities that are part of the same combo chain should share the same tag (see more in the combo system section);

### Combo Ability Class
**Type:** TSubclassOf;

**Usage:** If a combo is performed, the ability stored here will be used (see more in the combo system section);

## Using abilities
To use an ability, the function `TryUseAbility(TSubclassOf<UTPAbility> UsedAbilityClass)` has to be called. This function will create a new ability object, clear up the old one (if it exists), play the apropriate animation, call and bind the necessary events and deduct health, mana and stamina, dependent on the ability itself. 

The function `TryUseAbility` first calls `CanUseAbility(TSubclassOf<UTPAbility> TriedAbilityClass)`. This function will check if the character can use abilities at all (a variable in the combat component controls this and can be set to true or false at will), if the tried ability should be a combo (read below for more), a chain (read below for more), if the character has enough health, mana and/or stamina (dependent on the ability itself), if the character is ragdolling, stuck in a parry/break animation, if the character is in air (or the ability is usable in air) and if the ability is on cooldown. If all checks pass, the function sets up combo/chain variables (if necessary) and return true, and the ability is performed.

## Combos
### Definition
A combo consists of any number of abilities used in succession by pressing the same input. Setting up a combo comes with challenges the plugin provides easy solutions for. 

>NOTE: Combos are defined as successive abilities performed after pressing the same input only! Performing successive abilities by different inputs is called "ability chaining" and is explained below!

### Combo window
One shouldn't be able to perform the next successive ability whenever. The plugin comes with a handy animation notify state, called `ComboWindowState`. This animation state will determine during what part of the ability animation a combo can be performed.
> Note: It is currently impossible to continue a combo if any ability ends naturally (by having the animation end). Successive abilities can only be performed while an ability is currently being performed!

### The problem and the solution
A great combo example would be a successive attack. Let's say pressing the left face button on a controller makes the player character perform a simple attack (`Attack_1)`. If the button is pressed again somewhere at the end of the animation, we want to do another attack (`Attack_2` that connects nicely with the first one. However, the main problem lies with the fact that we've mapped `Attack_1` to the left face button. Pressing it again would surely cause the player character to perform `Attack_1` again.

The plugin provides a very simple solution to this problem, by setting up two variables in an ability. The variables to set are `FGameplayTag AbilityComboTag`and `TSubclassOf<UTPAbility> ComboAbilityClass`. All abilities that make up a combo should *have the same tag*. If the combo window is open, and an ability with the same tag as the one currently being performed is tried, the system will take the `ComboAbilityClass` and perform it. 
> Example: Say a combo consists of three abilities: `Attack_1`, `Attack_2` and `Attack_3`. And let's also say that pressing the left face button makes the player perform `Attack_1`, or, to be more technical, calls `TryUseAbility` and passes the class of `Attack_1` as the argument. `Attack_1` has its `ComboAbilityClass` variable set to `Attack_2`, and `Attack_2` has the variable set to `Attack_3`. `Attack_3` doesn't have the variable set to anything because it's not necessary. All three abilities have the same gameplay tag. 
The player presses the left face button for the first time, all conditions are met, and `Attack_1` is performed. Sometimes, during the animation, the combo window opens, and the player presses the left face button again. `TryUseAbility` is called again, and `Attack_1` is passed as an argument again. However, this time, the system notices that the combo window is open, and that the passed ability class (`Attack_1`) has the same tag as the currently performed ability (`Attack_1`). Given that they have the same tag, the system will take the combo ability class (`Attack_2`) of the currently performed ability, check if all conditions pass and, if they do, perform that. Now, `Attack_2` is performed. Sometimes during the animation, a combo window is open, and the player presses the left face button. Again, `TryUseAbility` is called, and again, `Attack_1` is passed as an argument. The system notices that the combo window is open, and that the passed ability class (`Attack_1`) has the same tag as the currently performed ability (`Attack_2`). Given that they have the same tag, the system will take the combo ability class (`Attack_3`) of the currently performed ability, check if all conditions pass and, if they do, perform that. 


## Chains
### Definition
Ability chaining refers to using different abilities in succession. If one ability chains to another, it can be used in a defined time window of that ability. Chaining makes it possible to start the second ability animation at any given time. 
> Example: Let's imagine two different abilities that can be used independent of each other: a charged fire burst ability and a flame throwing ability. The first is performed by first charging up fire energy, and then releasing a concentrated short burst of it. The second consists of simply spawning flames in front of the character for a set duration. Now let's imagine that the second ability generates (charges) fire energy in the character's arms. It'd be possible to set up chaining in such a way so the character can immediately use the fire burst ability after the flame thrower ability, skipping the charging part of the animation (because the arms are already charged). 

Chaining would also be used to allow the player to interrupt one ability by using another (great example of this would be using a "dodge" ability while already performing an ability).

### Setting up chains
Chains can be set up using an animation notify state called `ChainWindowState`. An animation can theoretically contain an infinite amount of these states. In the state, one would define what abilities are usable during that section of the animation, and, if used, on what time their animation would start.