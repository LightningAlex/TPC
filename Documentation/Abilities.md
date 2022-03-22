# Abilities

## Definition
An ability consists of an animation (montage), various different events and stats typically used in combat. Abilities are the core of the combat system. A character can use only one ability at a time!

## Functionality details
### Code structure
The base class for abilities is `UTPAbility`, which inherits from `UObject` (might be changed to `AActor` later for replication purposes).  The base class contains variables used for any ability, as well as some optional ones. Each ability is supposed to be a blueprint which inherits from this base class.

### Ability animations
Each ability must play an animation. The played animation is determined by two things: `TArray<UAnimMontage*> AbilityAnimations` and `UAnimMontage* GetAbilityMontage(UTPCombatComponent* UsedCC)`. 
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

## Description
The ability system allows any character to use any ability. Abilities are 