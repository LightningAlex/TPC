# Equipment
## Description
The equipment system allows any character to equip any amount of equipment items. Equipment items encompass armaments (weapons and shields), armor and attachments (like rings and talismans). 
Every equipment item can increase/decrease any combat stats (see stats for more info).
> NOTE: In the future, we'd like to enable percentual increases/decreases

## Equipment slots
All equipment items are handled by equipment slots. Equipment slots are set up in the combat component, and there can be an infinite number of them. A slot consists of only two things: a slot name, and a slot socket. Technically the equipment slots are a map, consisting of a name and an `FEquipmentSlot` struct. The struct contains an editable name and a pointer to an equipment object. When an item is equipped, the object pointer will be stored in this map. Unequipping an item will set the pointer to a nullptr, *regardless whether the equipment item is destroyed or not!* This makes it possible for the characters to drop any equipment item (like Link does in Breath of the Wild, when struck by lightning, for instance).

## Classes and functions
The base class for equipment items is `ATPEquipmentBase`, which inherits from `AActor`. It is an abstract class. Equipping and unequipping items is handled in the combat component. When an item is equipped, `BeEquipped()` is called. Similarly, when an item is unequipped, `BeUnequipped` is called. Both functions are part of `ATPEquipmentBase`. The base functions handle the increasing/decreasing of stats, and should always be called in their overridden version. Aside the base functions, two similarly named blueprint implementable events are called.

There are three classes that inherit from `ATPEquipmentBase`, namely `ATPAttachmentBase`, `ATPArmorBase` and `ATPWeaponBase`. 
> NOTE: The word "weapon" should be renamed to "armament", given that the base weapon class encompasses shields, as well. 

### Attachment base
The attachment base class only has a static mesh component. It is meant for equipment items that can't be used as armaments and don't need any animation data, but rather attach to a socket. Examples would be rings, necklaces, talismans, etc.

### Armor base
The armor base class has a skeletal mesh component. When equipped, the appropriate equipment mesh (see below) is set as the master pose component. While it can be attached to a socket (see slots), it isn't recommended in any circumstance.

### Weapons
Weapons have a static mesh component that represents them visually. While equipped, collision on the static mesh should be turned off. Weapons have an array called `WeaponColliders`. It's an array of structs called `FWeaponCollider`. The struct allows a weapon to have multiple damage-dealing volumes (shape components) that can be (in)active during specific parts of any animation. The struct itself consists only of a name and a pointer to a shape component.

Weapons *must* initialize this array in their constructor. To activate a weapon during an animation, the notify state `ActiveWeaponState` should be used. The weapon slot, the colliders to activate and the attack type should be set in this state.

## Relevant functions
