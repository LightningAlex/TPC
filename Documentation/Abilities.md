# Abilities

## Definition
An ability consists of an animation (montage), various different events and stats typically used in combat. Abilities are the core of the combat system. A character can use only one ability at a time!

## Functionality details
### Code structure
The base class for abilities is `UTPAbility`, which inherits from `UObject` (might be changed to `AActor` later for replication purposes).  The base class contains variables used for any ability, as well as some optional ones. Each ability is supposed to be a blueprint which inherits from this base class.

### Ability animations
Each ability must play an animation. The played animation is determined by the following function: 
`UAnimMontage* GetAbilityMontage(UTPCombatComponent* UsedCC)`. The function is a blueprint implementable event, meaning it won't return anything unless overridden. 
In most cases, the overridden function will consists only of a return node, returning a very specific animation. However, for some abilities, the animation will be chosen depending on additional factors. 
> SIMPLE EXAMPLE: Imagine an ability where the character performs a spell to create and launch a fireball. The animation for an ability like this is usually very simple and unique. Thus, the function would consist of only a return node, in which the animation is literally selected in the drop-down.

>COMPLEX EXAMPLE: Imagine an ability called "right hand attack". As its name suggests, this ability would have the character perform a simple attack with the armament in their right hand. However, the animation side of things isn't that simple - it would depend on the armament. Hence, the function would search for the armament type (via equipment in the passed combat component) and return an animation based on that type.

> NOTE: Animationless abilities are planned for the future

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

### Ability Combo Tag
**Type:** FGameplayTag;
**Usage:** Used to check for potential combos. All abilities that are part of the same combo chain should share the same tag (see more in the combo system section);

### Combo Ability Class
**Type:** TSubclassOf<UTPAbility>;
**Usage:** If a combo is performed, the ability stored here will be used (see more in the combo system section);